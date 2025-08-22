package com.github.quantum_number.qubit;

public class ShorStep3 {

    /**
     * Performs classical post-processing to estimate the period r from a measured value.
     *
     * @param measuredValue The result of measuring the quantum register after QFT.
     * @param nQubits       The number of qubits used (determines the denominator size: 2^n).
     * @param maxDenominator The maximum denominator to consider in continued fraction.
     * @return Estimated period r, or -1 if not found.
     */
    public static int estimatePeriod(int measuredValue, int nQubits, int maxDenominator) {
        if (measuredValue == 0) {
            return -1; // Avoid division by zero and meaningless results
        }

        int q = 1 << nQubits;  // Total states (denominator in fraction)
        double fraction = (double) measuredValue / (double) q;

        return findPeriodViaContinuedFraction(fraction, maxDenominator);
    }

    /**
     * Uses continued fraction expansion to find an approximate fraction a/r where r is the period.
     *
     * @param x              The decimal value (measuredIndex / 2^n).
     * @param maxDenominator The maximum denominator to try (e.g., 100).
     * @return Estimated r or -1 if not found.
     */
    public static int findPeriodViaContinuedFraction(double x, int maxDenominator) {
        int[] numerators = new int[maxDenominator + 2];
        int[] denominators = new int[maxDenominator + 2];
        double epsilon = 1e-6;

        double z = x;
        int i = 0;
        int maxSteps = maxDenominator;

        numerators[0] = 0;
        numerators[1] = 1;
        denominators[0] = 1;
        denominators[1] = 0;

        while (i < maxSteps) {
            int a = (int) Math.floor(z);
            int n = a * numerators[i + 1] + numerators[i];
            int d = a * denominators[i + 1] + denominators[i];

            if (d > maxDenominator) {
                break;
            }

            numerators[i + 2] = n;
            denominators[i + 2] = d;

            double approximation = (double) n / (double) d;
            if (Math.abs((double) approximation - (double) x) < (double) epsilon) {
                return d;
            }

            z = 1.0d / (double) ((double) z - (double) a);
            i++;
        }

        return -1;
    }
}

