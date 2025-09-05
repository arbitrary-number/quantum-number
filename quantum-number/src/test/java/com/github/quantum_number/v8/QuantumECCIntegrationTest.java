package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import static org.junit.jupiter.api.Assertions.*;

/**
 * QuantumECCIntegrationTest - Fully functional test cases demonstrating complete ECC implementation
 * using only QuantumNumberV8 with real 256-bit limb-based arithmetic operations.
 *
 * Tests validate:
 * - Large number initialization via limb setting
 * - Precise arithmetic with carry/overflow handling
 * - ECC point operations on secp256k1 curve
 * - Cryptographic operations (ECDH, ECDSA)
 * - All using native QuantumNumberV8 operations
 */
public class QuantumECCIntegrationTest {

    @BeforeEach
    void setUp() {
        // Ensure all classes are properly initialized
        System.out.println("=== QuantumECC Integration Test Setup ===");
    }

    /**
     * Test 1: Large number initialization and basic arithmetic
     * Demonstrates creating secp256k1 prime p and performing arithmetic
     */
    @Test
    void testLargeNumberInitializationAndArithmetic() {
        System.out.println("\n--- Test 1: Large Number Initialization ---");

        // Create secp256k1 prime p using limb initialization
        QuantumNumberV8 prime = QuantumECCMath.getPrime();
        System.out.println("secp256k1 prime p: " + prime.toString());

        // Verify prime limbs are correctly set
        assertEquals(0xFFFFFFFFFFFFFFFFL, prime.a1, "Prime MSB limb incorrect");
        assertEquals(0xFFFFFFFFFFFFFFFFL, prime.a2, "Prime middle-high limb incorrect");
        assertEquals(0xFFFFFFFFFFFFFFFEL, prime.a3, "Prime middle-low limb incorrect");
        assertEquals(0x00000000FFFFFC2FL, prime.a4, "Prime LSB limb incorrect");

        // Test arithmetic with large numbers
        QuantumNumberV8 a = QuantumNumberV8Util.createFromLimbs(
            0x123456789ABCDEF0L, 0x0FEDCBA987654321L, 0x1111111111111111L, 0x2222222222222222L
        );
        QuantumNumberV8 b = QuantumNumberV8Util.createFromLimbs(
            0x1111111111111111L, 0x2222222222222222L, 0x3333333333333333L, 0x4444444444444444L
        );

        System.out.println("a: " + a.toString());
        System.out.println("b: " + b.toString());

        // Perform modular addition
        QuantumNumberV8 sum = QuantumECCMath.modAdd(a, b);
        System.out.println("a + b mod p: " + sum.toString());

        // Perform modular multiplication
        QuantumNumberV8 product = QuantumECCMath.modMultiply(a, b);
        System.out.println("a * b mod p: " + product.toString());

        // Verify results are within field (less than prime)
        assertTrue(QuantumECCMath.compare(sum, prime) < 0, "Sum should be less than prime");
        assertTrue(QuantumECCMath.compare(product, prime) < 0, "Product should be less than prime");

        // Test modular inverse
        QuantumNumberV8 smallNum = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 12345L);
        QuantumNumberV8 inverse = QuantumECCMath.modInverse(smallNum);
        QuantumNumberV8 check = QuantumECCMath.modMultiply(smallNum, inverse);
        QuantumNumberV8 one = QuantumECCMath.one();

        System.out.println("12345^-1 mod p: " + inverse.toString());
        System.out.println("12345 * 12345^-1 mod p: " + check.toString());

