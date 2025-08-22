package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ShorStep3;

public class ShorStep3Test {

    @Test
    public void testPeriodEstimationExactMatch() {
        // Simulated measurement after QFT where r = 4
        // Measured index = 2 => 2/8 = 0.25, and continued fraction of 0.25 = 1/4
        int measured = 2;
        int nQubits = 3;  // 2^3 = 8
        int maxDenominator = 10;

        int estimatedPeriod = ShorStep3.estimatePeriod(measured, nQubits, maxDenominator);
        System.out.println("Estimated period: " + estimatedPeriod);

        assertEquals(4, estimatedPeriod);
    }

    @Test
    public void testPeriodEstimationApproximation() {
        // Measurement gives a close approximation to 3/7 (~0.428571)
        // Measured index = round(0.428571 * 16) = 7
        int measured = 7;
        int nQubits = 4;
        int maxDenominator = 20;

        int estimatedPeriod = ShorStep3.estimatePeriod(measured, nQubits, maxDenominator);
        System.out.println("Estimated period: " + estimatedPeriod);

        assertTrue(estimatedPeriod > 0 && estimatedPeriod <= maxDenominator);
    }

    @Test
    public void testZeroMeasuredValue() {
        // If measured = 0, should return -1
        int measured = 0;
        int nQubits = 3;
        int estimated = ShorStep3.estimatePeriod(measured, nQubits, 10);

        assertEquals(-1, estimated);
    }
}
