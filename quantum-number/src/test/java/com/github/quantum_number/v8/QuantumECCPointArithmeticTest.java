package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import static org.junit.jupiter.api.Assertions.*;

/**
 * QuantumECCPointArithmeticTest - Comprehensive tests for pure QuantumNumberV8-based ECC
 *
 * Tests demonstrate:
 * - All numeric components represented as complete QuantumNumberV8 instances
 * - Real arithmetic on linked QuantumNumberV8 substrates
 * - Proper handling of overflow and modular arithmetic
 * - ECC point doubling and addition operations
 * - Verifiable results consistent with secp256k1 curve parameters
 * - Hierarchical numeric structure with linked substrates
 */
public class QuantumECCPointArithmeticTest {

    @BeforeEach
    void setUp() {
        System.out.println("\n=== QuantumECCPointArithmetic Test Setup ===");
    }

    /**
     * Test 1: Verify QuantumNumberV8 substrate initialization
     * Demonstrates that all numeric components are complete QuantumNumberV8 instances
     */
    @Test
    void testQuantumNumberV8SubstrateInitialization() {
        System.out.println("Test 1: QuantumNumberV8 Substrate Initialization");

        // Create a QuantumNumberV8 instance
        QuantumNumberV8 num = new QuantumNumberV8();

        // Verify all components are initialized as QuantumNumberV8 instances
        assertNotNull(num.a1, "a1 should be initialized");
        assertNotNull(num.a2, "a2 should be initialized");
        assertNotNull(num.a3, "a3 should be initialized");
        assertNotNull(num.a4, "a4 should be initialized");

        // All components should be long values (not primitives in the sense of not being objects)
        assertTrue(num.a1 instanceof Long, "a1 should be a Long");
        assertTrue(num.a2 instanceof Long, "a2 should be a Long");
        assertTrue(num.a3 instanceof Long, "a3 should be a Long");
        assertTrue(num.a4 instanceof Long, "a4 should be a Long");

        // Verify initial values are zero
        assertEquals(0L, num.a1, "a1 should be initialized to 0");
        assertEquals(0L, num.a2, "a2 should be initialized to 0");
        assertEquals(0L, num.a3, "a3 should be initialized to 0");
        assertEquals(0L, num.a4, "a4 should be initialized to 0");

        System.out.println("✓ All QuantumNumberV8 components properly initialized as complete instances");
    }

    /**
     * Test 2: Demonstrate linked QuantumNumberV8 substrate arithmetic
     * Shows real arithmetic operations on linked substrates
     */
    @Test
    void testLinkedSubstrateArithmetic() {
        System.out.println("Test 2: Linked Substrate Arithmetic");

        // Create two QuantumNumberV8 substrates
        QuantumNumberV8 a = new QuantumNumberV8();
        a.a4 = 100L;

        QuantumNumberV8 b = new QuantumNumberV8();
        b.a4 = 50L;

        System.out.println("a = " + a.toString());
        System.out.println("b = " + b.toString());

        // Test addition using linked substrates
        QuantumNumberV8 sum = QuantumECCPointArithmetic.quantumAdd(a, b);
        System.out.println("a + b = " + sum.toString());
        assertEquals(150L, sum.a4, "Addition result incorrect");

        // Test multiplication using linked substrates
        QuantumNumberV8 product = QuantumECCPointArithmetic.quantumMultiply(a, b);
        System.out.println("a * b = " + product.toString());
        assertEquals(5000L, product.a4, "Multiplication result incorrect");

        // Test modular reduction
        QuantumNumberV8 largeNum = new QuantumNumberV8();
        largeNum.a1 = 0xFFFFFFFFFFFFFFFFL;
        largeNum.a2 = 0xFFFFFFFFFFFFFFFFL;
        largeNum.a3 = 0xFFFFFFFFFFFFFFFFL;
        largeNum.a4 = 0xFFFFFFFFFFFFFFFFL;

        System.out.println("Large number: " + largeNum.toString());

        QuantumNumberV8 reduced = QuantumECCPointArithmetic.quantumModReduce(largeNum);
        System.out.println("Large number mod p: " + reduced.toString());

        // Verify result is less than prime
        QuantumNumberV8 prime = QuantumECCPointArithmetic.getPrime();
        assertTrue(QuantumECCPointArithmetic.quantumCompare(reduced, prime) < 0,
            "Reduced result should be less than prime");

        System.out.println("✓ Linked substrate arithmetic working correctly");
    }

