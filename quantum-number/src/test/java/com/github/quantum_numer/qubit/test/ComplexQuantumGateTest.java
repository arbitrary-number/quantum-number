package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.*;

import com.github.quantum_number.qubit.*;
import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class ComplexQuantumGateTest {

    private QuantumNumberComponent a;
    private Complex one = new Complex(1, 0);
    private Complex zero = Complex.ZERO;

    private ComplexQuantumRegister input;

    @BeforeEach
    public void setup() {
        // Simple basis
        a = QuantumNumberComponent.a;

        // |ψ⟩ = [1, 0]
        input = new ComplexQuantumRegister(2);
        ComplexQuantumNumber state0 = new ComplexQuantumNumber();
        state0.addComponent(a, one);

        input.setAmplitude(0, state0); // |0⟩ state
        input.setAmplitude(1, new ComplexQuantumNumber()); // |1⟩ is zero
    }

    @Test
    public void testIdentityGate() {
        Complex[][] identity = {
            {one, zero},
            {zero, one}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(identity);
        ComplexQuantumRegister result = gate.apply(input);

        assertEquals(input.getAmplitude(0), result.getAmplitude(0));
        assertEquals(input.getAmplitude(1), result.getAmplitude(1));
    }

    @Test
    public void testXGate() {
        // Pauli-X gate
        Complex[][] xGate = {
            {zero, one},
            {one, zero}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(xGate);
        ComplexQuantumRegister result = gate.apply(input);

        assertEquals(input.getAmplitude(0), result.getAmplitude(1));
        assertTrue(result.getAmplitude(0).isZero());
    }

    @Test
    public void testHadamardGate() {
        double sqrt2inv = 1.0 / Math.sqrt(2);
        Complex h = new Complex(sqrt2inv, 0);

        Complex[][] hadamard = {
            {h, h},
            {h, h.negate()}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(hadamard);
        ComplexQuantumRegister result = gate.apply(input);

        ComplexQuantumNumber out0 = result.getAmplitude(0);
        ComplexQuantumNumber out1 = result.getAmplitude(1);

        Complex expected = new Complex(sqrt2inv, 0);
        assertEquals(expected.getReal(), out0.getCoefficient(a).getReal(), 1e-10);
        assertEquals(expected.getImaginary(), out0.getCoefficient(a).getImaginary(), 1e-10);
    }

    @Test
    public void testInvalidDimensions() {
        Complex[][] badMatrix = {
            {one, zero}
            // Not square
        };

        assertThrows(IllegalArgumentException.class, () -> new ComplexQuantumGate(badMatrix));
    }

    @Test
    public void testMismatchedInputSize() {
        Complex[][] matrix = {
            {one, zero},
            {zero, one}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(matrix);

        ComplexQuantumRegister smallRegister = new ComplexQuantumRegister(1);

        assertThrows(IllegalArgumentException.class, () -> gate.apply(smallRegister));
    }

    @Test
    public void testYGate() {
        Complex i = Complex.I;
        Complex minusI = i.negate();

        Complex[][] yGate = {
            {zero, minusI},
            {i, zero}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(yGate);
        ComplexQuantumRegister result = gate.apply(input);

        Complex expected = Complex.I; // Pauli-Y on |0⟩ → i|1⟩
        assertTrue(result.getAmplitude(0).isZero());
        assertComplexEquals(expected, result.getAmplitude(1).getCoefficient(a), 1e-10);
    }

    @Test
    public void testZGate() {
        Complex[][] zGate = {
            {one, zero},
            {zero, one.negate()}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(zGate);
        ComplexQuantumRegister result = gate.apply(input);

        // Should remain |0⟩ unchanged
        assertEquals(input.getAmplitude(0), result.getAmplitude(0));
        assertTrue(result.getAmplitude(1).isZero());
    }

    @Test
    public void testPhaseGate() {
        Complex i = Complex.I;

        Complex[][] sGate = {
            {one, zero},
            {zero, i}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(sGate);
        ComplexQuantumRegister result = gate.apply(input);

        // Since |0⟩ → |0⟩ under phase gate
        assertEquals(input.getAmplitude(0), result.getAmplitude(0));
        assertTrue(result.getAmplitude(1).isZero());
    }

    @Test
    public void testCNOTGate() {
        // Setup 2-qubit register: |10⟩ (control = 1, target = 0)
        ComplexQuantumRegister twoQubitInput = new ComplexQuantumRegister(4);
        ComplexQuantumNumber state = new ComplexQuantumNumber();
        state.addComponent(a, one);
        twoQubitInput.setAmplitude(2, state); // |10⟩ → index 2

        // CNOT Gate (4x4)
        Complex[][] cnot = {
            {one, zero, zero, zero},
            {zero, one, zero, zero},
            {zero, zero, zero, one},
            {zero, zero, one, zero}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(cnot);
        ComplexQuantumRegister result = gate.apply(twoQubitInput);

        // CNOT on |10⟩ → |11⟩ → index 3
        assertTrue(result.getAmplitude(2).isZero());
        assertEquals(one, result.getAmplitude(3).getCoefficient(a));
    }

    private void assertComplexEquals(Complex expected, Complex actual, double delta) {
        assertEquals(expected.getReal(), actual.getReal(), delta, "Real parts differ");
        assertEquals(expected.getImaginary(), actual.getImaginary(), delta, "Imaginary parts differ");
    }

}
