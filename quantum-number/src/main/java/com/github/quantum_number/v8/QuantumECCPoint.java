package com.github.quantum_number.v8;

/**
 * QuantumECCPoint - ECC point representation using QuantumNumberV8
 * Implements point operations for secp256k1 curve using limb-based arithmetic
 */
public class QuantumECCPoint {

    public QuantumNumberV8 x;
    public QuantumNumberV8 y;
    public boolean infinity;

    // Curve parameters
    private static final QuantumNumberV8 CURVE_A = QuantumNumberV8Util.createCurveParameterA(); // 0
    private static final QuantumNumberV8 CURVE_B = QuantumNumberV8Util.createCurveParameterB(); // 7

    /**
     * Constructor for affine point
     */
    public QuantumECCPoint(QuantumNumberV8 x, QuantumNumberV8 y) {
        this.x = new QuantumNumberV8();
        this.x.a1 = x.a1; this.x.a2 = x.a2; this.x.a3 = x.a3; this.x.a4 = x.a4;
        this.x.b1 = x.b1; this.x.b2 = x.b2; this.x.b3 = x.b3; this.x.b4 = x.b4;
        this.x.c1 = x.c1; this.x.c2 = x.c2; this.x.c3 = x.c3; this.x.c4 = x.c4;
        this.x.d1 = x.d1; this.x.d2 = x.d2; this.x.d3 = x.d3; this.x.d4 = x.d4;
        this.x.e1 = x.e1; this.x.e2 = x.e2; this.x.e3 = x.e3; this.x.e4 = x.e4;
        this.x.f1 = x.f1; this.x.f2 = x.f2; this.x.f3 = x.f3; this.x.f4 = x.f4;

        this.y = new QuantumNumberV8();
        this.y.a1 = y.a1; this.y.a2 = y.a2; this.y.a3 = y.a3; this.y.a4 = y.a4;
        this.y.b1 = y.b1; this.y.b2 = y.b2; this.y.b3 = y.b3; this.y.b4 = y.b4;
        this.y.c1 = y.c1; this.y.c2 = y.c2; this.y.c3 = y.c3; this.y.c4 = y.c4;
        this.y.d1 = y.d1; this.y.d2 = y.d2; this.y.d3 = y.d3; this.y.d4 = y.d4;
        this.y.e1 = y.e1; this.y.e2 = y.e2; this.y.e3 = y.e3; this.y.e4 = y.e4;
        this.y.f1 = y.f1; this.y.f2 = y.f2; this.y.f3 = y.f3; this.y.f4 = y.f4;

        this.infinity = false;
    }

    /**
     * Constructor for point at infinity
     */
    public QuantumECCPoint() {
        this.x = QuantumECCMath.zero();
        this.y = QuantumECCMath.zero();
        this.infinity = true;
    }

    /**
     * Copy constructor
     */
    public QuantumECCPoint(QuantumECCPoint other) {
        this.x = new QuantumNumberV8();
        this.x.a1 = other.x.a1; this.x.a2 = other.x.a2; this.x.a3 = other.x.a3; this.x.a4 = other.x.a4;
        this.x.b1 = other.x.b1; this.x.b2 = other.x.b2; this.x.b3 = other.x.b3; this.x.b4 = other.x.b4;
        this.x.c1 = other.x.c1; this.x.c2 = other.x.c2; this.x.c3 = other.x.c3; this.x.c4 = other.x.c4;
        this.x.d1 = other.x.d1; this.x.d2 = other.x.d2; this.x.d3 = other.x.d3; this.x.d4 = other.x.d4;
        this.x.e1 = other.x.e1; this.x.e2 = other.x.e2; this.x.e3 = other.x.e3; this.x.e4 = other.x.e4;
        this.x.f1 = other.x.f1; this.x.f2 = other.x.f2; this.x.f3 = other.x.f3; this.x.f4 = other.x.f4;

        this.y = new QuantumNumberV8();
        this.y.a1 = other.y.a1; this.y.a2 = other.y.a2; this.y.a3 = other.y.a3; this.y.a4 = other.y.a4;
        this.y.b1 = other.y.b1; this.y.b2 = other.y.b2; this.y.b3 = other.y.b3; this.y.b4 = other.y.b4;
        this.y.c1 = other.y.c1; this.y.c2 = other.y.c2; this.y.c3 = other.y.c3; this.y.c4 = other.y.c4;
        this.y.d1 = other.y.d1; this.y.d2 = other.y.d2; this.y.d3 = other.y.d3; this.y.d4 = other.y.d4;
        this.y.e1 = other.y.e1; this.y.e2 = other.y.e2; this.y.e3 = other.y.e3; this.y.e4 = other.y.e4;
        this.y.f1 = other.y.f1; this.y.f2 = other.y.f2; this.y.f3 = other.y.f3; this.y.f4 = other.y.f4;

        this.infinity = other.infinity;
    }

    /**
     * Check if point is at infinity
     */
    public boolean isInfinity() {
        return infinity;
    }

    /**
     * Check if point is on the curve: y² = x³ + ax + b
     */
    public boolean isOnCurve() {
        if (infinity) return true;

        // Calculate y²
        QuantumNumberV8 y2 = QuantumECCMath.square(y);

        // Calculate x³
        QuantumNumberV8 x2 = QuantumECCMath.square(x);
        QuantumNumberV8 x3 = QuantumECCMath.modMultiply(x2, x);

        // Calculate ax (a = 0 for secp256k1)
        QuantumNumberV8 ax = QuantumECCMath.zero();

        // Calculate x³ + ax + b
        QuantumNumberV8 right = QuantumECCMath.modAdd(x3, ax);
        right = QuantumECCMath.modAdd(right, CURVE_B);

        return QuantumECCMath.equals(y2, right);
    }

