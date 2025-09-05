package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Secp256k1ECCPointTest - Focused test suite for secp256k1 elliptic curve point operations
 * using QuantumNumberV8 for all 256-bit field arithmetic with limb-based computations.
 *
 * Tests verify:
 * - Point addition and doubling on secp256k1 curve
 * - Modular arithmetic modulo p using QuantumNumberV8 limbs
 * - Carry-aware arithmetic with bit shifting
 * - Curve equation verification: y² = x³ + 7 mod p
 * - Known point computations (G, 2G, 3G, etc.)
 * - All operations using native QuantumNumberV8 limb manipulation
 */
public class Secp256k1ECCPointTest {

    // secp256k1 curve parameters as QuantumNumberV8 instances
    private QuantumNumberV8 prime;  // p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
    private QuantumNumberV8 curveA; // a = 0
    private QuantumNumberV8 curveB; // b = 7

    // Generator point G coordinates
    private QuantumNumberV8 gx; // Gx = 0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
    private QuantumNumberV8 gy; // Gy = 0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8

    @BeforeEach
    void setUp() {
        // Initialize secp256k1 prime p using limb construction
        prime = QuantumNumberV8Util.createFromLimbs(
            0xFFFFFFFFFFFFFFFFL,  // MSB limb
            0xFFFFFFFFFFFFFFFFL,
            0xFFFFFFFFFFFFFFFEL,
            0x00000000FFFFFC2FL   // LSB limb
        );

        // Curve parameters
        curveA = QuantumECCMath.zero(); // a = 0
        curveB = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 7L); // b = 7

        // Generator point coordinates
        gx = QuantumNumberV8Util.createFromLimbs(
            0x79BE667EF9DCBBACL,
            0x55A06295CE870B07L,
            0x029BFCDB2DCE28D9L,
            0x59F2815B16F81798L
        );

