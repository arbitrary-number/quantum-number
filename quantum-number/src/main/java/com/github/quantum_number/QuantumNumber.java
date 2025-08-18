package com.github.quantum_number;

/**
 * QuantumNumber - 256-bit Symbolic Numeric Unit
 * 
 * Represents the fundamental building block of the Quantum Number System.
 * A Quantum Number is a 256-bit structured entity composed of:
 * - 12 ordinals (a-l): 20-bit signed integers
 * - 12 sign bits: independent sign control for each ordinal
 * - 4-bit checksum: integrity verification
 * 
 * The structure represents the nested division expression:
 * (±a ± g) / (±b ± g) / (±c ± h) / ((±d(±b ± h)) / (±eb(±i)) / (±fb(±j)))
 * 
 * @author Quantum Number Project Team
 * @version 1.0
 */
public class QuantumNumber {
    
    // 12 ordinals (a, b, c, d, e, f, g, h, i, j, k, l)
    // Each ordinal is a 20-bit signed integer (-524288 to 524287)
    private int[] ordinals;
    
    // 12 sign bits (one for each ordinal)
    private boolean[] signs;
    
    // 4-bit checksum for integrity verification
    private byte checksum;
    
    // Constants
    public static final int NUM_ORDINALS = 12;
    public static final int ORDINAL_MIN = -524288;  // -2^19
    public static final int ORDINAL_MAX = 524287;   // 2^19 - 1
    public static final String[] ORDINAL_NAMES = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l"};
    
    /**
     * Default constructor - creates a zero Quantum Number
     */
    public QuantumNumber() {
        this.ordinals = new int[NUM_ORDINALS];
        this.signs = new boolean[NUM_ORDINALS];
        this.checksum = 0;
        
        // Initialize to mathematical identity (most ordinals = 1)
        for (int i = 0; i < NUM_ORDINALS; i++) {
            this.ordinals[i] = (i == 6 || i == 7) ? 0 : 1; // g=0, h=0, others=1
            this.signs[i] = false; // positive
        }
        updateChecksum();
    }
    
    /**
     * Constructor with ordinal values
     */
    public QuantumNumber(int[] ordinals, boolean[] signs) {
        if (ordinals.length != NUM_ORDINALS || signs.length != NUM_ORDINALS) {
            throw new IllegalArgumentException("Must provide exactly " + NUM_ORDINALS + " ordinals and signs");
        }
        
        this.ordinals = new int[NUM_ORDINALS];
        this.signs = new boolean[NUM_ORDINALS];
        
        for (int i = 0; i < NUM_ORDINALS; i++) {
            setOrdinal(i, ordinals[i]);
            this.signs[i] = signs[i];
        }
        
        updateChecksum();
    }
    
    /**
     * Copy constructor
     */
    public QuantumNumber(QuantumNumber other) {
        this.ordinals = other.ordinals.clone();
        this.signs = other.signs.clone();
        this.checksum = other.checksum;
    }
    
    /**
     * Set an ordinal value with bounds checking
     */
    public void setOrdinal(int index, int value) {
        if (index < 0 || index >= NUM_ORDINALS) {
            throw new IndexOutOfBoundsException("Ordinal index must be 0-" + (NUM_ORDINALS - 1));
        }
        if (value < ORDINAL_MIN || value > ORDINAL_MAX) {
            throw new IllegalArgumentException("Ordinal value must be between " + ORDINAL_MIN + " and " + ORDINAL_MAX);
        }
        this.ordinals[index] = value;
    }
    
    /**
     * Get an ordinal value
     */
    public int getOrdinal(int index) {
        if (index < 0 || index >= NUM_ORDINALS) {
            throw new IndexOutOfBoundsException("Ordinal index must be 0-" + (NUM_ORDINALS - 1));
        }
        return this.ordinals[index];
    }
    
    /**
     * Set a sign bit
     */
    public void setSign(int index, boolean negative) {
        if (index < 0 || index >= NUM_ORDINALS) {
            throw new IndexOutOfBoundsException("Sign index must be 0-" + (NUM_ORDINALS - 1));
        }
        this.signs[index] = negative;
    }
    
