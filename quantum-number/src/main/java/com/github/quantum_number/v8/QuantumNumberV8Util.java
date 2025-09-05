package com.github.quantum_number.v8;

/**
 * QuantumNumberV8Util - Utility class for QuantumNumberV8 operations
 * Provides initialization and utility functions for ECC operations
 */
public class QuantumNumberV8Util {

    /**
     * Initialize a QuantumNumberV8 instance representing the secp256k1 prime p in 'a'
     * and all other components (b, c, d, e, f) set to 1 (256-bit unity).
     */
    public static QuantumNumberV8 createSecp256k1PrimePWithUnits() {
        QuantumNumberV8 q = new QuantumNumberV8();

        // Assign p to 'a' component
        q.a1 = 0xFFFFFFFFFFFFFFFFL; // MSB limb
        q.a2 = 0xFFFFFFFFFFFFFFFFL;
        q.a3 = 0xFFFFFFFFFFFFFFFEL;
        q.a4 = 0x00000000FFFFFC2FL; // LSB limb

        // Set b, c, d, e, f to 1 (256-bit one)
        q.b1 = 0L; q.b2 = 0L; q.b3 = 0L; q.b4 = 1L;
        q.c1 = 0L; q.c2 = 0L; q.c3 = 0L; q.c4 = 1L;
        q.d1 = 0L; q.d2 = 0L; q.d3 = 0L; q.d4 = 1L;
        q.e1 = 0L; q.e2 = 0L; q.e3 = 0L; q.e4 = 1L;
        q.f1 = 0L; q.f2 = 0L; q.f3 = 0L; q.f4 = 1L;

        // Clear signs and metadata
        q.signs = 0L;
        q.metadata1 = 0L;
        q.metadata2 = 0L;
        q.metadata3 = 0L;

        return q;
    }

    /**
     * Create a QuantumNumberV8 instance with a specific 256-bit value
     */
    public static QuantumNumberV8 createFromLimbs(long a1, long a2, long a3, long a4) {
        QuantumNumberV8 q = new QuantumNumberV8();
        q.a1 = a1; q.a2 = a2; q.a3 = a3; q.a4 = a4;
        q.b1 = 0L; q.b2 = 0L; q.b3 = 0L; q.b4 = 0L;
        q.c1 = 0L; q.c2 = 0L; q.c3 = 0L; q.c4 = 0L;
        q.d1 = 0L; q.d2 = 0L; q.d3 = 0L; q.d4 = 0L;
        q.e1 = 0L; q.e2 = 0L; q.e3 = 0L; q.e4 = 0L;
        q.f1 = 0L; q.f2 = 0L; q.f3 = 0L; q.f4 = 0L;
        q.signs = 0L;
        q.metadata1 = 0L;
        q.metadata2 = 0L;
        q.metadata3 = 0L;
        return q;
    }

    /**
     * Create the secp256k1 generator point G
     */
    public static QuantumECCPoint createGeneratorPoint() {
        // Gx = 0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
        QuantumNumberV8 gx = createFromLimbs(
            0x79BE667EF9DCBBACL,
            0x55A06295CE870B07L,
            0x029BFCDB2DCE28D9L,
            0x59F2815B16F81798L
        );

        // Gy = 0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8
        QuantumNumberV8 gy = createFromLimbs(
            0x483ADA7726A3C465L,
            0x5DA4FBFC0E1108A8L,
            0xFD17B448A6855419L,
            0x9C47D08FFB10D4B8L
        );

        return new QuantumECCPoint(gx, gy);
    }

    /**
     * Create the secp256k1 curve parameter A (which is 0)
     */
    public static QuantumNumberV8 createCurveParameterA() {
        return createFromLimbs(0L, 0L, 0L, 0L);
    }

    /**
     * Create the secp256k1 curve parameter B (which is 7)
     */
    public static QuantumNumberV8 createCurveParameterB() {
        return createFromLimbs(0L, 0L, 0L, 7L);
    }

    /**
     * Utility: Prints a 256-bit component (4 limbs) as a hex string.
     */
    public static void printQuantumNumberComponent(String label, long w1, long w2, long w3, long w4) {
        System.out.printf("%s = 0x%016X%016X%016X%016X%n", label, w1, w2, w3, w4);
    }

    /**
     * Print all components of a QuantumNumberV8
     */
    public static void printQuantumNumberV8(String label, QuantumNumberV8 q) {
        System.out.println(label + ":");
        printQuantumNumberComponent("  a", q.a1, q.a2, q.a3, q.a4);
        printQuantumNumberComponent("  b", q.b1, q.b2, q.b3, q.b4);
        printQuantumNumberComponent("  c", q.c1, q.c2, q.c3, q.c4);
        printQuantumNumberComponent("  d", q.d1, q.d2, q.d3, q.d4);
        printQuantumNumberComponent("  e", q.e1, q.e2, q.e3, q.e4);
        printQuantumNumberComponent("  f", q.f1, q.f2, q.f3, q.f4);
    }

    public static void main(String[] args) {
        QuantumNumberV8 q = createSecp256k1PrimePWithUnits();

        printQuantumNumberComponent("a (prime p)", q.a1, q.a2, q.a3, q.a4);
        printQuantumNumberComponent("b", q.b1, q.b2, q.b3, q.b4);
        printQuantumNumberComponent("c", q.c1, q.c2, q.c3, q.c4);
        printQuantumNumberComponent("d", q.d1, q.d2, q.d3, q.d4);
        printQuantumNumberComponent("e", q.e1, q.e2, q.e3, q.e4);
        printQuantumNumberComponent("f", q.f1, q.f2, q.f3, q.f4);

        System.out.println("\nGenerator point G:");
        QuantumECCPoint g = createGeneratorPoint();
        printQuantumNumberV8("Gx", g.x);
        printQuantumNumberV8("Gy", g.y);
    }
}
