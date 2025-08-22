package com.github.quantum_number.qubit;

import org.apache.commons.math3.complex.Complex;

public class ComplexQuantumNumberQFTStep2 {

    /**
     * Applies the Quantum Fourier Transform to the input quantum register.
     * This is Step 2 of Shor's Algorithm.
     *
     * @param input The quantum register in superposition after modular exponentiation (Step 1).
     * @return A new register with QFT applied.
     */
    public static ComplexQuantumRegister applyQFT(ComplexQuantumRegister input) {
        int nQubits = (int) (Math.log(input.size()) / Math.log(2));
        ComplexQuantumRegister state = input.copy();

        // Apply Hadamard and controlled phase rotations
        for (int target = 0; target < nQubits; target++) {
            // Apply Hadamard gate
            Complex[][] hadamard = QuantumGateUtils.singleQubitGateOnNQubits(
            		ComplexQuantumGate.hadamard().getMatrix(), nQubits, target);
            state = new ComplexQuantumGate(hadamard).apply(state);

            // Apply controlled phase rotation with increasing angle
            for (int control = target + 1; control < nQubits; control++) {
                double angle = Math.PI / Math.pow(2, control - target);
                ComplexQuantumGate cphase = ComplexQuantumGate.controlledPhaseGateOnNQubits(
                    nQubits, control, target, angle);
                state = cphase.apply(state);
            }
        }

        // Apply bit-reversal permutation
        state = ComplexQuantumRegister.reverseQubitOrder(state);

        return state;
    }
}
