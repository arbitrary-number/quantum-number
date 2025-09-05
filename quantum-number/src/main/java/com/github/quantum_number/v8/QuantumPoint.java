package com.github.quantum_number.v8;

/**
 * QuantumPoint - Elliptic curve point using QuantumFieldElement
 * Based on QuantumNumberV8 principles with linked structure and bit manipulation
 *
 * Structure: Uses QuantumFieldElement for x,y coordinates with pointer system
 * left: previous point in chain, right: next point in chain
 * up: parent in tree structure, down: child in tree structure
 */
public class QuantumPoint {

    // Instance data - following QuantumNumberV8 structure
    public long signs;      // Sign and metadata flags
    public long metadata;   // Additional quantum state

    public QuantumFieldElement x;  // X coordinate (QuantumFieldElement)
    public QuantumFieldElement y;  // Y coordinate (QuantumFieldElement)
    public boolean infinity;       // Point at infinity flag

    public long c;          // Carry/overflow accumulator
    public long d;          // Temporary workspace
    public long e;          // Point operation workspace
    public long f;          // Bit manipulation workspace

    // Pointers to other QuantumPoint instances (following QuantumNumberV8)
    public QuantumPoint left;   // Left pointer for point chains
    public QuantumPoint right;  // Right pointer for point operations
    public QuantumPoint up;     // Up pointer for tree structures
    public QuantumPoint down;   // Down pointer for child points
    public QuantumPoint in_;    // In pointer for internal operations
    public QuantumPoint out;    // Out pointer for external operations

    /**
     * Constructor - point at infinity
     */
    public QuantumPoint() {
        this.signs = 0;
        this.metadata = 0;
        this.x = new QuantumFieldElement(0);
        this.y = new QuantumFieldElement(0);
        this.infinity = true;
        this.c = 0;
        this.d = 0;
        this.e = 0;
        this.f = 0;
    }

    /**
     * Constructor - finite point
     */
    public QuantumPoint(QuantumFieldElement x, QuantumFieldElement y) {
        this.signs = 0;
        this.metadata = 0;
        this.x = new QuantumFieldElement(x);
        this.y = new QuantumFieldElement(y);
        this.infinity = false;
        this.c = 0;
        this.d = 0;
        this.e = 0;
        this.f = 0;
    }

    /**
     * Constructor - point from long coordinates
     */
    public QuantumPoint(long x, long y) {
        this.signs = 0;
        this.metadata = 0;
        this.x = new QuantumFieldElement(x);
        this.y = new QuantumFieldElement(y);
        this.infinity = false;
        this.c = 0;
        this.d = 0;
        this.e = 0;
        this.f = 0;
    }

    /**
     * Copy constructor
     */
    public QuantumPoint(QuantumPoint other) {
        this.signs = other.signs;
        this.metadata = other.metadata;
        this.x = new QuantumFieldElement(other.x);
        this.y = new QuantumFieldElement(other.y);
        this.infinity = other.infinity;
        this.c = other.c;
        this.d = other.d;
        this.e = other.e;
        this.f = other.f;
    }

    /**
     * Fast point addition using bit manipulation
     */
    public QuantumPoint add(QuantumPoint other) {
        if (this.infinity) return new QuantumPoint(other);
        if (other.infinity) return new QuantumPoint(this);

        QuantumPoint result = new QuantumPoint();

        // Check if points are the same (point doubling)
        if (this.x.equals(other.x) && this.y.equals(other.y)) {
            return this.doublePoint();
        }

        // Check if points are inverses (vertical line)
        if (this.x.equals(other.x) && !this.y.equals(other.y)) {
            result.infinity = true;
            return result;
        }

        // Standard point addition: (x3,y3) = (x1,y1) + (x2,y2)
        // slope m = (y2 - y1) / (x2 - x1)
        QuantumFieldElement dx = other.x.subtract(this.x);
        QuantumFieldElement dy = other.y.subtract(this.y);

        // Modular inverse of dx
        QuantumFieldElement dxInv = dx.modInverse();
        QuantumFieldElement m = dy.multiply(dxInv);

        // x3 = m² - x1 - x2
        QuantumFieldElement m2 = m.square();
        QuantumFieldElement x3 = m2.subtract(this.x).subtract(other.x);

        // y3 = m(x1 - x3) - y1
        QuantumFieldElement x1_minus_x3 = this.x.subtract(x3);
        QuantumFieldElement mx1_minus_x3 = m.multiply(x1_minus_x3);
        QuantumFieldElement y3 = mx1_minus_x3.subtract(this.y);

        result.x = x3;
        result.y = y3;
        result.infinity = false;

        return result;
    }

