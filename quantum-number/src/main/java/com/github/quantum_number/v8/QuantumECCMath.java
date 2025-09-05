package com.github.quantum_number.v8;

/**
 * QuantumECCMath - Limb-based modular arithmetic for ECC operations
 * Implements 256-bit arithmetic using QuantumNumberV8's 4×64-bit limb structure
 * No external libraries - pure QuantumNumberV8 operations
 */
public class QuantumECCMath {

    // secp256k1 prime: 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
    public static final long P1 = 0xFFFFFFFFFFFFFFFFL; // MSB
    public static final long P2 = 0xFFFFFFFFFFFFFFFFL;
    public static final long P3 = 0xFFFFFFFFFFFFFFFEL;
    public static final long P4 = 0x00000000FFFFFC2FL; // LSB

    /**
     * Create zero in QuantumNumberV8 format
     */
    public static QuantumNumberV8 zero() {
        return QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 0L);
    }

    /**
     * Create one in QuantumNumberV8 format
     */
    public static QuantumNumberV8 one() {
        return QuantumNumberV8Util.createFromLimbs(0L, 0L, 0L, 1L);
    }

    /**
     * Check if a QuantumNumberV8 is zero
     */
    public static boolean isZero(QuantumNumberV8 a) {
        return a.a1 == 0 && a.a2 == 0 && a.a3 == 0 && a.a4 == 0;
    }

    /**
     * Compare two QuantumNumberV8 values
     */
    public static int compare(QuantumNumberV8 a, QuantumNumberV8 b) {
        if (a.a1 != b.a1) return Long.compareUnsigned(a.a1, b.a1);
        if (a.a2 != b.a2) return Long.compareUnsigned(a.a2, b.a2);
        if (a.a3 != b.a3) return Long.compareUnsigned(a.a3, b.a3);
        return Long.compareUnsigned(a.a4, b.a4);
    }

    /**
     * Modular addition: (a + b) mod p
     */
    public static QuantumNumberV8 modAdd(QuantumNumberV8 a, QuantumNumberV8 b) {
        QuantumNumberV8 result = add(a, b);
        return modReduce(result);
    }

    /**
     * Modular subtraction: (a - b) mod p
     */
    public static QuantumNumberV8 modSubtract(QuantumNumberV8 a, QuantumNumberV8 b) {
        QuantumNumberV8 result = subtract(a, b);
        if (compare(result, zero()) < 0) {
            result = add(result, getPrime());
        }
        return result;
    }

    /**
     * Modular multiplication: (a * b) mod p
     */
    public static QuantumNumberV8 modMultiply(QuantumNumberV8 a, QuantumNumberV8 b) {
        QuantumNumberV8 result = multiply(a, b);
        return modReduce(result);
    }

    /**
     * Modular inverse using Fermat's Little Theorem
     */
    public static QuantumNumberV8 modInverse(QuantumNumberV8 a) {
        // For secp256k1: a^(p-2) mod p
        QuantumNumberV8 result = one();
        QuantumNumberV8 base = new QuantumNumberV8();
        base.a1 = a.a1; base.a2 = a.a2; base.a3 = a.a3; base.a4 = a.a4; // Copy
        QuantumNumberV8 exponent = QuantumNumberV8Util.createFromLimbs(
            0xFFFFFFFFL, 0xFFFFFFFFL, 0xFFFFFFFFL, 0xFFFFFFFDFFFFFC2DL
        ); // p-2

        while (!isZero(exponent)) {
            if ((exponent.a4 & 1) == 1) {
                result = modMultiply(result, base);
            }
            base = modMultiply(base, base);
            exponent = shiftRight(exponent, 1);
        }

        return result;
    }

    /**
     * Raw addition without modular reduction
     */
    public static QuantumNumberV8 add(QuantumNumberV8 a, QuantumNumberV8 b) {
        QuantumNumberV8 result = new QuantumNumberV8();

        long carry = 0;
        long sum;

        // Add LSB first
        sum = a.a4 + b.a4 + carry;
        result.a4 = sum & 0xFFFFFFFFFFFFFFFFL;
        carry = sum >>> 64;

        sum = a.a3 + b.a3 + carry;
        result.a3 = sum & 0xFFFFFFFFFFFFFFFFL;
        carry = sum >>> 64;

        sum = a.a2 + b.a2 + carry;
        result.a2 = sum & 0xFFFFFFFFFFFFFFFFL;
        carry = sum >>> 64;

        sum = a.a1 + b.a1 + carry;
        result.a1 = sum & 0xFFFFFFFFFFFFFFFFL;
        // carry would overflow, but we handle this in modReduce

        return result;
    }

    /**
     * Raw subtraction without modular reduction
     */
    public static QuantumNumberV8 subtract(QuantumNumberV8 a, QuantumNumberV8 b) {
        QuantumNumberV8 result = new QuantumNumberV8();

        long borrow = 0;
        long diff;

        // Subtract LSB first
        diff = a.a4 - b.a4 - borrow;
        if (diff < 0) {
            diff += (1L << 64); // 2^64
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.a4 = diff & 0xFFFFFFFFFFFFFFFFL;

        diff = a.a3 - b.a3 - borrow;
        if (diff < 0) {
            diff += (1L << 64);
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.a3 = diff & 0xFFFFFFFFFFFFFFFFL;

        diff = a.a2 - b.a2 - borrow;
        if (diff < 0) {
            diff += (1L << 64);
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.a2 = diff & 0xFFFFFFFFFFFFFFFFL;

        diff = a.a1 - b.a1 - borrow;
        if (diff < 0) {
            diff += (1L << 64);
            // Final borrow would indicate negative result
        }
        result.a1 = diff & 0xFFFFFFFFFFFFFFFFL;

        return result;
    }

    /**
     * Raw multiplication without modular reduction
     */
    public static QuantumNumberV8 multiply(QuantumNumberV8 a, QuantumNumberV8 b) {
        QuantumNumberV8 result = zero();

        // Simple bit-by-bit multiplication
        for (int i = 0; i < 256; i++) {
            if (getBit(b, i)) {
                QuantumNumberV8 shifted = shiftLeft(a, i);
                result = add(result, shifted);
            }
        }

        return result;
    }

    /**
     * Left shift by n bits
     */
    public static QuantumNumberV8 shiftLeft(QuantumNumberV8 a, int n) {
        if (n >= 256) return zero();
        if (n == 0) {
            QuantumNumberV8 result = new QuantumNumberV8();
            result.a1 = a.a1; result.a2 = a.a2; result.a3 = a.a3; result.a4 = a.a4;
            return result;
        }

        QuantumNumberV8 result = new QuantumNumberV8();

        // Calculate which limbs get data from which
        int limbShift = n / 64;
        int bitShift = n % 64;

        if (limbShift >= 4) return zero();

        // Shift limbs
        if (bitShift == 0) {
            // Simple limb shift
            switch (limbShift) {
                case 0:
                    result.a1 = a.a1; result.a2 = a.a2; result.a3 = a.a3; result.a4 = a.a4;
                    break;
                case 1:
                    result.a1 = a.a2; result.a2 = a.a3; result.a3 = a.a4; result.a4 = 0;
                    break;
                case 2:
                    result.a1 = a.a3; result.a2 = a.a4; result.a3 = 0; result.a4 = 0;
                    break;
                case 3:
                    result.a1 = a.a4; result.a2 = 0; result.a3 = 0; result.a4 = 0;
                    break;
            }
        } else {
            // Bit shift across limbs
            long carry = 0;
            for (int i = 3; i >= 0; i--) {
                int targetLimb = i + limbShift;
                if (targetLimb >= 4) continue;

                long current = getLimb(a, i);
                long shifted = (current << bitShift) | carry;
                setLimb(result, targetLimb, shifted & 0xFFFFFFFFFFFFFFFFL);
                carry = current >>> (64 - bitShift);
            }
        }

        return result;
    }

    /**
     * Right shift by n bits
     */
    public static QuantumNumberV8 shiftRight(QuantumNumberV8 a, int n) {
        if (n >= 256) return zero();
        if (n == 0) {
            QuantumNumberV8 result = new QuantumNumberV8();
            result.a1 = a.a1; result.a2 = a.a2; result.a3 = a.a3; result.a4 = a.a4;
            return result;
        }

        QuantumNumberV8 result = new QuantumNumberV8();

        int limbShift = n / 64;
        int bitShift = n % 64;

        if (limbShift >= 4) return zero();

        if (bitShift == 0) {
            // Simple limb shift
            switch (limbShift) {
                case 0:
                    result.a1 = a.a1; result.a2 = a.a2; result.a3 = a.a3; result.a4 = a.a4;
                    break;
                case 1:
                    result.a1 = 0; result.a2 = a.a1; result.a3 = a.a2; result.a4 = a.a3;
                    break;
                case 2:
                    result.a1 = 0; result.a2 = 0; result.a3 = a.a1; result.a4 = a.a2;
                    break;
                case 3:
                    result.a1 = 0; result.a2 = 0; result.a3 = 0; result.a4 = a.a1;
                    break;
            }
        } else {
            // Bit shift across limbs
            long carry = 0;
            for (int i = 0; i < 4; i++) {
                int sourceLimb = i + limbShift;
                if (sourceLimb >= 4) continue;

                long current = getLimb(a, sourceLimb);
                long shifted = (current >>> bitShift) | carry;
                setLimb(result, i, shifted);
                carry = current << (64 - bitShift);
            }
        }

        return result;
    }

    /**
     * Modular reduction using secp256k1 prime
     */
    public static QuantumNumberV8 modReduce(QuantumNumberV8 a) {
        QuantumNumberV8 result = new QuantumNumberV8();
        result.a1 = a.a1; result.a2 = a.a2; result.a3 = a.a3; result.a4 = a.a4;

        // Simple reduction: while result >= p, result -= p
        while (compare(result, getPrime()) >= 0) {
            result = subtract(result, getPrime());
        }

        return result;
    }

    /**
     * Get the secp256k1 prime as QuantumNumberV8
     */
    public static QuantumNumberV8 getPrime() {
        return QuantumNumberV8Util.createFromLimbs(P1, P2, P3, P4);
    }

    /**
     * Get a specific bit from a QuantumNumberV8
     */
    private static boolean getBit(QuantumNumberV8 a, int bit) {
        int limb = bit / 64;
        int bitInLimb = bit % 64;
        long limbValue = getLimb(a, limb);
        return (limbValue & (1L << bitInLimb)) != 0;
    }

    /**
     * Get a specific limb from a QuantumNumberV8
     */
    private static long getLimb(QuantumNumberV8 a, int limb) {
        switch (limb) {
            case 0: return a.a4; // LSB
            case 1: return a.a3;
            case 2: return a.a2;
            case 3: return a.a1; // MSB
            default: return 0;
        }
    }

    /**
     * Set a specific limb in a QuantumNumberV8
     */
    private static void setLimb(QuantumNumberV8 a, int limb, long value) {
        switch (limb) {
            case 0: a.a4 = value; break; // LSB
            case 1: a.a3 = value; break;
            case 2: a.a2 = value; break;
            case 3: a.a1 = value; break; // MSB
        }
    }

    /**
     * Square a number: a^2 mod p
     */
    public static QuantumNumberV8 square(QuantumNumberV8 a) {
        return modMultiply(a, a);
    }

    /**
     * Check if two QuantumNumberV8 values are equal
     */
    public static boolean equals(QuantumNumberV8 a, QuantumNumberV8 b) {
        return a.a1 == b.a1 && a.a2 == b.a2 && a.a3 == b.a3 && a.a4 == b.a4;
    }
}
