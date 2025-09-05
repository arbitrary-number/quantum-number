package com.github.quantum_number.v8;

/**
 * QuantumFieldElement - High-performance modular arithmetic using native long operations
 * Based on QuantumNumberV8 principles with explicit carry management and bit manipulation
 *
 * Structure: Uses two longs for 128-bit arithmetic with overflow handling
 * left: high 64 bits, right: low 64 bits
 * overflow: carry management through left pointer system
 */
public class QuantumFieldElement {

    // secp256k1 prime: 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
    public static final long P_HIGH = 0xFFFFFFFFFFFFFFFFL;
    public static final long P_LOW = 0xFFFFFFFEFFFFFC2FL;

    // Constants for bit operations
    private static final long TWO_POW_32 = getTwoPow32();

    private static long getTwoPow32() {
        return 1L << 32;
    }

    // Instance data - following QuantumNumberV8 structure
    public long signs;      // Sign and metadata flags
    public long metadata;   // Additional quantum state

    public long a;          // Primary value (low 64 bits)
    public long b;          // Secondary value (high 64 bits)
    public long c;          // Carry/overflow accumulator
    public long d;          // Temporary workspace
    public long e;          // Modular reduction workspace
    public long f;          // Bit manipulation workspace

    // Pointers to other QuantumFieldElement instances (following QuantumNumberV8)
    public QuantumFieldElement left;   // Left pointer for carry propagation
    public QuantumFieldElement right;  // Right pointer for bit operations
    public QuantumFieldElement up;     // Up pointer for overflow handling
    public QuantumFieldElement down;   // Down pointer for reduction
    public QuantumFieldElement in_;    // In pointer for internal operations
    public QuantumFieldElement out;    // Out pointer for external operations

    /**
     * Constructor - initialize with value
     */
    public QuantumFieldElement(long value) {
        this.signs = 0;
        this.metadata = 0;
        this.a = value & 0xFFFFFFFFFFFFFFFFL;
        this.b = 0;
        this.c = 0;
        this.d = 0;
        this.e = 0;
        this.f = 0;
    }

    /**
     * Constructor - initialize with high/low parts
     */
    public QuantumFieldElement(long high, long low) {
        this.signs = 0;
        this.metadata = 0;
        this.a = low & 0xFFFFFFFFFFFFFFFFL;
        this.b = high & 0xFFFFFFFFFFFFFFFFL;
        this.c = 0;
        this.d = 0;
        this.e = 0;
        this.f = 0;
    }

    /**
     * Copy constructor
     */
    public QuantumFieldElement(QuantumFieldElement other) {
        this.signs = other.signs;
        this.metadata = other.metadata;
        this.a = other.a;
        this.b = other.b;
        this.c = other.c;
        this.d = other.d;
        this.e = other.e;
        this.f = other.f;
    }

    /**
     * Fast modular addition with carry management
     */
    public QuantumFieldElement add(QuantumFieldElement other) {
        QuantumFieldElement result = new QuantumFieldElement(0);

        // Add low parts
        long sumLow = this.a + other.a;
        result.a = sumLow & 0xFFFFFFFFFFFFFFFFL;
        long carry1 = (sumLow >>> 64) & 1;

        // Add high parts with carry
        long sumHigh = this.b + other.b + carry1;
        result.b = sumHigh & 0xFFFFFFFFFFFFFFFFL;
        long carry2 = (sumHigh >>> 64) & 1;

        // Store carry in overflow accumulator
        result.c = carry2;

        // Modular reduction if needed
        if (result.c > 0 || result.b > P_HIGH || (result.b == P_HIGH && result.a >= P_LOW)) {
            result.reduce();
        }

        return result;
    }

    /**
     * Fast modular subtraction with borrow management
     */
    public QuantumFieldElement subtract(QuantumFieldElement other) {
        QuantumFieldElement result = new QuantumFieldElement(0);

        // Subtract low parts
        long diffLow = this.a - other.a;
        result.a = diffLow & 0xFFFFFFFFFFFFFFFFL;
        long borrow1 = (diffLow >>> 63) & 1; // Sign bit indicates borrow

        // Subtract high parts with borrow
        long diffHigh = this.b - other.b - borrow1;
        result.b = diffHigh & 0xFFFFFFFFFFFFFFFFL;
        long borrow2 = (diffHigh >>> 63) & 1;

        // Handle negative results by adding modulus
        if (borrow2 != 0 || result.b < 0) {
            result.a += P_LOW;
            result.b += P_HIGH;
            if (result.a > 0xFFFFFFFFFFFFFFFFL) {
                result.a &= 0xFFFFFFFFFFFFFFFFL;
                result.b++;
            }
        }

        return result;
    }