    /**
     * Fast point doubling using bit manipulation
     */
    public QuantumPoint doublePoint() {
        if (this.infinity) return new QuantumPoint();

        QuantumPoint result = new QuantumPoint();

        // Point doubling formula for y² = x³ + ax + b
        // For secp256k1: a = 0, so y² = x³ + b

        // slope m = (3x² + a) / (2y)
        QuantumFieldElement x2 = this.x.square();
        QuantumFieldElement three_x2 = x2.multiply(new QuantumFieldElement(3));
        QuantumFieldElement numerator = three_x2; // a = 0

        QuantumFieldElement two_y = this.y.multiply(new QuantumFieldElement(2));
        QuantumFieldElement denominator = two_y;

        QuantumFieldElement m = numerator.multiply(denominator.modInverse());

        // x3 = m² - 2x
        QuantumFieldElement m2 = m.square();
        QuantumFieldElement two_x = this.x.multiply(new QuantumFieldElement(2));
        QuantumFieldElement x3 = m2.subtract(two_x);

        // y3 = m(x - x3) - y
        QuantumFieldElement x_minus_x3 = this.x.subtract(x3);
        QuantumFieldElement m_x_minus_x3 = m.multiply(x_minus_x3);
        QuantumFieldElement y3 = m_x_minus_x3.subtract(this.y);

        result.x = x3;
        result.y = y3;
        result.infinity = false;

        return result;
    }

    /**
     * Fast scalar multiplication using bit manipulation
     */
    public QuantumPoint multiply(long scalar) {
        if (scalar == 0) return new QuantumPoint();
        if (scalar == 1) return new QuantumPoint(this);

        QuantumPoint result = new QuantumPoint();
        QuantumPoint current = new QuantumPoint(this);

        // Use bit manipulation for scalar multiplication
        while (scalar > 0) {
            if ((scalar & 1) == 1) {
                result = result.add(current);
            }
            current = current.doublePoint();
            scalar >>>= 1;
        }

        return result;
    }

    /**
     * Fast scalar multiplication with bit shifting optimization
     */
    public QuantumPoint multiplyBitShift(long scalar) {
        if (scalar == 0) return new QuantumPoint();
        if (scalar == 1) return new QuantumPoint(this);

        QuantumPoint result = new QuantumPoint();
        QuantumPoint current = new QuantumPoint(this);

        // Process bits from LSB to MSB using bit manipulation
        int bitLength = 64 - Long.numberOfLeadingZeros(scalar);

        for (int i = 0; i < bitLength; i++) {
            if ((scalar & (1L << i)) != 0) {
                result = result.add(current);
            }
            current = current.doublePoint();
        }

        return result;
    }

    /**
     * Check if point is on the elliptic curve
     */
    public boolean isOnCurve() {
        if (this.infinity) return true;

        // For secp256k1: y² = x³ + b
        QuantumFieldElement left = this.y.square();
        QuantumFieldElement x3 = this.x.square().multiply(this.x);
        QuantumFieldElement right = x3.add(new QuantumFieldElement(7)); // b = 7

        return left.equals(right);
    }

    /**
     * Negate point (elliptic curve inverse)
     */
    public QuantumPoint negate() {
        if (this.infinity) return new QuantumPoint();
        return new QuantumPoint(this.x, this.y.multiply(new QuantumFieldElement(-1)));
    }

    /**
     * Check equality
     */
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof QuantumPoint)) return false;
        QuantumPoint other = (QuantumPoint) obj;
        if (this.infinity != other.infinity) return false;
        if (this.infinity) return true;
        return this.x.equals(other.x) && this.y.equals(other.y);
    }

    @Override
    public int hashCode() {
        if (infinity) return 0;
        return x.hashCode() * 31 + y.hashCode();
    }

    /**
     * String representation
     */
    @Override
    public String toString() {
        if (infinity) return "∞";
        return "(" + x + ", " + y + ")";
    }

    /**
     * Create generator point G for secp256k1
     */
    public static QuantumPoint createGenerator() {
        // secp256k1 generator point
        long gx_high = 0x79BE667EF9DCBBACL;
        long gx_low = 0x55A06295CE870B07L;
        long gy_high = 0x483ADA7726A3C465L;
        long gy_low = 0x5DA4FBFC0E1108A8L;

        QuantumFieldElement x = new QuantumFieldElement(gx_high, gx_low);
        QuantumFieldElement y = new QuantumFieldElement(gy_high, gy_low);

        return new QuantumPoint(x, y);
    }

    /**
     * Create point from hex strings
     */
    public static QuantumPoint fromHex(String xHex, String yHex) {
        long x_high = Long.parseUnsignedLong(xHex.substring(0, 16), 16);
        long x_low = Long.parseUnsignedLong(xHex.substring(16, 32), 16);
        long y_high = Long.parseUnsignedLong(yHex.substring(0, 16), 16);
        long y_low = Long.parseUnsignedLong(yHex.substring(16, 32), 16);

        QuantumFieldElement x = new QuantumFieldElement(x_high, x_low);
        QuantumFieldElement y = new QuantumFieldElement(y_high, y_low);

        return new QuantumPoint(x, y);
    }
}
