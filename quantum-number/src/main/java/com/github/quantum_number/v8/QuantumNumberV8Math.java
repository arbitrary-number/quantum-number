package com.github.quantum_number.v8;

public class QuantumNumberV8Math {

    /**
     * Multiply a 256-bit component by a small integer factor (e.g., 2 or 3).
     * The component is represented as 4 longs in little-endian order:
     * index 0 = least significant 64 bits, index 3 = most significant 64 bits.
     *
     * @param comp   long array of length 4 representing the 256-bit component
     * @param factor small integer multiplier (>= 0)
     * @return the multiplied 256-bit result (new long array)
     */
    public static long[] multiplyBySmallFactor(long[] comp, int factor) {
        if (factor < 0) {
            throw new IllegalArgumentException("Factor must be non-negative");
        }
        if (factor == 0) {
            return new long[4]; // zero result
        }
        if (factor == 1) {
            return comp.clone();
        }

        long carry = 0;
        long[] result = new long[4];

        for (int i = 0; i < 4; i++) {
            // Multiply comp[i] (unsigned 64-bit) by factor (unsigned 32-bit),
            // add carry from previous step,
            // result might overflow 64 bits, so split manually.

            // Split comp[i] into two 32-bit parts:
            long low = comp[i] & 0xFFFFFFFFL;
            long high = comp[i] >>> 32;

            long f = factor & 0xFFFFFFFFL;

            // Multiply low parts:
            long lowMul = low * f;
            // Multiply high parts:
            long highMul = high * f;

            // Add carry to lowMul's high 32 bits:
            long lowMulLow = lowMul & 0xFFFFFFFFL;
            long lowMulHigh = lowMul >>> 32;

            // Add carry to low part:
            long sumLow = lowMulLow + (carry & 0xFFFFFFFFL);
            long carryOutLow = sumLow >>> 32;

            // Add all high parts and carry:
            long sumHigh = highMul + lowMulHigh + carryOutLow + (carry >>> 32);

            // Compose result 64-bit part:
            long partLow = (sumLow & 0xFFFFFFFFL) | (sumHigh << 32);

            result[i] = partLow;

            // New carry is upper 32 bits of sumHigh:
            carry = sumHigh >>> 32;
        }

        // Overflow beyond 256 bits is discarded

        return result;
    }

    /**
     * Multiply the 'b' component by a small integer factor (modifies in place).
     *
     * @param q      QuantumNumberV8 instance to modify
     * @param factor small integer multiplier (>= 0)
     */
    public static void multiplyB(QuantumNumberV8 q, int factor) {
        long[] b = new long[]{q.b1, q.b2, q.b3, q.b4};
        long[] result = multiplyBySmallFactor(b, factor);
        q.b1 = result[0];
        q.b2 = result[1];
        q.b3 = result[2];
        q.b4 = result[3];
    }

    /**
     * Multiply the 'd' component by a small integer factor (modifies in place).
     *
     * @param q      QuantumNumberV8 instance to modify
     * @param factor small integer multiplier (>= 0)
     */
    public static void multiplyD(QuantumNumberV8 q, int factor) {
        long[] d = new long[]{q.d1, q.d2, q.d3, q.d4};
        long[] result = multiplyBySmallFactor(d, factor);
        q.d1 = result[0];
        q.d2 = result[1];
        q.d3 = result[2];
        q.d4 = result[3];
    }

    /**
     * Multiply the 'f' component by a small integer factor (modifies in place).
     *
     * @param q      QuantumNumberV8 instance to modify
     * @param factor small integer multiplier (>= 0)
     */
    public static void multiplyF(QuantumNumberV8 q, int factor) {
        long[] f = new long[]{q.f1, q.f2, q.f3, q.f4};
        long[] result = multiplyBySmallFactor(f, factor);
        q.f1 = result[0];
        q.f2 = result[1];
        q.f3 = result[2];
        q.f4 = result[3];
    }
}
