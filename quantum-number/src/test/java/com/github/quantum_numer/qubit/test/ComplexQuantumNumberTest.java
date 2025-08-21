package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.Map;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.Complex;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.QuantumNumber;

public class ComplexQuantumNumberTest {

    private QuantumNumber a;
    private QuantumNumber b;
    private QuantumNumber c;
    private QuantumNumber d;
    private QuantumNumber e;
    private QuantumNumber f;
    private QuantumNumber g;

    private Complex one = new Complex(1, 0);
    private Complex i = new Complex(0, 1);
    private Complex half = new Complex(0.5, 0);

    @BeforeEach
    public void setup() {
        // Example ordinals for a-g (mostly one ordinal set to 1, others zero)
        a = new QuantumNumber(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        b = new QuantumNumber(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        c = new QuantumNumber(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        d = new QuantumNumber(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0);
        e = new QuantumNumber(0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);
        f = new QuantumNumber(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
        g = new QuantumNumber(0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
    }

    @Test
    public void testAddComponentAndGetCoefficient() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber();
        cqn.addComponent(a, one);
        assertEquals(one, cqn.getCoefficient(a));

        cqn.addComponent(b, i);
        assertEquals(i, cqn.getCoefficient(b));

        cqn.addComponent(a, new Complex(0, 0)); // no change
        assertEquals(one, cqn.getCoefficient(a));
    }

    @Test
    public void testAddComplexQuantumNumbers() {
        ComplexQuantumNumber cqn1 = new ComplexQuantumNumber(Map.of(a, one));
        ComplexQuantumNumber cqn2 = new ComplexQuantumNumber(Map.of(b, i));

        ComplexQuantumNumber sum = cqn1.add(cqn2);
        assertEquals(one, sum.getCoefficient(a));
        assertEquals(i, sum.getCoefficient(b));

        ComplexQuantumNumber cqn3 = new ComplexQuantumNumber(Map.of(a, half));
        ComplexQuantumNumber sum2 = cqn1.add(cqn3);
        assertEquals(new Complex(1.5, 0), sum2.getCoefficient(a));
    }

    @Test
    public void testMultiplyScalar() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber(Map.of(a, one, b, i));
        Complex scalar = new Complex(2, 0);

        ComplexQuantumNumber result = cqn.multiply(scalar);
        assertEquals(new Complex(2, 0), result.getCoefficient(a));
        assertEquals(new Complex(0, 2), result.getCoefficient(b));
    }

    @Test
    public void testNormSquaredAndNormalize() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber();
        cqn.addComponent(a, new Complex(3, 4)); // magnitude 5

        assertEquals(25.0, cqn.normSquared(), 1e-10);

        ComplexQuantumNumber normalized = cqn.normalize();
        double normSq = normalized.normSquared();
        assertEquals(1.0, normSq, 1e-10);

        Complex coeff = normalized.getCoefficient(a);
        assertEquals(3.0/5.0, coeff.real(), 1e-10);
        assertEquals(4.0/5.0, coeff.imag(), 1e-10);
    }

    @Test
    public void testZeroCoefficientsRemoved() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber();
        cqn.addComponent(a, new Complex(1, 0));
        cqn.addComponent(a, new Complex(-1, 0)); // sum zero -> removed

        assertEquals(Complex.ZERO, cqn.getCoefficient(a));
        assertTrue(cqn.toString().equals("0") || !cqn.toString().contains(a.toString()));
    }

    @Test
    public void testToStringNonEmpty() {
        ComplexQuantumNumber cqn = new ComplexQuantumNumber();
        cqn.addComponent(a, one);
        String s = cqn.toString();
        assertTrue(s.contains(a.toString()));
        assertTrue(s.contains(one.toString()));
    }
}