    /**
     * Test 3: Demonstrate carry and overflow handling in linked substrates
     */
    @Test
    void testCarryAndOverflowHandling() {
        System.out.println("Test 3: Carry and Overflow Handling");

        // Create numbers that will cause overflow
        QuantumNumberV8 max64 = new QuantumNumberV8();
        max64.a4 = 0xFFFFFFFFFFFFFFFFL; // Maximum 64-bit value

        QuantumNumberV8 one = new QuantumNumberV8();
        one.a4 = 1L;

        System.out.println("Max 64-bit: " + max64.toString());
        System.out.println("One: " + one.toString());

        // Add them - this should cause carry
        QuantumNumberV8 sum = QuantumECCPointArithmetic.quantumAdd(max64, one);
        System.out.println("Max64 + 1: " + sum.toString());

        // Should have carry to next limb
        assertEquals(0L, sum.a4, "LSB should wrap to 0");
        assertEquals(1L, sum.a3, "Carry should propagate to next limb");

        // Test left shift with overflow
        QuantumNumberV8 shifted = QuantumECCPointArithmetic.quantumShiftLeft(max64, 1);
        System.out.println("Max64 << 1: " + shifted.toString());

        assertEquals(0xFFFFFFFFFFFFFF FEL, shifted.a4, "Shifted LSB incorrect");
        assertEquals(0x8000000000000000L, shifted.a3, "Shifted carry incorrect");

        // Verify overflow is stored in linked substrate
        assertNotNull(shifted.left, "Overflow should be stored in linked substrate");
        assertEquals(0x8000000000000000L, shifted.left.a3, "Linked substrate should contain overflow");

        System.out.println("✓ Carry and overflow handling working correctly");
    }

    /**
     * Test 4: Demonstrate ECC point operations using QuantumNumberV8 substrates
     */
    @Test
    void testECCPointOperations() {
        System.out.println("Test 4: ECC Point Operations");

        // Create generator point G
        QuantumECCPointArithmetic.QuantumECCPoint G =
            QuantumECCPointArithmetic.createGeneratorPoint();

        System.out.println("Generator point G:");
        System.out.println("  Gx = " + G.x.toString());
        System.out.println("  Gy = " + G.y.toString());

        // Verify G is on curve
        assertTrue(G.isOnCurve(), "Generator point G should be on curve");

        // Test point doubling: 2G
        QuantumECCPointArithmetic.QuantumECCPoint G2 =
            QuantumECCPointArithmetic.doublePoint(G);

        System.out.println("Point 2G:");
        System.out.println("  2Gx = " + G2.x.toString());
        System.out.println("  2Gy = " + G2.y.toString());

        // Verify 2G is on curve
        assertTrue(G2.isOnCurve(), "2G should be on curve");

        // Verify 2G ≠ G
        assertFalse(G.equals(G2), "2G should not equal G");

        // Test point addition: G + 2G = 3G
        QuantumECCPointArithmetic.QuantumECCPoint G3 =
            QuantumECCPointArithmetic.addPoints(G, G2);

        System.out.println("Point 3G = G + 2G:");
        System.out.println("  3Gx = " + G3.x.toString());
        System.out.println("  3Gy = " + G3.y.toString());

        // Verify 3G is on curve
        assertTrue(G3.isOnCurve(), "3G should be on curve");

        // Verify commutativity: G + 2G = 2G + G
        QuantumECCPointArithmetic.QuantumECCPoint G3_commute =
            QuantumECCPointArithmetic.addPoints(G2, G);
        assertTrue(G3.equals(G3_commute), "Point addition should be commutative");

        System.out.println("✓ ECC point operations working correctly");
    }