    /**
     * Get a sign bit
     */
    public boolean getSign(int index) {
        if (index < 0 || index >= NUM_ORDINALS) {
            throw new IndexOutOfBoundsException("Sign index must be 0-" + (NUM_ORDINALS - 1));
        }
        return this.signs[index];
    }
    
    /**
     * Get signed ordinal value (applying sign bit)
     */
    public int getSignedOrdinal(int index) {
        int value = getOrdinal(index);
        return getSign(index) ? -value : value;
    }
    
    /**
     * Update the 4-bit checksum based on current ordinals and signs
     */
    public void updateChecksum() {
        int sum = 0;
        for (int i = 0; i < NUM_ORDINALS; i++) {
            sum += ordinals[i];
            if (signs[i]) sum += 1;
        }
        this.checksum = (byte)(sum & 0x0F); // 4-bit checksum
    }
    
    /**
     * Verify checksum integrity
     */
    public boolean verifyChecksum() {
        byte oldChecksum = this.checksum;
        updateChecksum();
        boolean valid = (oldChecksum == this.checksum);
        this.checksum = oldChecksum; // restore original
        return valid;
    }
    
    /**
     * Get the checksum value
     */
    public byte getChecksum() {
        return this.checksum;
    }
    
    /**
     * Create a zero Quantum Number (additive identity)
     */
    public static QuantumNumber zero() {
        QuantumNumber zero = new QuantumNumber();
        for (int i = 0; i < NUM_ORDINALS; i++) {
            zero.setOrdinal(i, 0);
        }
        zero.updateChecksum();
        return zero;
    }
    
    /**
     * Create a one Quantum Number (multiplicative identity)
     */
    public static QuantumNumber one() {
        QuantumNumber one = new QuantumNumber();
        // Default constructor already creates identity-like structure
        return one;
    }
    
    /**
     * Check if this is a zero Quantum Number
     */
    public boolean isZero() {
        for (int i = 0; i < NUM_ORDINALS; i++) {
            if (ordinals[i] != 0) return false;
        }
        return true;
    }
    
    /**
     * Symbolic addition of two Quantum Numbers
     * Returns a new QuantumNumber representing the sum
     */
    public QuantumNumber add(QuantumNumber other) {
        // For this reference implementation, we perform ordinal-wise addition
        // In a full implementation, this would create symbolic AST structures
        QuantumNumber result = new QuantumNumber();
        
        for (int i = 0; i < NUM_ORDINALS; i++) {
            int thisValue = this.getSignedOrdinal(i);
            int otherValue = other.getSignedOrdinal(i);
            int sum = thisValue + otherValue;
            
            // Handle overflow by clamping to valid range
            if (sum > ORDINAL_MAX) sum = ORDINAL_MAX;
            if (sum < ORDINAL_MIN) sum = ORDINAL_MIN;
            
            result.setOrdinal(i, Math.abs(sum));
            result.setSign(i, sum < 0);
        }
        
        result.updateChecksum();
        return result;
    }
    
    /**
     * Symbolic subtraction of two Quantum Numbers
     */
    public QuantumNumber subtract(QuantumNumber other) {
        QuantumNumber negated = other.negate();
        return this.add(negated);
    }
    
    /**
     * Negate this Quantum Number (flip all signs)
     */
    public QuantumNumber negate() {
        QuantumNumber result = new QuantumNumber(this);
        for (int i = 0; i < NUM_ORDINALS; i++) {
            result.setSign(i, !result.getSign(i));
        }
        result.updateChecksum();
        return result;
    }
    
