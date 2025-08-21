package com.github.quantum_number.qubit;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import org.apache.commons.math3.complex.Complex;

public class ComplexQuantumNumber {

    private final Map<QuantumNumberComponent, Complex> components;

    public ComplexQuantumNumber() {
        this.components = new HashMap<>();
    }

    // Copy constructor
    public ComplexQuantumNumber(ComplexQuantumNumber other) {
        this.components = new HashMap<>(other.components);
    }

    public ComplexQuantumNumber(Map<QuantumNumberComponent, Complex> components) {
        this();
        if (components != null) {
            components.forEach((basis, coeff) -> {
                if (basis != null && coeff != null && !coeff.equals(Complex.ZERO)) {
                    this.components.put(basis, coeff);
                }
            });
        }
    }

    public void clearComponents() {
        components.clear();
    }

    public void scale(double scalar) {
        if (scalar == 0.0) {
            components.clear();
        } else {
            components.replaceAll((basis, coeff) -> coeff.multiply(scalar));
        }
    }

    /**
     * Add or update a component with the given coefficient.
     * If the coefficient is zero, remove the component.
     */
    public void addComponent(QuantumNumberComponent basis, Complex coefficient) {
        Objects.requireNonNull(basis, "QuantumNumberComponent cannot be null");
        Objects.requireNonNull(coefficient, "Coefficient cannot be null");

        Complex existing = components.getOrDefault(basis, Complex.ZERO);
        Complex sum = existing.add(coefficient);

        if (sum.equals(Complex.ZERO)) {
            components.remove(basis);
        } else {
            components.put(basis, sum);
        }
    }

    public double magnitudeSquared() {
        return normSquared();
    }


    public boolean hasComponent(QuantumNumberComponent basis) {
        return components.containsKey(basis);
    }

    public Complex getCoefficient(QuantumNumberComponent basis) {
        return components.getOrDefault(basis, Complex.ZERO);
    }

    public Map<QuantumNumberComponent, Complex> getComponents() {
        return Collections.unmodifiableMap(components);
    }

    /**
     * Computes the sum of the squared magnitudes of all components.
     */
    public double normSquared() {
        return components.values().stream()
                .mapToDouble(c -> c.abs() * c.abs())
                .sum();
    }

    /**
     * Multiply all coefficients by the given scalar (Complex).
     * Returns a new ComplexQuantumNumber.
     */
    public ComplexQuantumNumber multiply(Complex scalar) {
        if (scalar.equals(Complex.ZERO)) {
            return new ComplexQuantumNumber(); // zero vector
        }
        ComplexQuantumNumber result = new ComplexQuantumNumber();
        components.forEach((basis, coeff) -> {
            result.addComponent(basis, coeff.multiply(scalar));
        });
        return result;
    }

    /**
     * Add another ComplexQuantumNumber to this one (component-wise).
     * Returns a new ComplexQuantumNumber.
     */
    public ComplexQuantumNumber add(ComplexQuantumNumber other) {
        ComplexQuantumNumber result = new ComplexQuantumNumber(this);
        other.components.forEach((basis, coeff) -> {
            Complex existing = result.getCoefficient(basis);
            Complex sum = existing.add(coeff);
            if (sum.equals(Complex.ZERO)) {
                result.components.remove(basis);
            } else {
                result.components.put(basis, sum);
            }
        });
        return result;
    }

    /**
     * Returns true if there are no components (zero vector).
     */
    public boolean isZero() {
        return components.isEmpty();
    }

    /**
     * Return a new ComplexQuantumNumber normalized to unit norm.
     * Throws ArithmeticException if norm is zero.
     */
    public ComplexQuantumNumber normalize() {
        double norm = Math.sqrt(normSquared());
        if (norm == 0) {
            throw new ArithmeticException("Cannot normalize zero quantum number");
        }
        return multiply(new Complex(1.0 / norm, 0));
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof ComplexQuantumNumber)) return false;
        ComplexQuantumNumber that = (ComplexQuantumNumber) o;
        return Objects.equals(components, that.components);
    }

    @Override
    public int hashCode() {
        return Objects.hash(components);
    }

    @Override
    public String toString() {
        if (components.isEmpty()) return "0";
        StringBuilder sb = new StringBuilder();
        components.forEach((basis, coeff) -> {
            sb.append(coeff).append("*").append(basis).append(" + ");
        });
        // Remove trailing " + "
        if (sb.length() >= 3) {
            sb.setLength(sb.length() - 3);
        }
        return sb.toString();
    }
}