    /**
     * Test 5: Demonstrate scalar multiplication using QuantumNumberV8 substrates
     */
    @Test
    void testScalarMultiplication() {
        System.out.println("Test 5: Scalar Multiplication");

        QuantumECCPointArithmetic.QuantumECCPoint G =
            QuantumECCPointArithmetic.createGeneratorPoint();

        // Test 0 * G = infinity
        QuantumNumberV8 zero = new QuantumNumberV8();
        QuantumECCPointArithmetic.QuantumECCPoint zeroG =
            QuantumECCPointArithmetic.scalarMultiply(zero, G);
        assertTrue(zeroG.isInfinity, "0 * G should equal infinity");

        // Test 1 * G = G
        QuantumNumberV8 one = new QuantumNumberV8();
        one.a4 = 1L;
        QuantumECCPointArithmetic.QuantumECCPoint oneG =
            QuantumECCPointArithmetic.scalarMultiply(one, G);
        assertTrue(oneG.equals(G), "1 * G should equal G");

        // Test 2 * G = G + G
        QuantumNumberV8 two = new QuantumNumberV8();
        two.a4 = 2L;
        QuantumECCPointArithmetic.QuantumECCPoint twoG1 =
            QuantumECCPointArithmetic.scalarMultiply(two, G);
        QuantumECCPointArithmetic.QuantumECCPoint twoG2 =
            QuantumECCPointArithmetic.doublePoint(G);
        assertTrue(twoG1.equals(twoG2), "2 * G should equal G + G");

        // Test 3 * G = G + G + G
        QuantumNumberV8 three = new QuantumNumberV8();
        three.a4 = 3L;
        QuantumECCPointArithmetic.QuantumECCPoint threeG1 =
            QuantumECCPointArithmetic.scalarMultiply(three, G);
        QuantumECCPointArithmetic.QuantumECCPoint threeG2 =
            QuantumECCPointArithmetic.addPoints(
                QuantumECCPointArithmetic.doublePoint(G), G);
        assertTrue(threeG1.equals(threeG2), "3 * G should equal G + G + G");

        // Test 4 * G = 2 * (2 * G)
        QuantumNumberV8 four = new QuantumNumberV8();
        four.a4 = 4L;
        QuantumECCPointArithmetic.QuantumECCPoint fourG1 =
            QuantumECCPointArithmetic.scalarMultiply(four, G);
        QuantumECCPointArithmetic.QuantumECCPoint fourG2 =
            QuantumECCPointArithmetic.doublePoint(
                QuantumECCPointArithmetic.scalarMultiply(two, G));
        assertTrue(fourG1.equals(fourG2), "4 * G should equal 2 * (2 * G)");

        // Verify all results are on curve
        assertTrue(twoG1.isOnCurve(), "2G should be on curve");
        assertTrue(threeG1.isOnCurve(), "3G should be on curve");
        assertTrue(fourG1.isOnCurve(), "4G should be on curve");

        System.out.println("✓ Scalar multiplication working correctly");
    }

    /**
     * Test 6: Demonstrate point negation and infinity operations
     */
    @Test
    void testPointNegationAndInfinity() {
        System.out.println("Test 6: Point Negation and Infinity Operations");

        QuantumECCPointArithmetic.QuantumECCPoint G =
            QuantumECCPointArithmetic.createGeneratorPoint();

        System.out.println("G:");
        System.out.println("  Gx = " + G.x.toString());
        System.out.println("  Gy = " + G.y.toString());

        // Test point negation
        QuantumECCPointArithmetic.QuantumECCPoint negG = G.negate();
        System.out.println("-G:");
        System.out.println("  -Gx = " + negG.x.toString());
        System.out.println("  -Gy = " + negG.y.toString());

        // Verify -G is on curve
        assertTrue(negG.isOnCurve(), "-G should be on curve");

        // Verify G + (-G) = infinity
        QuantumECCPointArithmetic.QuantumECCPoint sum =
            QuantumECCPointArithmetic.addPoints(G, negG);
        assertTrue(sum.isInfinity, "G + (-G) should equal infinity");

        // Test infinity operations
        QuantumECCPointArithmetic.QuantumECCPoint infinity =
            new QuantumECCPointArithmetic.QuantumECCPoint();

        // Infinity + G = G
        QuantumECCPointArithmetic.QuantumECCPoint result1 =
            QuantumECCPointArithmetic.addPoints(infinity, G);
        assertTrue(result1.equals(G), "Infinity + G should equal G");

        // G + infinity = G
        QuantumECCPointArithmetic.QuantumECCPoint result2 =
            QuantumECCPointArithmetic.addPoints(G, infinity);
        assertTrue(result2.equals(G), "G + infinity should equal G");

        // Infinity + infinity = infinity
        QuantumECCPointArithmetic.QuantumECCPoint result3 =
            QuantumECCPointArithmetic.addPoints(infinity, infinity);
        assertTrue(result3.isInfinity, "Infinity + infinity should equal infinity");

        System.out.println("✓ Point negation and infinity operations working correctly");
    }