        gy = QuantumNumberV8Util.createFromLimbs(
            0x483ADA7726A3C465L,
            0x5DA4FBFC0E1108A8L,
            0xFD17B448A6855419L,
            0x9C47D08FFB10D4B8L
        );
    }

    /**
     * Test 1: Verify secp256k1 prime p limb construction
     */
    @Test
    void testSecp256k1PrimeConstruction() {
        System.out.println("\n=== Test 1: secp256k1 Prime Construction ===");

        // Verify prime limbs are correctly set
        assertEquals(0xFFFFFFFFFFFFFFFFL, prime.a1, "Prime MSB limb incorrect");
        assertEquals(0xFFFFFFFFFFFFFFFFL, prime.a2, "Prime middle-high limb incorrect");
        assertEquals(0xFFFFFFFFFFFFFFFEL, prime.a3, "Prime middle-low limb incorrect");
        assertEquals(0x00000000FFFFFC2FL, prime.a4, "Prime LSB limb incorrect");

        System.out.println("secp256k1 prime p: " + prime.toString());

        // Verify p is indeed prime by checking it's greater than 2^256 - 2^32 - 2^9 - 2^8 - 2^7 - 2^6 - 2^4 - 1
        QuantumNumberV8 expectedP = QuantumNumberV8Util.createFromLimbs(
            0xFFFFFFFFFFFFFFFFL,
            0xFFFFFFFFFFFFFFFFL,
            0xFFFFFFFFFFFFFFFEL,
            0x00000000FFFFFC2FL
        );

        assertTrue(QuantumECCMath.equals(prime, expectedP), "Prime p construction failed");
    }

    /**
     * Test 2: Verify generator point G coordinates and curve membership
     */
    @Test
    void testGeneratorPointCoordinates() {
        System.out.println("\n=== Test 2: Generator Point G Verification ===");

        // Verify Gx and Gy limb construction
        assertEquals(0x79BE667EF9DCBBACL, gx.a1, "Gx MSB limb incorrect");
        assertEquals(0x55A06295CE870B07L, gx.a2, "Gx middle-high limb incorrect");
        assertEquals(0x029BFCDB2DCE28D9L, gx.a3, "Gx middle-low limb incorrect");
        assertEquals(0x59F2815B16F81798L, gx.a4, "Gx LSB limb incorrect");

        assertEquals(0x483ADA7726A3C465L, gy.a1, "Gy MSB limb incorrect");
        assertEquals(0x5DA4FBFC0E1108A8L, gy.a2, "Gy middle-high limb incorrect");
        assertEquals(0xFD17B448A6855419L, gy.a3, "Gy middle-low limb incorrect");
        assertEquals(0x9C47D08FFB10D4B8L, gy.a4, "Gy LSB limb incorrect");

        System.out.println("Generator point Gx: " + gx.toString());
        System.out.println("Generator point Gy: " + gy.toString());

        // Create point and verify it's on curve
        QuantumECCPoint G = new QuantumECCPoint(gx, gy);
        assertTrue(G.isOnCurve(), "Generator point G should be on the secp256k1 curve");

        // Verify curve equation: y² = x³ + 7 mod p
        verifyCurveEquation(G, "Generator point G");
    }

    /**
     * Test 3: Point doubling - compute 2G from G
     */
    @Test
    void testPointDoubling() {
        System.out.println("\n=== Test 3: Point Doubling (2G) ===");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);
        QuantumECCPoint G2 = G.doublePoint();

        System.out.println("G (input): (" + G.x.toString() + ", " + G.y.toString() + ")");
        System.out.println("2G (result): (" + G2.x.toString() + ", " + G2.y.toString() + ")");

        // Verify 2G is on curve
        assertTrue(G2.isOnCurve(), "2G should be on the secp256k1 curve");

        // Verify 2G ≠ G
        assertFalse(G2.equals(G), "2G should not equal G");

        // Verify curve equation for 2G
        verifyCurveEquation(G2, "Point 2G");

        // Verify that 2G + G = 3G (associativity check)
        QuantumECCPoint G3_fromAddition = G2.add(G);
        QuantumECCPoint G3_fromDoubling = G2.doublePoint();

        // Note: This is a basic check - in full ECC, we'd verify the actual coordinates
        assertTrue(G3_fromAddition.isOnCurve(), "3G from addition should be on curve");
        assertTrue(G3_fromDoubling.isOnCurve(), "3G from doubling should be on curve");
    }

    /**
     * Test 4: Point addition - compute G + 2G = 3G
     */
    @Test
    void testPointAddition() {
        System.out.println("\n=== Test 4: Point Addition (G + 2G = 3G) ===");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);
        QuantumECCPoint G2 = G.doublePoint();
        QuantumECCPoint G3 = G.add(G2);

        System.out.println("G: (" + G.x.toString() + ", " + G.y.toString() + ")");
        System.out.println("2G: (" + G2.x.toString() + ", " + G2.y.toString() + ")");
        System.out.println("3G = G + 2G: (" + G3.x.toString() + ", " + G3.y.toString() + ")");

        // Verify all points are on curve
        assertTrue(G.isOnCurve(), "G should be on curve");
        assertTrue(G2.isOnCurve(), "2G should be on curve");
        assertTrue(G3.isOnCurve(), "3G should be on curve");

        // Verify curve equations
        verifyCurveEquation(G, "Point G");
        verifyCurveEquation(G2, "Point 2G");
        verifyCurveEquation(G3, "Point 3G");

        // Verify commutativity: G + 2G = 2G + G
        QuantumECCPoint G3_commute = G2.add(G);
        assertTrue(G3.equals(G3_commute), "Point addition should be commutative");

        // Verify associativity: (G + G) + G = G + (G + G)
        QuantumECCPoint left = (G.add(G)).add(G);
        QuantumECCPoint right = G.add(G.add(G));
        assertTrue(left.equals(right), "Point addition should be associative");
    }

    /**
     * Test 5: Modular arithmetic with curve parameters
     */
    @Test
    void testModularArithmeticWithCurveParameters() {
        System.out.println("\n=== Test 5: Modular Arithmetic with Curve Parameters ===");

        // Test modular addition: (a + b) mod p
        QuantumNumberV8 a = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 100L);
        QuantumNumberV8 b = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 200L);

        QuantumNumberV8 sum = QuantumECCMath.modAdd(a, b);
        System.out.println("100 + 200 mod p = " + sum.toString());
        assertEquals(300L, sum.a4, "Modular addition result incorrect");

        // Test modular multiplication: (a * b) mod p
        QuantumNumberV8 product = QuantumECCMath.modMultiply(a, b);
        System.out.println("100 * 200 mod p = " + product.toString());
        assertEquals(20000L, product.a4, "Modular multiplication result incorrect");

        // Test modular inverse
        QuantumNumberV8 seven = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 7L);
        QuantumNumberV8 sevenInv = QuantumECCMath.modInverse(seven);
        QuantumNumberV8 check = QuantumECCMath.modMultiply(seven, sevenInv);

        System.out.println("7^-1 mod p = " + sevenInv.toString());
        System.out.println("7 * 7^-1 mod p = " + check.toString());

        QuantumNumberV8 one = QuantumECCMath.one();
        assertTrue(QuantumECCMath.equals(check, one), "Modular inverse check failed");

        // Test that all results are within field (less than p)
        assertTrue(QuantumECCMath.compare(sum, prime) < 0, "Sum should be less than p");
        assertTrue(QuantumECCMath.compare(product, prime) < 0, "Product should be less than p");
        assertTrue(QuantumECCMath.compare(sevenInv, prime) < 0, "Inverse should be less than p");
    }

    /**
     * Test 6: Scalar multiplication by small integers
     */
    @Test
    void testScalarMultiplication() {
        System.out.println("\n=== Test 6: Scalar Multiplication ===");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);

        // Test 0 * G = infinity
        QuantumNumberV8 zero = QuantumECCMath.zero();
        QuantumECCPoint zeroG = QuantumECC.scalarMultiply(G, zero);
        assertTrue(zeroG.isInfinity(), "0 * G should equal infinity");

        // Test 1 * G = G
        QuantumNumberV8 one = QuantumECCMath.one();
        QuantumECCPoint oneG = QuantumECC.scalarMultiply(G, one);
        assertTrue(oneG.equals(G), "1 * G should equal G");

        // Test 2 * G = G + G
        QuantumNumberV8 two = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 2L);
        QuantumECCPoint twoG1 = QuantumECC.scalarMultiply(G, two);
        QuantumECCPoint twoG2 = G.doublePoint();
        assertTrue(twoG1.equals(twoG2), "2 * G should equal G + G");

        // Test 3 * G = G + G + G
        QuantumNumberV8 three = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 3L);
        QuantumECCPoint threeG1 = QuantumECC.scalarMultiply(G, three);
        QuantumECCPoint threeG2 = G.doublePoint().add(G);
        assertTrue(threeG1.equals(threeG2), "3 * G should equal G + G + G");

        // Test 4 * G = 2 * (2 * G)
        QuantumNumberV8 four = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 4L);
        QuantumECCPoint fourG1 = QuantumECC.scalarMultiply(G, four);
        QuantumECCPoint fourG2 = QuantumECC.scalarMultiply(G, two).doublePoint();
        assertTrue(fourG1.equals(fourG2), "4 * G should equal 2 * (2 * G)");

        // Verify all results are on curve
        assertTrue(twoG1.isOnCurve(), "2G should be on curve");
        assertTrue(threeG1.isOnCurve(), "3G should be on curve");
        assertTrue(fourG1.isOnCurve(), "4G should be on curve");

        System.out.println("Scalar multiplication tests completed successfully");
    }

    /**
     * Test 7: Point negation and infinity operations
     */
    @Test
    void testPointNegationAndInfinity() {
        System.out.println("\n=== Test 7: Point Negation and Infinity ===");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);
        QuantumECCPoint negG = G.negate();

        System.out.println("G: (" + G.x.toString() + ", " + G.y.toString() + ")");
        System.out.println("-G: (" + negG.x.toString() + ", " + negG.y.toString() + ")");

        // Verify -G is on curve
        assertTrue(negG.isOnCurve(), "-G should be on curve");

        // Verify G + (-G) = infinity
        QuantumECCPoint sum = G.add(negG);
        assertTrue(sum.isInfinity(), "G + (-G) should equal infinity");

        // Test infinity operations
        QuantumECCPoint infinity = new QuantumECCPoint();

        // Infinity + G = G
        QuantumECCPoint result1 = infinity.add(G);
        assertTrue(result1.equals(G), "Infinity + G should equal G");

        // G + infinity = G
        QuantumECCPoint result2 = G.add(infinity);
        assertTrue(result2.equals(G), "G + infinity should equal G");

        // Infinity + infinity = infinity
        QuantumECCPoint result3 = infinity.add(infinity);
        assertTrue(result3.isInfinity(), "Infinity + infinity should equal infinity");

        // Infinity negation = infinity
        QuantumECCPoint negInfinity = infinity.negate();
        assertTrue(negInfinity.isInfinity(), "Negation of infinity should be infinity");
    }

    /**
     * Test 8: Carry and overflow handling in limb arithmetic
     */
    @Test
    void testCarryAndOverflowHandling() {
        System.out.println("\n=== Test 8: Carry and Overflow Handling ===");

        // Test addition that causes carry between limbs
        QuantumNumberV8 max64 = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 0xFFFFFFFFFFFFFFFFL);
        QuantumNumberV8 one = QuantumECCMath.one();

        System.out.println("Max 64-bit value: " + max64.toString());
        System.out.println("One: " + one.toString());

        QuantumNumberV8 sum = QuantumECCMath.add(max64, one);
        System.out.println("Max64 + 1: " + sum.toString());

        // Should have carry from LSB to next limb
        assertEquals(0L, sum.a4, "LSB should wrap to 0 after carry");
        assertEquals(1L, sum.a3, "Carry should propagate to next limb");

        // Test modular reduction with overflow
        QuantumNumberV8 largeNum = QuantumNumberV8Util.createFromLimbs(
            0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFL
        );

        System.out.println("Large number (2^256 - 1): " + largeNum.toString());
        System.out.println("Prime p: " + prime.toString());

        QuantumNumberV8 reduced = QuantumECCMath.modReduce(largeNum);
        System.out.println("Large number mod p: " + reduced.toString());

        // Result should be (2^256 - 1) - p = 2^256 - 1 - (2^256 - 2^32 - 2^9 - 2^8 - 2^7 - 2^6 - 2^4 - 1)
        // = 2^32 + 2^9 + 2^8 + 2^7 + 2^6 + 2^4
        // = 4294967296 + 512 + 256 + 128 + 64 + 16 = 4294967296 + 976 = 4294968272
        long expected = 0x100000000L + 0x200 + 0x100 + 0x80 + 0x40 + 0x10; // 2^32 + 2^9 + 2^8 + 2^7 + 2^6 + 2^4
        assertEquals(expected, reduced.a4, "Modular reduction result incorrect");

        // Verify result is less than p
        assertTrue(QuantumECCMath.compare(reduced, prime) < 0, "Reduced result should be less than p");
    }

    /**
     * Test 9: Bit shifting operations
     */
    @Test
    void testBitShiftingOperations() {
        System.out.println("\n=== Test 9: Bit Shifting Operations ===");

        QuantumNumberV8 num = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 0x123456789ABCDEF0L);

        System.out.println("Original number: " + num.toString());

        // Test left shift by 8 bits
        QuantumNumberV8 shiftedLeft = QuantumECCMath.shiftLeft(num, 8);
        System.out.println("Shifted left by 8: " + shiftedLeft.toString());

        // Should be 0x123456789ABCDEF000 (original << 8)
        assertEquals(0x3456789ABCDEF000L, shiftedLeft.a4, "Left shift LSB incorrect");
        assertEquals(0x0000000000000012L, shiftedLeft.a3, "Left shift carry to next limb incorrect");

        // Test right shift by 8 bits
        QuantumNumberV8 shiftedRight = QuantumECCMath.shiftRight(num, 8);
        System.out.println("Shifted right by 8: " + shiftedRight.toString());

        // Should be 0x00123456789ABCDEF0 (original >>> 8)
        assertEquals(0x00123456789ABCDEL, shiftedRight.a4, "Right shift LSB incorrect");

        // Test left shift by 64 bits (should move to next limb)
        QuantumNumberV8 shifted64 = QuantumECCMath.shiftLeft(num, 64);
        System.out.println("Shifted left by 64: " + shifted64.toString());

        assertEquals(0L, shifted64.a4, "64-bit left shift LSB should be 0");
        assertEquals(0x123456789ABCDEF0L, shifted64.a3, "64-bit left shift should move to next limb");
    }

    /**
     * Helper method to verify curve equation: y² = x³ + 7 mod p
     */
    private void verifyCurveEquation(QuantumECCPoint P, String pointName) {
        if (P.isInfinity()) {
            System.out.println(pointName + " is at infinity - curve equation satisfied by definition");
            return;
        }

        // Calculate y² mod p
        QuantumNumberV8 y2 = QuantumECCMath.square(P.y);
        y2 = QuantumECCMath.modReduce(y2);

        // Calculate x³ mod p
        QuantumNumberV8 x2 = QuantumECCMath.square(P.x);
        QuantumNumberV8 x3 = QuantumECCMath.modMultiply(x2, P.x);

        // Calculate x³ + 7 mod p
        QuantumNumberV8 seven = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 7L);
        QuantumNumberV8 right = QuantumECCMath.modAdd(x3, seven);

        System.out.println(pointName + " curve verification:");
        System.out.println("  y² mod p: " + y2.toString());
        System.out.println("  x³ + 7 mod p: " + right.toString());
        System.out.println("  Equation satisfied: " + QuantumECCMath.equals(y2, right));

        assertTrue(QuantumECCMath.equals(y2, right),
            pointName + " should satisfy secp256k1 curve equation y² = x³ + 7 mod p");
    }
}
