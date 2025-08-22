package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumNumberQFTStep2;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.QuantumNumberComponent;

public class ComplexQuantumNumberQFTStep2Test {

    @Test
    public void testApplyQFTOnSimpleRegister() {
        int nQubits = 3;
        int dim = 1 << nQubits;  // 8

        // Initialize register with |1> state (amplitude 1 at index 1, 0 elsewhere)
        ComplexQuantumRegister input = ComplexQuantumRegister.ofDimension(dim);
        for (int i = 0; i < dim; i++) {
            input.setAmplitude(i, ComplexQuantumNumber.zero());
        }
        input.setAmplitude(1, ComplexQuantumNumber.one());  // |001>

        // Apply QFT (Step 2)
        ComplexQuantumRegister output = ComplexQuantumNumberQFTStep2.applyQFT(input);

        // Assert output dimension unchanged
        assertEquals(dim, output.size());

        // Check normalization: sum of probabilities (amplitude squared) = 1
        double sumProb = 0;
        for (int i = 0; i < dim; i++) {
            ComplexQuantumNumber amp = output.getAmplitude(i);
            Complex c = new Complex(
                amp.getComponent(QuantumNumberComponent.a).getReal(),
                amp.getComponent(QuantumNumberComponent.a).getImaginary()
            );
            sumProb += c.abs() * c.abs();
        }
        assertEquals(1.0, sumProb, 1e-10);

        // Optionally, check first amplitude magnitude ~ 1/sqrt(8)
        ComplexQuantumNumber amp0 = output.getAmplitude(0);
        Complex c0 = new Complex(
            amp0.getComponent(QuantumNumberComponent.a).getReal(),
            amp0.getComponent(QuantumNumberComponent.a).getImaginary()
        );
        double expectedMag = 1.0 / Math.sqrt(dim);
        assertEquals(expectedMag, c0.abs(), 1e-10);

        // You can add more detailed checks on phases if needed
    }
}