    /**
     * Convert to 256-bit binary representation (conceptual)
     * In actual hardware, this would be the native format
     */
    public byte[] toBinaryRepresentation() {
        byte[] binary = new byte[32]; // 256 bits = 32 bytes
        
        // Pack ordinals (20 bits each) and signs (1 bit each) into binary format
        // This is a simplified representation for demonstration
        int bitOffset = 0;
        
        for (int i = 0; i < NUM_ORDINALS; i++) {
            // Pack 20-bit ordinal
            int value = ordinals[i];
            for (int bit = 0; bit < 20; bit++) {
                int byteIndex = bitOffset / 8;
                int bitIndex = bitOffset % 8;
                if ((value & (1 << bit)) != 0) {
                    binary[byteIndex] |= (1 << bitIndex);
                }
                bitOffset++;
            }
            
            // Pack sign bit
            int byteIndex = bitOffset / 8;
            int bitIndex = bitOffset % 8;
            if (signs[i]) {
                binary[byteIndex] |= (1 << bitIndex);
            }
            bitOffset++;
        }
        
        // Pack 4-bit checksum
        for (int bit = 0; bit < 4; bit++) {
            int byteIndex = bitOffset / 8;
            int bitIndex = bitOffset % 8;
            if ((checksum & (1 << bit)) != 0) {
                binary[byteIndex] |= (1 << bitIndex);
            }
            bitOffset++;
        }
        
        return binary;
    }
    
    /**
     * String representation showing the nested division structure
     */
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("QuantumNumber {\n");
        
        // Show ordinals with their signs, highlighting imaginary component
        for (int i = 0; i < NUM_ORDINALS; i++) {
            sb.append("  ").append(ORDINAL_NAMES[i]).append(" = ");
            sb.append(signs[i] ? "-" : "+").append(ordinals[i]);
            if (i == 8) { // ordinal 'i' is the imaginary component
                sb.append(" <imaginary>");
            }
            sb.append("\n");
        }
        
        sb.append("  checksum = 0x").append(Integer.toHexString(checksum & 0x0F).toUpperCase());
        sb.append("\n");
        
        // Show complex number representation
        sb.append("  Complex Form: ");
        sb.append("Real = (±a ± g) / (±b ± g) / (±c ± h) / ((±d(±b ± h)) / (±eb) / (±fb))");
        sb.append(", Imaginary = ").append(getSignedOrdinal(8)).append("i");
        sb.append("\n");
        
        // Show the mathematical structure
        sb.append("  Structure: (±a ± g) / (±b ± g) / (±c ± h) / ((±d(±b ± h)) / (±eb(±i)) / (±fb(±j)))\n");
        sb.append("  Expanded: ");
        sb.append("(").append(getSignedOrdinal(0)).append(" + ").append(getSignedOrdinal(6)).append(") / ");
        sb.append("(").append(getSignedOrdinal(1)).append(" + ").append(getSignedOrdinal(6)).append(") / ");
        sb.append("(").append(getSignedOrdinal(2)).append(" + ").append(getSignedOrdinal(7)).append(") / ");
        sb.append("((").append(getSignedOrdinal(3)).append("(").append(getSignedOrdinal(1)).append(" + ").append(getSignedOrdinal(7)).append(")) / ");
        sb.append("(").append(getSignedOrdinal(4)).append("*").append(getSignedOrdinal(1)).append("(").append(getSignedOrdinal(8)).append("i)) / ");
        sb.append("(").append(getSignedOrdinal(5)).append("*").append(getSignedOrdinal(1)).append("(").append(getSignedOrdinal(9)).append(")))");
        sb.append("\n}");
        
