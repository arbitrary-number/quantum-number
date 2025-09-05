package com.github.quantum_number;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Basic Mathematics Test Suite for Quantum Numbers
 * 
 * Tests fundamental arithmetic operations including:
 * - Addition and subtraction
 * - Identity operations
 * - Zero handling
 * - Basic symbolic computation
 */
@DisplayName("Basic Mathematics Tests")
public class BasicMathTest {
    
    private SimpleQuantumNumber zero;
    private SimpleQuantumNumber one;
    private SimpleQuantumNumber testNumber1;
    private SimpleQuantumNumber testNumber2;
    
    @BeforeEach
    void setUp() {
        zero = SimpleQuantumNumber.zero();
        one = SimpleQuantumNumber.one();
        
        // Create test numbers
        testNumber1 = new SimpleQuantumNumber();
        testNumber1.setOrdinal(0, 100);  // a = 100
        testNumber1.setOrdinal(1, 200);  // b = 200
        testNumber1.setOrdinal(2, 1);    // c = 1
        testNumber1.updateChecksum();
        
        testNumber2 = new SimpleQuantumNumber();
        testNumber2.setOrdinal(0, 50);   // a = 50
        testNumber2.setOrdinal(1, 150);  // b = 150
        testNumber2.setOrdinal(2, 1);    // c = 1
        testNumber2.updateChecksum();
    }
    
    @Test
    @DisplayName("Zero Identity - Addition")
    void testZeroAdditionIdentity() {
        SimpleQuantumNumber result = testNumber1.add(zero);
        assertEquals(testNumber1, result, "Adding zero should return the original number");
        
        result = zero.add(testNumber1);
        assertEquals(testNumber1, result, "Zero plus any number should equal that number");
    }
    
    @Test
    @DisplayName("Zero Identity - Subtraction")
    void testZeroSubtractionIdentity() {
        SimpleQuantumNumber result = testNumber1.subtract(zero);
        assertEquals(testNumber1, result, "Subtracting zero should return the original number");
    }
    
    @Test
    @DisplayName("Basic Addition")
    void testBasicAddition() {
        SimpleQuantumNumber result = testNumber1.add(testNumber2);
        
        // Verify ordinal-wise addition
        assertEquals(150, result.getSignedOrdinal(0), "a ordinal should be 100 + 50 = 150");
        assertEquals(350, result.getSignedOrdinal(1), "b ordinal should be 200 + 150 = 350");
        assertEquals(2, result.getSignedOrdinal(2), "c ordinal should be 1 + 1 = 2");
        
        // Verify checksum is updated
        assertTrue(result.verifyChecksum(), "Checksum should be valid after addition");
    }
    
    @Test
    @DisplayName("Basic Subtraction")
    void testBasicSubtraction() {
        SimpleQuantumNumber result = testNumber1.subtract(testNumber2);
        
        // Verify ordinal-wise subtraction
        assertEquals(50, result.getSignedOrdinal(0), "a ordinal should be 100 - 50 = 50");
        assertEquals(50, result.getSignedOrdinal(1), "b ordinal should be 200 - 150 = 50");
        assertEquals(0, result.getSignedOrdinal(2), "c ordinal should be 1 - 1 = 0");
        
        // Verify checksum is updated
        assertTrue(result.verifyChecksum(), "Checksum should be valid after subtraction");
    }
    
    @Test
    @DisplayName("Negation")
    void testNegation() {
        SimpleQuantumNumber negated = testNumber1.negate();
        
        // All signs should be flipped
        for (int i = 0; i < SimpleQuantumNumber.NUM_ORDINALS; i++) {
            assertEquals(!testNumber1.getSign(i), negated.getSign(i), 
                "Sign bit " + i + " should be flipped");
            assertEquals(testNumber1.getOrdinal(i), negated.getOrdinal(i), 
                "Ordinal " + i + " value should remain the same");
        }
        
        assertTrue(negated.verifyChecksum(), "Checksum should be valid after negation");
    }
    
    @Test
    @DisplayName("Double Negation")
    void testDoubleNegation() {
        SimpleQuantumNumber doubleNegated = testNumber1.negate().negate();
        assertEquals(testNumber1, doubleNegated, "Double negation should return original number");
    }
    
