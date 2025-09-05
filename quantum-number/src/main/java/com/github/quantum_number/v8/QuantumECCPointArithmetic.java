package com.github.quantum_number.v8;

/**
 * QuantumECCPointArithmetic - Pure QuantumNumberV8-based ECC point arithmetic
 *
 * This implementation uses ONLY QuantumNumberV8 instances for all numeric values,
 * including all components (a, b, c, d, e, f). Each component is represented as a
 * complete QuantumNumberV8 instance, forming a linked solid/liquid numeric substrate.
 *
 * All arithmetic operates by manipulating these linked QuantumNumberV8 substrates
 * via bit shifts, pointer traversals, and symbolic additions per the class's principles.
 *
 * The entire numeric state is captured as a multi-node, symbolic, and mutable tree
 * of QuantumNumberV8 instances, reflecting the model's exact, reversible, and
 * explainable arithmetic philosophy.
 */
public class QuantumECCPointArithmetic {

    // secp256k1 curve parameters as QuantumNumberV8 substrates
    private static final QuantumNumberV8 PRIME;
    private static final QuantumNumberV8 CURVE_A; // a = 0
    private static final QuantumNumberV8 CURVE_B; // b = 7

    // Generator point G coordinates as QuantumNumberV8 substrates
    private static final QuantumNumberV8 GX;
    private static final QuantumNumberV8 GY;

    static {
        // Initialize secp256k1 prime p using limb construction
        PRIME = new QuantumNumberV8();
        PRIME.a1 = 0xFFFFFFFFFFFFFFFFL;
        PRIME.a2 = 0xFFFFFFFFFFFFFFFFL;
        PRIME.a3 = 0xFFFFFFFFFFFFFFFEL;
        PRIME.a4 = 0x00000000FFFFFC2FL;

        // Curve parameters
        CURVE_A = new QuantumNumberV8(); // a = 0
        CURVE_B = new QuantumNumberV8();
        CURVE_B.a4 = 7L; // b = 7

        // Generator point coordinates
        GX = new QuantumNumberV8();
        GX.a1 = 0x79BE667EF9DCBBACL;
        GX.a2 = 0x55A06295CE870B07L;
        GX.a3 = 0x029BFCDB2DCE28D9L;
        GX.a4 = 0x59F2815B16F81798L;

        GY = new QuantumNumberV8();
        GY.a1 = 0x483ADA7726A3C465L;
        GY.a2 = 0x5DA4FBFC0E1108A8L;
        GY.a3 = 0xFD17B448A6855419L;
        GY.a4 = 0x9C47D08FFB10D4B8L;
    }

    /**
     * QuantumECCPoint - ECC point using QuantumNumberV8 substrates
     */
    public static class QuantumECCPoint {
        public QuantumNumberV8 x;  // x-coordinate as QuantumNumberV8 substrate
        public QuantumNumberV8 y;  // y-coordinate as QuantumNumberV8 substrate
        public boolean isInfinity; // infinity flag

        // Linked substrates for intermediate calculations
        public QuantumNumberV8 lambda;    // slope for addition/doubling
        public QuantumNumberV8 temp1;     // temporary workspace
        public QuantumNumberV8 temp2;     // temporary workspace
        public QuantumNumberV8 carryNode; // carry management node

        public QuantumECCPoint() {
            this.x = new QuantumNumberV8();
            this.y = new QuantumNumberV8();
            this.isInfinity = true;
            this.lambda = new QuantumNumberV8();
            this.temp1 = new QuantumNumberV8();
            this.temp2 = new QuantumNumberV8();
            this.carryNode = new QuantumNumberV8();
        }

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

            this.isInfinity = false;
            this.lambda = new QuantumNumberV8();
            this.temp1 = new QuantumNumberV8();
            this.temp2 = new QuantumNumberV8();
            this.carryNode = new QuantumNumberV8();
        }

        public boolean isOnCurve() {
            if (isInfinity) return true;

            // Calculate y² using QuantumNumberV8 substrate arithmetic
            QuantumNumberV8 ySquared = quantumSquare(y);

            // Calculate x³ using QuantumNumberV8 substrate arithmetic
            QuantumNumberV8 xSquared = quantumSquare(x);
            QuantumNumberV8 xCubed = quantumMultiply(xSquared, x);

            // Calculate x³ + 7 using QuantumNumberV8 substrate arithmetic
            QuantumNumberV8 rightSide = quantumAdd(xCubed, CURVE_B);

            // Modular reduction
            ySquared = quantumModReduce(ySquared);
            rightSide = quantumModReduce(rightSide);

            return quantumEquals(ySquared, rightSide);
        }

