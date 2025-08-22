package com.github.quantum_number.qubit;

import org.apache.commons.math3.complex.Complex;

import com.github.quantum_number.qubit.ComplexQuantumGate;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;

public class ComplexQuantumNumberQFT {

    /**
     * Initializes the quantum register for Shor's algorithm Step 1:
     * - Creates a superposition on input register qubits
     * - Initializes output register to |1>
     *
     * @param nInputQubits  Number of qubits in input register
     * @param nOutputQubits Number of qubits in output register
     * @return Initialized ComplexQuantumRegister
     */
    public static ComplexQuantumRegister initializeRegisters(int nInputQubits, int nOutputQubits) {
        int totalQubits = nInputQubits + nOutputQubits;

        // Create empty quantum register
        ComplexQuantumRegister register = ComplexQuantumRegister.ofQubits(totalQubits);

        // Zero all amplitudes
        for (int i = 0; i < register.size(); i++) {
            register.setAmplitude(i, new ComplexQuantumNumber());
        }

        // Set amplitude for |0...0>|1> (output register in |1>)
        register.setAmplitude(1, ComplexQuantumNumber.fromComplex(Complex.ONE));

        // Hadamard gate instance
        ComplexQuantumGate hadamard = ComplexQuantumGate.hadamard();

        // Apply Hadamard to each input qubit (0 to nInputQubits -1)
        for (int qubit = 0; qubit < nInputQubits; qubit++) {
            ComplexQuantumGate H = singleQubitGateOnNQubits(hadamard, totalQubits, qubit);
            register = H.apply(register);
        }

        return register;
    }

    // Helper method: embed a single-qubit gate into an n-qubit system on targetQubit
    static ComplexQuantumGate singleQubitGateOnNQubits(ComplexQuantumGate gate, int totalQubits, int targetQubit) {
        ComplexQuantumGate result = ComplexQuantumGate.identityOfQubits(1);
        for (int i = 0; i < totalQubits; i++) {
            if (i == targetQubit) {
                result = ComplexQuantumGate.tensorProduct(result, gate);
            } else {
                result = ComplexQuantumGate.tensorProduct(result, ComplexQuantumGate.identityOfQubits(1));
            }
        }
        return result;
    }
}
