package com.github.quantum_number.v8;

import java.math.BigDecimal;
import java.math.BigInteger;

public class QuantumNumberV8Expander {

    /**
     * Expands a BigDecimal into a QuantumNumberV8.
     *
     * This implementation maps:
     * - The unscaled value of BigDecimal into component 'a'.
     * - The scale of BigDecimal into the scaling factor (d / (e / f)).
     * - Keeps b=1, c=1 for simplicity.
     *
     * Note: This is a simplified approach and does not handle negative values
     * or symbolic states yet.
     *
     * @param value the BigDecimal to expand
     * @return QuantumNumberV8 representation
     */
    public static QuantumNumberV8 expand(BigDecimal value) {
        QuantumNumberV8 q = new QuantumNumberV8();

        BigInteger unscaled = value.unscaledValue();
        int scale = value.scale();

        // Set 'a' to unscaled value
        setComponentFromBigInteger(unscaled, q, 'a');

        // Set b and c to 1 (identity)
        setComponentFromBigInteger(BigInteger.ONE, q, 'b');
        setComponentFromBigInteger(BigInteger.ONE, q, 'c');

        // Set scaling coefficient based on scale:
        // If scale >= 0, then multiply by 10^(-scale)
        // So d = 1, e = 10^scale, f = 1 (scaling = d / (e / f) = 1 / (10^scale / 1) = 10^-scale)
        //
        // If scale < 0, then multiply by 10^(-scale) = 10^abs(scale)
        // So d = 10^abs(scale), e = 1, f = 1
        if (scale >= 0) {
            setComponentFromBigInteger(BigInteger.ONE, q, 'd');
            setComponentFromBigInteger(BigInteger.TEN.pow(scale), q, 'e');
            setComponentFromBigInteger(BigInteger.ONE, q, 'f');
        } else {
            int absScale = -scale;
            setComponentFromBigInteger(BigInteger.TEN.pow(absScale), q, 'd');
            setComponentFromBigInteger(BigInteger.ONE, q, 'e');
            setComponentFromBigInteger(BigInteger.ONE, q, 'f');
        }

        // Set signs and metadata as zero for now
        q.signs = 0;
        q.metadata1 = 0;
        q.metadata2 = 0;
        q.metadata3 = 0;

        return q;
    }

    /**
     * Helper to set the specified component ('a', 'b', 'c', 'd', 'e', or 'f')
     * from a BigInteger into the corresponding 4 long fields of QuantumNumberV8.
     *
     * @param value BigInteger (non-negative)
     * @param q QuantumNumberV8 instance to modify
     * @param component component identifier
     */
    private static void setComponentFromBigInteger(BigInteger value, QuantumNumberV8 q, char component) {
        byte[] bytes = value.toByteArray();

        // We'll interpret bytes as big-endian, convert to little-endian longs
        // 32 bytes needed for 256 bits, pad or truncate as needed
        byte[] padded = new byte[32];
        int copyStart = Math.max(0, bytes.length - 32);
        int copyLength = Math.min(32, bytes.length);
        System.arraycopy(bytes, copyStart, padded, 32 - copyLength, copyLength);

        // Convert little-endian 64-bit segments from big-endian byte array
        long w1 = toLongLE(padded, 0);
        long w2 = toLongLE(padded, 8);
        long w3 = toLongLE(padded, 16);
        long w4 = toLongLE(padded, 24);

        switch (component) {
            case 'a':
                q.a1 = w1;
                q.a2 = w2;
                q.a3 = w3;
                q.a4 = w4;
                break;
            case 'b':
                q.b1 = w1;
                q.b2 = w2;
                q.b3 = w3;
                q.b4 = w4;
                break;
            case 'c':
                q.c1 = w1;
                q.c2 = w2;
                q.c3 = w3;
                q.c4 = w4;
                break;
            case 'd':
                q.d1 = w1;
                q.d2 = w2;
                q.d3 = w3;
                q.d4 = w4;
                break;
            case 'e':
                q.e1 = w1;
                q.e2 = w2;
                q.e3 = w3;
                q.e4 = w4;
                break;
            case 'f':
                q.f1 = w1;
                q.f2 = w2;
                q.f3 = w3;
                q.f4 = w4;
                break;
            default:
                throw new IllegalArgumentException("Invalid component: " + component);
        }
    }

    /**
     * Converts 8 bytes from big-endian byte array to a little-endian long.
     *
     * @param bytes big-endian byte array
     * @param offset starting offset
     * @return little-endian long
     */
    private static long toLongLE(byte[] bytes, int offset) {
        // Extract bytes offset..offset+7 (big-endian)
        long value = 0;
        for (int i = 0; i < 8; i++) {
            value <<= 8;
            value |= (bytes[offset + i] & 0xFF);
        }
        // Now convert big-endian long to little-endian
        return Long.reverseBytes(value);
    }
}
