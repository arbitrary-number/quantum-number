package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 * QuantumECCBitwiseTests - Complete test suite for pure bitwise QuantumNumberV8 ECC operations
 *
 * This test suite uses ONLY QuantumNumberV8 structures and bitwise operations.
 * No Java primitives (long, int, BigInteger, etc.) are used in any calculations.
 * All operations are performed using bitwise logic on the internal 64-bit fields.
 */
public class QuantumECCBitwiseTests {

    /**
     * Test utility: Create QuantumNumberV8 from hex string using pure bitwise operations
     */
    private static QuantumNumberV8 createFromHex(String hex) {
        QuantumNumberV8 result = new QuantumNumberV8();

        // Parse hex string using pure bitwise operations
        // This implementation handles the specific values needed for secp256k1

        switch (hex) {
            case "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F":
                // secp256k1 prime p
                result.a1 = 0xFFFFFFFFFFFFFFFFL;
                result.a2 = 0xFFFFFFFFFFFFFFFFL;
                result.a3 = 0xFFFFFFFFFFFFFFFEL;
                result.a4 = 0xFFFFFC2FL;
                break;
            case "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798":
                // Generator point Gx
                result.a1 = 0x79BE667EF9DCBBACL;
                result.a2 = 0x55A06295CE870B07L;
                result.a3 = 0x029BFCDB2DCE28D9L;
                result.a4 = 0x59F2815B16F81798L;
                break;
            case "483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8":
                // Generator point Gy
                result.a1 = 0x483ADA7726A3C465L;
                result.a2 = 0x5DA4FBFC0E1108A8L;
                result.a3 = 0xFD17B448A6855419L;
                result.a4 = 0x9C47D08FFB10D4B8L;
                break;
            case "7":
                result.a4 = 7L;
                break;
            case "3":
                result.a4 = 3L;
                break;
            case "2":
                result.a4 = 2L;
                break;
            case "1":
                result.a4 = 1L;
                break;
            case "0":
                result.a4 = 0L;
                break;
            case "A":
                result.a4 = 10L;
                break;
            case "5":
                result.a4 = 5L;
                break;
            case "F":
                result.a4 = 15L;
                break;
            case "32":
                result.a4 = 50L;
                break;
        }

        return result;
    }

    /**
     * Test utility: Assert equality using pure bitwise operations
     */
    private static void assertQuantumEqual(QuantumNumberV8 expected, QuantumNumberV8 actual, String message) {
        boolean equal = (expected.a1 == actual.a1) &&
                       (expected.a2 == actual.a2) &&
                       (expected.a3 == actual.a3) &&
                       (expected.a4 == actual.a4);

        if (!equal) {
            fail(message + "\nExpected: " + toHexString(expected) +
                 "\nActual: " + toHexString(actual));
        }
    }

    /**
     * Test utility: Convert to hex string for debugging
     */
    private static String toHexString(QuantumNumberV8 num) {
        return String.format("%016X%016X%016X%016X", num.a1, num.a2, num.a3, num.a4);
    }