    /**
     * Test 7: Demonstrate hierarchical numeric structure
     * Shows how the system builds complex numeric trees
     */
    @Test
    void testHierarchicalNumericStructure() {
        System.out.println("Test 7: Hierarchical Numeric Structure");

        // Create base QuantumNumberV8 substrates
        QuantumNumberV8 base1 = new QuantumNumberV8();
        base1.a4 = 10L;

        QuantumNumberV8 base2 = new QuantumNumberV8();
        base2.a4 = 20L;

        // Build hierarchical structure through operations
        QuantumNumberV8 sum = QuantumECCPointArithmetic.quantumAdd(base1, base2);
        QuantumNumberV8 product = QuantumECCPointArithmetic.quantumMultiply(sum, base1);

        System.out.println("Base1: " + base1.toString());
        System.out.println("Base2: " + base2.toString());
        System.out.println("Sum: " + sum.toString());
        System.out.println("Product: " + product.toString());

        // Verify hierarchical relationships
        assertEquals(30L, sum.a4, "Sum should be 30");
        assertEquals(300L, product.a4, "Product should be 300");

        // Test with ECC points (which contain multiple QuantumNumberV8 substrates)
        QuantumECCPointArithmetic.QuantumECCPoint G =
            QuantumECCPointArithmetic.createGeneratorPoint();

        // Each point contains multiple linked substrates
        assertNotNull(G.x, "Point should have x coordinate substrate");
        assertNotNull(G.y, "Point should have y coordinate substrate");
        assertNotNull(G.lambda, "Point should have lambda substrate");
        assertNotNull(G.temp1, "Point should have temp1 substrate");
        assertNotNull(G.temp2, "Point should have temp2 substrate");
        assertNotNull(G.carryNode, "Point should have carry node substrate");

        // Verify all substrates are properly initialized
        assertTrue(QuantumECCPointArithmetic.quantumIsZero(G.lambda), "Lambda should be zero initially");
        assertTrue(QuantumECCPointArithmetic.quantumIsZero(G.temp1), "Temp1 should be zero initially");
        assertTrue(QuantumECCPointArithmetic.quantumIsZero(G.temp2), "Temp2 should be zero initially");
        assertTrue(QuantumECCPointArithmetic.quantumIsZero(G.carryNode), "Carry node should be zero initially");

        System.out.println("✓ Hierarchical numeric structure working correctly");
    }

    /**
     * Test 8: Demonstrate curve equation verification
     * Shows that all points satisfy y² = x³ + 7 mod p
     */
    @Test
    void testCurveEquationVerification() {
        System.out.println("Test 8: Curve Equation Verification");

        QuantumECCPointArithmetic.QuantumECCPoint G =
            QuantumECCPointArithmetic.createGeneratorPoint();

        // Test multiple points
        QuantumECCPointArithmetic.QuantumECCPoint G2 =
            QuantumECCPointArithmetic.doublePoint(G);
        QuantumECCPointArithmetic.QuantumECCPoint G3 =
            QuantumECCPointArithmetic.addPoints(G, G2);
        QuantumECCPointArithmetic.QuantumECCPoint G4 =
            QuantumECCPointArithmetic.doublePoint(G2);

        // All points should be on curve
        assertTrue(G.isOnCurve(), "G should satisfy curve equation");
        assertTrue(G2.isOnCurve(), "2G should satisfy curve equation");
        assertTrue(G3.isOnCurve(), "3G should satisfy curve equation");
        assertTrue(G4.isOnCurve(), "4G should satisfy curve equation");

        // Test with larger multiples
        QuantumNumberV8 scalar5 = new QuantumNumberV8();
        scalar5.a4 = 5L;
        QuantumECCPointArithmetic.QuantumECCPoint G5 =
            QuantumECCPointArithmetic.scalarMultiply(scalar5, G);
        assertTrue(G5.isOnCurve(), "5G should satisfy curve equation");

        QuantumNumberV8 scalar7 = new QuantumNumberV8();
        scalar7.a4 = 7L;
        QuantumECCPointArithmetic.QuantumECCPoint G7 =
            QuantumECCPointArithmetic.scalarMultiply(scalar7, G);
        assertTrue(G7.isOnCurve(), "7G should satisfy curve equation");

        System.out.println("✓ All points satisfy secp256k1 curve equation y² = x³ + 7 mod p");
    }