        return sb.toString();
    }
    
    /**
     * Get the real component of this Quantum Number
     */
    public QuantumNumber getRealComponent() {
        QuantumNumber real = new QuantumNumber(this);
        // Set imaginary ordinal to 1 (neutral) for real component
        real.setOrdinal(8, 1);
        real.setSign(8, false);
        real.updateChecksum();
        return real;
    }
    
    /**
     * Get the imaginary component of this Quantum Number
     */
    public int getImaginaryComponent() {
        return getSignedOrdinal(8); // ordinal 'i' represents the imaginary component
    }
    
    /**
     * Create a complex Quantum Number with real and imaginary components
     */
    public static QuantumNumber createComplex(QuantumNumber real, int imaginary) {
        QuantumNumber complex = new QuantumNumber(real);
        complex.setOrdinal(8, Math.abs(imaginary));
        complex.setSign(8, imaginary < 0);
        complex.updateChecksum();
        return complex;
    }
    
    /**
     * Create a complex Quantum Number from real and imaginary integer values
     */
    public static QuantumNumber createComplex(int real, int imaginary) {
        QuantumNumber complex = new QuantumNumber();
        // Set the primary real component (ordinal 'a')
        complex.setOrdinal(0, Math.abs(real));
        complex.setSign(0, real < 0);
        // Set imaginary component
        complex.setOrdinal(8, Math.abs(imaginary));
        complex.setSign(8, imaginary < 0);
        complex.updateChecksum();
        return complex;
    }
    
    /**
     * Complex multiplication: (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
     */
    public QuantumNumber multiplyComplex(QuantumNumber other) {
        // Extract real and imaginary components
        int thisReal = this.getSignedOrdinal(0);  // Use ordinal 'a' as primary real component
        int thisImag = this.getImaginaryComponent();
        int otherReal = other.getSignedOrdinal(0);
        int otherImag = other.getImaginaryComponent();
        
        // Complex multiplication formula: (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
        long realPart = (long)thisReal * otherReal - (long)thisImag * otherImag;
        long imagPart = (long)thisReal * otherImag + (long)thisImag * otherReal;
        
        // Clamp to valid range
        int clampedReal = (int)Math.max(ORDINAL_MIN, Math.min(ORDINAL_MAX, realPart));
        int clampedImag = (int)Math.max(ORDINAL_MIN, Math.min(ORDINAL_MAX, imagPart));
        
        return createComplex(clampedReal, clampedImag);
    }
    
    /**
     * Complex division: (a + bi) / (c + di) = ((ac + bd) + (bc - ad)i) / (c² + d²)
     */
    public QuantumNumber divideComplex(QuantumNumber other) {
        int thisReal = this.getSignedOrdinal(0);
        int thisImag = this.getImaginaryComponent();
        int otherReal = other.getSignedOrdinal(0);
        int otherImag = other.getImaginaryComponent();
        
        // Check for division by zero
        long denominator = (long)otherReal * otherReal + (long)otherImag * otherImag;
        if (denominator == 0) {
            throw new ArithmeticException("Division by zero in complex division");
        }
        
        // Complex division formula
        long realNumerator = (long)thisReal * otherReal + (long)thisImag * otherImag;
        long imagNumerator = (long)thisImag * otherReal - (long)thisReal * otherImag;
        
        int realPart = (int)(realNumerator / denominator);
        int imagPart = (int)(imagNumerator / denominator);
        
        return createComplex(realPart, imagPart);
    }
    
    /**
     * Complex conjugate: (a + bi)* = (a - bi)
     */
    public QuantumNumber conjugate() {
        QuantumNumber result = new QuantumNumber(this);
        result.setSign(8, !result.getSign(8)); // Flip imaginary sign
        result.updateChecksum();
        return result;
    }
    
    /**
     * Complex magnitude squared: |a + bi|² = a² + b²
     */
    public long magnitudeSquared() {
        int real = this.getSignedOrdinal(0);
        int imag = this.getImaginaryComponent();
        return (long)real * real + (long)imag * imag;
    }
    
    /**
     * Complex magnitude: |a + bi| = √(a² + b²)
     */
    public double magnitude() {
        return Math.sqrt(magnitudeSquared());
    }
    
    /**
     * Complex argument (phase angle) in radians
     */
    public double argument() {
        int real = this.getSignedOrdinal(0);
        int imag = this.getImaginaryComponent();
        return Math.atan2(imag, real);
    }
    
    /**
     * Check if this is a pure real number (imaginary component is 0 or 1)
     */
    public boolean isPureReal() {
        int imag = this.getImaginaryComponent();
        return imag == 0 || imag == 1;
    }
    
    /**
     * Check if this is a pure imaginary number (real component is 0)
     */
    public boolean isPureImaginary() {
        return this.getSignedOrdinal(0) == 0;
    }
    
    /**
     * Symbolic multiplication of two Quantum Numbers
     * For non-complex multiplication, this performs ordinal-wise operations
     */
    public QuantumNumber multiply(QuantumNumber other) {
        // For this reference implementation, we perform ordinal-wise multiplication
        // In a full implementation, this would create symbolic AST structures
        QuantumNumber result = new QuantumNumber();
        
        for (int i = 0; i < NUM_ORDINALS; i++) {
            long thisValue = this.getSignedOrdinal(i);
            long otherValue = other.getSignedOrdinal(i);
            long product = thisValue * otherValue;
            
            // Handle overflow by clamping to valid range
            if (product > ORDINAL_MAX) product = ORDINAL_MAX;
            if (product < ORDINAL_MIN) product = ORDINAL_MIN;
            
            result.setOrdinal(i, (int)Math.abs(product));
            result.setSign(i, product < 0);
        }
        
        result.updateChecksum();
        return result;
    }
    
    /**
     * Symbolic division of two Quantum Numbers
     */
    public QuantumNumber divide(QuantumNumber other) {
        // Check for division by zero
        if (other.isZero()) {
            throw new ArithmeticException("Division by zero");
        }
        
        QuantumNumber result = new QuantumNumber();
        
        for (int i = 0; i < NUM_ORDINALS; i++) {
            long thisValue = this.getSignedOrdinal(i);
            long otherValue = other.getSignedOrdinal(i);
            
            if (otherValue == 0) {
                // Handle division by zero in individual ordinal
                result.setOrdinal(i, ORDINAL_MAX);
                result.setSign(i, thisValue < 0);
            } else {
                long quotient = thisValue / otherValue;
                
                // Handle overflow by clamping to valid range
                if (quotient > ORDINAL_MAX) quotient = ORDINAL_MAX;
                if (quotient < ORDINAL_MIN) quotient = ORDINAL_MIN;
                
                result.setOrdinal(i, (int)Math.abs(quotient));
                result.setSign(i, quotient < 0);
            }
        }
        
        result.updateChecksum();
        return result;
    }
    
    /**
     * Power operation: raise this Quantum Number to an integer power
     */
    public QuantumNumber power(int exponent) {
        if (exponent == 0) {
            return QuantumNumber.one();
        }
        if (exponent == 1) {
            return new QuantumNumber(this);
        }
        if (exponent < 0) {
            throw new IllegalArgumentException("Negative exponents not supported in this implementation");
        }
        
        QuantumNumber result = QuantumNumber.one();
        QuantumNumber base = new QuantumNumber(this);
        
        // Exponentiation by squaring
        while (exponent > 0) {
            if (exponent % 2 == 1) {
                result = result.multiply(base);
            }
            base = base.multiply(base);
            exponent /= 2;
        }
        
        return result;
    }
    
    /**
     * Complex power using De Moivre's theorem: (r(cos θ + i sin θ))^n = r^n(cos nθ + i sin nθ)
     */
    public QuantumNumber complexPower(int exponent) {
        if (exponent == 0) {
            return createComplex(1, 0);
        }
        if (exponent == 1) {
            return new QuantumNumber(this);
        }
        
        double r = magnitude();
        double theta = argument();
        
        double newR = Math.pow(r, exponent);
        double newTheta = exponent * theta;
        
        int realPart = (int)(newR * Math.cos(newTheta));
        int imagPart = (int)(newR * Math.sin(newTheta));
        
        // Clamp to valid range
        realPart = Math.max(ORDINAL_MIN, Math.min(ORDINAL_MAX, realPart));
        imagPart = Math.max(ORDINAL_MIN, Math.min(ORDINAL_MAX, imagPart));
        
        return createComplex(realPart, imagPart);
    }
    
    /**
     * Equals method for Quantum Number comparison
     */
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        
        QuantumNumber other = (QuantumNumber) obj;
        
        for (int i = 0; i < NUM_ORDINALS; i++) {
            if (ordinals[i] != other.ordinals[i]) return false;
            if (signs[i] != other.signs[i]) return false;
        }
        
        return checksum == other.checksum;
    }
    
    /**
     * Hash code for Quantum Numbers
     */
    @Override
    public int hashCode() {
        int hash = 0;
        for (int i = 0; i < NUM_ORDINALS; i++) {
            hash = hash * 31 + ordinals[i];
            hash = hash * 31 + (signs[i] ? 1 : 0);
        }
        hash = hash * 31 + checksum;
        return hash;
    }
}