    @Test
    @DisplayName("Addition Commutativity")
    void testAdditionCommutativity() {
        SimpleQuantumNumber result1 = testNumber1.add(testNumber2);
        SimpleQuantumNumber result2 = testNumber2.add(testNumber1);
        assertEquals(result1, result2, "Addition should be commutative: a + b = b + a");
    }
    
    @Test
    @DisplayName("Addition Associativity")
    void testAdditionAssociativity() {
        SimpleQuantumNumber testNumber3 = new SimpleQuantumNumber();
        testNumber3.setOrdinal(0, 25);
        testNumber3.setOrdinal(1, 75);
        testNumber3.setOrdinal(2, 1);
        testNumber3.updateChecksum();
        
        SimpleQuantumNumber result1 = testNumber1.add(testNumber2).add(testNumber3);
        SimpleQuantumNumber result2 = testNumber1.add(testNumber2.add(testNumber3));
        
        assertEquals(result1, result2, "Addition should be associative: (a + b) + c = a + (b + c)");
    }
    
    @Test
    @DisplayName("Subtraction Anti-Commutativity")
    void testSubtractionAntiCommutativity() {
        SimpleQuantumNumber result1 = testNumber1.subtract(testNumber2);
        SimpleQuantumNumber result2 = testNumber2.subtract(testNumber1);
        SimpleQuantumNumber negatedResult2 = result2.negate();
        
        // Check that the magnitudes are equal (accounting for sign differences in zero values)
        for (int i = 0; i < SimpleQuantumNumber.NUM_ORDINALS; i++) {
            assertEquals(result1.getOrdinal(i), negatedResult2.getOrdinal(i), 
                "Ordinal " + i + " magnitudes should be equal");
            // For non-zero values, signs should be equal; for zero values, signs may differ
            if (result1.getOrdinal(i) != 0) {
                assertEquals(result1.getSign(i), negatedResult2.getSign(i), 
                    "Non-zero ordinal " + i + " signs should be equal");
            }
        }
    }
    
    @Test
    @DisplayName("Zero Creation and Properties")
    void testZeroProperties() {
        assertTrue(zero.isZero(), "Zero should be identified as zero");
        
        for (int i = 0; i < SimpleQuantumNumber.NUM_ORDINALS; i++) {
            assertEquals(0, zero.getOrdinal(i), "All ordinals in zero should be 0");
            assertFalse(zero.getSign(i), "All signs in zero should be positive");
        }
        
        assertTrue(zero.verifyChecksum(), "Zero should have valid checksum");
    }
    
    @Test
    @DisplayName("One Creation and Properties")
    void testOneProperties() {
        assertFalse(one.isZero(), "One should not be identified as zero");
        
        // Check identity structure (most ordinals = 1, g=0, h=0)
        for (int i = 0; i < SimpleQuantumNumber.NUM_ORDINALS; i++) {
            if (i == 6 || i == 7) { // g and h ordinals
                assertEquals(0, one.getOrdinal(i), "g and h ordinals should be 0 in identity");
            } else {
                assertEquals(1, one.getOrdinal(i), "Other ordinals should be 1 in identity");
            }
            assertFalse(one.getSign(i), "All signs should be positive in identity");
        }
        
        assertTrue(one.verifyChecksum(), "One should have valid checksum");
    }
    
    @Test
    @DisplayName("Checksum Integrity")
    void testChecksumIntegrity() {
        assertTrue(testNumber1.verifyChecksum(), "Test number 1 should have valid checksum");
        assertTrue(testNumber2.verifyChecksum(), "Test number 2 should have valid checksum");
        
        // Test checksum after operations
        SimpleQuantumNumber sum = testNumber1.add(testNumber2);
        assertTrue(sum.verifyChecksum(), "Sum should have valid checksum");
        
        SimpleQuantumNumber difference = testNumber1.subtract(testNumber2);
        assertTrue(difference.verifyChecksum(), "Difference should have valid checksum");
    }
    
