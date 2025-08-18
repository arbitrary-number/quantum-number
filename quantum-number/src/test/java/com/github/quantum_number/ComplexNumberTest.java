package com.github.quantum_number;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Complex Number Test Suite for Quantum Numbers
 * 
 * Tests complex number functionality including:
 * - Real and imaginary component handling
 * - Complex arithmetic operations
 * - Complex number creation and manipulation
 * - Imaginary unit properties
 */
@DisplayName("Complex Number Tests")
public class ComplexNumberTest {
    
    private QuantumNumber realNumber;
    private QuantumNumber complexNumber;
    private QuantumNumber pureImaginary;
    
    @BeforeEach
    void setUp() {
        // Create a real number (imaginary component = 1, which is neutral)
        realNumber = new QuantumNumber();
        realNumber.setOrdinal(0, 100);  // a = 100
        realNumber.setOrdinal(1, 200);  // b = 200
        realNumber.setOrdinal(8, 1);    // i = 1 (neutral for real numbers)
        realNumber.updateChecksum();
        
        // Create a complex number with both real and imaginary parts
        complexNumber = new QuantumNumber();
        complexNumber.setOrdinal(0, 50);   // a = 50 (real part)
        complexNumber.setOrdinal(1, 100);  // b = 100 (real part)
        complexNumber.setOrdinal(8, 75);   // i = 75 (imaginary part)
        complexNumber.updateChecksum();
        
        // Create a pure imaginary number
        pureImaginary = QuantumNumber.zero();
        pureImaginary.setOrdinal(8, 42);   // i = 42 (pure imaginary)
        pureImaginary.updateChecksum();
    }
    
    @Test
    @DisplayName("Real Component Extraction")
    void testRealComponentExtraction() {
        QuantumNumber realPart = complexNumber.getRealComponent();
        
        // Real component should have imaginary ordinal set to 1 (neutral)
        assertEquals(1, realPart.getOrdinal(8), "Real component should have imaginary ordinal = 1");
        assertFalse(realPart.getSign(8), "Real component imaginary sign should be positive");
        
        // Other ordinals should be preserved
        assertEquals(complexNumber.getOrdinal(0), realPart.getOrdinal(0), "a ordinal should be preserved");
        assertEquals(complexNumber.getOrdinal(1), realPart.getOrdinal(1), "b ordinal should be preserved");
        
        assertTrue(realPart.verifyChecksum(), "Real component should have valid checksum");
    }
    
    @Test
    @DisplayName("Imaginary Component Extraction")
    void testImaginaryComponentExtraction() {
        int imaginaryPart = complexNumber.getImaginaryComponent();
        assertEquals(75, imaginaryPart, "Imaginary component should be 75");
        
        // Test negative imaginary component
        QuantumNumber negativeImaginary = new QuantumNumber(complexNumber);
        negativeImaginary.setSign(8, true); // Make imaginary part negative
        negativeImaginary.updateChecksum();
        
        assertEquals(-75, negativeImaginary.getImaginaryComponent(), "Negative imaginary component should be -75");
    }
    
