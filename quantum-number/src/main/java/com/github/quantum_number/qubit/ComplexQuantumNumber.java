/*
 * Copyright (c) Arbitrary Number Project Team. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
package com.github.quantum_number.qubit;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import org.apache.commons.math3.complex.Complex;

public class ComplexQuantumNumber {

    // Map basis component -> complex coefficient
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
        components.forEach(this::addComponent);
    }


    /**
     * Add or update a component with the given coefficient.
     * If the coefficient is zero, remove the component.
     */
    public void addComponent(QuantumNumberComponent basis, Complex coefficient) {
        Objects.requireNonNull(basis, "QuantumNumberComponent cannot be null");
        Objects.requireNonNull(coefficient, "Coefficient cannot be null");
        if (coefficient.equals(Complex.ZERO)) {
            components.remove(basis);
        } else {
            components.put(basis, coefficient);
        }
    }

    /**
     * Check if this ComplexQuantumNumber has the given basis component.
     */
    public boolean hasComponent(QuantumNumberComponent basis) {
        return components.containsKey(basis);
    }

    /**
     * Get the coefficient associated with the basis component.
     * Returns Complex.ZERO if not present.
     */
    public Complex getCoefficient(QuantumNumberComponent basis) {
        return components.getOrDefault(basis, Complex.ZERO);
    }

    /**
     * Returns an unmodifiable view of the components map.
     */
    public Map<QuantumNumberComponent, Complex> getComponents() {
        return Collections.unmodifiableMap(components);
    }

    /**
     * Computes the sum of the squared magnitudes of all components.
     */
    public double magnitudeSquared() {
        return components.values().stream()
                .mapToDouble(c -> c.abs() * c.abs())
                .sum();
    }

    /**
     * Scale all coefficients by the given scalar.
     */
    public void scale(double scalar) {
        if (scalar == 0.0) {
            components.clear();
        } else {
            components.replaceAll((basis, coeff) -> coeff.multiply(scalar));
        }
    }

    /**
     * Returns true if there are no components (zero vector).
     */
    public boolean isZero() {
        return components.isEmpty();
    }

    /**
     * Add another ComplexQuantumNumber to this one (component-wise).
     */
    public ComplexQuantumNumber add(ComplexQuantumNumber other) {
        ComplexQuantumNumber result = new ComplexQuantumNumber();

        // Add all components from this
        for (Map.Entry<QuantumNumberComponent, Complex> entry : this.components.entrySet()) {
            result.addComponent(entry.getKey(), entry.getValue());
        }
        // Add all components from other
        for (Map.Entry<QuantumNumberComponent, Complex> entry : other.components.entrySet()) {
            Complex existing = result.getCoefficient(entry.getKey());
            result.addComponent(entry.getKey(), existing.add(entry.getValue()));
        }
        return result;
    }

    // Multiply all coefficients by scalar
    public ComplexQuantumNumber multiply(Complex scalar) {
        ComplexQuantumNumber result = new ComplexQuantumNumber();
        for (Map.Entry<QuantumNumberComponent, Complex> entry : components.entrySet()) {
            result.addComponent(entry.getKey(), entry.getValue().multiply(scalar));
        }
        return result;
    }

    // Compute sum of squared magnitudes of coefficients
    public double normSquared() {
        double sum = 0.0;
        for (Complex c : components.values()) {
            sum += c.abs() * c.abs();
        }
        return sum;
    }

    // Return a new ComplexQuantumNumber normalized to unit norm
    public ComplexQuantumNumber normalize() {
        double norm = Math.sqrt(normSquared());
        if (norm == 0) {
            throw new ArithmeticException("Cannot normalize zero quantum number");
        }
        return this.multiply(new Complex(1.0 / norm, 0));
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
        for (Map.Entry<QuantumNumberComponent, Complex> entry : components.entrySet()) {
            sb.append(entry.getValue())
              .append("*")
              .append(entry.getKey())
              .append(" + ");
        }
        // Remove trailing " + "
        return sb.substring(0, sb.length() - 3);
    }
}