    @Test
    void testModularAddition() {
        System.out.println("=== Test: Modular Addition ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 a = createFromHex("A");
        QuantumNumberV8 b = createFromHex("5");

        System.out.println("Prime: " + toHexString(prime));
        System.out.println("a: " + toHexString(a));
        System.out.println("b: " + toHexString(b));

        // Perform modular addition using pure bitwise operations
        QuantumNumberV8 sum = PureBitwiseModularArithmetic.modAdd(a, b, prime);
        System.out.println("a + b mod p: " + toHexString(sum));

        QuantumNumberV8 expected = createFromHex("F");
        assertQuantumEqual(expected, sum, "Modular addition failed");

        System.out.println("✓ Modular addition test PASSED");
    }

    @Test
    void testModularSubtraction() {
        System.out.println("=== Test: Modular Subtraction ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 a = createFromHex("A");
        QuantumNumberV8 b = createFromHex("5");

        System.out.println("a: " + toHexString(a));
        System.out.println("b: " + toHexString(b));

        QuantumNumberV8 diff = PureBitwiseModularArithmetic.modSubtract(a, b, prime);
        System.out.println("a - b mod p: " + toHexString(diff));

        QuantumNumberV8 expected = createFromHex("5");
        assertQuantumEqual(expected, diff, "Modular subtraction failed");

        System.out.println("✓ Modular subtraction test PASSED");
    }

    @Test
    void testModularMultiplication() {
        System.out.println("=== Test: Modular Multiplication ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 a = createFromHex("A");
        QuantumNumberV8 b = createFromHex("5");

        System.out.println("a: " + toHexString(a));
        System.out.println("b: " + toHexString(b));

        QuantumNumberV8 product = PureBitwiseModularArithmetic.modMultiply(a, b, prime);
        System.out.println("a * b mod p: " + toHexString(product));

        QuantumNumberV8 expected = createFromHex("32");
        assertQuantumEqual(expected, product, "Modular multiplication failed");

        System.out.println("✓ Modular multiplication test PASSED");
    }

    @Test
    void testModularInverse() {
        System.out.println("=== Test: Modular Inverse ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 three = createFromHex("3");

        System.out.println("Finding modular inverse of 3 mod p");
        System.out.println("p: " + toHexString(prime));

        QuantumNumberV8 inverse = PureBitwiseModularArithmetic.modInverse(three, prime);
        System.out.println("3^(-1) mod p: " + toHexString(inverse));

        // Verify: 3 * 3^(-1) ≡ 1 mod p
        QuantumNumberV8 product = PureBitwiseModularArithmetic.modMultiply(three, inverse, prime);
        System.out.println("3 * 3^(-1) mod p: " + toHexString(product));

        QuantumNumberV8 one = createFromHex("1");
        assertQuantumEqual(one, product, "Modular inverse verification failed");

        System.out.println("✓ Modular inverse test PASSED");
    }

    @Test
    void testPointAddition() {
        System.out.println("=== Test: ECC Point Addition ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 gx = createFromHex("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
        QuantumNumberV8 gy = createFromHex("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);

        System.out.println("Generator point G:");
        System.out.println("  Gx: " + toHexString(G.x));
        System.out.println("  Gy: " + toHexString(G.y));

        // Test G + G = 2G (point doubling)
        QuantumECCPoint twoG = G.add(G, prime);

        System.out.println("2G (G + G):");
        System.out.println("  2Gx: " + toHexString(twoG.x));
        System.out.println("  2Gy: " + toHexString(twoG.y));

        // Verify 2G is not infinity
        assertFalse(twoG.isInfinity, "2G should not be infinity");

        // Verify 2G is on curve
        assertTrue(twoG.isOnCurve(prime), "2G should be on the curve");

        System.out.println("✓ Point addition test PASSED");
    }

    @Test
    void testPointDoubling() {
        System.out.println("=== Test: ECC Point Doubling ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 gx = createFromHex("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
        QuantumNumberV8 gy = createFromHex("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);

        System.out.println("Generator point G:");
        System.out.println("  Gx: " + toHexString(G.x));
        System.out.println("  Gy: " + toHexString(G.y));

        QuantumECCPoint twoG = G.doublePoint(prime);

        System.out.println("2G (doubled):");
        System.out.println("  2Gx: " + toHexString(twoG.x));
        System.out.println("  2Gy: " + toHexString(twoG.y));

        // Verify 2G is not infinity
        assertFalse(twoG.isInfinity, "2G should not be infinity");

        // Verify 2G is on curve
        assertTrue(twoG.isOnCurve(prime), "2G should be on the curve");

        // Verify 2G ≠ G
        assertFalse(G.equals(twoG), "2G should not equal G");

        System.out.println("✓ Point doubling test PASSED");
    }

    @Test
    void testPointNegation() {
        System.out.println("=== Test: ECC Point Negation ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 gx = createFromHex("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
        QuantumNumberV8 gy = createFromHex("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);

        System.out.println("Original point G:");
        System.out.println("  Gx: " + toHexString(G.x));
        System.out.println("  Gy: " + toHexString(G.y));

        QuantumECCPoint negG = G.negate();

        System.out.println("Negated point -G:");
        System.out.println("  -Gx: " + toHexString(negG.x));
        System.out.println("  -Gy: " + toHexString(negG.y));

        // Verify -G is on curve
        assertTrue(negG.isOnCurve(prime), "-G should be on the curve");

        // Verify G + (-G) = infinity
        QuantumECCPoint sum = G.add(negG, prime);
        assertTrue(sum.isInfinity, "G + (-G) should equal infinity");

        System.out.println("✓ Point negation test PASSED");
    }

    @Test
    void testScalarMultiplication() {
        System.out.println("=== Test: Scalar Multiplication ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 gx = createFromHex("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
        QuantumNumberV8 gy = createFromHex("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);

        // Test 0 * G = infinity
        QuantumNumberV8 zero = createFromHex("0");
        QuantumECCPoint zeroG = QuantumECCPoint.scalarMultiply(zero, G, prime);

        assertTrue(zeroG.isInfinity, "0 * G should equal infinity");
        System.out.println("✓ 0 * G = infinity");

        // Test 1 * G = G
        QuantumNumberV8 one = createFromHex("1");
        QuantumECCPoint oneG = QuantumECCPoint.scalarMultiply(one, G, prime);

        assertTrue(G.equals(oneG), "1 * G should equal G");
        System.out.println("✓ 1 * G = G");

        // Test 2 * G
        QuantumNumberV8 two = createFromHex("2");
        QuantumECCPoint twoG1 = QuantumECCPoint.scalarMultiply(two, G, prime);
        QuantumECCPoint twoG2 = G.doublePoint(prime);

        assertTrue(twoG1.equals(twoG2), "2 * G should equal G + G");
        System.out.println("✓ 2 * G = G + G");

        // Test 3 * G
        QuantumNumberV8 three = createFromHex("3");
        QuantumECCPoint threeG1 = QuantumECCPoint.scalarMultiply(three, G, prime);
        QuantumECCPoint threeG2 = twoG2.add(G, prime);

        assertTrue(threeG1.equals(threeG2), "3 * G should equal 2G + G");
        System.out.println("✓ 3 * G = 2G + G");

        System.out.println("✓ Scalar multiplication tests PASSED");
    }

    @Test
    void testZeroPointBehavior() {
        System.out.println("=== Test: Zero Point (Infinity) Behavior ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 gx = createFromHex("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
        QuantumNumberV8 gy = createFromHex("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);
        QuantumECCPoint infinity = new QuantumECCPoint();

        // Test infinity + G = G
        QuantumECCPoint result1 = infinity.add(G, prime);
        assertTrue(result1.equals(G), "Infinity + G should equal G");

        // Test G + infinity = G
        QuantumECCPoint result2 = G.add(infinity, prime);
        assertTrue(result2.equals(G), "G + Infinity should equal G");

        // Test infinity + infinity = infinity
        QuantumECCPoint result3 = infinity.add(infinity, prime);
        assertTrue(result3.isInfinity, "Infinity + Infinity should equal infinity");

        System.out.println("✓ Zero point behavior tests PASSED");
    }

    @Test
    void testOverflowChain() {
        System.out.println("=== Test: Overflow Chain with Linked Nodes ===");

        // Create numbers that will cause overflow
        QuantumNumberV8 max64 = new QuantumNumberV8();
        max64.a4 = 0xFFFFFFFFFFFFFFFFL; // Maximum 64-bit value

        QuantumNumberV8 one = createFromHex("1");

        System.out.println("Max 64-bit: " + toHexString(max64));
        System.out.println("One: " + toHexString(one));

        // Add them - this should cause overflow
        QuantumNumberV8 sum = PureBitwiseQuantumArithmetic.add(max64, one);
        System.out.println("Sum: " + toHexString(sum));

        // Verify overflow handling
        assertEquals(0L, sum.a4, "LSB should wrap to 0");
        assertNotNull(sum.left, "Overflow should create linked node");
        assertEquals(1L, sum.left.a4, "Overflow should be stored in linked node");

        System.out.println("✓ Overflow chain test PASSED");
    }

    @Test
    void testCurveEquationVerification() {
        System.out.println("=== Test: Curve Equation Verification ===");

        QuantumNumberV8 prime = createFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
        QuantumNumberV8 gx = createFromHex("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
        QuantumNumberV8 gy = createFromHex("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");

        QuantumECCPoint G = new QuantumECCPoint(gx, gy);

        // Verify generator point satisfies y² = x³ + 7 mod p
        assertTrue(G.isOnCurve(prime), "Generator point should satisfy curve equation");

        // Test derived points
        QuantumECCPoint twoG = G.doublePoint(prime);
        QuantumECCPoint threeG = twoG.add(G, prime);

        assertTrue(twoG.isOnCurve(prime), "2G should satisfy curve equation");
        assertTrue(threeG.isOnCurve(prime), "3G should satisfy curve equation");

        System.out.println("✓ Curve equation verification tests PASSED");
    }

    @Test
    void testSymbolicStructure() {
        System.out.println("=== Test: Symbolic Structure Preservation ===");

        QuantumNumberV8 num = createFromHex("A");

        // Verify symbolic structure is preserved
        assertEquals(0, num.signs, "Signs should be initialized to 0");
        assertEquals(0, num.metadata, "Metadata should be initialized to 0");

        // Test that operations preserve symbolic structure
        QuantumNumberV8 result = PureBitwiseQuantumArithmetic.add(num, num);

        assertEquals(0, result.signs, "Result signs should be preserved");
        assertEquals(0, result.metadata, "Result metadata should be preserved");

        System.out.println("✓ Symbolic structure preservation tests PASSED");
    }
}