    /**
     * Fast modular multiplication using bit manipulation
     */
    public QuantumFieldElement multiply(QuantumFieldElement other) {
        QuantumFieldElement result = new QuantumFieldElement(0);

        // Use bit manipulation for multiplication
        for (int i = 0; i < 64; i++) {
            if ((other.a & (1L << i)) != 0) {
                QuantumFieldElement temp = this.shiftLeft(i);
                result = result.add(temp);
            }
        }

        for (int i = 0; i < 64; i++) {
            if ((other.b & (1L << i)) != 0) {
                QuantumFieldElement temp = this.shiftLeft(i + 64);
                result = result.add(temp);
            }
        }

        result.reduce();
        return result;
    }

    /**
     * Fast left shift with overflow management
     */
    public QuantumFieldElement shiftLeft(int bits) {
        QuantumFieldElement result = new QuantumFieldElement(this);

        if (bits >= 128) {
            result.a = 0;
            result.b = 0;
            return result;
        }

        if (bits >= 64) {
            result.b = result.a << (bits - 64);
            result.a = 0;
        } else {
            long highBits = result.a >>> (64 - bits);
            result.b = (result.b << bits) | highBits;
            result.a = result.a << bits;
        }

        // Store overflow in carry accumulator
        result.c = result.b >>> 64;
        result.b &= 0xFFFFFFFFFFFFFFFFL;

        return result;
    }

    /**
     * Modular reduction using secp256k1 prime
     */
    public void reduce() {
        // Fast reduction for secp256k1
        // P = 2^256 - 2^32 - 2^9 - 2^8 - 2^7 - 2^6 - 2^4 - 1

        while (this.c > 0 || this.b > P_HIGH || (this.b == P_HIGH && this.a >= P_LOW)) {
            // Subtract P using bit manipulation
            long borrow = 0;

            // Subtract low part
            long diffLow = this.a - P_LOW;
            if (diffLow < 0) {
                diffLow += getTwoPow32(); // Add 2^32
                borrow = 1;
            }
            this.a = diffLow & 0xFFFFFFFFFFFFFFFFL;

            // Subtract high part
            long diffHigh = this.b - P_HIGH - borrow;
            if (diffHigh < 0) {
                diffHigh += getTwoPow32(); // Add 2^32
                this.c = (this.c > 0) ? this.c - 1 : -1L;
            }
            this.b = diffHigh & 0xFFFFFFFFFFFFFFFFL;
        }
    }

    /**
     * Modular inverse using Fermat's Little Theorem (fast for prime fields)
     */
    public QuantumFieldElement modInverse() {
        // For secp256k1, use Fermat's Little Theorem: a^(p-2) mod p
        QuantumFieldElement result = new QuantumFieldElement(1);
        QuantumFieldElement base = new QuantumFieldElement(this);
        long exponent = 0xFFFFFFFEFFFFFC2DL; // P - 2

        // Fast exponentiation using bit manipulation
        while (exponent > 0) {
            if ((exponent & 1) == 1) {
                result = result.multiply(base);
            }
            base = base.multiply(base);
            exponent >>>= 1;
        }

        return result;
    }

    /**
     * Fast squaring (optimization of multiplication)
     */
    public QuantumFieldElement square() {
        return this.multiply(this);
    }

    /**
     * Get the full 128-bit value as two longs
     */
    public long[] getValue() {
        return new long[]{this.b, this.a};
    }

    /**
     * Set value from two longs
     */
    public void setValue(long high, long low) {
        this.a = low & 0xFFFFFFFFFFFFFFFFL;
        this.b = high & 0xFFFFFFFFFFFFFFFFL;
        this.c = 0;
    }

    /**
     * Convert to string representation
     */
    @Override
    public String toString() {
        return String.format("0x%016X%016X", this.b, this.a);
    }

    /**
     * Check equality
     */
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof QuantumFieldElement)) return false;
        QuantumFieldElement other = (QuantumFieldElement) obj;
        return this.a == other.a && this.b == other.b;
    }

    @Override
    public int hashCode() {
        return Long.hashCode(this.a) * 31 + Long.hashCode(this.b);
    }
}