    /**
     * Point negation: -P = (x, -y)
     */
    public QuantumECCPoint negate() {
        if (infinity) return new QuantumECCPoint();
        return new QuantumECCPoint(x, QuantumECCMath.modSubtract(QuantumECCMath.zero(), y));
    }

    /**
     * Point doubling: 2P
     */
    public QuantumECCPoint doublePoint() {
        if (infinity) return new QuantumECCPoint();

        // Check if y = 0 (point of order 2)
        if (QuantumECCMath.isZero(y)) return new QuantumECCPoint();

        // Calculate slope: m = (3x² + a) / (2y)
        QuantumNumberV8 x2 = QuantumECCMath.square(x);
        QuantumNumberV8 threeX2 = QuantumECCMath.modMultiply(
            QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 3L), x2
        );
        QuantumNumberV8 numerator = QuantumECCMath.modAdd(threeX2, CURVE_A);

        QuantumNumberV8 twoY = QuantumECCMath.modAdd(y, y);
        QuantumNumberV8 denominator = twoY;

        QuantumNumberV8 m = QuantumECCMath.modMultiply(numerator, QuantumECCMath.modInverse(denominator));

        // Calculate x' = m² - 2x
        QuantumNumberV8 m2 = QuantumECCMath.square(m);
        QuantumNumberV8 twoX = QuantumECCMath.modAdd(x, x);
        QuantumNumberV8 xPrime = QuantumECCMath.modSubtract(m2, twoX);

        // Calculate y' = m(x - x') - y
        QuantumNumberV8 xDiff = QuantumECCMath.modSubtract(x, xPrime);
        QuantumNumberV8 mTimesXDiff = QuantumECCMath.modMultiply(m, xDiff);
        QuantumNumberV8 yPrime = QuantumECCMath.modSubtract(mTimesXDiff, y);

        return new QuantumECCPoint(xPrime, yPrime);
    }

    /**
     * Point addition: P + Q
     */
    public QuantumECCPoint add(QuantumECCPoint other) {
        // Handle infinity cases
        if (infinity) return new QuantumECCPoint(other);
        if (other.infinity) return new QuantumECCPoint(this);

        // Check if P = Q (doubling case)
        if (QuantumECCMath.equals(x, other.x) && QuantumECCMath.equals(y, other.y)) {
            return doublePoint();
        }

        // Check if P = -Q (result is infinity)
        QuantumNumberV8 negY = QuantumECCMath.modSubtract(QuantumECCMath.zero(), y);
        if (QuantumECCMath.equals(x, other.x) && QuantumECCMath.equals(negY, other.y)) {
            return new QuantumECCPoint();
        }

        // General case: P + Q where P ≠ Q
        // Calculate slope: m = (y2 - y1) / (x2 - x1)
        QuantumNumberV8 numerator = QuantumECCMath.modSubtract(other.y, y);
        QuantumNumberV8 denominator = QuantumECCMath.modSubtract(other.x, x);
        QuantumNumberV8 m = QuantumECCMath.modMultiply(numerator, QuantumECCMath.modInverse(denominator));

        // Calculate x' = m² - x1 - x2
        QuantumNumberV8 m2 = QuantumECCMath.square(m);
        QuantumNumberV8 xSum = QuantumECCMath.modAdd(x, other.x);
        QuantumNumberV8 xPrime = QuantumECCMath.modSubtract(m2, xSum);

        // Calculate y' = m(x1 - x') - y1
        QuantumNumberV8 xDiff = QuantumECCMath.modSubtract(x, xPrime);
        QuantumNumberV8 mTimesXDiff = QuantumECCMath.modMultiply(m, xDiff);
        QuantumNumberV8 yPrime = QuantumECCMath.modSubtract(mTimesXDiff, y);

        return new QuantumECCPoint(xPrime, yPrime);
    }

    /**
     * Scalar multiplication: k * P
     */
    public QuantumECCPoint multiply(long scalar) {
        if (scalar == 0) return new QuantumECCPoint();
        if (scalar == 1) return new QuantumECCPoint(this);
        if (infinity) return new QuantumECCPoint();

        QuantumECCPoint result = new QuantumECCPoint();
        QuantumECCPoint current = new QuantumECCPoint(this);

        long k = scalar;
        while (k > 0) {
            if ((k & 1) == 1) {
                result = result.add(current);
            }
            current = current.doublePoint();
            k >>>= 1;
        }

        return result;
    }

    /**
     * Check equality of points
     */
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof QuantumECCPoint)) return false;
        QuantumECCPoint other = (QuantumECCPoint) obj;

        if (infinity != other.infinity) return false;
        if (infinity) return true; // Both infinity

        return QuantumECCMath.equals(x, other.x) && QuantumECCMath.equals(y, other.y);
    }

    @Override
    public int hashCode() {
        if (infinity) return 0;
        return x.hashCode() * 31 + y.hashCode();
    }

    @Override
    public String toString() {
        if (infinity) return "Point at Infinity";
        return String.format("(%s, %s)", x.toString(), y.toString());
    }
}
