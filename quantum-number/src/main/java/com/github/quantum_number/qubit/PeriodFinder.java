package com.github.quantum_number.qubit;

public class PeriodFinder {

    /**
     * Attempts to find the period r such that measuredValue / Q ≈ k / r.
     *
     * @param measuredValue Result of measuring the quantum register.
     * @param nQubits Number of qubits (Q = 2^n).
     * @param maxDenominator Maximum r to consider.
     * @return Estimated period r, or -1 if not found.
     */
    public static int findPeriod(int measuredValue, int nQubits, int maxDenominator) {
        int Q = 1 << nQubits;

        if (measuredValue <= 0 || measuredValue >= Q) {
            return -1;  // Invalid measurement
        }

        double x = (double) measuredValue / (double) Q;
        System.out.println("x " + x);
        // Use continued fraction expansion
        for (int denominator = 1; denominator <= maxDenominator; denominator++) {
            double approx = Math.round(x * ((double) denominator)) / (double) denominator;
            System.out.println("Trying denominator " + denominator);
            double abs = Math.abs(approx - x);
            System.out.println("Trying approx - x (abs) " + abs);
			if (abs < (1.0d / (2.0d * (double) Q))) {
                return denominator;
            }


        }

        return -1; // Couldn't find a suitable period
    }
}
