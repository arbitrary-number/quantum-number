package com.github.quantum_numer.qubit.test;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumNumberQFT;
import com.github.quantum_number.qubit.ComplexQuantumRegister;

public class ComplexQuantumNumberQFTTest {

    @Test
    public void testInitializeRegisters() {
        int nInputQubits = 4;
        int nOutputQubits = 4;

        ComplexQuantumRegister reg = ComplexQuantumNumberQFT.initializeRegisters(nInputQubits, nOutputQubits);

        // Total qubits
        int totalQubits = nInputQubits + nOutputQubits;

        // Register size should be 2^totalQubits
        int expectedSize = 1 << totalQubits;
        Assertions.assertEquals(expectedSize, reg.size());

        // After initialization, amplitudes should sum to 1 (normalization)
        double totalProb = 0.0;
        for (int i = 0; i < reg.size(); i++) {
            totalProb += reg.getAmplitude(i).absSquared();
        }

        Assertions.assertEquals(1.0, totalProb, 1e-10);

        // For a 4-qubit input in superposition, output qubits initialized to |1>
        // The amplitude of basis states where output bits == 1 should be nonzero

        // For example, check amplitude of |0000>|0001> (decimal 1) should not be zero
        Assertions.assertTrue(reg.getAmplitude(1).absSquared() > 0);

        System.out.println("Initialized Quantum Register:\n" + reg);
    }
}