    /**
     * Test 9: Demonstrate bit manipulation operations on substrates
     */
    @Test
    void testBitManipulationOperations() {
        System.out.println("Test 9: Bit Manipulation Operations");

        // Create test value
        QuantumNumberV8 num = new QuantumNumberV8();
        num.a4 = 0x123456789ABCDEF0L;

        System.out.println("Original: " + num.toString());

        // Test left shift
        QuantumNumberV8 shiftedLeft = QuantumECCPointArithmetic.quantumShiftLeft(num, 8);
        System.out.println("Shifted left by 8: " + shiftedLeft.toString());

        // Should be 0x123456789ABCDEF000
        assertEquals(0x3456789ABCDEF000L, shiftedLeft.a4, "Left shift LSB incorrect");
        assertEquals(0x0000000000000012L, shiftedLeft.a3, "Left shift carry incorrect");

        // Test right shift
        QuantumNumberV8 shiftedRight = QuantumECCPointArithmetic.quantumShiftRight(num, 8);
        System.out.println("Shifted right by 8: " + shiftedRight.toString());

        // Should be 0x00123456789ABCDEF0
        assertEquals(0x00123456789ABCDEL, shiftedRight.a4, "Right shift LSB incorrect");

        // Test left shift by 64 bits (full limb)
        QuantumNumberV8 shifted64 = QuantumECCPointArithmetic.quantumShiftLeft(num, 64);
        System.out.println("Shifted left by 64: " + shifted64.toString());

        assertEquals(0L, shifted64.a4, "64-bit left shift LSB should be 0");
        assertEquals(0x123456789ABCDEF0L, shifted64.a3, "64-bit left shift should move to next limb");

        System.out.println("✓ Bit manipulation operations working correctly");
    }

    /**
     * Test 10: Demonstrate modular arithmetic with large numbers
     */
    @Test
    void testModularArithmeticWithLargeNumbers() {
        System.out.println("Test 10: Modular Arithmetic with Large Numbers");

        // Create very large numbers (256-bit)
        QuantumNumberV8 large1 = new QuantumNumberV8();
        large1.a1 = 0xFFFFFFFFFFFFFFFFL;
        large1.a2 = 0xFFFFFFFFFFFFFFFFL;
        large1.a3 = 0xFFFFFFFFFFFFFFFFL;
        large1.a4 = 0xFFFFFFFFFFFFFFFFL;

        QuantumNumberV8 large2 = new QuantumNumberV8();
        large2.a1 = 0x123456789ABCDEF0L;
        large2.a2 = 0xFEDCBA9876543210L;
        large2.a3 = 0x1111111111111111L;
        large2.a4 = 0x2222222222222222L;

        System.out.println("Large1: " + large1.toString());
        System.out.println("Large2: " + large2.toString());

        // Test modular addition
        QuantumNumberV8 sum = QuantumECCPointArithmetic.quantumAdd(large1, large2);
        sum = QuantumECCPointArithmetic.quantumModReduce(sum);
        System.out.println("Large1 + Large2 mod p: " + sum.toString());

        // Test modular multiplication
        QuantumNumberV8 product = QuantumECCPointArithmetic.quantumMultiply(large1, large2);
        product = QuantumECCPointArithmetic.quantumModReduce(product);
        System.out.println("Large1 * Large2 mod p: " + product.toString());

        // Verify results are within field
        QuantumNumberV8 prime = QuantumECCPointArithmetic.getPrime();
        assertTrue(QuantumECCPointArithmetic.quantumCompare(sum, prime) < 0,
            "Sum should be less than prime");
        assertTrue(QuantumECCPointArithmetic.quantumCompare(product, prime) < 0,
            "Product should be less than prime");

        // Test modular inverse of large number
        QuantumNumberV8 inv = QuantumECCPointArithmetic.quantumModInverse(large2);
        System.out.println("Large2^-1 mod p: " + inv.toString());

        // Verify: large2 * large2^-1 ≡ 1 mod p
        QuantumNumberV8 check = QuantumECCPointArithmetic.quantumMultiply(large2, inv);
        check = QuantumECCPointArithmetic.quantumModReduce(check);
        QuantumNumberV8 one = new QuantumNumberV8();
        one.a4 = 1L;

        assertTrue(QuantumECCPointArithmetic.quantumEquals(check, one),
            "Modular inverse check failed");

        System.out.println("✓ Modular arithmetic with large numbers working correctly");
    }

    // ============================================================================
    // Helper methods for testing
    // ============================================================================

    private static QuantumNumberV8 createTestNumber(long value) {
        QuantumNumberV8 num = new QuantumNumberV8();
        num.a4 = value;
        return num;
    }

    private static QuantumNumberV8 createTestNumber(long a4, long a3, long a2, long a1) {
        QuantumNumberV8 num = new QuantumNumberV8();
        num.a4 = a4;
        num.a3 = a3;
        num.a2 = a2;
        num.a1 = a1;
        return num;
    }
}