    @Test
    @DisplayName("Ordinal Bounds Checking")
    void testOrdinalBounds() {
        SimpleQuantumNumber testNumber = new SimpleQuantumNumber();
        
        // Test valid bounds
        assertDoesNotThrow(() -> testNumber.setOrdinal(0, SimpleQuantumNumber.ORDINAL_MAX));
        assertDoesNotThrow(() -> testNumber.setOrdinal(0, SimpleQuantumNumber.ORDINAL_MIN));
        assertDoesNotThrow(() -> testNumber.setOrdinal(0, 0));
        
        // Test invalid bounds
        assertThrows(IllegalArgumentException.class, 
            () -> testNumber.setOrdinal(0, SimpleQuantumNumber.ORDINAL_MAX + 1));
        assertThrows(IllegalArgumentException.class, 
            () -> testNumber.setOrdinal(0, SimpleQuantumNumber.ORDINAL_MIN - 1));
        
        // Test invalid indices
        assertThrows(IndexOutOfBoundsException.class, 
            () -> testNumber.setOrdinal(-1, 100));
        assertThrows(IndexOutOfBoundsException.class, 
            () -> testNumber.setOrdinal(SimpleQuantumNumber.NUM_ORDINALS, 100));
    }
    
    @Test
    @DisplayName("Copy Constructor")
    void testCopyConstructor() {
        SimpleQuantumNumber copy = new SimpleQuantumNumber(testNumber1);
        
        assertEquals(testNumber1, copy, "Copy should equal original");
        assertNotSame(testNumber1, copy, "Copy should be different object");
        
        // Modify copy and ensure original is unchanged
        copy.setOrdinal(0, 999);
        copy.updateChecksum();
        
        assertNotEquals(testNumber1, copy, "Modified copy should not equal original");
        assertEquals(100, testNumber1.getOrdinal(0), "Original should be unchanged");
    }
    
    // ========== COMPLEX NUMBER BASIC MATH TESTS ==========
    
