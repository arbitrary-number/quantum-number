package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.Map;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.QuantumNumberComponent;

public class ComplexQuantumNumberTest {

    private Complex one = new Complex(1, 0);
    private Complex i = new Complex(0, 1);
    private Complex half = new Complex(0.5, 0);

    @BeforeEach
    public void setup() {
        // No setup needed for QuantumNumberComponent as they are static constants
    }

    @Test
    public void testAddComponentAndGetCoefficient() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber();
        cqn.addComponent(QuantumNumberComponent.a, one);
        assertEquals(one, cqn.getCoefficient(QuantumNumberComponent.a));

        cqn.addComponent(QuantumNumberComponent.b, i);
        assertEquals(i, cqn.getCoefficient(QuantumNumberComponent.b));

        cqn.addComponent(QuantumNumberComponent.a, new Complex(0, 0)); // no change
        assertEquals(one, cqn.getCoefficient(QuantumNumberComponent.a));
    }

    @Test
    public void testAddComplexQuantumNumbers() {
        ComplexQuantumNumber cqn1 = new ComplexQuantumNumber(Map.of(QuantumNumberComponent.a, one));
        ComplexQuantumNumber cqn2 = new ComplexQuantumNumber(Map.of(QuantumNumberComponent.b, i));

        ComplexQuantumNumber sum = cqn1.add(cqn2);
        assertEquals(one, sum.getCoefficient(QuantumNumberComponent.a));
        assertEquals(i, sum.getCoefficient(QuantumNumberComponent.b));

        ComplexQuantumNumber cqn3 = new ComplexQuantumNumber(Map.of(QuantumNumberComponent.a, half));
        ComplexQuantumNumber sum2 = cqn1.add(cqn3);
        assertEquals(new Complex(1.5, 0), sum2.getCoefficient(QuantumNumberComponent.a));
    }

    @Test
    public void testMultiplyScalar() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber(Map.of(QuantumNumberComponent.a, one, QuantumNumberComponent.b, i));
        Complex scalar = new Complex(2, 0);

        ComplexQuantumNumber result = cqn.multiply(scalar);
        assertEquals(new Complex(2, 0), result.getCoefficient(QuantumNumberComponent.a));
        assertEquals(new Complex(0, 2), result.getCoefficient(QuantumNumberComponent.b));
    }

    @Test
    public void testNormSquaredAndNormalize() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber();
        cqn.addComponent(QuantumNumberComponent.a, new Complex(3, 4)); // magnitude 5

        assertEquals(25.0, cqn.normSquared(), 1e-10);

        ComplexQuantumNumber normalized = cqn.normalize();
        double normSq = normalized.normSquared();
        assertEquals(1.0, normSq, 1e-10);

        Complex coeff = normalized.getCoefficient(QuantumNumberComponent.a);
        assertEquals(3.0 / 5.0, coeff.getReal(), 1e-10);
        assertEquals(4.0 / 5.0, coeff.getImaginary(), 1e-10);
    }

    @Test
    public void testZeroCoefficientsRemoved() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber();
        cqn.addComponent(QuantumNumberComponent.a, new Complex(1, 0));
        cqn.addComponent(QuantumNumberComponent.a, new Complex(-1, 0)); // sum zero -> removed

        assertEquals(Complex.ZERO, cqn.getCoefficient(QuantumNumberComponent.a));
        assertTrue(cqn.toString().equals("0") || !cqn.toString().contains(QuantumNumberComponent.a.toString()));
    }

    @Test
    public void testToStringNonEmpty() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber();
        cqn.addComponent(QuantumNumberComponent.a, one);
        String s = cqn.toString();
        assertTrue(s.contains(QuantumNumberComponent.a.toString()));
        assertTrue(s.contains(one.toString()));
    }
}
