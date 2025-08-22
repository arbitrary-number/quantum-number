package com.github.quantum_number.qubit;

public class ContinuedFraction {

    /**
     * Given a fraction x, find the best rational approximation s/r with denominator <= maxDenominator.
     */
    public static int[] approximateFraction(double x, int maxDenominator) {
        int numerator = 0;
        int denominator = 1;
        int prevNumerator = 1;
        int prevDenominator = 0;

        double fraction = x;
        while (denominator <= maxDenominator) {
            int a = (int) Math.floor(fraction);
            int tempNumerator = numerator;
            numerator = a * numerator + prevNumerator;
            prevNumerator = tempNumerator;

            int tempDenominator = denominator;
            denominator = a * denominator + prevDenominator;
            prevDenominator = tempDenominator;

            if (Math.abs(x - ((double) numerator) / ((double) denominator)) < 1e-10) {
                break;
            }

            fraction = 1.0 / (fraction - a);
        }

        return new int[] { numerator, denominator };
    }
}
