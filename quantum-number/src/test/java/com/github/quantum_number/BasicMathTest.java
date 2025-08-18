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
    
    private QuantumNumber zero;
    private QuantumNumber one;
    private QuantumNumber testNumber1;
    private QuantumNumber testNumber2;
    
    @BeforeEach
    void setUp() {
        zero = QuantumNumber.zero();
        one = QuantumNumber.one();
        
        // Create test numbers
        testNumber1 = new QuantumNumber();
        testNumber1.setOrdinal(0, 100);  // a = 100
        testNumber1.setOrdinal(1, 200);  // b = 200
        testNumber1.setOrdinal(2, 1);    // c = 1
        testNumber1.updateChecksum();
        
        testNumber2 = new QuantumNumber();
        testNumber2.setOrdinal(0, 50);   // a = 50
        testNumber2.setOrdinal(1, 150);  // b = 150
        testNumber2.setOrdinal(2, 1);    // c = 1
        testNumber2.updateChecksum();
    }
    
    @Test
    @DisplayName("Zero Identity - Addition")
    void testZeroAdditionIdentity() {
        QuantumNumber result = testNumber1.add(zero);
        assertEquals(testNumber1, result, "Adding zero should return the original number");
        
        result = zero.add(testNumber1);
        assertEquals(testNumber1, result, "Zero plus any number should equal that number");
    }
    
    @Test
    @DisplayName("Zero Identity - Subtraction")
    void testZeroSubtractionIdentity() {
        QuantumNumber result = testNumber1.subtract(zero);
        assertEquals(testNumber1, result, "Subtracting zero should return the original number");
    }
    
    @Test
    @DisplayName("Basic Addition")
    void testBasicAddition() {
        QuantumNumber result = testNumber1.add(testNumber2);
        
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
        QuantumNumber result = testNumber1.subtract(testNumber2);
        
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
        QuantumNumber negated = testNumber1.negate();
        
        // All signs should be flipped
        for (int i = 0; i < QuantumNumber.NUM_ORDINALS; i++) {
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
        QuantumNumber doubleNegated = testNumber1.negate().negate();
        assertEquals(testNumber1, doubleNegated, "Double negation should return original number");
    }
    
    @Test
    @DisplayName("Addition Commutativity")
    void testAdditionCommutativity() {
        QuantumNumber result1 = testNumber1.add(testNumber2);
        QuantumNumber result2 = testNumber2.add(testNumber1);
        assertEquals(result1, result2, "Addition should be commutative: a + b = b + a");
    }
    
    @Test
    @DisplayName("Addition Associativity")
    void testAdditionAssociativity() {
        QuantumNumber testNumber3 = new QuantumNumber();
        testNumber3.setOrdinal(0, 25);
        testNumber3.setOrdinal(1, 75);
        testNumber3.setOrdinal(2, 1);
        testNumber3.updateChecksum();
        
        QuantumNumber result1 = testNumber1.add(testNumber2).add(testNumber3);
        QuantumNumber result2 = testNumber1.add(testNumber2.add(testNumber3));
        
        assertEquals(result1, result2, "Addition should be associative: (a + b) + c = a + (b + c)");
    }
    
    @Test
    @DisplayName("Subtraction Anti-Commutativity")
    void testSubtractionAntiCommutativity() {
        QuantumNumber result1 = testNumber1.subtract(testNumber2);
        QuantumNumber result2 = testNumber2.subtract(testNumber1);
        QuantumNumber negatedResult2 = result2.negate();
        
        // Check that the magnitudes are equal (accounting for sign differences in zero values)
        for (int i = 0; i < QuantumNumber.NUM_ORDINALS; i++) {
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
        
        for (int i = 0; i < QuantumNumber.NUM_ORDINALS; i++) {
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
        for (int i = 0; i < QuantumNumber.NUM_ORDINALS; i++) {
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
        QuantumNumber sum = testNumber1.add(testNumber2);
        assertTrue(sum.verifyChecksum(), "Sum should have valid checksum");
        
        QuantumNumber difference = testNumber1.subtract(testNumber2);
        assertTrue(difference.verifyChecksum(), "Difference should have valid checksum");
    }
    
    @Test
    @DisplayName("Ordinal Bounds Checking")
    void testOrdinalBounds() {
        QuantumNumber testNumber = new QuantumNumber();
        
        // Test valid bounds
        assertDoesNotThrow(() -> testNumber.setOrdinal(0, QuantumNumber.ORDINAL_MAX));
        assertDoesNotThrow(() -> testNumber.setOrdinal(0, QuantumNumber.ORDINAL_MIN));
        assertDoesNotThrow(() -> testNumber.setOrdinal(0, 0));
        
        // Test invalid bounds
        assertThrows(IllegalArgumentException.class, 
            () -> testNumber.setOrdinal(0, QuantumNumber.ORDINAL_MAX + 1));
        assertThrows(IllegalArgumentException.class, 
            () -> testNumber.setOrdinal(0, QuantumNumber.ORDINAL_MIN - 1));
        
        // Test invalid indices
        assertThrows(IndexOutOfBoundsException.class, 
            () -> testNumber.setOrdinal(-1, 100));
        assertThrows(IndexOutOfBoundsException.class, 
            () -> testNumber.setOrdinal(QuantumNumber.NUM_ORDINALS, 100));
    }
    
    @Test
    @DisplayName("Copy Constructor")
    void testCopyConstructor() {
        QuantumNumber copy = new QuantumNumber(testNumber1);
        
        assertEquals(testNumber1, copy, "Copy should equal original");
        assertNotSame(testNumber1, copy, "Copy should be different object");
        
        // Modify copy and ensure original is unchanged
        copy.setOrdinal(0, 999);
        copy.updateChecksum();
        
        assertNotEquals(testNumber1, copy, "Modified copy should not equal original");
        assertEquals(100, testNumber1.getOrdinal(0), "Original should be unchanged");
    }
}