    @Test
    @DisplayName("Complex Number Creation")
    void testComplexNumberCreation() {
        QuantumNumber baseReal = new QuantumNumber();
        baseReal.setOrdinal(0, 30);
        baseReal.setOrdinal(1, 60);
        baseReal.updateChecksum();
        
        QuantumNumber complex = QuantumNumber.createComplex(baseReal, 25);
        
        // Check that real part is preserved
        assertEquals(30, complex.getOrdinal(0), "Real part a ordinal should be preserved");
        assertEquals(60, complex.getOrdinal(1), "Real part b ordinal should be preserved");
        
        // Check imaginary part
        assertEquals(25, complex.getImaginaryComponent(), "Imaginary component should be 25");
        
        assertTrue(complex.verifyChecksum(), "Complex number should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Number Creation with Negative Imaginary")
    void testComplexNumberCreationNegativeImaginary() {
        QuantumNumber baseReal = new QuantumNumber();
        baseReal.setOrdinal(0, 40);
        baseReal.updateChecksum();
        
        QuantumNumber complex = QuantumNumber.createComplex(baseReal, -30);
        
        assertEquals(-30, complex.getImaginaryComponent(), "Negative imaginary component should be -30");
        assertEquals(30, complex.getOrdinal(8), "Imaginary ordinal magnitude should be 30");
        assertTrue(complex.getSign(8), "Imaginary sign should be negative");
    }
    
    @Test
    @DisplayName("Pure Real Number Properties")
    void testPureRealNumberProperties() {
        assertEquals(1, realNumber.getImaginaryComponent(), "Pure real number should have imaginary component = 1");
        
        QuantumNumber realPart = realNumber.getRealComponent();
        assertEquals(realNumber, realPart, "Real component of pure real number should equal itself");
    }
    
    @Test
    @DisplayName("Pure Imaginary Number Properties")
    void testPureImaginaryNumberProperties() {
        assertEquals(42, pureImaginary.getImaginaryComponent(), "Pure imaginary should have imaginary component = 42");
        
        // Real part should be zero except for imaginary ordinal
        QuantumNumber realPart = pureImaginary.getRealComponent();
        for (int i = 0; i < QuantumNumber.NUM_ORDINALS; i++) {
            if (i == 8) {
                assertEquals(1, realPart.getOrdinal(i), "Imaginary ordinal in real part should be 1");
            } else {
                assertEquals(0, realPart.getOrdinal(i), "Other ordinals in real part should be 0");
            }
        }
    }
    
    @Test
    @DisplayName("Complex Addition")
    void testComplexAddition() {
        QuantumNumber complex1 = QuantumNumber.createComplex(realNumber, 10);
        QuantumNumber complex2 = QuantumNumber.createComplex(realNumber, 20);
        
        QuantumNumber sum = complex1.add(complex2);
        
        // Real parts should add
        assertEquals(200, sum.getSignedOrdinal(0), "Real parts should add: 100 + 100 = 200");
        assertEquals(400, sum.getSignedOrdinal(1), "Real parts should add: 200 + 200 = 400");
        
        // Imaginary parts should add
        assertEquals(30, sum.getImaginaryComponent(), "Imaginary parts should add: 10 + 20 = 30");
        
        assertTrue(sum.verifyChecksum(), "Sum should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Subtraction")
    void testComplexSubtraction() {
        QuantumNumber complex1 = QuantumNumber.createComplex(realNumber, 25);
        QuantumNumber complex2 = QuantumNumber.createComplex(realNumber, 15);
        
        QuantumNumber difference = complex1.subtract(complex2);
        
        // Real parts should subtract (but will be zero since same base)
        assertEquals(0, difference.getSignedOrdinal(0), "Real parts should subtract to 0");
        assertEquals(0, difference.getSignedOrdinal(1), "Real parts should subtract to 0");
        
        // Imaginary parts should subtract
        assertEquals(10, difference.getImaginaryComponent(), "Imaginary parts should subtract: 25 - 15 = 10");
        
        assertTrue(difference.verifyChecksum(), "Difference should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Negation")
    void testComplexNegation() {
        QuantumNumber negated = complexNumber.negate();
        
        // All components should be negated
        assertEquals(-complexNumber.getSignedOrdinal(0), negated.getSignedOrdinal(0), "Real part a should be negated");
        assertEquals(-complexNumber.getSignedOrdinal(1), negated.getSignedOrdinal(1), "Real part b should be negated");
        assertEquals(-complexNumber.getImaginaryComponent(), negated.getImaginaryComponent(), "Imaginary part should be negated");
        
        assertTrue(negated.verifyChecksum(), "Negated complex should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Conjugate Simulation")
    void testComplexConjugateSimulation() {
        // Simulate complex conjugate by negating only the imaginary part
        QuantumNumber conjugate = new QuantumNumber(complexNumber);
        conjugate.setSign(8, !conjugate.getSign(8)); // Flip imaginary sign
        conjugate.updateChecksum();
        
        // Real parts should be the same
        assertEquals(complexNumber.getSignedOrdinal(0), conjugate.getSignedOrdinal(0), "Real part a should be unchanged");
        assertEquals(complexNumber.getSignedOrdinal(1), conjugate.getSignedOrdinal(1), "Real part b should be unchanged");
        
        // Imaginary part should be negated
        assertEquals(-complexNumber.getImaginaryComponent(), conjugate.getImaginaryComponent(), "Imaginary part should be negated");
        
        assertTrue(conjugate.verifyChecksum(), "Conjugate should have valid checksum");
    }
    
    @Test
    @DisplayName("Imaginary Unit Properties")
    void testImaginaryUnitProperties() {
        // Create imaginary unit (0 + 1i)
        QuantumNumber imaginaryUnit = QuantumNumber.zero();
        imaginaryUnit.setOrdinal(8, 1); // i = 1
        imaginaryUnit.updateChecksum();
        
        assertEquals(1, imaginaryUnit.getImaginaryComponent(), "Imaginary unit should have imaginary component = 1");
        assertTrue(imaginaryUnit.getRealComponent().isZero(), "Imaginary unit should have zero real component");
    }
    
    @Test
    @DisplayName("Complex Number String Representation")
    void testComplexStringRepresentation() {
        String representation = complexNumber.toString();
        
        // Should contain imaginary marker
        assertTrue(representation.contains("<imaginary>"), "String should contain imaginary marker");
        assertTrue(representation.contains("Complex Form:"), "String should contain complex form");
        assertTrue(representation.contains("75i"), "String should show imaginary component with 'i'");
        
        // Should show both real and imaginary parts
        assertTrue(representation.contains("Real ="), "String should show real part");
        assertTrue(representation.contains("Imaginary ="), "String should show imaginary part");
    }
    
    @Test
    @DisplayName("Zero Imaginary Component")
    void testZeroImaginaryComponent() {
        QuantumNumber zeroImaginary = QuantumNumber.createComplex(realNumber, 0);
        
        assertEquals(0, zeroImaginary.getImaginaryComponent(), "Zero imaginary component should be 0");
        assertEquals(0, zeroImaginary.getOrdinal(8), "Imaginary ordinal should be 0");
        assertFalse(zeroImaginary.getSign(8), "Imaginary sign should be positive");
    }
    
    @Test
    @DisplayName("Large Imaginary Components")
    void testLargeImaginaryComponents() {
        int largeImaginary = QuantumNumber.ORDINAL_MAX;
        QuantumNumber largeComplex = QuantumNumber.createComplex(realNumber, largeImaginary);
        
        assertEquals(largeImaginary, largeComplex.getImaginaryComponent(), "Large imaginary component should be preserved");
        assertTrue(largeComplex.verifyChecksum(), "Large complex should have valid checksum");
        
        // Test negative large imaginary
        int negLargeImaginary = QuantumNumber.ORDINAL_MIN;
        QuantumNumber negLargeComplex = QuantumNumber.createComplex(realNumber, negLargeImaginary);
        
        assertEquals(negLargeImaginary, negLargeComplex.getImaginaryComponent(), "Large negative imaginary should be preserved");
        assertTrue(negLargeComplex.verifyChecksum(), "Large negative complex should have valid checksum");
    }
}
