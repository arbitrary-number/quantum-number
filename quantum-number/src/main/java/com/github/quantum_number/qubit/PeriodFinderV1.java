package com.github.quantum_number.qubit;

public class PeriodFinderV1 {

    /**
     * Extract period r from measurement m and number of qubits n.
     */
    public static int findPeriod(int m, int n, int maxPeriod) {
        double fraction = (double) m / (1 << n);
        int[] approx = ContinuedFraction.approximateFraction(fraction, maxPeriod);
        int r = approx[1];
        return r;
    }
}
