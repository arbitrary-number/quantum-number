package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Comprehensive test suite for QuantumECC implementation
 * Tests limb-based arithmetic and ECC operations using only QuantumNumberV8
 */
public class QuantumECCTest {

    private QuantumNumberV8 prime;
    private QuantumECCPoint G;

    @BeforeEach
    void setUp() {
        prime = QuantumECCMath.getPrime();
        G = QuantumNumberV8Util.createGeneratorPoint();
    }

    /**
     * Test QuantumNumberV8 limb construction
     */
    @Test
    void testLimbConstruction() {
        // Test creating numbers from limbs
        QuantumNumberV8 num = QuantumNumberV8Util.createFromLimbs(0x123456789ABCDEF0L, 0x0FEDCBA987654321L, 0x1111111111111111L, 0x2222222222222222L);

        assertEquals(0x123456789ABCDEF0L, num.a1);
        assertEquals(0x0FEDCBA987654321L, num.a2);
        assertEquals(0x1111111111111111L, num.a3);
        assertEquals(0x2222222222222222L, num.a4);
    }

    /**
     * Test basic arithmetic operations
     */
    @Test
    void testBasicArithmetic() {
        QuantumNumberV8 a = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 10L);
        QuantumNumberV8 b = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 5L);

        // Test addition
        QuantumNumberV8 sum = QuantumECCMath.modAdd(a, b);
        assertEquals(15L, sum.a4);

        // Test multiplication
        QuantumNumberV8 product = QuantumECCMath.modMultiply(a, b);
        assertEquals(50L, product.a4);

        // Test subtraction
        QuantumNumberV8 diff = QuantumECCMath.modSubtract(a, b);
        assertEquals(5L, diff.a4);
    }

    /**
     * Test modular arithmetic properties
     */
    @Test
    void testModularArithmetic() {
        QuantumNumberV8 a = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 100L);
        QuantumNumberV8 b = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 50L);

        // Test (a + b) mod p = ((a mod p) + (b mod p)) mod p
        QuantumNumberV8 aMod = QuantumECCMath.modReduce(a);
        QuantumNumberV8 bMod = QuantumECCMath.modReduce(b);
        QuantumNumberV8 sum1 = QuantumECCMath.modAdd(aMod, bMod);
        QuantumNumberV8 sum2 = QuantumECCMath.modAdd(a, b);
        assertTrue(QuantumECCMath.equals(sum1, sum2));

        // Test (a * b) mod p = ((a mod p) * (b mod p)) mod p
        QuantumNumberV8 prod1 = QuantumECCMath.modMultiply(aMod, bMod);
        QuantumNumberV8 prod2 = QuantumECCMath.modMultiply(a, b);
        assertTrue(QuantumECCMath.equals(prod1, prod2));
    }

    /**
     * Test modular inverse
     */
    @Test
    void testModularInverse() {
        QuantumNumberV8 a = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 7L);
        QuantumNumberV8 inv = QuantumECCMath.modInverse(a);

        // Test a * inv ≡ 1 mod p
        QuantumNumberV8 product = QuantumECCMath.modMultiply(a, inv);
        QuantumNumberV8 one = QuantumECCMath.one();
        assertTrue(QuantumECCMath.equals(product, one));
    }

    /**
     * Test generator point properties
     */
    @Test
    void testGeneratorPoint() {
        // Generator should be on curve
        assertTrue(G.isOnCurve());

        // Generator should not be infinity
        assertFalse(G.isInfinity());

        // Coordinates should be within field
        assertTrue(QuantumECCMath.compare(G.x, QuantumECCMath.zero()) > 0);
        assertTrue(QuantumECCMath.compare(G.y, QuantumECCMath.zero()) > 0);
        assertTrue(QuantumECCMath.compare(G.x, prime) < 0);
        assertTrue(QuantumECCMath.compare(G.y, prime) < 0);
    }

    /**
     * Test point doubling
     */
    @Test
    void testPointDoubling() {
        QuantumECCPoint G2 = G.doublePoint();

        // 2G should be on curve
        assertTrue(G2.isOnCurve());

        // 2G should not be infinity
        assertFalse(G2.isInfinity());

        // 2G should not equal G
        assertFalse(G2.equals(G));

        // Verify 2G = G + G
        QuantumECCPoint G_plus_G = G.add(G);
        assertTrue(G2.equals(G_plus_G));
    }

    /**
     * Test point addition
     */
    @Test
    void testPointAddition() {
        QuantumECCPoint G2 = G.doublePoint();
        QuantumECCPoint G3 = G2.add(G);

        // 3G should be on curve
        assertTrue(G3.isOnCurve());

        // 3G should not be infinity
        assertFalse(G3.isInfinity());

        // Verify 3G = 2G + G
        assertTrue(G3.equals(G2.add(G)));
    }

    /**
     * Test scalar multiplication
     */
    @Test
    void testScalarMultiplication() {
        // Test 0*G = infinity
        QuantumNumberV8 zero = QuantumECCMath.zero();
        QuantumECCPoint zeroG = QuantumECC.scalarMultiply(G, zero);
        assertTrue(zeroG.isInfinity());

        // Test 1*G = G
        QuantumNumberV8 one = QuantumECCMath.one();
        QuantumECCPoint oneG = QuantumECC.scalarMultiply(G, one);
        assertTrue(oneG.equals(G));

        // Test 2*G
        QuantumNumberV8 two = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 2L);
        QuantumECCPoint twoG1 = QuantumECC.scalarMultiply(G, two);
        QuantumECCPoint twoG2 = G.doublePoint();
        assertTrue(twoG1.equals(twoG2));

        // Test 3*G
        QuantumNumberV8 three = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 3L);
        QuantumECCPoint threeG1 = QuantumECC.scalarMultiply(G, three);
        QuantumECCPoint threeG2 = G.doublePoint().add(G);
        assertTrue(threeG1.equals(threeG2));
    }

    /**
     * Test point negation
     */
    @Test
    void testPointNegation() {
        QuantumECCPoint negG = G.negate();

        // -G should be on curve
        assertTrue(negG.isOnCurve());

        // -G + G should be infinity
        QuantumECCPoint sum = negG.add(G);
        assertTrue(sum.isInfinity());
    }

    /**
     * Test commutative property of point addition
     */
    @Test
    void testPointAdditionCommutative() {
        QuantumECCPoint G2 = G.doublePoint();
        QuantumECCPoint G3 = G2.add(G);
        QuantumECCPoint G3_commute = G.add(G2);

        assertTrue(G3.equals(G3_commute));
    }

    /**
     * Test associative property of scalar multiplication
     */
    @Test
    void testScalarMultiplicationAssociative() {
        // Test (2*G) + G = 3*G
        QuantumNumberV8 two = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 2L);
        QuantumNumberV8 three = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 3L);

        QuantumECCPoint twoG = QuantumECC.scalarMultiply(G, two);
        QuantumECCPoint threeG1 = twoG.add(G);
        QuantumECCPoint threeG2 = QuantumECC.scalarMultiply(G, three);

        assertTrue(threeG1.equals(threeG2));
    }

    /**
     * Test point at infinity operations
     */
    @Test
    void testInfinityOperations() {
        QuantumECCPoint infinity = new QuantumECCPoint();

        // Infinity + G = G
        QuantumECCPoint result1 = infinity.add(G);
        assertTrue(result1.equals(G));

        // G + infinity = G
        QuantumECCPoint result2 = G.add(infinity);
        assertTrue(result2.equals(G));

        // Infinity + infinity = infinity
        QuantumECCPoint result3 = infinity.add(infinity);
        assertTrue(result3.isInfinity());

        // 0 * G = infinity
        QuantumNumberV8 zero = QuantumECCMath.zero();
        QuantumECCPoint result4 = QuantumECC.scalarMultiply(G, zero);
        assertTrue(result4.isInfinity());
    }

    /**
     * Test curve equation verification
     */
    @Test
    void testCurveEquation() {
        // Verify that generator point satisfies y² = x³ + 7 mod p
        QuantumNumberV8 left = QuantumECCMath.square(G.y);
        QuantumNumberV8 x2 = QuantumECCMath.square(G.x);
        QuantumNumberV8 x3 = QuantumECCMath.modMultiply(x2, G.x);
        QuantumNumberV8 seven = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 7L);
        QuantumNumberV8 right = QuantumECCMath.modAdd(x3, seven);

        assertTrue(QuantumECCMath.equals(left, right));
    }

    /**
     * Test ECC key operations
     */
    @Test
    void testECCKeyOperations() {
        // Generate a private key
        QuantumNumberV8 privateKey = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 12345L);

        // Generate public key
        QuantumECCPoint publicKey = QuantumECC.generatePublicKey(privateKey);

        // Verify public key is valid
        assertTrue(QuantumECC.isValidPublicKey(publicKey));

        // Test ECDH
        QuantumECCPoint shared1 = QuantumECC.ecdh(privateKey, publicKey);
        assertFalse(shared1.isInfinity());
        assertTrue(shared1.isOnCurve());
    }

    /**
     * Test ECDSA operations
     */
    @Test
    void testECDSAOperations() {
        // Generate key pair
        QuantumNumberV8 privateKey = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 54321L);
        QuantumECCPoint publicKey = QuantumECC.generatePublicKey(privateKey);

        // Create a message hash
        QuantumNumberV8 messageHash = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 999999L);

        // Sign the message
        QuantumECC.ECDSASignature signature = QuantumECC.sign(privateKey, messageHash);

        // Verify the signature
        boolean verified = QuantumECC.verify(publicKey, messageHash, signature);
        assertTrue(verified);
    }

    /**
     * Test limb-based arithmetic edge cases
     */
    @Test
    void testLimbArithmeticEdgeCases() {
        // Test with numbers near limb boundaries
        QuantumNumberV8 large1 = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 0xFFFFFFFFFFFFFFFFL);
        QuantumNumberV8 large2 = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 1L);

        // Test addition that causes carry
        QuantumNumberV8 sum = QuantumECCMath.modAdd(large1, large2);
        assertEquals(0L, sum.a4); // Should wrap to 0

        // Test multiplication that causes overflow
        QuantumNumberV8 product = QuantumECCMath.modMultiply(large1, large2);
        assertTrue(QuantumECCMath.compare(product, prime) < 0); // Should be reduced
    }

    /**
     * Test point equality and hashing
     */
    @Test
    void testPointEquality() {
        QuantumECCPoint G_copy = new QuantumECCPoint(G);
        assertTrue(G.equals(G_copy));
        assertEquals(G.hashCode(), G_copy.hashCode());

        QuantumECCPoint G2 = G.doublePoint();
        assertFalse(G.equals(G2));
        assertNotEquals(G.hashCode(), G2.hashCode());

        QuantumECCPoint infinity = new QuantumECCPoint();
        assertFalse(G.equals(infinity));
    }

    /**
     * Test ECC validation methods
     */
    @Test
    void testECCValidation() {
        // Test private key validation
        QuantumNumberV8 validKey = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 12345L);
        assertTrue(QuantumECC.isValidPrivateKey(validKey));

        QuantumNumberV8 zeroKey = QuantumECCMath.zero();
        assertFalse(QuantumECC.isValidPrivateKey(zeroKey));

        // Test public key validation
        assertTrue(QuantumECC.isValidPublicKey(G));

        QuantumECCPoint infinity = new QuantumECCPoint();
        assertFalse(QuantumECC.isValidPublicKey(infinity));
    }

    /**
     * Test comprehensive ECC operations
     */
    @Test
    void testComprehensiveECCOperations() {
        // Test multiple scalar multiplications
        for (long k = 1; k <= 10; k++) {
            QuantumNumberV8 scalar = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, k);
            QuantumECCPoint result = QuantumECC.scalarMultiply(G, scalar);
            assertTrue(result.isOnCurve());
            assertFalse(result.isInfinity());
        }

        // Test point addition commutativity for multiple points
        QuantumECCPoint G2 = G.doublePoint();
        QuantumECCPoint G3 = G2.add(G);
        QuantumECCPoint G3_alt = G.add(G2);
        assertTrue(G3.equals(G3_alt));

        // Test that G * order = infinity (where order is curve order)
        QuantumECCPoint orderG = QuantumECC.scalarMultiply(G, QuantumECC.ORDER);
        assertTrue(orderG.isInfinity());
    }

    /**
     * Test limb manipulation operations
     */
    @Test
    void testLimbManipulation() {
        QuantumNumberV8 num = QuantumNumberV8Util.createFromLimbs(
            0x123456789ABCDEF0L,
            0x0FEDCBA987654321L,
            0x1111111111111111L,
            0x2222222222222222L
        );

        // Test shift left
        QuantumNumberV8 shifted = QuantumECCMath.shiftLeft(num, 4);
        assertEquals(0x23456789ABCDEF00L, shifted.a4);
        assertEquals(0x1111111111111111L, shifted.a3);

        // Test shift right
        QuantumNumberV8 shiftedRight = QuantumECCMath.shiftRight(num, 4);
        assertEquals(0x0222222222222222L, shiftedRight.a4);
        assertEquals(0x10FEDCBA98765432L, shiftedRight.a3);
    }
}
