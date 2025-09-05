package com.github.quantum_number.v8;

/**
 * QuantumECC - Complete ECC implementation using QuantumNumberV8 limb-based arithmetic
 * Implements secp256k1 curve operations with full point arithmetic and scalar multiplication
 */
public class QuantumECC {

    // Generator point G for secp256k1
    public static final QuantumECCPoint G = QuantumNumberV8Util.createGeneratorPoint();

    // Curve order n: 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
    public static final QuantumNumberV8 ORDER = QuantumNumberV8Util.createFromLimbs(
        0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFEL, 0xBAAEDCE6AF48A03BL, 0xBFD25E8CD0364141L
    );

    /**
     * Generate public key from private key: Q = d * G
     */
    public static QuantumECCPoint generatePublicKey(QuantumNumberV8 privateKey) {
        // Convert private key to long for scalar multiplication
        // For full implementation, we'd need big integer scalar multiplication
        // For now, use a simplified approach with small scalars
        return scalarMultiply(G, privateKey);
    }

    /**
     * Scalar multiplication: k * P
     */
    public static QuantumECCPoint scalarMultiply(QuantumECCPoint P, QuantumNumberV8 scalar) {
        if (P.isInfinity()) return new QuantumECCPoint();

        QuantumECCPoint result = new QuantumECCPoint();
        QuantumECCPoint current = new QuantumECCPoint(P);

        // Simple binary ladder for demonstration
        // In practice, this should handle the full 256-bit scalar
        for (int i = 0; i < 256; i++) {
            if (getBit(scalar, i)) {
                result = result.add(current);
            }
            current = current.doublePoint();
        }

        return result;
    }

    /**
     * Point addition wrapper
     */
    public static QuantumECCPoint addPoints(QuantumECCPoint P, QuantumECCPoint Q) {
        return P.add(Q);
    }

    /**
     * Point doubling wrapper
     */
    public static QuantumECCPoint doublePoint(QuantumECCPoint P) {
        return P.doublePoint();
    }

    /**
     * Check if point is on curve
     */
    public static boolean isOnCurve(QuantumECCPoint P) {
        return P.isOnCurve();
    }

    /**
     * Validate private key (0 < d < n)
     */
    public static boolean isValidPrivateKey(QuantumNumberV8 privateKey) {
        if (QuantumECCMath.isZero(privateKey)) return false;
        if (QuantumECCMath.compare(privateKey, ORDER) >= 0) return false;
        return true;
    }

    /**
     * Validate public key
     */
    public static boolean isValidPublicKey(QuantumECCPoint publicKey) {
        if (publicKey.isInfinity()) return false;
        if (!publicKey.isOnCurve()) return false;

        // Check that Q is not the point at infinity and Q * n = infinity
        // (where n is the curve order)
        QuantumECCPoint check = scalarMultiply(publicKey, ORDER);
        return check.isInfinity();
    }

    /**
     * ECDH key agreement: shared = d * Q where Q is other party's public key
     */
    public static QuantumECCPoint ecdh(QuantumNumberV8 privateKey, QuantumECCPoint publicKey) {
        if (!isValidPrivateKey(privateKey) || !isValidPublicKey(publicKey)) {
            throw new IllegalArgumentException("Invalid keys for ECDH");
        }
        return scalarMultiply(publicKey, privateKey);
    }

    /**
     * ECDSA signature generation (simplified)
     */
    public static ECDSASignature sign(QuantumNumberV8 privateKey, QuantumNumberV8 messageHash) {
        if (!isValidPrivateKey(privateKey)) {
            throw new IllegalArgumentException("Invalid private key");
        }

        // Generate random k (in practice, use secure random)
        QuantumNumberV8 k = QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 12345L);

        // Calculate r = (k * G).x mod n
        QuantumECCPoint kG = scalarMultiply(G, k);
        QuantumNumberV8 r = QuantumECCMath.modReduce(kG.x);

        if (QuantumECCMath.isZero(r)) {
            throw new RuntimeException("r is zero, try different k");
        }

        // Calculate s = (message + r * privateKey) * k^(-1) mod n
        QuantumNumberV8 rPriv = QuantumECCMath.modMultiply(r, privateKey);
        QuantumNumberV8 messagePlusRPriv = QuantumECCMath.modAdd(messageHash, rPriv);
        QuantumNumberV8 kInv = QuantumECCMath.modInverse(k);
        QuantumNumberV8 s = QuantumECCMath.modMultiply(messagePlusRPriv, kInv);

