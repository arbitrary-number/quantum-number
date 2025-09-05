package com.github.quantum_number.v8;

import java.math.BigInteger;
import java.util.logging.Logger;

/**
 * Clean implementation of secp256k1 elliptic curve operations.
 * Demonstrates G, 2G, and 3G coordinate calculations.
 */
public class Secp256k1ECCV2 {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCV2.class.getName());

    // secp256k1 curve parameters
    public static final BigInteger P = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F", 16);
    public static final BigInteger A = BigInteger.ZERO;
    public static final BigInteger B = new BigInteger("7", 16);
    public static final BigInteger GX = new BigInteger("79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798", 16);
    public static final BigInteger GY = new BigInteger("483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8", 16);
    public static final BigInteger N = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141", 16);

    /**
     * Represents a point on the elliptic curve
     */
    public static class Point {
        public final BigInteger x;
        public final BigInteger y;
        public final boolean infinity;

        public Point(BigInteger x, BigInteger y) {
            this.x = x;
            this.y = y;
            this.infinity = false;
        }

        public Point(boolean infinity) {
            this.x = BigInteger.ZERO;
            this.y = BigInteger.ZERO;
            this.infinity = infinity;
        }

        @Override
        public String toString() {
            if (infinity) {
                return "∞";
            }
            return String.format("(0x%s, 0x%s)", x.toString(16).toUpperCase(), y.toString(16).toUpperCase());
        }
    }

    /**
     * Generator point G
     */
    public static final Point G = new Point(GX, GY);

    /**
     * Point addition on elliptic curve
     */
    public static Point add(Point p1, Point p2) {
        if (p1.infinity) return p2;
        if (p2.infinity) return p1;

        if (p1.x.equals(p2.x) && !p1.y.equals(p2.y)) {
            return new Point(true); // Point at infinity
        }

        BigInteger m;
        if (p1.x.equals(p2.x) && p1.y.equals(p2.y)) {
            // Point doubling
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
     * Scalar multiplication: k * P
     */
    public static Point multiply(BigInteger k, Point p) {
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
     * Calculate 2G (G + G)
     */
    public static Point calculate2G() {
        return add(G, G);
    }

    /**
     * Calculate 3G (2G + G)
     */
    public static Point calculate3G() {
        return add(calculate2G(), G);
    }

    /**
     * Verify point is on curve
     */
    public static boolean isOnCurve(Point p) {
        if (p.infinity) return true;

        BigInteger left = p.y.pow(2).mod(P);
        BigInteger right = p.x.pow(3).add(A.multiply(p.x)).add(B).mod(P);

        return left.equals(right);
    }

    /**
     * Log coordinate demonstration
     */
    public static void logCoordinates() {
        logger.info("=== secp256k1 Elliptic Curve Coordinate Demonstration ===");
        logger.info("Generator Point G: " + G);
        logger.info("Point G is on curve: " + isOnCurve(G));

        Point g2 = calculate2G();
        logger.info("2G = G + G: " + g2);
        logger.info("Point 2G is on curve: " + isOnCurve(g2));

        Point g3 = calculate3G();
        logger.info("3G = 2G + G: " + g3);
        logger.info("Point 3G is on curve: " + isOnCurve(g3));

        // Verify 3G = 3 * G using scalar multiplication
        Point g3_scalar = multiply(BigInteger.valueOf(3), G);
        logger.info("3G via scalar multiplication: " + g3_scalar);
        logger.info("3G coordinates match: " + (g3.x.equals(g3_scalar.x) && g3.y.equals(g3_scalar.y)));
    }
}
