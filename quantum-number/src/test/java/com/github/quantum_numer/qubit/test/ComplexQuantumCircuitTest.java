package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.*;

import com.github.quantum_number.qubit.*;
import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class ComplexQuantumCircuitTest {

    private ComplexQuantumGate hadamardGate;
    private ComplexQuantumGate identityGate;
    private ComplexQuantumGate xGate;
    private ComplexQuantumRegister input;

    @BeforeEach
    public void setup() {
        double sqrt2inv = 1.0 / Math.sqrt(2);
        Complex h = new Complex(sqrt2inv, 0);
        Complex one = Complex.ONE;
        Complex zero = Complex.ZERO;

        Complex[][] hadamard = {
            {h, h},
            {h, h.negate()}
        };

        Complex[][] identity = {
            {one, zero},
            {zero, one}
        };

        Complex[][] x = {
            {zero, one},
            {one, zero}
        };

        hadamardGate = new ComplexQuantumGate(hadamard);
        identityGate = new ComplexQuantumGate(identity);
        xGate = new ComplexQuantumGate(x);

        input = ComplexQuantumRegister.ofDimension(2);
        // Initialize |0⟩ state: amplitude[0] = |a⟩ with coeff 1
        input.getAmplitude(0).addComponent(QuantumNumberComponent.a, Complex.ONE);
        for (int i = 1; i < input.size(); i++) {
            input.getAmplitude(i).clearComponents();
        }
    }

    @Test
    public void testAddGate() {
        ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
        assertEquals(0, circuit.size());

        circuit.addGate(hadamardGate);
        assertEquals(1, circuit.size());

        circuit.addGate(xGate);
        assertEquals(2, circuit.size());
    }

    @Test
    public void testRunSingleGate() {
        ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
        circuit.addGate(hadamardGate);

        ComplexQuantumRegister result = circuit.run(input);

        // Result amplitude 0 coefficient approx 1/sqrt(2)
        Complex expected = new Complex(1.0 / Math.sqrt(2), 0);
        ComplexQuantumNumber out0 = result.getAmplitude(0);

        assertEquals(expected.getReal(), out0.getCoefficient(QuantumNumberComponent.a).getReal(), 1e-10);
        assertEquals(expected.getImaginary(), out0.getCoefficient(QuantumNumberComponent.a).getImaginary(), 1e-10);
    }

    @Test
    public void testRunMultipleGates() {
        ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
        circuit.addGate(hadamardGate);
        circuit.addGate(xGate);

        ComplexQuantumRegister result = circuit.run(input);

        // After Hadamard then X on |0⟩
        // Expected: amplitude at |1⟩ is 1/sqrt(2)
        Complex expected = new Complex(1.0 / Math.sqrt(2), 0);
        ComplexQuantumNumber out1 = result.getAmplitude(1);

        assertEquals(expected.getReal(), out1.getCoefficient(QuantumNumberComponent.a).getReal(), 1e-10);
        assertEquals(expected.getImaginary(), out1.getCoefficient(QuantumNumberComponent.a).getImaginary(), 1e-10);
    }

    @Test
    public void testClearCircuit() {
        ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
        circuit.addGate(hadamardGate);
        circuit.addGate(xGate);

        assertEquals(2, circuit.size());

        circuit.clear();
        assertEquals(0, circuit.size());
    }

    @Test
    public void testAddNullGateThrows() {
        ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
        assertThrows(IllegalArgumentException.class, () -> circuit.addGate(null));
    }
}
