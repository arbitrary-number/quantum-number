package com.github.quantum_number.v8;

import java.math.BigDecimal;
import java.math.BigInteger;
import java.math.MathContext;

public class QuantumNumberV8Collapser {

    private static final MathContext MATH_CONTEXT = MathContext.DECIMAL128;

    /**
     * Converts a 256-bit component represented by 4 longs (little-endian)
     * into a BigInteger.
     *
     * @param w1 least significant 64 bits
     * @param w2
     * @param w3
     * @param w4 most significant 64 bits
     * @return BigInteger representation (unsigned)
     */
    private static BigInteger toBigInteger(long w1, long w2, long w3, long w4) {
        BigInteger part1 = BigInteger.valueOf(w1).and(BigInteger.valueOf(0xFFFFFFFFFFFFFFFFL));
        BigInteger part2 = BigInteger.valueOf(w2).and(BigInteger.valueOf(0xFFFFFFFFFFFFFFFFL)).shiftLeft(64);
        BigInteger part3 = BigInteger.valueOf(w3).and(BigInteger.valueOf(0xFFFFFFFFFFFFFFFFL)).shiftLeft(128);
        BigInteger part4 = BigInteger.valueOf(w4).and(BigInteger.valueOf(0xFFFFFFFFFFFFFFFFL)).shiftLeft(192);

        return part1.or(part2).or(part3).or(part4);
    }

    public static BigDecimal collapse(QuantumNumberV8 q) {
    	return collapseToBigDecimal(q);
    }

    /**
     * Collapses the QuantumNumberV8 into a BigDecimal approximation.
     *
     * This method attempts to safely handle division by zero by
     * returning null if any denominator is zero (symbolic state).
     *
     * @param q QuantumNumberV8 instance
     * @return BigDecimal approximation or null if symbolic/invalid
     */
    public static BigDecimal collapseToBigDecimal(QuantumNumberV8 q) {
        BigInteger a = toBigInteger(q.a1, q.a2, q.a3, q.a4);
        BigInteger b = toBigInteger(q.b1, q.b2, q.b3, q.b4);
        BigInteger c = toBigInteger(q.c1, q.c2, q.c3, q.c4);
        BigInteger d = toBigInteger(q.d1, q.d2, q.d3, q.d4);
        BigInteger e = toBigInteger(q.e1, q.e2, q.e3, q.e4);
        BigInteger f = toBigInteger(q.f1, q.f2, q.f3, q.f4);

        // Check denominators for zero - symbolic states not supported here
        if (b.equals(BigInteger.ZERO) || e.equals(BigInteger.ZERO)) {
            return null;
        }

        // Calculate inner denominators: b/c and e/f (careful with division)
        BigDecimal bc, ef;

        // b/c = b / c, if c == 0, symbolic, return null
        if (c.equals(BigInteger.ZERO)) {
            return null;
        } else {
            bc = new BigDecimal(b).divide(new BigDecimal(c), MATH_CONTEXT);
        }

        // e/f = e / f, if f == 0, symbolic, return null
        if (f.equals(BigInteger.ZERO)) {
            return null;
        } else {
            ef = new BigDecimal(e).divide(new BigDecimal(f), MATH_CONTEXT);
        }

        // Compute (a / (b / c)) * (d / (e / f))
        // = (a * c / b) * (d * f / e)
        BigDecimal left = new BigDecimal(a).multiply(new BigDecimal(c)).divide(new BigDecimal(b), MATH_CONTEXT);
        BigDecimal right = new BigDecimal(d).multiply(new BigDecimal(f)).divide(new BigDecimal(e), MATH_CONTEXT);

        return left.multiply(right, MATH_CONTEXT);
    }
}
