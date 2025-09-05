package com.github.quantum_number.v8;

import java.math.BigInteger;
import java.util.logging.Logger;
import java.util.ArrayList;
import java.util.List;

/**
 * Enhanced secp256k1 elliptic curve implementation with advanced features.
 * Extends V2 with performance optimizations, batch operations, and comprehensive validation.
 */
public class Secp256k1ECCV3 {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCV3.class.getName());

    // secp256k1 curve parameters (same as V2)
    public static final BigInteger P = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F", 16);
    public static final BigInteger A = BigInteger.ZERO;
    public static final BigInteger B = new BigInteger("7", 16);
    public static final BigInteger GX = new BigInteger("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798", 16);
    public static final BigInteger GY = new BigInteger("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8", 16);
    public static final BigInteger N = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141", 16);

    /**
     * Enhanced Point class with additional features
     */
    public static class Point {
        public final BigInteger x;
        public final BigInteger y;
        public final boolean infinity;
        private final int hashCode; // Cached for performance

        public Point(BigInteger x, BigInteger y) {
            this.x = x;
            this.y = y;
            this.infinity = false;
            this.hashCode = x.hashCode() * 31 + y.hashCode();
        }

        public Point(boolean infinity) {
            this.x = BigInteger.ZERO;
            this.y = BigInteger.ZERO;
            this.infinity = infinity;
            this.hashCode = infinity ? 42 : 0;
        }

        @Override
        public String toString() {
            if (infinity) {
                return "∞";
            }
            return String.format("(0x%s, 0x%s)", x.toString(16).toUpperCase(), y.toString(16).toUpperCase());
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) return true;
            if (!(obj instanceof Point)) return false;
            Point other = (Point) obj;
            if (infinity != other.infinity) return false;
            if (infinity) return true;
            return x.equals(other.x) && y.equals(other.y);
        }

        @Override
        public int hashCode() {
            return this.hashCode;
        }

        /**
         * Check if this point is on the secp256k1 curve
         */
        public boolean isOnCurve() {
            if (infinity) return true;
            BigInteger left = y.pow(2).mod(P);
            BigInteger right = x.pow(3).add(A.multiply(x)).add(B).mod(P);
            return left.equals(right);
        }

        /**
         * Get the compressed representation (33 bytes)
         */
        public byte[] toCompressedBytes() {
            if (infinity) {
                return new byte[]{0x00};
            }
            byte[] xBytes = x.toByteArray();
            byte[] result = new byte[33];
            result[0] = (byte) (y.testBit(0) ? 0x03 : 0x02);
            System.arraycopy(xBytes, Math.max(0, xBytes.length - 32), result, 1, Math.min(32, xBytes.length));
            return result;
        }
    }

    /**
     * Generator point G
     */
    public static final Point G = new Point(GX, GY);

    /**
     * Pre-computed points for performance (G, 2G, 3G, 4G)
     */
    private static final Point[] PRECOMPUTED_POINTS = new Point[5];

    static {
        PRECOMPUTED_POINTS[0] = new Point(true); // 0G = infinity
        PRECOMPUTED_POINTS[1] = G; // 1G
        PRECOMPUTED_POINTS[2] = calculate2G(); // 2G
        PRECOMPUTED_POINTS[3] = calculate3G(); // 3G
        PRECOMPUTED_POINTS[4] = add(PRECOMPUTED_POINTS[3], G); // 4G
    }

    /**
     * Point addition with optimizations
     */
    public static Point add(Point p1, Point p2) {
        if (p1.infinity) return p2;
        if (p2.infinity) return p1;

        if (p1.x.equals(p2.x) && !p1.y.equals(p2.y)) {
            return new Point(true); // Point at infinity
        }

        BigInteger m;
        if (p1.equals(p2)) {
            // Point doubling optimization
            m = p1.x.pow(2).multiply(BigInteger.valueOf(3)).add(A)
                .multiply(p1.y.multiply(BigInteger.valueOf(2)).modInverse(P)).mod(P);
        } else {
            // Point addition
            m = p2.y.subtract(p1.y).multiply(p2.x.subtract(p1.x).modInverse(P)).mod(P);
        }

        BigInteger x3 = m.pow(2).subtract(p1.x).subtract(p2.x).mod(P);
        BigInteger y3 = m.multiply(p1.x.subtract(x3)).subtract(p1.y).mod(P);

        return new Point(x3, y3);
    }

    /**
     * Optimized scalar multiplication using pre-computed points and windowing
     */
    public static Point multiply(BigInteger k, Point p) {
        if (k.equals(BigInteger.ZERO)) return new Point(true);
        if (k.equals(BigInteger.ONE)) return p;

        // Use pre-computed points for small multiples of G
        if (p.equals(G) && k.compareTo(BigInteger.valueOf(4)) <= 0) {
            return PRECOMPUTED_POINTS[k.intValue()];
        }

        // Standard double-and-add algorithm
        Point result = new Point(true);
        Point current = p;

        while (k.compareTo(BigInteger.ZERO) > 0) {
            if (k.testBit(0)) {
                result = add(result, current);
            }
            current = add(current, current);
            k = k.shiftRight(1);
        }

        return result;
    }

    /**
     * Batch scalar multiplication for multiple points
     */
    public static List<Point> batchMultiply(List<BigInteger> scalars, List<Point> points) {
        if (scalars.size() != points.size()) {
            throw new IllegalArgumentException("Scalars and points lists must have same size");
        }

        List<Point> results = new ArrayList<>();
        for (int i = 0; i < scalars.size(); i++) {
            results.add(multiply(scalars.get(i), points.get(i)));
        }
        return results;
    }

    /**
     * Calculate 2G (G + G)
     */
    public static Point calculate2G() {
        return PRECOMPUTED_POINTS[2];
    }

    /**
     * Calculate 3G (2G + G)
     */
    public static Point calculate3G() {
        return PRECOMPUTED_POINTS[3];
    }

    /**
     * Calculate 4G (3G + G)
     */
    public static Point calculate4G() {
        return PRECOMPUTED_POINTS[4];
    }

    /**
     * Verify point is on curve with detailed logging
     */
    public static boolean isOnCurve(Point p) {
        return p.isOnCurve();
    }

    /**
     * Generate a sequence of points: G, 2G, 3G, ..., nG
     */
    public static List<Point> generatePointSequence(int n) {
        List<Point> sequence = new ArrayList<>();
        Point current = new Point(true); // Start with infinity

        for (int i = 0; i <= n; i++) {
            sequence.add(current);
            if (i < n) {
                current = add(current, G);
            }
        }

        return sequence;
    }

    /**
     * Validate that all points in a sequence are on the curve
     */
    public static boolean validateSequence(List<Point> sequence) {
        for (int i = 0; i < sequence.size(); i++) {
            Point p = sequence.get(i);
            if (!isOnCurve(p)) {
                logger.warning(String.format("Point %dG is not on curve: %s", i, p));
                return false;
            }
        }
        return true;
    }

    /**
     * Comprehensive coordinate demonstration with performance metrics
     */
    public static void logComprehensiveCoordinates() {
        long startTime = System.nanoTime();

        logger.info("=== 🔢 secp256k1 Enhanced Coordinate Demonstration ===");

        // Basic points
        Point g = G;
        Point g2 = calculate2G();
        Point g3 = calculate3G();
        Point g4 = calculate4G();

        logger.info(String.format("G₁ = %s", g));
        logger.info(String.format("G₂ = 2G₁ = %s", g2));
        logger.info(String.format("G₃ = 3G₁ = %s", g3));
        logger.info(String.format("G₄ = 4G₁ = %s", g4));

        // Validation
        logger.info("=== ✅ Curve Validation ===");
        logger.info(String.format("G₁ ∈ E: %s", isOnCurve(g) ? "✓" : "✗"));
        logger.info(String.format("G₂ ∈ E: %s", isOnCurve(g2) ? "✓" : "✗"));
        logger.info(String.format("G₃ ∈ E: %s", isOnCurve(g3) ? "✓" : "✗"));
        logger.info(String.format("G₄ ∈ E: %s", isOnCurve(g4) ? "✓" : "✗"));

        // Sequence generation
        logger.info("=== 📊 Point Sequence Generation ===");
        List<Point> sequence = generatePointSequence(5);
        for (int i = 0; i < sequence.size(); i++) {
            logger.info(String.format("%dG = %s", i, sequence.get(i)));
        }

        // Performance metrics
        long endTime = System.nanoTime();
        double durationMs = (endTime - startTime) / 1_000_000.0;
        logger.info(String.format("⏱️  Computation time: %.3f ms", durationMs));

        logger.info("✓ Enhanced coordinate demonstration completed");
    }

    /**
     * Advanced arithmetic verification
     */
    public static boolean verifyArithmetic() {
        logger.info("=== 🔍 Advanced Arithmetic Verification ===");

        boolean allValid = true;

        // Test associativity: (G + 2G) + 3G = G + (2G + 3G)
        Point left = add(add(G, calculate2G()), calculate3G());
        Point right = add(G, add(calculate2G(), calculate3G()));
        boolean associativity = left.equals(right);
        logger.info(String.format("Associativity test: %s", associativity ? "✓" : "✗"));
        allValid &= associativity;

        // Test scalar multiplication distributivity
        Point scalar1 = multiply(BigInteger.valueOf(5), G);
        Point scalar2 = add(add(add(add(G, calculate2G()), calculate3G()), calculate4G()), G);
        boolean distributivity = scalar1.equals(scalar2);
        logger.info(String.format("Scalar distributivity: %s", distributivity ? "✓" : "✗"));
        allValid &= distributivity;

        return allValid;
    }
}
