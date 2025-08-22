package com.github.quantum_number.qubit;

import java.util.Random;

public class ComplexQuantumMeasurement {

    private static final Random random = new Random();

    /**
     * Measures a single qubit and collapses the register accordingly.
     * @param reg Quantum register
     * @param qubitIndex Index of the qubit to measure (0 = least significant)
     * @return Measured value: 0 or 1
     */
    public static int measureQubit(ComplexQuantumRegister reg, int qubitIndex) {
        double prob0 = 0.0;
        int size = reg.size();

        // Sum probabilities where qubitIndex bit is 0
        for (int i = 0; i < size; i++) {
            if (((i >> qubitIndex) & 1) == 0) {
                ComplexQuantumNumber amp = reg.getAmplitude(i);
                prob0 += amp.normSquared();
            }
        }

        int result = (random.nextDouble() < prob0) ? 0 : 1;

        // Collapse state
        for (int i = 0; i < size; i++) {
            if (((i >> qubitIndex) & 1) != result) {
                reg.setAmplitude(i, new ComplexQuantumNumber()); // Zero out
            }
        }

        reg.normalize(); // Normalize remaining amplitudes
        return result;
    }

    /**
     * Measures multiple qubits and returns their classical bits in order.
     */
    public static int[] measureQubits(ComplexQuantumRegister reg, int... qubits) {
        int[] results = new int[qubits.length];
        for (int i = 0; i < qubits.length; i++) {
            results[i] = measureQubit(reg, qubits[i]);
        }
        return results;
    }
}
