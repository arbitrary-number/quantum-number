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
//    public ComplexQuantumNumber multiply(Complex scalar) {
//        if (scalar.equals(Complex.ZERO)) {
//            return new ComplexQuantumNumber(); // zero vector
//        }
//        ComplexQuantumNumber result = new ComplexQuantumNumber();
//        components.forEach((basis, coeff) -> {
//            result.addComponent(basis, coeff.multiply(scalar));
//        });
//        return result;
//    }
    public ComplexQuantumNumber multiply(Complex scalar) {
        if (scalar.equals(Complex.ZERO)) {
            return new ComplexQuantumNumber(); // zero vector
        }
        ComplexQuantumNumber result = new ComplexQuantumNumber();
        for (Map.Entry<QuantumNumberComponent, Complex> entry :
        		components.entrySet()) {
            QuantumNumberComponent basis = entry.getKey();
            Complex coeff = entry.getValue();
            result.addComponent(basis, coeff.multiply(scalar));
        }
        return result;
    }

 // In ComplexQuantumNumber class
    public ComplexQuantumNumber multiply(ComplexQuantumNumber other) {
        ComplexQuantumNumber result = new ComplexQuantumNumber();
        for (QuantumNumberComponent comp1 : this.getComponents().keySet()) {
            Complex coeff1 = this.getCoefficient(comp1);
            for (QuantumNumberComponent comp2 : other.getComponents().keySet()) {
                Complex coeff2 = other.getCoefficient(comp2);
                // Combine components if needed or just add them;
                // assuming you only have one component 'a' for now:
                if (comp1.equals(comp2)) {
                    result.addComponent(comp1, coeff1.multiply(coeff2));
                } else {
                    // If different components exist, handle accordingly
                    // For simplicity, ignoring or throwing error here
                }
            }
        }
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
//
//    public static ComplexQuantumNumber tensorProduct(ComplexQuantumNumber amp1, ComplexQuantumNumber amp2) {
//        ComplexQuantumNumber result = new ComplexQuantumNumber();
//
//        for (QuantumNumberComponent comp1 : amp1.getComponents().keySet()) {
//            Complex coeff1 = amp1.getCoefficient(comp1);
//
//            for (QuantumNumberComponent comp2 : amp2.getComponents().keySet()) {
//                Complex coeff2 = amp2.getCoefficient(comp2);
//
//                QuantumNumberComponent combinedComponent = QuantumNumberComponent.combine(comp1, comp2);
//                Complex combinedCoeff = coeff1.multiply(coeff2);
//
//                result.addComponent(combinedComponent, combinedCoeff);
//            }
//        }
//
//        return result;
//    }

//    public static ComplexQuantumNumber tensorProduct(ComplexQuantumNumber amp1, ComplexQuantumNumber amp2) {
//        ComplexQuantumNumber result = new ComplexQuantumNumber();
//
//        for (QuantumNumberComponent comp1 : amp1.getComponents().keySet()) {
//            Complex coeff1 = amp1.getCoefficient(comp1);
//
//            for (QuantumNumberComponent comp2 : amp2.getComponents().keySet()) {
//                Complex coeff2 = amp2.getCoefficient(comp2);
//
//                // Combine the quantum number components
//                QuantumNumberComponent combinedComponent = QuantumNumberComponent.combine(comp1, comp2);
//
//                // Multiply coefficients (complex numbers)
//                Complex combinedCoeff = coeff1.multiply(coeff2);
//
//                // Add combined component and coefficient to result
//                result.addComponent(combinedComponent, combinedCoeff);
//            }
//        }
//
//        return result;
//    }


    public static ComplexQuantumNumber tensorProduct(ComplexQuantumNumber amp1, ComplexQuantumNumber amp2) {
        ComplexQuantumNumber result = new ComplexQuantumNumber();

        for (Map.Entry<QuantumNumberComponent, Complex> entry1 : amp1.getComponents().entrySet()) {
            QuantumNumberComponent comp1 = entry1.getKey();
            Complex coeff1 = entry1.getValue();

            for (Map.Entry<QuantumNumberComponent, Complex> entry2 : amp2.getComponents().entrySet()) {
                QuantumNumberComponent comp2 = entry2.getKey();
                Complex coeff2 = entry2.getValue();

                QuantumNumberComponent combinedComponent = QuantumNumberComponent.combine(comp1, comp2);
                if (combinedComponent == null) {
                    // Handle error or skip if components can't be combined
                    continue;
                }

                Complex combinedCoeff = coeff1.multiply(coeff2);
                result.addComponent(combinedComponent, combinedCoeff);
            }
        }

        return result;
    }


    /**
     * Adds all components from another ComplexQuantumNumber into this one,
     * summing coefficients for matching components.
     * @param other the ComplexQuantumNumber whose components will be added
     */
    public void addComponents(ComplexQuantumNumber other) {
        for (QuantumNumberComponent comp : other.getComponents().keySet()) {
            Complex coeff = other.getCoefficient(comp);
            this.addComponent(comp, coeff);
        }
    }

}