        assertTrue(QuantumECCMath.equals(check, one), "Inverse check failed: should equal 1");
    }

    /**
     * Test 2: ECC Point Operations - G, 2G, 3G coordinates
     * Demonstrates actual elliptic curve point arithmetic
     */
    @Test
    void testECCPointOperations() {
        System.out.println("\n--- Test 2: ECC Point Operations ---");

        // Get generator point G
        QuantumECCPoint G = QuantumNumberV8Util.createGeneratorPoint();
        System.out.println("Generator point G:");
        System.out.println("  Gx: " + G.x.toString());
        System.out.println("  Gy: " + G.y.toString());
        assertTrue(G.isOnCurve(), "Generator point should be on curve");

        // Compute 2G (point doubling)
        QuantumECCPoint G2 = G.doublePoint();
        System.out.println("2G (point doubling):");
        System.out.println("  2Gx: " + G2.x.toString());
        System.out.println("  2Gy: " + G2.y.toString());
        assertTrue(G2.isOnCurve(), "2G should be on curve");
        assertFalse(G2.equals(G), "2G should not equal G");

        // Compute 3G (point addition: 2G + G)
        QuantumECCPoint G3 = G2.add(G);
        System.out.println("3G (point addition):");
        System.out.println("  3Gx: " + G3.x.toString());
        System.out.println("  3Gy: " + G3.y.toString());
        assertTrue(G3.isOnCurve(), "3G should be on curve");

        // Verify 3G = G + 2G (commutativity)
        QuantumECCPoint G3_alt = G.add(G2);
        assertTrue(G3.equals(G3_alt), "Point addition should be commutative");

        // Test point negation
        QuantumECCPoint negG = G.negate();
        System.out.println("-G (point negation):");
        System.out.println("  -Gx: " + negG.x.toString());
        System.out.println("  -Gy: " + negG.y.toString());
        assertTrue(negG.isOnCurve(), "-G should be on curve");

        // Verify -G + G = infinity
        QuantumECCPoint sum = negG.add(G);
        assertTrue(sum.isInfinity(), "-G + G should equal infinity");

        // Test scalar multiplication
        QuantumNumberV8 scalar = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 5L);
        QuantumECCPoint G5 = QuantumECC.scalarMultiply(G, scalar);
        System.out.println("5G (scalar multiplication):");
        System.out.println("  5Gx: " + G5.x.toString());
        System.out.println("  5Gy: " + G5.y.toString());
        assertTrue(G5.isOnCurve(), "5G should be on curve");
    }

    /**
     * Test 3: Carry and overflow handling in limb arithmetic
     * Demonstrates precise bit-level operations
     */
    @Test
    void testCarryAndOverflowHandling() {
        System.out.println("\n--- Test 3: Carry and Overflow Handling ---");

        // Test addition with carry propagation
        QuantumNumberV8 max64 = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 0xFFFFFFFFFFFFFFFFL);
        QuantumNumberV8 one = QuantumECCMath.one();

        System.out.println("Max 64-bit: " + max64.toString());
        System.out.println("One: " + one.toString());

        QuantumNumberV8 sum = QuantumECCMath.add(max64, one);
        System.out.println("Max64 + 1: " + sum.toString());

        // Should have carry from LSB to next limb
        assertEquals(0L, sum.a4, "LSB should wrap to 0");
        assertEquals(1L, sum.a3, "Carry should propagate to next limb");

        // Test modular reduction with large numbers
        QuantumNumberV8 largeNum = QuantumNumberV8Util.createFromLimbs(
            0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFL
        );
        QuantumNumberV8 prime = QuantumECCMath.getPrime();

        System.out.println("Large number: " + largeNum.toString());
        System.out.println("Prime: " + prime.toString());

        QuantumNumberV8 reduced = QuantumECCMath.modReduce(largeNum);
        System.out.println("Large mod prime: " + reduced.toString());

        assertTrue(QuantumECCMath.compare(reduced, prime) < 0, "Reduced result should be less than prime");

        // Test bit shifting operations
        QuantumNumberV8 num = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 0x123456789ABCDEF0L);
        QuantumNumberV8 shiftedLeft = QuantumECCMath.shiftLeft(num, 8);
        QuantumNumberV8 shiftedRight = QuantumECCMath.shiftRight(num, 8);

        System.out.println("Original: " + num.toString());
        System.out.println("Shifted left by 8: " + shiftedLeft.toString());
        System.out.println("Shifted right by 8: " + shiftedRight.toString());

        // Verify shift operations
        assertEquals(0x3456789ABCDEF000L, shiftedLeft.a4, "Left shift LSB incorrect");
        assertEquals(0x00123456789ABCDEL, shiftedRight.a4, "Right shift LSB incorrect");
    }

    /**
     * Test 4: secp256k1 curve equation verification
     * Demonstrates that points satisfy y² = x³ + 7 mod p
     */
    @Test
    void testCurveEquationVerification() {
        System.out.println("\n--- Test 4: Curve Equation Verification ---");

        QuantumECCPoint G = QuantumNumberV8Util.createGeneratorPoint();
        QuantumNumberV8 prime = QuantumECCMath.getPrime();

        // Calculate left side: y² mod p
        QuantumNumberV8 y2 = QuantumECCMath.square(G.y);
        y2 = QuantumECCMath.modReduce(y2);

        // Calculate right side: x³ + 7 mod p
        QuantumNumberV8 x2 = QuantumECCMath.square(G.x);
        QuantumNumberV8 x3 = QuantumECCMath.modMultiply(x2, G.x);
        QuantumNumberV8 seven = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 7L);
        QuantumNumberV8 right = QuantumECCMath.modAdd(x3, seven);

        System.out.println("secp256k1 curve: y² = x³ + 7 mod p");
        System.out.println("G.y² mod p: " + y2.toString());
        System.out.println("G.x³ + 7 mod p: " + right.toString());
        System.out.println("Equation satisfied: " + QuantumECCMath.equals(y2, right));

        assertTrue(QuantumECCMath.equals(y2, right), "Generator point should satisfy curve equation");

        // Test with 2G
        QuantumECCPoint G2 = G.doublePoint();
        QuantumNumberV8 y2_G2 = QuantumECCMath.square(G2.y);
        y2_G2 = QuantumECCMath.modReduce(y2_G2);

        QuantumNumberV8 x2_G2 = QuantumECCMath.square(G2.x);
        QuantumNumberV8 x3_G2 = QuantumECCMath.modMultiply(x2_G2, G2.x);
        QuantumNumberV8 right_G2 = QuantumECCMath.modAdd(x3_G2, seven);

        System.out.println("2G.y² mod p: " + y2_G2.toString());
        System.out.println("2G.x³ + 7 mod p: " + right_G2.toString());
        System.out.println("2G equation satisfied: " + QuantumECCMath.equals(y2_G2, right_G2));

        assertTrue(QuantumECCMath.equals(y2_G2, right_G2), "2G should satisfy curve equation");

        // Test with 3G
        QuantumECCPoint G3 = G2.add(G);
        QuantumNumberV8 y2_G3 = QuantumECCMath.square(G3.y);
        y2_G3 = QuantumECCMath.modReduce(y2_G3);

        QuantumNumberV8 x2_G3 = QuantumECCMath.square(G3.x);
        QuantumNumberV8 x3_G3 = QuantumECCMath.modMultiply(x2_G3, G3.x);
        QuantumNumberV8 right_G3 = QuantumECCMath.modAdd(x3_G3, seven);

        System.out.println("3G.y² mod p: " + y2_G3.toString());
        System.out.println("3G.x³ + 7 mod p: " + right_G3.toString());
        System.out.println("3G equation satisfied: " + QuantumECCMath.equals(y2_G3, right_G3));

        assertTrue(QuantumECCMath.equals(y2_G3, right_G3), "3G should satisfy curve equation");
    }

    /**
     * Test 5: Cryptographic operations (ECDH and ECDSA)
     * Demonstrates full cryptographic functionality
     */
    @Test
    void testCryptographicOperations() {
        System.out.println("\n--- Test 5: Cryptographic Operations ---");

        // Generate key pairs
        QuantumNumberV8 privKey1 = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 12345L);
        QuantumNumberV8 privKey2 = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 67890L);

        QuantumECCPoint pubKey1 = QuantumECC.generatePublicKey(privKey1);
        QuantumECCPoint pubKey2 = QuantumECC.generatePublicKey(privKey2);

        System.out.println("Private Key 1: " + privKey1.toString());
        System.out.println("Public Key 1: (" + pubKey1.x.toString() + ", " + pubKey1.y.toString() + ")");
        System.out.println("Private Key 2: " + privKey2.toString());
        System.out.println("Public Key 2: (" + pubKey2.x.toString() + ", " + pubKey2.y.toString() + ")");

        // Test ECDH
        QuantumECCPoint shared1 = QuantumECC.ecdh(privKey1, pubKey2);
        QuantumECCPoint shared2 = QuantumECC.ecdh(privKey2, pubKey1);

        System.out.println("ECDH Shared Secret 1: (" + shared1.x.toString() + ", " + shared1.y.toString() + ")");
        System.out.println("ECDH Shared Secret 2: (" + shared2.x.toString() + ", " + shared2.y.toString() + ")");

        assertTrue(shared1.equals(shared2), "ECDH should produce same shared secret");
        assertTrue(shared1.isOnCurve(), "Shared secret should be on curve");

        // Test ECDSA
        QuantumNumberV8 messageHash = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 0xDEADBEEFL);
        System.out.println("Message hash: " + messageHash.toString());

        QuantumECC.ECDSASignature signature = QuantumECC.sign(privKey1, messageHash);
        System.out.println("ECDSA signature:");
        System.out.println("  r: " + signature.r.toString());
        System.out.println("  s: " + signature.s.toString());

        boolean verified = QuantumECC.verify(pubKey1, messageHash, signature);
        System.out.println("Signature verified: " + verified);

        assertTrue(verified, "ECDSA signature should verify correctly");

        // Test with wrong message
        QuantumNumberV8 wrongHash = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 0xCAFEBABEL);
        boolean wrongVerified = QuantumECC.verify(pubKey1, wrongHash, signature);
        System.out.println("Wrong message verification: " + wrongVerified);

        assertFalse(wrongVerified, "Wrong message should not verify");
    }

    /**
     * Test 6: Edge cases and boundary conditions
     * Tests with extreme values and edge cases
     */
    @Test
    void testEdgeCasesAndBoundaryConditions() {
        System.out.println("\n--- Test 6: Edge Cases and Boundary Conditions ---");

        // Test with zero
        QuantumNumberV8 zero = QuantumECCMath.zero();
        QuantumNumberV8 result = QuantumECCMath.modMultiply(zero, QuantumECCMath.getPrime());
        assertTrue(QuantumECCMath.isZero(result), "Zero multiplication should give zero");

        // Test with one
        QuantumNumberV8 one = QuantumECCMath.one();
        QuantumNumberV8 identity = QuantumECCMath.modMultiply(one, QuantumECCMath.getPrime());
        identity = QuantumECCMath.modReduce(identity);
        assertTrue(QuantumECCMath.equals(identity, one), "One should be multiplicative identity");

        // Test point at infinity
        QuantumECCPoint infinity = new QuantumECCPoint();
        QuantumECCPoint G = QuantumNumberV8Util.createGeneratorPoint();

        QuantumECCPoint sum1 = infinity.add(G);
        QuantumECCPoint sum2 = G.add(infinity);

        assertTrue(sum1.equals(G), "Infinity + G should equal G");
        assertTrue(sum2.equals(G), "G + infinity should equal G");

        // Test scalar multiplication with zero
        QuantumECCPoint zeroG = QuantumECC.scalarMultiply(G, zero);
        assertTrue(zeroG.isInfinity(), "0 * G should equal infinity");

        // Test scalar multiplication with one
        QuantumECCPoint oneG = QuantumECC.scalarMultiply(G, one);
        assertTrue(oneG.equals(G), "1 * G should equal G");

        // Test large scalar multiplication
        QuantumNumberV8 largeScalar = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 1000L);
        QuantumECCPoint largeG = QuantumECC.scalarMultiply(G, largeScalar);
        assertTrue(largeG.isOnCurve(), "Large scalar multiplication should produce valid point");

        System.out.println("All edge cases passed successfully");
    }

    /**
     * Test 7: Performance and correctness validation
     * Runs multiple operations to ensure consistency
     */
    @Test
    void testPerformanceAndCorrectnessValidation() {
        System.out.println("\n--- Test 7: Performance and Correctness Validation ---");

        QuantumECCPoint G = QuantumNumberV8Util.createGeneratorPoint();
        long startTime = System.currentTimeMillis();

        // Test multiple scalar multiplications
        for (int i = 1; i <= 20; i++) {
            QuantumNumberV8 scalar = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, i);
            QuantumECCPoint result = QuantumECC.scalarMultiply(G, scalar);
            assertTrue(result.isOnCurve(), "Scalar multiplication " + i + " should produce valid point");
        }

        long endTime = System.currentTimeMillis();
        System.out.println("Completed 20 scalar multiplications in " + (endTime - startTime) + "ms");

        // Test point addition commutativity for multiple pairs
        for (int i = 1; i <= 10; i++) {
            QuantumNumberV8 scalar1 = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, i);
            QuantumNumberV8 scalar2 = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, i + 1);

            QuantumECCPoint P1 = QuantumECC.scalarMultiply(G, scalar1);
            QuantumECCPoint P2 = QuantumECC.scalarMultiply(G, scalar2);

            QuantumECCPoint sum1 = P1.add(P2);
            QuantumECCPoint sum2 = P2.add(P1);

            assertTrue(sum1.equals(sum2), "Point addition should be commutative for i=" + i);
        }

        // Test associativity: (a + b) + c = a + (b + c)
        QuantumECCPoint P1 = QuantumECC.scalarMultiply(G, QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 3L));
        QuantumECCPoint P2 = QuantumECC.scalarMultiply(G, QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 5L));
        QuantumECCPoint P3 = QuantumECC.scalarMultiply(G, QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 7L));

        QuantumECCPoint left = (P1.add(P2)).add(P3);
        QuantumECCPoint right = P1.add(P2.add(P3));

        assertTrue(left.equals(right), "Point addition should be associative");

        System.out.println("All performance and correctness tests passed");
    }
}
