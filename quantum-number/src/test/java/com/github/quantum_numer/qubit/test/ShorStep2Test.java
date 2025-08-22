package com.github.quantum_numer.qubit.test;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.PeriodFinder;
import com.github.quantum_number.qubit.QuantumMeasurement;

public class ShorStep2Test {

    @Test
    public void testMeasurementAndPeriodExtraction() {
        // Setup a 3-qubit register (size=8)
        int nQubits = 3;
        int size = 1 << nQubits;

        // Prepare a simple quantum register with known amplitudes
        ComplexQuantumRegister register = ComplexQuantumRegister.ofDimension(size);

        // Example amplitudes: equal superposition for states 0, 2, 4, 6 (simulate partial state)
        for (int i = 0; i < size; i++) {
            if (i % 2 == 0) {
                //register.setAmplitude(i, new ComplexQuantumNumber(1.0 / 2, 0)); // amplitude 0.5 for simplicity
            	register.setAmplitude(i, ComplexQuantumNumber.fromComplex(
            			new Complex(1.0d / 2d, 0)));
            } else {
                register.setAmplitude(i, ComplexQuantumNumber.zero());
            }
        }

        // Normalize amplitudes
        register.normalize();

        // Perform measurement
        int measuredIndex = QuantumMeasurement.measure(register);
        System.out.println("Measured state: " + measuredIndex);

        // Verify measured state is one of the expected states (0,2,4,6)
        assertTrue(measuredIndex == 0 || measuredIndex == 2 || measuredIndex == 4 || measuredIndex == 6);

        // Extract period r from measurement
        int maxPeriodGuess = 8;  // max denominator in continued fraction approx
        int period = PeriodFinder.findPeriod(measuredIndex, nQubits, maxPeriodGuess);

        System.out.println("Estimated period r: " + period);


        // The period should be positive and reasonable
        assertTrue(period > 0 && period <= maxPeriodGuess);
    }
}
