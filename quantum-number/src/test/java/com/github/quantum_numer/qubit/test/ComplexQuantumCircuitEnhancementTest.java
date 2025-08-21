package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.*;

import com.github.quantum_number.qubit.*;
import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.List;

public class ComplexQuantumCircuitEnhancementTest {

    private ComplexQuantumGate hadamardGate;
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

        Complex[][] x = {
            {zero, one},
            {one, zero}
        };

        hadamardGate = new ComplexQuantumGate(hadamard);
        xGate = new ComplexQuantumGate(x);

        input = new ComplexQuantumRegister(2);
        // Initialize |0⟩ state: amplitude[0] = |a⟩ with coeff 1
        input.getAmplitude(0).addComponent(QuantumNumberComponent.a, Complex.ONE);
        for (int i = 1; i < input.size(); i++) {
            input.getAmplitude(i).clearComponents();
        }
    }

    @Test
    public void testInsertGate() {
        ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
        circuit.addGate(hadamardGate);

        circuit.insertGate(0, xGate);
        assertEquals(2, circuit.size());
        assertEquals(xGate, circuit.getGate(0));
        assertEquals(hadamardGate, circuit.getGate(1));

        // Insert at end
        circuit.insertGate(2, hadamardGate);
        assertEquals(3, circuit.size());
        assertEquals(hadamardGate, circuit.getGate(2));

        // Insert invalid index throws
        assertThrows(IndexOutOfBoundsException.class, () -> circuit.insertGate(4, xGate));
        assertThrows(IndexOutOfBoundsException.class, () -> circuit.insertGate(-1, xGate));

        // Insert null throws
        assertThrows(IllegalArgumentException.class, () -> circuit.insertGate(1, null));
    }

    @Test
    public void testRemoveGate() {
        ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
        circuit.addGate(hadamardGate);
        circuit.addGate(xGate);

        ComplexQuantumGate removed = circuit.removeGate(0);
        assertEquals(hadamardGate, removed);
        assertEquals(1, circuit.size());
        assertEquals(xGate, circuit.getGate(0));

        removed = circuit.removeGate(0);
        assertEquals(xGate, removed);
        assertEquals(0, circuit.size());

        // Remove invalid index throws
        assertThrows(IndexOutOfBoundsException.class, () -> circuit.removeGate(0));
    }

    @Test
    public void testGetGate() {
        ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
        circuit.addGate(hadamardGate);
        circuit.addGate(xGate);

        assertEquals(hadamardGate, circuit.getGate(0));
        assertEquals(xGate, circuit.getGate(1));

        assertThrows(IndexOutOfBoundsException.class, () -> circuit.getGate(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> circuit.getGate(2));
    }

    @Test
    public void testRunStepwise() {
        ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
        circuit.addGate(hadamardGate);
        circuit.addGate(xGate);

        List<ComplexQuantumRegister> states = circuit.runStepwise(input);
        assertEquals(2, states.size());

        // After Hadamard: amplitude at |0⟩ ~ 1/sqrt(2)
        Complex expected = new Complex(1.0 / Math.sqrt(2), 0);
        ComplexQuantumNumber step1Amp0 = states.get(0).getAmplitude(0);
        assertEquals(expected.getReal(), step1Amp0.getCoefficient(QuantumNumberComponent.a).getReal(), 1e-10);
        assertEquals(expected.getImaginary(), step1Amp0.getCoefficient(QuantumNumberComponent.a).getImaginary(), 1e-10);

        // After X gate, amplitude at |1⟩ ~ 1/sqrt(2)
        ComplexQuantumNumber step2Amp1 = states.get(1).getAmplitude(1);
        assertEquals(expected.getReal(), step2Amp1.getCoefficient(QuantumNumberComponent.a).getReal(), 1e-10);
        assertEquals(expected.getImaginary(), step2Amp1.getCoefficient(QuantumNumberComponent.a).getImaginary(), 1e-10);
    }
}