        if (QuantumECCMath.isZero(s)) {
            throw new RuntimeException("s is zero, try different k");
        }

        return new ECDSASignature(r, s);
    }

    /**
     * ECDSA signature verification
     */
    public static boolean verify(QuantumECCPoint publicKey, QuantumNumberV8 messageHash, ECDSASignature signature) {
        if (!isValidPublicKey(publicKey)) return false;
        if (QuantumECCMath.isZero(signature.r) || QuantumECCMath.isZero(signature.s)) return false;
        if (QuantumECCMath.compare(signature.r, ORDER) >= 0) return false;
        if (QuantumECCMath.compare(signature.s, ORDER) >= 0) return false;

        // Calculate w = s^(-1) mod n
        QuantumNumberV8 w = QuantumECCMath.modInverse(signature.s);

        // Calculate u1 = message * w mod n
        QuantumNumberV8 u1 = QuantumECCMath.modMultiply(messageHash, w);

        // Calculate u2 = r * w mod n
        QuantumNumberV8 u2 = QuantumECCMath.modMultiply(signature.r, w);

        // Calculate (u1 * G) + (u2 * Q)
        QuantumECCPoint u1G = scalarMultiply(G, u1);
        QuantumECCPoint u2Q = scalarMultiply(publicKey, u2);
        QuantumECCPoint point = u1G.add(u2Q);

        if (point.isInfinity()) return false;

        // Check r ≡ x mod n
        QuantumNumberV8 x = QuantumECCMath.modReduce(point.x);
        return QuantumECCMath.equals(x, signature.r);
    }

    /**
     * Get a specific bit from a QuantumNumberV8
     */
    private static boolean getBit(QuantumNumberV8 a, int bit) {
        int limb = bit / 64;
        int bitInLimb = bit % 64;
        long limbValue = getLimb(a, limb);
        return (limbValue & (1L << bitInLimb)) != 0;
    }

    /**
     * Get a specific limb from a QuantumNumberV8
     */
    private static long getLimb(QuantumNumberV8 a, int limb) {
        switch (limb) {
            case 0: return a.a4; // LSB
            case 1: return a.a3;
            case 2: return a.a2;
            case 3: return a.a1; // MSB
            default: return 0;
        }
    }

    /**
     * ECDSA Signature container
     */
    public static class ECDSASignature {
        public final QuantumNumberV8 r;
        public final QuantumNumberV8 s;

        public ECDSASignature(QuantumNumberV8 r, QuantumNumberV8 s) {
            this.r = r;
            this.s = s;
        }

        @Override
        public String toString() {
            return String.format("ECDSA(r=%s, s=%s)", r.toString(), s.toString());
        }
    }

    /**
     * Demonstrate basic ECC operations
     */
    public static void demonstrateECC() {
        System.out.println("=== QuantumECC Demonstration ===");

        // Show generator point
        System.out.println("Generator point G:");
        System.out.println("Gx: " + G.x.toString());
        System.out.println("Gy: " + G.y.toString());
        System.out.println("G is on curve: " + G.isOnCurve());

        // Demonstrate point doubling
        QuantumECCPoint G2 = G.doublePoint();
        System.out.println("\n2G:");
        System.out.println("2Gx: " + G2.x.toString());
        System.out.println("2Gy: " + G2.y.toString());
        System.out.println("2G is on curve: " + G2.isOnCurve());

        // Demonstrate point addition
        QuantumECCPoint G3 = G2.add(G);
        System.out.println("\n3G:");
        System.out.println("3Gx: " + G3.x.toString());
        System.out.println("3Gy: " + G3.y.toString());
        System.out.println("3G is on curve: " + G3.isOnCurve());

        // Demonstrate scalar multiplication
        QuantumECCPoint G10 = scalarMultiply(G, QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 10L));
        System.out.println("\n10G:");
        System.out.println("10Gx: " + G10.x.toString());
        System.out.println("10Gy: " + G10.y.toString());
        System.out.println("10G is on curve: " + G10.isOnCurve());

        System.out.println("\n=== ECC Operations Complete ===");
    }
}