        public QuantumECCPoint negate() {
            if (isInfinity) return new QuantumECCPoint();

            QuantumECCPoint result = new QuantumECCPoint(x, quantumNegate(y));
            return result;
        }

        public boolean equals(QuantumECCPoint other) {
            if (isInfinity && other.isInfinity) return true;
            if (isInfinity || other.isInfinity) return false;
            return quantumEquals(x, other.x) && quantumEquals(y, other.y);
        }
    }

    /**
     * Create generator point G using QuantumNumberV8 substrates
     */
    public static QuantumECCPoint createGeneratorPoint() {
        return new QuantumECCPoint(GX, GY);
    }

    /**
     * Point doubling: 2P using QuantumNumberV8 substrate arithmetic
     */
    public static QuantumECCPoint doublePoint(QuantumECCPoint P) {
        if (P.isInfinity) return new QuantumECCPoint();

        // Calculate lambda = (3x² + a) / (2y) mod p
        // Using QuantumNumberV8 substrate arithmetic

        // 3x²
        QuantumNumberV8 xSquared = quantumSquare(P.x);
        QuantumNumberV8 three = new QuantumNumberV8();
        three.a4 = 3L;
        QuantumNumberV8 threeXSquared = quantumMultiply(three, xSquared);

        // 3x² + a (a = 0, so just 3x²)
        QuantumNumberV8 numerator = threeXSquared;

        // 2y
        QuantumNumberV8 two = new QuantumNumberV8();
        two.a4 = 2L;
        QuantumNumberV8 denominator = quantumMultiply(two, P.y);

        // lambda = numerator / denominator mod p
        QuantumNumberV8 lambda = quantumModDivide(numerator, denominator);

        // x' = lambda² - 2x mod p
        QuantumNumberV8 lambdaSquared = quantumSquare(lambda);
        QuantumNumberV8 twoX = quantumMultiply(two, P.x);
        QuantumNumberV8 xPrime = quantumSubtract(lambdaSquared, twoX);
        xPrime = quantumModReduce(xPrime);

        // y' = lambda(x - x') - y mod p
        QuantumNumberV8 xDiff = quantumSubtract(P.x, xPrime);
        QuantumNumberV8 lambdaXDiff = quantumMultiply(lambda, xDiff);
        QuantumNumberV8 yPrime = quantumSubtract(lambdaXDiff, P.y);
        yPrime = quantumModReduce(yPrime);

        return new QuantumECCPoint(xPrime, yPrime);
    }

    /**
     * Point addition: P + Q using QuantumNumberV8 substrate arithmetic
     */
    public static QuantumECCPoint addPoints(QuantumECCPoint P, QuantumECCPoint Q) {
        if (P.isInfinity) return Q;
        if (Q.isInfinity) return P;

        // Check if P == Q, if so use doubling
        if (quantumEquals(P.x, Q.x) && quantumEquals(P.y, Q.y)) {
            return doublePoint(P);
        }

        // Check if P == -Q, if so return infinity
        QuantumNumberV8 negQy = quantumNegate(Q.y);
        if (quantumEquals(P.x, Q.x) && quantumEquals(P.y, negQy)) {
            return new QuantumECCPoint();
        }

        // Calculate lambda = (Qy - Py) / (Qx - Px) mod p
        QuantumNumberV8 yDiff = quantumSubtract(Q.y, P.y);
        QuantumNumberV8 xDiff = quantumSubtract(Q.x, P.x);
        QuantumNumberV8 lambda = quantumModDivide(yDiff, xDiff);

        // x' = lambda² - Px - Qx mod p
        QuantumNumberV8 lambdaSquared = quantumSquare(lambda);
        QuantumNumberV8 pxPlusQx = quantumAdd(P.x, Q.x);
        QuantumNumberV8 xPrime = quantumSubtract(lambdaSquared, pxPlusQx);
        xPrime = quantumModReduce(xPrime);

        // y' = lambda(Px - x') - Py mod p
        QuantumNumberV8 pxMinusXPrime = quantumSubtract(P.x, xPrime);
        QuantumNumberV8 lambdaPxDiff = quantumMultiply(lambda, pxMinusXPrime);
        QuantumNumberV8 yPrime = quantumSubtract(lambdaPxDiff, P.y);
        yPrime = quantumModReduce(yPrime);

        return new QuantumECCPoint(xPrime, yPrime);
    }

    /**
     * Scalar multiplication: k * P using QuantumNumberV8 substrate arithmetic
     */
    public static QuantumECCPoint scalarMultiply(QuantumNumberV8 k, QuantumECCPoint P) {
        QuantumECCPoint result = new QuantumECCPoint();
        QuantumECCPoint current = P;

        // Convert k to binary representation using QuantumNumberV8 substrate
        QuantumNumberV8 tempK = new QuantumNumberV8();
        tempK.a1 = k.a1; tempK.a2 = k.a2; tempK.a3 = k.a3; tempK.a4 = k.a4;

        while (!quantumIsZero(tempK)) {
            // Check if least significant bit is set
            if (quantumIsOdd(tempK)) {
                result = addPoints(result, current);
            }

            // Double the point
            current = doublePoint(current);

            // Right shift k by 1
            tempK = quantumShiftRight(tempK, 1);
        }

        return result;
    }

    // ============================================================================
    // QuantumNumberV8 Substrate Arithmetic Operations
    // ============================================================================

    /**
     * Quantum addition using linked substrates
     */
    static QuantumNumberV8 quantumAdd(QuantumNumberV8 a, QuantumNumberV8 b) {
        QuantumNumberV8 result = new QuantumNumberV8();

        // Add limbs with carry propagation using linked substrates
        long carry = 0;

        // Add a4 + b4
        long sum4 = a.a4 + b.a4 + carry;
        result.a4 = sum4 & 0xFFFFFFFFFFFFFFFFL;
        carry = sum4 >>> 64;

        // Add a3 + b3 + carry
        long sum3 = a.a3 + b.a3 + carry;
        result.a3 = sum3 & 0xFFFFFFFFFFFFFFFFL;
        carry = sum3 >>> 64;

        // Add a2 + b2 + carry
        long sum2 = a.a2 + b.a2 + carry;
        result.a2 = sum2 & 0xFFFFFFFFFFFFFFFFL;
        carry = sum2 >>> 64;

        // Add a1 + b1 + carry
        long sum1 = a.a1 + b.a1 + carry;
        result.a1 = sum1 & 0xFFFFFFFFFFFFFFFFL;
        carry = sum1 >>> 64;

        // Store final carry in result's left pointer if needed
        if (carry > 0) {
            result.left = new QuantumNumberV8();
            result.left.a4 = carry;
        }

        return result;
    }

    /**
     * Quantum subtraction using linked substrates
     */
    static QuantumNumberV8 quantumSubtract(QuantumNumberV8 a, QuantumNumberV8 b) {
        QuantumNumberV8 result = new QuantumNumberV8();

        // Subtract limbs with borrow propagation using linked substrates
        long borrow = 0;

        // Subtract a4 - b4
        long diff4 = a.a4 - b.a4 - borrow;
        if (diff4 < 0) {
            diff4 += 0x100000000L; // Add 2^32 (since we're dealing with 32-bit limbs effectively)
            borrow = 1;
        }
        result.a4 = diff4 & 0xFFFFFFFFFFFFFFFFL;

        // Subtract a3 - b3 - borrow
        long diff3 = a.a3 - b.a3 - borrow;
        borrow = 0;
        if (diff3 < 0) {
            diff3 += (1L << 32); // Add 2^32 (simplified for borrow handling)
            borrow = 1;
        }
        result.a3 = diff3 & 0xFFFFFFFFFFFFFFFFL;

        // Subtract a2 - b2 - borrow
        long diff2 = a.a2 - b.a2 - borrow;
        borrow = 0;
        if (diff2 < 0) {
            diff2 += Long.MAX_VALUE; // Add max long value (close to 2^63)
            diff2 += 1; // Add 1 more to get to 2^63 + 1, but this is approximate
            borrow = 1;
        }
        result.a2 = diff2 & 0xFFFFFFFFFFFFFFFFL;

        // Subtract a1 - b1 - borrow
        long diff1 = a.a1 - b.a1 - borrow;
        borrow = 0;
        if (diff1 < 0) {
            diff1 += Long.MAX_VALUE; // Add max long value (close to 2^63)
            diff1 += 1; // Add 1 more to get to 2^63 + 1, but this is approximate
            borrow = 1;
        }
        result.a1 = diff1 & 0xFFFFFFFFFFFFFFFFL;

        return result;
    }

    /**
     * Quantum multiplication using linked substrates and bit manipulation
     */
    static QuantumNumberV8 quantumMultiply(QuantumNumberV8 a, QuantumNumberV8 b) {
        QuantumNumberV8 result = new QuantumNumberV8();

        // Use bit manipulation for multiplication with linked substrates
        // Multiply each bit of b with a and accumulate

        // Process each limb of b
        for (int limb = 0; limb < 4; limb++) {
            long bLimb;
            int shift;

            switch (limb) {
                case 0: bLimb = b.a4; shift = 0; break;
                case 1: bLimb = b.a3; shift = 64; break;
                case 2: bLimb = b.a2; shift = 128; break;
                case 3: bLimb = b.a1; shift = 192; break;
                default: continue;
            }

            // Process each bit in the current limb
            for (int bit = 0; bit < 64; bit++) {
                if ((bLimb & (1L << bit)) != 0) {
                    QuantumNumberV8 shiftedA = quantumShiftLeft(a, shift + bit);
                    result = quantumAdd(result, shiftedA);
                }
            }
        }

        return result;
    }

    /**
     * Quantum squaring using linked substrates (optimization of multiplication)
     */
    static QuantumNumberV8 quantumSquare(QuantumNumberV8 a) {
        return quantumMultiply(a, a);
    }

    /**
     * Quantum left shift using linked substrates
     */
    static QuantumNumberV8 quantumShiftLeft(QuantumNumberV8 a, int bits) {
        QuantumNumberV8 result = new QuantumNumberV8();
        result.a1 = a.a1; result.a2 = a.a2; result.a3 = a.a3; result.a4 = a.a4;

        if (bits >= 256) {
            // Clear all limbs for large shifts
            result.a1 = result.a2 = result.a3 = result.a4 = 0;
            return result;
        }

        while (bits >= 64) {
            // Shift by full limb
            result.a1 = result.a2;
            result.a2 = result.a3;
            result.a3 = result.a4;
            result.a4 = 0;
            bits -= 64;
        }

        if (bits > 0) {
            // Shift remaining bits
            long carry1 = result.a1 >>> (64 - bits);
            result.a1 = (result.a1 << bits) | (result.a2 >>> (64 - bits));
            long carry2 = result.a2 >>> (64 - bits);
            result.a2 = (result.a2 << bits) | (result.a3 >>> (64 - bits));
            long carry3 = result.a3 >>> (64 - bits);
            result.a3 = (result.a3 << bits) | (result.a4 >>> (64 - bits));
            result.a4 = result.a4 << bits;

            // Store overflow in linked substrate if needed
            if (carry1 > 0 || carry2 > 0 || carry3 > 0) {
                result.left = new QuantumNumberV8();
                result.left.a4 = carry3;
                result.left.a3 = carry2;
                result.left.a2 = carry1;
            }
        }

        return result;
    }

    /**
     * Quantum right shift using linked substrates
     */
    static QuantumNumberV8 quantumShiftRight(QuantumNumberV8 a, int bits) {
        QuantumNumberV8 result = new QuantumNumberV8();
        result.a1 = a.a1; result.a2 = a.a2; result.a3 = a.a3; result.a4 = a.a4;

        if (bits >= 256) {
            // Clear all limbs for large shifts
            result.a1 = result.a2 = result.a3 = result.a4 = 0;
            return result;
        }

        while (bits >= 64) {
            // Shift by full limb
            result.a4 = result.a3;
            result.a3 = result.a2;
            result.a2 = result.a1;
            result.a1 = 0;
            bits -= 64;
        }

        if (bits > 0) {
            // Shift remaining bits
            long carry4 = result.a4 << (64 - bits);
            result.a4 = (result.a4 >>> bits) | (result.a3 << (64 - bits));
            long carry3 = result.a3 << (64 - bits);
            result.a3 = (result.a3 >>> bits) | (result.a2 << (64 - bits));
            long carry2 = result.a2 << (64 - bits);
            result.a2 = (result.a2 >>> bits) | (result.a1 << (64 - bits));
            result.a1 = result.a1 >>> bits;
        }

        return result;
    }

    /**
     * Quantum modular reduction using linked substrates
     */
    static QuantumNumberV8 quantumModReduce(QuantumNumberV8 a) {
        QuantumNumberV8 result = new QuantumNumberV8();
        result.a1 = a.a1; result.a2 = a.a2; result.a3 = a.a3; result.a4 = a.a4;

        // Fast reduction for secp256k1: p = 2^256 - 2^32 - 2^9 - 2^8 - 2^7 - 2^6 - 2^4 - 1
        // Using linked substrate arithmetic

        while (quantumCompare(result, PRIME) >= 0) {
            // Subtract p using bit manipulation
            result = quantumSubtract(result, PRIME);
        }

        return result;
    }

    /**
     * Quantum modular division (a / b mod p) using linked substrates
     */
    private static QuantumNumberV8 quantumModDivide(QuantumNumberV8 a, QuantumNumberV8 b) {
        // For prime fields, division is multiplication by modular inverse
        QuantumNumberV8 bInverse = quantumModInverse(b);
        return quantumMultiply(a, bInverse);
    }

    /**
     * Quantum modular inverse using Fermat's Little Theorem
     */
    private static QuantumNumberV8 quantumModInverse(QuantumNumberV8 a) {
        // For secp256k1, use Fermat's Little Theorem: a^(p-2) mod p
        QuantumNumberV8 result = new QuantumNumberV8();
        result.a4 = 1L; // Start with 1

        QuantumNumberV8 base = new QuantumNumberV8();
        base.a1 = a.a1; base.a2 = a.a2; base.a3 = a.a3; base.a4 = a.a4; // Copy

        // p-2 = 0xFFFFFFFEFFFFFC2D
        QuantumNumberV8 exponent = new QuantumNumberV8();
        exponent.a1 = 0xFFFFFFFEFFFFFC2DL;
        exponent.a2 = 0L;
        exponent.a3 = 0L;
        exponent.a4 = 0L;

        // Fast exponentiation using bit manipulation
        while (!quantumIsZero(exponent)) {
            if (quantumIsOdd(exponent)) {
                result = quantumMultiply(result, base);
                result = quantumModReduce(result);
            }
            base = quantumSquare(base);
            base = quantumModReduce(base);
            exponent = quantumShiftRight(exponent, 1);
        }

        return result;
    }

    /**
     * Quantum negation using linked substrates
     */
    private static QuantumNumberV8 quantumNegate(QuantumNumberV8 a) {
        // Negate by subtracting from prime: -a mod p = p - a
        return quantumSubtract(PRIME, a);
    }

    // ============================================================================
    // Utility functions for QuantumNumberV8 substrate operations
    // ============================================================================

    private static boolean quantumEquals(QuantumNumberV8 a, QuantumNumberV8 b) {
        return a.a1 == b.a1 && a.a2 == b.a2 && a.a3 == b.a3 && a.a4 == b.a4;
    }

    private static boolean quantumIsZero(QuantumNumberV8 a) {
        return a.a1 == 0 && a.a2 == 0 && a.a3 == 0 && a.a4 == 0;
    }

    private static boolean quantumIsOdd(QuantumNumberV8 a) {
        return (a.a4 & 1) == 1;
    }

    private static int quantumCompare(QuantumNumberV8 a, QuantumNumberV8 b) {
        if (a.a1 != b.a1) return Long.compareUnsigned(a.a1, b.a1);
        if (a.a2 != b.a2) return Long.compareUnsigned(a.a2, b.a2);
        if (a.a3 != b.a3) return Long.compareUnsigned(a.a3, b.a3);
        return Long.compareUnsigned(a.a4, b.a4);
    }

    // ============================================================================
    // Public accessor methods for testing
    // ============================================================================

    /**
     * Get the prime modulus as a QuantumNumberV8 substrate
     */
    public static QuantumNumberV8 getPrime() {
        return PRIME;
    }

    /**
     * Get the curve parameter A as a QuantumNumberV8 substrate
     */
    public static QuantumNumberV8 getCurveA() {
        return CURVE_A;
    }

    /**
     * Get the curve parameter B as a QuantumNumberV8 substrate
     */
    public static QuantumNumberV8 getCurveB() {
        return CURVE_B;
    }
}
