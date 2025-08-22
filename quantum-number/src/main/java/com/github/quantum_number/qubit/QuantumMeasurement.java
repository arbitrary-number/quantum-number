package com.github.quantum_number.qubit;
import java.util.Random;

import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;

public class QuantumMeasurement {

    private static final Random random = new Random();

    /**
     * Measures the quantum register collapsing it to a classical state.
     * Returns the measured index.
     */
    public static int measure(ComplexQuantumRegister reg) {
        int size = reg.size();
        double[] probabilities = new double[size];
        double sum = 0;

        // Calculate probabilities = |amplitude|^2
        for (int i = 0; i < size; i++) {
            ComplexQuantumNumber amp = reg.getAmplitude(i);
            double prob = amp.magnitudeSquared();  // You need to implement this
            probabilities[i] = prob;
            sum += prob;
        }

        // Normalize if sum != 1 due to rounding errors
        for (int i = 0; i < size; i++) {
            probabilities[i] /= sum;
        }

        // Sample according to probability distribution
        double r = random.nextDouble();
        double cumulative = 0;
        for (int i = 0; i < size; i++) {
            cumulative += probabilities[i];
            if (r < cumulative) {
                return i;
            }
        }

        // Fallback (shouldn't happen)
        return size - 1;
    }

    /**
     * Measures only the first nQubits of the register (i.e. the "input" register),
     * collapsing the register accordingly and returning the measured index for those qubits.
     *
     * @param reg Quantum register to measure
     * @param nQubits Number of leading qubits to measure
     * @return Measured integer outcome (0 to 2^nQubits - 1)
     */
    public static int measureFirstNQubits(ComplexQuantumRegister reg, int nQubits) {
        int totalQubits = (int) (Math.log(reg.size()) / Math.log(2));
        int trailingQubits = totalQubits - nQubits;
        int measuredSize = 1 << nQubits;
        int trailingSize = 1 << trailingQubits;

        double[] probabilities = new double[measuredSize];

        // Sum probabilities of all states sharing the same first n qubits
        for (int i = 0; i < reg.size(); i++) {
            int leadingBits = i >> trailingQubits;  // Extract first nQubits bits (most significant bits)
            ComplexQuantumNumber amp = reg.getAmplitude(i);
            probabilities[leadingBits] += amp.magnitudeSquared();
        }

        // Normalize probabilities just in case
        double sum = 0;
        for (double p : probabilities) sum += p;
        for (int i = 0; i < probabilities.length; i++) {
            probabilities[i] /= sum;
        }

        // Sample measurement outcome for first n qubits
        double r = random.nextDouble();
        double cumulative = 0;
        for (int i = 0; i < probabilities.length; i++) {
            cumulative += probabilities[i];
            if (r < cumulative) {
                // Collapse the register to states consistent with measurement i (first n qubits)
                collapseToFirstNQubits(reg, i, nQubits, trailingQubits);
                return i;
            }
        }

        // fallback, shouldn't happen
        return measuredSize - 1;
    }

    /**
     * Collapses the quantum register amplitudes to only those consistent with
     * measured first n qubits = measuredValue, and normalizes the register.
     */
    private static void collapseToFirstNQubits(ComplexQuantumRegister reg, int measuredValue, int nQubits, int trailingQubits) {
        int totalQubits = nQubits + trailingQubits;
        int totalSize = 1 << totalQubits;
        int trailingSize = 1 << trailingQubits;

        for (int i = 0; i < totalSize; i++) {
            int leadingBits = i >> trailingQubits;
            if (leadingBits != measuredValue) {
                // Set amplitude to zero if not consistent with measurement
                reg.setAmplitude(i, ComplexQuantumNumber.zero());
            }
        }

        reg.normalize();
    }

}
