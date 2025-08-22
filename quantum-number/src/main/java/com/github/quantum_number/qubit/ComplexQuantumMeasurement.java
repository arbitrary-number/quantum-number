package com.github.quantum_number.qubit;

import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.Random;

import org.apache.commons.math3.complex.Complex;

import com.github.quantum_number.qubit.measurement.ComplexMeasurementResult;

public class ComplexQuantumMeasurement {

    private static final Random random = new Random();

    public static ComplexMeasurementResult measureQubitsComplex(ComplexQuantumRegister reg, int[] qubitsToMeasure) {
        int nQubits = (int)(Math.log(reg.size()) / Math.log(2));
        int measuredBitsCount = qubitsToMeasure.length;
        int dim = reg.size();

        double[] probabilities = new double[1 << measuredBitsCount];

        for (int i = 0; i < dim; i++) {
            int outcome = 0;
            for (int b = 0; b < measuredBitsCount; b++) {
                int qubit = qubitsToMeasure[b];
                int bitVal = (i >> (nQubits - 1 - qubit)) & 1;
                outcome |= (bitVal << (measuredBitsCount - 1 - b));
            }
            ComplexQuantumNumber amp = reg.getAmplitude(i);
            double ampSquared = amp.normSquared();
            probabilities[outcome] += ampSquared;
        }

        //double r = Math.random();
	    double r;
		try {
			r = SecureRandom.getInstanceStrong().nextDouble();
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			throw new IllegalStateException(e);
		}
        double cumProb = 0;
        int measuredOutcome = 0;
        for (int i = 0; i < probabilities.length; i++) {
            cumProb += probabilities[i];
            if (r <= cumProb) {
                measuredOutcome = i;
                break;
            }
        }

        ComplexQuantumRegister collapsed = ComplexQuantumRegister.ofDimension(dim);
        double normFactor = 0;
        for (int i = 0; i < dim; i++) {
            int outcome = 0;
            for (int b = 0; b < measuredBitsCount; b++) {
                int qubit = qubitsToMeasure[b];
                int bitVal = (i >> (nQubits - 1 - qubit)) & 1;
                outcome |= (bitVal << (measuredBitsCount - 1 - b));
            }
            if (outcome == measuredOutcome) {
                ComplexQuantumNumber amp = reg.getAmplitude(i);
                normFactor += amp.normSquared();
                collapsed.setAmplitude(i, amp);
            } else {
                collapsed.setAmplitude(i, new ComplexQuantumNumber());
            }
        }

        double invSqrt = 1.0 / Math.sqrt(normFactor);
        for (int i = 0; i < dim; i++) {
            ComplexQuantumNumber amp = collapsed.getAmplitude(i);
            collapsed.setAmplitude(i, amp.multiply(new Complex(invSqrt, 0d)));
        }

        return new ComplexMeasurementResult(measuredOutcome, collapsed);
    }


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
