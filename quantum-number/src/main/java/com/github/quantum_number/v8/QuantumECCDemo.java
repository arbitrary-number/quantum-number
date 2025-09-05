package com.github.quantum_number.v8;

/**
 * QuantumECCDemo - Demonstration of complete ECC implementation using QuantumNumberV8
 * Shows limb-based arithmetic and ECC operations without external libraries
 */
public class QuantumECCDemo {

    public static void main(String[] args) {
        System.out.println("=== QuantumECC Complete Implementation Demo ===\n");

        // Demonstrate limb-based arithmetic
        demonstrateLimbArithmetic();

        // Demonstrate ECC operations
        demonstrateECCOperations();

        // Demonstrate cryptographic operations
        demonstrateCryptoOperations();

        System.out.println("=== Demo Complete ===");
    }

    /**
     * Demonstrate limb-based arithmetic operations
     */
    public static void demonstrateLimbArithmetic() {
        System.out.println("1. Limb-Based Arithmetic Operations:");

        // Create numbers from limbs
        QuantumNumberV8 a = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 100L);
        QuantumNumberV8 b = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 50L);

        System.out.println("a = " + a.toString());
        System.out.println("b = " + b.toString());

        // Demonstrate modular arithmetic
        QuantumNumberV8 sum = QuantumECCMath.modAdd(a, b);
        QuantumNumberV8 product = QuantumECCMath.modMultiply(a, b);
        QuantumNumberV8 diff = QuantumECCMath.modSubtract(a, b);

        System.out.println("a + b mod p = " + sum.toString());
        System.out.println("a * b mod p = " + product.toString());
        System.out.println("a - b mod p = " + diff.toString());

        // Demonstrate modular inverse
        QuantumNumberV8 seven = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 7L);
        QuantumNumberV8 inv = QuantumECCMath.modInverse(seven);
        QuantumNumberV8 check = QuantumECCMath.modMultiply(seven, inv);

        System.out.println("7^-1 mod p = " + inv.toString());
        System.out.println("7 * 7^-1 mod p = " + check.toString() + " (should be 1)");

        System.out.println();
    }

    /**
     * Demonstrate ECC point operations
     */
    public static void demonstrateECCOperations() {
        System.out.println("2. ECC Point Operations:");

        // Get generator point
        QuantumECCPoint G = QuantumNumberV8Util.createGeneratorPoint();
        System.out.println("Generator point G:");
        System.out.println("Gx = " + G.x.toString());
        System.out.println("Gy = " + G.y.toString());
        System.out.println("G is on curve: " + G.isOnCurve());

        // Demonstrate point doubling
        QuantumECCPoint G2 = G.doublePoint();
        System.out.println("\n2G (point doubling):");
        System.out.println("2Gx = " + G2.x.toString());
        System.out.println("2Gy = " + G2.y.toString());
        System.out.println("2G is on curve: " + G2.isOnCurve());

        // Demonstrate point addition
        QuantumECCPoint G3 = G2.add(G);
        System.out.println("\n3G (point addition):");
        System.out.println("3Gx = " + G3.x.toString());
        System.out.println("3Gy = " + G3.y.toString());
        System.out.println("3G is on curve: " + G3.isOnCurve());

        // Demonstrate scalar multiplication
        QuantumNumberV8 scalar = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 10L);
        QuantumECCPoint G10 = QuantumECC.scalarMultiply(G, scalar);
        System.out.println("\n10G (scalar multiplication):");
        System.out.println("10Gx = " + G10.x.toString());
        System.out.println("10Gy = " + G10.y.toString());
        System.out.println("10G is on curve: " + G10.isOnCurve());

        // Demonstrate point negation
        QuantumECCPoint negG = G.negate();
        System.out.println("\n-G (point negation):");
        System.out.println("-Gx = " + negG.x.toString());
        System.out.println("-Gy = " + negG.y.toString());
        System.out.println("-G + G = infinity: " + negG.add(G).isInfinity());

        System.out.println();
    }

    /**
     * Demonstrate cryptographic operations
     */
    public static void demonstrateCryptoOperations() {
        System.out.println("3. Cryptographic Operations:");

        // Generate key pair
        QuantumNumberV8 privateKey = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 12345L);
        QuantumECCPoint publicKey = QuantumECC.generatePublicKey(privateKey);

        System.out.println("Private key: " + privateKey.toString());
        System.out.println("Public key:");
        System.out.println("  Px = " + publicKey.x.toString());
        System.out.println("  Py = " + publicKey.y.toString());
        System.out.println("Public key is valid: " + QuantumECC.isValidPublicKey(publicKey));

        // Demonstrate ECDH
        QuantumECCPoint sharedSecret = QuantumECC.ecdh(privateKey, publicKey);
        System.out.println("\nECDH shared secret:");
        System.out.println("  Sx = " + sharedSecret.x.toString());
        System.out.println("  Sy = " + sharedSecret.y.toString());
        System.out.println("Shared secret is on curve: " + sharedSecret.isOnCurve());

        // Demonstrate ECDSA
        QuantumNumberV8 messageHash = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 999999L);
        System.out.println("\nMessage hash: " + messageHash.toString());

        QuantumECC.ECDSASignature signature = QuantumECC.sign(privateKey, messageHash);
        System.out.println("ECDSA signature:");
        System.out.println("  r = " + signature.r.toString());
        System.out.println("  s = " + signature.s.toString());

        boolean verified = QuantumECC.verify(publicKey, messageHash, signature);
        System.out.println("Signature verification: " + verified);

        System.out.println();
    }

    /**
     * Demonstrate limb manipulation
     */
    public static void demonstrateLimbManipulation() {
        System.out.println("4. Limb Manipulation Operations:");

        QuantumNumberV8 num = QuantumNumberV8Util.createFromLimbs(
            0x123456789ABCDEF0L,
            0x0FEDCBA987654321L,
            0x1111111111111111L,
            0x2222222222222222L
        );

        System.out.println("Original number: " + num.toString());

        // Demonstrate shifting
        QuantumNumberV8 shiftedLeft = QuantumECCMath.shiftLeft(num, 8);
        System.out.println("Shifted left by 8 bits: " + shiftedLeft.toString());

        QuantumNumberV8 shiftedRight = QuantumECCMath.shiftRight(num, 8);
        System.out.println("Shifted right by 8 bits: " + shiftedRight.toString());

        // Demonstrate modular reduction
        QuantumNumberV8 prime = QuantumECCMath.getPrime();
        QuantumNumberV8 reduced = QuantumECCMath.modReduce(num);
        System.out.println("Reduced mod p: " + reduced.toString());
        System.out.println("Prime p: " + prime.toString());

        System.out.println();
    }

    /**
     * Demonstrate curve properties
     */
    public static void demonstrateCurveProperties() {
        System.out.println("5. Curve Properties:");

        QuantumECCPoint G = QuantumNumberV8Util.createGeneratorPoint();
        QuantumNumberV8 prime = QuantumECCMath.getPrime();

        System.out.println("secp256k1 curve parameters:");
        System.out.println("Prime p: " + prime.toString());
        System.out.println("Curve equation: y² = x³ + 7 mod p");

        // Verify generator satisfies curve equation
        QuantumNumberV8 y2 = QuantumECCMath.square(G.y);
        QuantumNumberV8 x2 = QuantumECCMath.square(G.x);
        QuantumNumberV8 x3 = QuantumECCMath.modMultiply(x2, G.x);
        QuantumNumberV8 seven = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 7L);
        QuantumNumberV8 right = QuantumECCMath.modAdd(x3, seven);

        System.out.println("G.y² = " + y2.toString());
        System.out.println("G.x³ + 7 = " + right.toString());
        System.out.println("Equation satisfied: " + QuantumECCMath.equals(y2, right));

        // Demonstrate order property
        QuantumECCPoint orderG = QuantumECC.scalarMultiply(G, QuantumECC.ORDER);
        System.out.println("G * order = infinity: " + orderG.isInfinity());

        System.out.println();
    }
}
