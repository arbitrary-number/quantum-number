package com.github.quantum_number.qubit;

import org.apache.commons.math3.complex.Complex;
import java.util.Arrays;

/**
 * Utilities to build multi-qubit gates from single- and controlled gates.
 */
public class QuantumGateUtils {

    private static final Complex ONE = Complex.ONE;
    private static final Complex ZERO = Complex.ZERO;

    /**
     * Build a single-qubit gate acting on qubitIndex in an n-qubit system.
     *
     * @param singleQubitGate 2x2 matrix
     * @param n number of qubits total
     * @param qubitIndex target qubit index (0-based, from right to left)
     * @return full 2^n x 2^n matrix applying gate on qubitIndex only
     */
    public static Complex[][] singleQubitGateOnNQubits(Complex[][] singleQubitGate, int n, int qubitIndex) {
        int dim = 1 << n; // 2^n
        Complex[][] fullGate = new Complex[dim][dim];
        for (int i = 0; i < dim; i++) {
            Arrays.fill(fullGate[i], ZERO);
        }

        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                // Check if i and j differ only on the target qubit
                int mask = 1 << qubitIndex;
                if ((i & ~mask) == (j & ~mask)) {
                    int iBit = (i & mask) >> qubitIndex;
                    int jBit = (j & mask) >> qubitIndex;
                    fullGate[i][j] = singleQubitGate[iBit][jBit];
                } else {
                    fullGate[i][j] = ZERO;
                }
            }
        }
        return fullGate;
    }

    /**
     * Build a controlled gate on n qubits:
     * controlQubit controls targetQubit applying singleQubitGate.
     *
     * @param singleQubitGate 2x2 matrix
     * @param n number of qubits total
     * @param controlQubit control qubit index
     * @param targetQubit target qubit index
     * @return full 2^n x 2^n matrix for controlled gate
     */
    public static Complex[][] controlledGateOnNQubits(Complex[][] singleQubitGate, int n, int controlQubit, int targetQubit) {
        int dim = 1 << n;
        Complex[][] fullGate = new Complex[dim][dim];
        for (int i = 0; i < dim; i++) {
            Arrays.fill(fullGate[i], ZERO);
        }

        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                if (i == j) {
                    // By default identity
                    fullGate[i][j] = ONE;
                }
            }
        }

        int controlMask = 1 << controlQubit;
        int targetMask = 1 << targetQubit;

        for (int i = 0; i < dim; i++) {
            // Only apply gate when control bit is 1
            if ((i & controlMask) != 0) {
                // Flip target bit using the gate matrix elements
                int bitTarget = (i & targetMask) >> targetQubit;

                // For each possible target bit outcome (0 or 1)
                for (int b = 0; b <= 1; b++) {
                    int j = (i & ~targetMask) | (b << targetQubit);
                    fullGate[j][i] = singleQubitGate[b][bitTarget];
                    if (j != i) {
                        fullGate[i][i] = ZERO; // Clear old identity when overwritten
                    }
                }
            }
        }
        return fullGate;
    }

}