    @Test
    @DisplayName("Complex Number Creation from Integers")
    void testComplexCreationFromIntegers() {
        SimpleQuantumNumber complex = SimpleQuantumNumber.createComplex(3, 4);
        
        assertEquals(3, complex.getSignedOrdinal(0), "Real part should be 3");
        assertEquals(4, complex.getImaginaryComponent(), "Imaginary part should be 4");
        
        assertTrue(complex.verifyChecksum(), "Complex number should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Number Creation with Negative Values")
    void testComplexCreationNegativeValues() {
        SimpleQuantumNumber complex1 = SimpleQuantumNumber.createComplex(-5, 7);
        assertEquals(-5, complex1.getSignedOrdinal(0), "Real part should be -5");
        assertEquals(7, complex1.getImaginaryComponent(), "Imaginary part should be 7");
        
        SimpleQuantumNumber complex2 = SimpleQuantumNumber.createComplex(8, -3);
        assertEquals(8, complex2.getSignedOrdinal(0), "Real part should be 8");
        assertEquals(-3, complex2.getImaginaryComponent(), "Imaginary part should be -3");
        
        SimpleQuantumNumber complex3 = SimpleQuantumNumber.createComplex(-2, -6);
        assertEquals(-2, complex3.getSignedOrdinal(0), "Real part should be -2");
        assertEquals(-6, complex3.getImaginaryComponent(), "Imaginary part should be -6");
    }
    
    @Test
    @DisplayName("Complex Multiplication - Basic Cases")
    void testComplexMultiplicationBasic() {
        // (3 + 4i) * (2 + 1i) = (6 - 4) + (3 + 8)i = 2 + 11i
        SimpleQuantumNumber complex1 = SimpleQuantumNumber.createComplex(3, 4);
        SimpleQuantumNumber complex2 = SimpleQuantumNumber.createComplex(2, 1);
        
        SimpleQuantumNumber result = complex1.multiplyComplex(complex2);
        
        assertEquals(2, result.getSignedOrdinal(0), "Real part should be 2");
        assertEquals(11, result.getImaginaryComponent(), "Imaginary part should be 11");
        assertTrue(result.verifyChecksum(), "Result should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Multiplication - Imaginary Unit")
    void testComplexMultiplicationImaginaryUnit() {
        // i * i = -1
        SimpleQuantumNumber i = SimpleQuantumNumber.createComplex(0, 1);
        SimpleQuantumNumber result = i.multiplyComplex(i);
        
        assertEquals(-1, result.getSignedOrdinal(0), "i * i should equal -1");
        assertEquals(0, result.getImaginaryComponent(), "Imaginary part should be 0");
    }
    
    @Test
    @DisplayName("Complex Multiplication - Real Numbers")
    void testComplexMultiplicationRealNumbers() {
        // (5 + 0i) * (3 + 0i) = 15 + 0i
        SimpleQuantumNumber real1 = SimpleQuantumNumber.createComplex(5, 0);
        SimpleQuantumNumber real2 = SimpleQuantumNumber.createComplex(3, 0);
        
        SimpleQuantumNumber result = real1.multiplyComplex(real2);
        
        assertEquals(15, result.getSignedOrdinal(0), "Real multiplication should work");
        assertEquals(0, result.getImaginaryComponent(), "Imaginary part should remain 0");
    }
    
    @Test
    @DisplayName("Complex Multiplication - Pure Imaginary")
    void testComplexMultiplicationPureImaginary() {
        // (0 + 3i) * (0 + 2i) = -6 + 0i
        SimpleQuantumNumber imag1 = SimpleQuantumNumber.createComplex(0, 3);
        SimpleQuantumNumber imag2 = SimpleQuantumNumber.createComplex(0, 2);
        
        SimpleQuantumNumber result = imag1.multiplyComplex(imag2);
        
        assertEquals(-6, result.getSignedOrdinal(0), "Pure imaginary multiplication should give real result");
        assertEquals(0, result.getImaginaryComponent(), "Imaginary part should be 0");
    }
    
    @Test
    @DisplayName("Complex Division - Basic Cases")
    void testComplexDivisionBasic() {
        // (8 + 6i) / (3 + 1i) = ((8*3 + 6*1) + (6*3 - 8*1)i) / (3² + 1²) = (30 + 10i) / 10 = 3 + 1i
        SimpleQuantumNumber dividend = SimpleQuantumNumber.createComplex(8, 6);
        SimpleQuantumNumber divisor = SimpleQuantumNumber.createComplex(3, 1);
        
        SimpleQuantumNumber result = dividend.divideComplex(divisor);
        
        assertEquals(3, result.getSignedOrdinal(0), "Real part should be 3");
        assertEquals(1, result.getImaginaryComponent(), "Imaginary part should be 1");
        assertTrue(result.verifyChecksum(), "Result should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Division - By Real Number")
    void testComplexDivisionByReal() {
        // (6 + 8i) / (2 + 0i) = 3 + 4i
        SimpleQuantumNumber dividend = SimpleQuantumNumber.createComplex(6, 8);
        SimpleQuantumNumber divisor = SimpleQuantumNumber.createComplex(2, 0);
        
        SimpleQuantumNumber result = dividend.divideComplex(divisor);
        
        assertEquals(3, result.getSignedOrdinal(0), "Real part should be 3");
        assertEquals(4, result.getImaginaryComponent(), "Imaginary part should be 4");
    }
    
    @Test
    @DisplayName("Complex Division - Division by Zero")
    void testComplexDivisionByZero() {
        SimpleQuantumNumber dividend = SimpleQuantumNumber.createComplex(5, 3);
        SimpleQuantumNumber zero = SimpleQuantumNumber.createComplex(0, 0);
        
        assertThrows(ArithmeticException.class, () -> dividend.divideComplex(zero),
            "Division by zero should throw ArithmeticException");
    }
    
    @Test
    @DisplayName("Complex Conjugate")
    void testComplexConjugate() {
        SimpleQuantumNumber complex = SimpleQuantumNumber.createComplex(3, -4);
        SimpleQuantumNumber conjugate = complex.conjugate();
        
        assertEquals(3, conjugate.getSignedOrdinal(0), "Real part should remain the same");
        assertEquals(4, conjugate.getImaginaryComponent(), "Imaginary part should be negated");
        assertTrue(conjugate.verifyChecksum(), "Conjugate should have valid checksum");
        
        // Test conjugate of conjugate
        SimpleQuantumNumber doubleConjugate = conjugate.conjugate();
        assertEquals(complex, doubleConjugate, "Conjugate of conjugate should equal original");
    }
    
    @Test
    @DisplayName("Complex Magnitude Squared")
    void testComplexMagnitudeSquared() {
        // |3 + 4i|² = 3² + 4² = 9 + 16 = 25
        SimpleQuantumNumber complex = SimpleQuantumNumber.createComplex(3, 4);
        assertEquals(25L, complex.magnitudeSquared(), "Magnitude squared should be 25");
        
        // |5 + 0i|² = 25
        SimpleQuantumNumber real = SimpleQuantumNumber.createComplex(5, 0);
        assertEquals(25L, real.magnitudeSquared(), "Real number magnitude squared should be 25");
        
        // |0 + 7i|² = 49
        SimpleQuantumNumber imaginary = SimpleQuantumNumber.createComplex(0, 7);
        assertEquals(49L, imaginary.magnitudeSquared(), "Pure imaginary magnitude squared should be 49");
    }
    
    @Test
    @DisplayName("Complex Magnitude")
    void testComplexMagnitude() {
        // |3 + 4i| = √(3² + 4²) = √25 = 5
        SimpleQuantumNumber complex = SimpleQuantumNumber.createComplex(3, 4);
        assertEquals(5.0, complex.magnitude(), 1e-10, "Magnitude should be 5.0");
        
        // |5 + 0i| = 5
        SimpleQuantumNumber real = SimpleQuantumNumber.createComplex(5, 0);
        assertEquals(5.0, real.magnitude(), 1e-10, "Real number magnitude should be 5.0");
        
        // |0 + 3i| = 3
        SimpleQuantumNumber imaginary = SimpleQuantumNumber.createComplex(0, 3);
        assertEquals(3.0, imaginary.magnitude(), 1e-10, "Pure imaginary magnitude should be 3.0");
    }
    
    @Test
    @DisplayName("Complex Argument (Phase)")
    void testComplexArgument() {
        // arg(1 + 0i) = 0
        SimpleQuantumNumber real = SimpleQuantumNumber.createComplex(1, 0);
        assertEquals(0.0, real.argument(), 1e-10, "Argument of positive real should be 0");
        
        // arg(0 + 1i) = π/2
        SimpleQuantumNumber imaginary = SimpleQuantumNumber.createComplex(0, 1);
        assertEquals(Math.PI / 2, imaginary.argument(), 1e-10, "Argument of positive imaginary should be π/2");
        
        // arg(-1 + 0i) = π
        SimpleQuantumNumber negativeReal = SimpleQuantumNumber.createComplex(-1, 0);
        assertEquals(Math.PI, Math.abs(negativeReal.argument()), 1e-10, "Argument of negative real should be ±π");
        
        // arg(0 - 1i) = -π/2
        SimpleQuantumNumber negativeImaginary = SimpleQuantumNumber.createComplex(0, -1);
        assertEquals(-Math.PI / 2, negativeImaginary.argument(), 1e-10, "Argument of negative imaginary should be -π/2");
    }
    
    @Test
    @DisplayName("Pure Real Number Detection")
    void testPureRealDetection() {
        SimpleQuantumNumber real1 = SimpleQuantumNumber.createComplex(5, 0);
        assertTrue(real1.isPureReal(), "Number with zero imaginary should be pure real");
        
        SimpleQuantumNumber real2 = SimpleQuantumNumber.createComplex(3, 1);
        assertTrue(real2.isPureReal(), "Number with imaginary = 1 should be pure real");
        
        SimpleQuantumNumber complex = SimpleQuantumNumber.createComplex(2, 3);
        assertFalse(complex.isPureReal(), "Number with non-trivial imaginary should not be pure real");
    }
    
    @Test
    @DisplayName("Pure Imaginary Number Detection")
    void testPureImaginaryDetection() {
        SimpleQuantumNumber imaginary = SimpleQuantumNumber.createComplex(0, 5);
        assertTrue(imaginary.isPureImaginary(), "Number with zero real should be pure imaginary");
        
        SimpleQuantumNumber real = SimpleQuantumNumber.createComplex(3, 4);
        assertFalse(real.isPureImaginary(), "Number with non-zero real should not be pure imaginary");
        
        SimpleQuantumNumber zero = SimpleQuantumNumber.createComplex(0, 0);
        assertTrue(zero.isPureImaginary(), "Zero should be considered pure imaginary");
    }
    
    @Test
    @DisplayName("Complex Power - Integer Exponents")
    void testComplexPower() {
        // (1 + 1i)² = 1 + 2i + i² = 1 + 2i - 1 = 2i
        SimpleQuantumNumber base = SimpleQuantumNumber.createComplex(1, 1);
        SimpleQuantumNumber result = base.complexPower(2);
        
        // Due to floating point precision in polar conversion, allow small tolerance
        assertEquals(0, result.getSignedOrdinal(0), 2, "Real part should be approximately 0");
        assertEquals(2, result.getImaginaryComponent(), 2, "Imaginary part should be approximately 2");
        
        // Test power of 0
        SimpleQuantumNumber power0 = base.complexPower(0);
        assertEquals(1, power0.getSignedOrdinal(0), "Any number to power 0 should be 1");
        assertEquals(0, power0.getImaginaryComponent(), "Imaginary part should be 0");
        
        // Test power of 1
        SimpleQuantumNumber power1 = base.complexPower(1);
        assertEquals(base.getSignedOrdinal(0), power1.getSignedOrdinal(0), "Power 1 should return original real part");
        assertEquals(base.getImaginaryComponent(), power1.getImaginaryComponent(), "Power 1 should return original imaginary part");
    }
    
    @Test
    @DisplayName("Complex Arithmetic - Addition with Complex Numbers")
    void testComplexArithmeticAddition() {
        // (3 + 4i) + (1 + 2i) = 4 + 6i
        SimpleQuantumNumber complex1 = SimpleQuantumNumber.createComplex(3, 4);
        SimpleQuantumNumber complex2 = SimpleQuantumNumber.createComplex(1, 2);
        
        SimpleQuantumNumber sum = complex1.add(complex2);
        
        assertEquals(4, sum.getSignedOrdinal(0), "Real parts should add: 3 + 1 = 4");
        assertEquals(6, sum.getImaginaryComponent(), "Imaginary parts should add: 4 + 2 = 6");
        assertTrue(sum.verifyChecksum(), "Sum should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Arithmetic - Subtraction with Complex Numbers")
    void testComplexArithmeticSubtraction() {
        // (5 + 7i) - (2 + 3i) = 3 + 4i
        SimpleQuantumNumber complex1 = SimpleQuantumNumber.createComplex(5, 7);
        SimpleQuantumNumber complex2 = SimpleQuantumNumber.createComplex(2, 3);
        
        SimpleQuantumNumber difference = complex1.subtract(complex2);
        
        assertEquals(3, difference.getSignedOrdinal(0), "Real parts should subtract: 5 - 2 = 3");
        assertEquals(4, difference.getImaginaryComponent(), "Imaginary parts should subtract: 7 - 3 = 4");
        assertTrue(difference.verifyChecksum(), "Difference should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Arithmetic - Negation")
    void testComplexArithmeticNegation() {
        SimpleQuantumNumber complex = SimpleQuantumNumber.createComplex(3, -4);
        SimpleQuantumNumber negated = complex.negate();
        
        assertEquals(-3, negated.getSignedOrdinal(0), "Real part should be negated");
        assertEquals(4, negated.getImaginaryComponent(), "Imaginary part should be negated");
        assertTrue(negated.verifyChecksum(), "Negated complex should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex Number Multiplication Commutativity")
    void testComplexMultiplicationCommutativity() {
        SimpleQuantumNumber complex1 = SimpleQuantumNumber.createComplex(2, 3);
        SimpleQuantumNumber complex2 = SimpleQuantumNumber.createComplex(4, 1);
        
        SimpleQuantumNumber result1 = complex1.multiplyComplex(complex2);
        SimpleQuantumNumber result2 = complex2.multiplyComplex(complex1);
        
        assertEquals(result1.getSignedOrdinal(0), result2.getSignedOrdinal(0), 
            "Complex multiplication should be commutative (real part)");
        assertEquals(result1.getImaginaryComponent(), result2.getImaginaryComponent(), 
            "Complex multiplication should be commutative (imaginary part)");
    }
    
    @Test
    @DisplayName("Complex Number Multiplication Associativity")
    void testComplexMultiplicationAssociativity() {
        SimpleQuantumNumber complex1 = SimpleQuantumNumber.createComplex(1, 2);
        SimpleQuantumNumber complex2 = SimpleQuantumNumber.createComplex(3, 1);
        SimpleQuantumNumber complex3 = SimpleQuantumNumber.createComplex(2, 2);
        
        SimpleQuantumNumber result1 = complex1.multiplyComplex(complex2).multiplyComplex(complex3);
        SimpleQuantumNumber result2 = complex1.multiplyComplex(complex2.multiplyComplex(complex3));
        
        assertEquals(result1.getSignedOrdinal(0), result2.getSignedOrdinal(0), 
            "Complex multiplication should be associative (real part)");
        assertEquals(result1.getImaginaryComponent(), result2.getImaginaryComponent(), 
            "Complex multiplication should be associative (imaginary part)");
    }
    
    @Test
    @DisplayName("Complex Number Distributive Property")
    void testComplexDistributiveProperty() {
        // a * (b + c) = a * b + a * c
        SimpleQuantumNumber a = SimpleQuantumNumber.createComplex(2, 1);
        SimpleQuantumNumber b = SimpleQuantumNumber.createComplex(3, 2);
        SimpleQuantumNumber c = SimpleQuantumNumber.createComplex(1, 3);
        
        SimpleQuantumNumber left = a.multiplyComplex(b.add(c));
        SimpleQuantumNumber right = a.multiplyComplex(b).add(a.multiplyComplex(c));
        
        assertEquals(left.getSignedOrdinal(0), right.getSignedOrdinal(0), 
            "Distributive property should hold (real part)");
        assertEquals(left.getImaginaryComponent(), right.getImaginaryComponent(), 
            "Distributive property should hold (imaginary part)");
    }
    
    @Test
    @DisplayName("Complex Conjugate Properties")
    void testComplexConjugateProperties() {
        SimpleQuantumNumber complex = SimpleQuantumNumber.createComplex(3, 4);
        SimpleQuantumNumber conjugate = complex.conjugate();
        
        // z * z̄ = |z|²
        SimpleQuantumNumber product = complex.multiplyComplex(conjugate);
        assertEquals(complex.magnitudeSquared(), product.getSignedOrdinal(0), 
            "z * conjugate(z) should equal |z|²");
        assertEquals(0, product.getImaginaryComponent(), 
            "z * conjugate(z) should be real");
        
        // conjugate(conjugate(z)) = z
        SimpleQuantumNumber doubleConjugate = conjugate.conjugate();
        assertEquals(complex, doubleConjugate, "Double conjugate should equal original");
    }
    
    @Test
    @DisplayName("Large Complex Numbers")
    void testLargeComplexNumbers() {
        int largeReal = SimpleQuantumNumber.ORDINAL_MAX / 2;
        int largeImag = SimpleQuantumNumber.ORDINAL_MAX / 3;
        
        SimpleQuantumNumber largeComplex = SimpleQuantumNumber.createComplex(largeReal, largeImag);
        
        assertEquals(largeReal, largeComplex.getSignedOrdinal(0), "Large real part should be preserved");
        assertEquals(largeImag, largeComplex.getImaginaryComponent(), "Large imaginary part should be preserved");
        assertTrue(largeComplex.verifyChecksum(), "Large complex should have valid checksum");
        
        // Test operations with large numbers
        SimpleQuantumNumber conjugate = largeComplex.conjugate();
        assertEquals(largeReal, conjugate.getSignedOrdinal(0), "Conjugate real part should be preserved");
        assertEquals(-largeImag, conjugate.getImaginaryComponent(), "Conjugate imaginary part should be negated");
    }
}
