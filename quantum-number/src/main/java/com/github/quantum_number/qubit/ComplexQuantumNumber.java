package com.github.quantum_number.qubit;
import java.util.HashMap;
import java.util.Map;

/**
 * Represents an additive superposition of QuantumNumbers,
 * weighted by complex coefficients.
 */
public class ComplexQuantumNumber {
    // Map basis QuantumNumber -> complex coefficient
    private final Map<QuantumNumber, Complex> components;

    public ComplexQuantumNumber() {
        this.components = new HashMap<>();
    }

    public ComplexQuantumNumber(Map<QuantumNumber, Complex> components) {
        this.components = new HashMap<>(components);
    }

    /**
     * Add a component: coefficient * qn
     * Adds to existing coefficient if present.
     */
    public void addComponent(QuantumNumber qn, Complex coefficient) {
        if (coefficient.equals(Complex.ZERO)) return;
        components.merge(qn, coefficient, Complex::add);
        // Remove zero coefficients to keep map clean
        if (components.get(qn).equals(Complex.ZERO)) {
            components.remove(qn);
        }
    }

    /**
     * Return the complex coefficient for the given QuantumNumber basis,
     * or zero if not present.
     */
    public Complex getCoefficient(QuantumNumber qn) {
        return components.getOrDefault(qn, Complex.ZERO);
    }

    /**
     * Scalar multiplication of this ComplexQuantumNumber by a complex scalar.
     */
    public ComplexQuantumNumber multiply(Complex scalar) {
        Map<QuantumNumber, Complex> result = new HashMap<>();
        for (var entry : components.entrySet()) {
            result.put(entry.getKey(), entry.getValue().multiply(scalar));
        }
        return new ComplexQuantumNumber(result);
    }

    /**
     * Add two ComplexQuantumNumbers together.
     */
    public ComplexQuantumNumber add(ComplexQuantumNumber other) {
        Map<QuantumNumber, Complex> result = new HashMap<>(this.components);
        for (var entry : other.components.entrySet()) {
            result.merge(entry.getKey(), entry.getValue(), Complex::add);
        }
        // Clean zero coefficients
        result.entrySet().removeIf(e -> e.getValue().equals(Complex.ZERO));
        return new ComplexQuantumNumber(result);
    }

    /**
     * Compute the squared norm (sum of squared magnitudes of coefficients).
     */
    public double normSquared() {
        double sum = 0.0;
        for (Complex coeff : components.values()) {
            double mag = coeff.magnitude();
            sum += mag * mag;
        }
        return sum;
    }

    /**
     * Normalize this ComplexQuantumNumber (in place).
     */
    public ComplexQuantumNumber normalize() {
        double norm = Math.sqrt(normSquared());
        if (norm == 0) return this;
        return multiply(new Complex(1.0 / norm, 0));
    }

    @Override
    public String toString() {
        if (components.isEmpty()) return "0";
        StringBuilder sb = new StringBuilder();
        for (var entry : components.entrySet()) {
            sb.append(entry.getValue().toString())
              .append(" * ")
              .append(entry.getKey().toString())
              .append(" + ");
        }
        sb.setLength(sb.length() - 3); // Remove last " + "
        return sb.toString();
    }

    // Equals and hashCode can be added as needed
}
