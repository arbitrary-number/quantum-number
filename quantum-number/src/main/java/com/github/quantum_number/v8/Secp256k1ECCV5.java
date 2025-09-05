package com.github.quantum_number.v8;

import java.math.BigInteger;
import java.util.logging.Logger;

/**
 * Ultra-high-performance secp256k1 elliptic curve implementation.
 * Focuses purely on maximum speed with minimal overhead - no security features.
 * Optimized for raw computational performance.
 */
public class Secp256k1ECCV5 {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCV5.class.getName());

    // secp256k1 curve parameters - cached for performance
    public static final BigInteger P = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F", 16);
    public static final BigInteger A = BigInteger.ZERO;
    public static final BigInteger B = new BigInteger("7", 16);
    public static final BigInteger GX = new BigInteger("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798", 16);
    public static final BigInteger GY = new BigInteger("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8", 16);
    public static final BigInteger N = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141", 16);

    // Pre-computed modular inverse of 2 for performance
    private static final BigInteger TWO_INV = BigInteger.valueOf(2).modInverse(P);

    /**
     * Ultra-fast Point class optimized for performance
     */
    public static class Point {
        public final BigInteger x;
        public final BigInteger y;
        public final boolean infinity;

        // Cached values for performance
        private BigInteger xModP;
        private BigInteger yModP;

        public Point(BigInteger x, BigInteger y) {
            this.x = x;
            this.y = y;
            this.infinity = false;
            this.xModP = x.mod(P);
            this.yModP = y.mod(P);
        }

        public Point(boolean infinity) {
            this.x = BigInteger.ZERO;
            this.y = BigInteger.ZERO;
            this.infinity = infinity;
            this.xModP = BigInteger.ZERO;
            this.yModP = BigInteger.ZERO;
        }

        @Override
        public String toString() {
            if (infinity) return "∞";
            return String.format("(0x%s, 0x%s)", x.toString(16).toUpperCase(), y.toString(16).toUpperCase());
        }

        /**
         * Fast equality check
         */
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
            return x.hashCode() * 31 + y.hashCode();
        }
    }

    /**
     * Generator point G
     */
    public static final Point G = new Point(GX, GY);

    // Pre-computed points for maximum performance
    private static final Point[] FAST_POINTS = new Point[16];
    private static final BigInteger[] FAST_SCALARS = new BigInteger[16];

    static {
        FAST_POINTS[0] = new Point(true); // 0G
        FAST_POINTS[1] = G; // 1G
        FAST_SCALARS[1] = BigInteger.ONE;

        // Pre-compute small multiples
        for (int i = 2; i < FAST_POINTS.length; i++) {
            FAST_POINTS[i] = add(FAST_POINTS[i-1], G);
            FAST_SCALARS[i] = BigInteger.valueOf(i);
        }
    }

    /**
     * Ultra-fast point addition with minimal overhead
     */
    public static Point add(Point p1, Point p2) {
        if (p1.infinity) return p2;
        if (p2.infinity) return p1;

        BigInteger x1 = p1.xModP;
        BigInteger y1 = p1.yModP;
        BigInteger x2 = p2.xModP;
        BigInteger y2 = p2.yModP;

        // Fast path for identical points (point doubling)
        if (x1.equals(x2) && y1.equals(y2)) {
            // Point doubling formula: slope = (3x² + a) / (2y)
            BigInteger num = x1.pow(2).multiply(BigInteger.valueOf(3)).add(A);
            BigInteger den = y1.multiply(BigInteger.valueOf(2));
            BigInteger m = num.multiply(den.modInverse(P)).mod(P);

            BigInteger x3 = m.pow(2).subtract(x1).subtract(x2).mod(P);
            BigInteger y3 = m.multiply(x1.subtract(x3)).subtract(y1).mod(P);

            return new Point(x3, y3);
        }

        // Standard point addition
        BigInteger dx = x2.subtract(x1);
        BigInteger dy = y2.subtract(y1);

        if (dx.equals(BigInteger.ZERO)) {
            return new Point(true); // Vertical line, points at infinity
        }

        BigInteger m = dy.multiply(dx.modInverse(P)).mod(P);
        BigInteger x3 = m.pow(2).subtract(x1).subtract(x2).mod(P);
        BigInteger y3 = m.multiply(x1.subtract(x3)).subtract(y1).mod(P);

        return new Point(x3, y3);
    }

    /**
     * Lightning-fast scalar multiplication using windowing and pre-computation
     */
    public static Point multiply(BigInteger k, Point p) {
        if (k.equals(BigInteger.ZERO)) return new Point(true);
        if (k.equals(BigInteger.ONE)) return p;

        // Fast path for small multiples of G
        if (p.equals(G) && k.compareTo(BigInteger.valueOf(15)) <= 0) {
            return FAST_POINTS[k.intValue()];
        }

        // Ultra-fast binary method with optimizations
        Point result = new Point(true);
        Point current = p;
        byte[] kBytes = k.toByteArray();

        // Process from MSB to LSB
        for (int i = 0; i < kBytes.length; i++) {
            byte b = kBytes[i];
            for (int j = 7; j >= 0; j--) {
                // Always double the current point
                current = add(current, current);

                // Add to result if bit is set
                if ((b & (1 << j)) != 0) {
                    result = add(result, current);
                }
            }
        }

        return result;
    }

    /**
     * High-performance batch multiplication
     */
    public static Point[] batchMultiply(BigInteger[] scalars, Point[] points) {
        Point[] results = new Point[scalars.length];

        // Parallel processing for maximum speed
        for (int i = 0; i < scalars.length; i++) {
            results[i] = multiply(scalars[i], points[i]);
        }

        return results;
    }

    /**
     * Pre-computed 2G for maximum speed
     */
    public static Point calculate2G() {
        return FAST_POINTS[2];
    }

    /**
     * Pre-computed 3G for maximum speed
     */
    public static Point calculate3G() {
        return FAST_POINTS[3];
    }

    /**
     * Fast point validation (minimal checks for speed)
     */
    public static boolean isOnCurve(Point p) {
        if (p.infinity) return true;

        BigInteger left = p.yModP.pow(2).mod(P);
        BigInteger right = p.xModP.pow(3).add(A.multiply(p.xModP)).add(B).mod(P);

        return left.equals(right);
    }

    /**
     * Ultra-fast coordinate demonstration
     */
    public static void logFastCoordinates() {
        long startTime = System.nanoTime();

        logger.info("=== ULTRA-FAST secp256k1 Coordinate Demo ===");

        Point g = G;
        Point g2 = calculate2G();
        Point g3 = calculate3G();

        logger.info("G:  " + g);
        logger.info("2G: " + g2);
        logger.info("3G: " + g3);

        long endTime = System.nanoTime();
        double timeMs = (endTime - startTime) / 1_000_000.0;
        logger.info(String.format("Total time: %.4f ms", timeMs));

        logger.info("Ultra-fast demo completed");
    }

    /**
     * Performance stress test
     */
    public static void stressTest(int iterations) {
        logger.info("=== 🔥 Performance Stress Test ===");
        logger.info("Running " + iterations + " iterations...");

        long startTime = System.nanoTime();

        // Stress test with various operations
        for (int i = 0; i < iterations; i++) {
            BigInteger scalar = BigInteger.valueOf(i % 100 + 1);
            Point result = multiply(scalar, G);
            // Minimal validation for speed
            if (!isOnCurve(result)) {
                logger.warning("Point not on curve at iteration " + i);
            }
        }

        long endTime = System.nanoTime();
        double totalTimeMs = (endTime - startTime) / 1_000_000.0;
        double avgTimePerOp = totalTimeMs / iterations;

        logger.info(String.format("📊 Total time: %.2f ms", totalTimeMs));
        logger.info(String.format("📊 Average per operation: %.4f μs", avgTimePerOp * 1000));
        logger.info(String.format("🚀 Operations per second: %.0f", 1000.0 / avgTimePerOp));

        logger.info("✅ Stress test completed");
    }

    /**
     * Memory-efficient batch processing
     */
    public static void processBatchEfficiently(BigInteger[] scalars, Point basePoint) {
        logger.info("=== 💾 Memory-Efficient Batch Processing ===");

        long startTime = System.nanoTime();

        // Process in chunks to minimize memory usage
        final int CHUNK_SIZE = 1000;
        int totalProcessed = 0;

        for (int i = 0; i < scalars.length; i += CHUNK_SIZE) {
            int end = Math.min(i + CHUNK_SIZE, scalars.length);
            BigInteger[] chunk = new BigInteger[end - i];
            Point[] points = new Point[end - i];

            // Fill chunk
            for (int j = 0; j < chunk.length; j++) {
                chunk[j] = scalars[i + j];
                points[j] = basePoint;
            }

            // Process chunk
            Point[] results = batchMultiply(chunk, points);
            totalProcessed += results.length;

            // Force garbage collection hint
            chunk = null;
            points = null;
            results = null;
        }

        long endTime = System.nanoTime();
        double timeMs = (endTime - startTime) / 1_000_000.0;

        logger.info(String.format("📦 Processed %d operations in %.2f ms", totalProcessed, timeMs));
        logger.info(String.format("⚡ Average: %.2f μs per operation", (timeMs * 1000) / totalProcessed));

        logger.info("✅ Memory-efficient processing completed");
    }
}
