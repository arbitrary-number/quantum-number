package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.*;

import com.github.quantum_number.qubit.*;
import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import java.util.Random;

public class QuantumTeleportationProtocolTest {

    private static final QuantumNumberComponent a = QuantumNumberComponent.a;
    private static final Random random = new Random();

    @Test
    public void testTeleportationWithMeasurementAndCorrections() {
        // Step 1: Prepare the unknown qubit |ψ⟩ = α|0⟩ + β|1⟩
        Complex alpha = new Complex(1 / Math.sqrt(3), 0);
        Complex beta = new Complex(Math.sqrt(2.0 / 3.0), 0);

        // Prepare |ψ⟩ state in a 1-qubit register
        ComplexQuantumRegister psi = new ComplexQuantumRegister(2);
        ComplexQuantumNumber psi0 = new ComplexQuantumNumber();
        psi0.addComponent(a, alpha);
        ComplexQuantumNumber psi1 = new ComplexQuantumNumber();
        psi1.addComponent(a, beta);
        psi.setAmplitude(0, psi0);
        psi.setAmplitude(1, psi1);

        // Step 2: Prepare entangled Bell pair |Φ+⟩ between qubits 1 and 2
        ComplexQuantumRegister bell = prepareBellPair();

        // Step 3: Initialize 3-qubit register |ψ⟩⊗|Φ+⟩
        ComplexQuantumRegister input = ComplexQuantumRegister.tensorProduct(psi, bell);

        // Step 4: Apply teleportation circuit: CNOT(0->1), Hadamard(0)
        ComplexQuantumCircuit teleportationCircuit = new ComplexQuantumCircuit();
        teleportationCircuit.addGate(ControlledX(0, 1, 3)); // control=0, target=1
        teleportationCircuit.addGate(HadamardOnQubit(0, 3));

        ComplexQuantumRegister afterGates = teleportationCircuit.run(input);

        // Step 5: Measure qubits 0 and 1, collapse state based on measurement outcome
        int measuredBits = measureQubits(afterGates, new int[]{0, 1});
        ComplexQuantumRegister collapsedState = collapseState(afterGates, new int[]{0, 1}, measuredBits);

        // Step 6: Apply corrections on qubit 2 depending on measurement
        ComplexQuantumRegister correctedState = applyCorrections(collapsedState, measuredBits, 2);

        // Step 7: Extract qubit 2 reduced state and verify equals original psi
        ComplexQuantumRegister teleportedQubit = reduceToSingleQubit(correctedState, 2, 3);

        // Now verify teleportedQubit amplitudes ≈ psi amplitudes
        for (int i = 0; i < 2; i++) {
            ComplexQuantumNumber teleportedAmp = teleportedQubit.getAmplitude(i);
            ComplexQuantumNumber originalAmp = psi.getAmplitude(i);

            Complex teleportedCoeff = teleportedAmp.getCoefficient(a);
            Complex originalCoeff = originalAmp.getCoefficient(a);

            assertEquals(originalCoeff.getReal(), teleportedCoeff.getReal(), 1e-6, "Real parts differ at amplitude " + i);
            assertEquals(originalCoeff.getImaginary(), teleportedCoeff.getImaginary(), 1e-6, "Imag parts differ at amplitude " + i);
        }
    }

    private ComplexQuantumRegister reduceToSingleQubit(ComplexQuantumRegister state, int totalQubits, int qubitIndex) {
        int size = 1 << totalQubits;   // 2^n total states
        int newSize = 2;               // single qubit -> 2 states

        ComplexQuantumRegister reduced = new ComplexQuantumRegister(newSize);

        for (int i = 0; i < size; i++) {
            int bit = (i >> qubitIndex) & 1;  // extract the qubitIndex-th bit from i
            ComplexQuantumNumber amplitude = state.getAmplitude(i);

            // Add amplitude to the reduced register's bit-th position
            ComplexQuantumNumber current = reduced.getAmplitude(bit);
            current.addComponents(amplitude); // assuming addComponents merges components correctly
        }

        return reduced;
    }


    private ComplexQuantumRegister applyCorrections(ComplexQuantumRegister state, int measuredBits, int targetQubit) {
        ComplexQuantumRegister corrected = state;

        // If measured bit 1 (second measured qubit) is 1 → apply X gate on target qubit
        if (((measuredBits >> 1) & 1) == 1) {
            corrected = applyGateOnQubit(corrected, XGate(), targetQubit);
        }

        // If measured bit 0 (first measured qubit) is 1 → apply Z gate on target qubit
        if (((measuredBits >> 0) & 1) == 1) {
            corrected = applyGateOnQubit(corrected, ZGate(), targetQubit);
        }

        return corrected;
    }

    public static ComplexQuantumRegister applyGateOnQubit(
            ComplexQuantumRegister input,
            ComplexQuantumGate gate,
            int targetQubit
        ) {
        int numQubits = (int) (Math.log(input.size()) / Math.log(2));
        if (targetQubit < 0 || targetQubit >= numQubits) {
            throw new IllegalArgumentException("Invalid target qubit index");
        }

        // The dimension of the full register
        int dim = input.size();

        ComplexQuantumRegister output = new ComplexQuantumRegister(dim);

        for (int basisIndex = 0; basisIndex < dim; basisIndex++) {
            ComplexQuantumNumber amplitude = input.getAmplitude(basisIndex);

            // For each element of the gate matrix (2x2)
            for (int gateRow = 0; gateRow < 2; gateRow++) {
                // Calculate which basis state this gateRow corresponds to for the target qubit
                int bit = (basisIndex >> targetQubit) & 1;

                if (bit == gateRow) {
                    // If bits match, just copy amplitude * gate element
                    int outputIndex = basisIndex;
                    Complex gateElement = gate.getMatrix()[gateRow][bit];
                    ComplexQuantumNumber multiplied = amplitude.multiply(gateElement);
                    output.getAmplitude(outputIndex).addComponents(multiplied);
                } else {
                    // Flip the bit at targetQubit position to get new output index
                    int outputIndex = basisIndex ^ (1 << targetQubit);
                    Complex gateElement = gate.getMatrix()[gateRow][bit];
                    ComplexQuantumNumber multiplied = amplitude.multiply(gateElement);
                    output.getAmplitude(outputIndex).addComponents(multiplied);
                }
            }
        }

        return output;
    }



    private ComplexQuantumGate ZGate() {
    	return ComplexQuantumGate.pauliZ();
	}

	private ComplexQuantumGate XGate() {
    	return ComplexQuantumGate.pauliX();
	}

	// Helper: Prepare Bell pair (|00> + |11>)/sqrt(2) in 2-qubit register
    private ComplexQuantumRegister prepareBellPair() {
        ComplexQuantumRegister bell = new ComplexQuantumRegister(4);
        double invSqrt2 = 1.0 / Math.sqrt(2);

        ComplexQuantumNumber zeroComp = new ComplexQuantumNumber();
        zeroComp.addComponent(a, new Complex(invSqrt2, 0));
        ComplexQuantumNumber threeComp = new ComplexQuantumNumber();
        threeComp.addComponent(a, new Complex(invSqrt2, 0));

        bell.setAmplitude(0, zeroComp); // |00⟩
        bell.setAmplitude(3, threeComp); // |11⟩

        // Others zero
        for (int i = 1; i < 4; i++) {
            if (i != 3) {
                bell.setAmplitude(i, new ComplexQuantumNumber());
            }
        }

        return bell;
    }

//    // Helper: Tensor product of two registers (sizes 2^n and 2^m → 2^(n+m))
//    private ComplexQuantumRegister tensorProduct(ComplexQuantumRegister r1, ComplexQuantumRegister r2) {
//        int size1 = r1.size();
//        int size2 = r2.size();
//        ComplexQuantumRegister result = new ComplexQuantumRegister(size1 * size2);
//
//        for (int i = 0; i < size1; i++) {
//            for (int j = 0; j < size2; j++) {
//                ComplexQuantumNumber amp1 = r1.getAmplitude(i);
//                ComplexQuantumNumber amp2 = r2.getAmplitude(j);
//
//                ComplexQuantumNumber combined = amp1.multiply(amp2);
//                result.setAmplitude(i * size2 + j, combined);
//            }
//        }
//        return result;
//    }

    // Helper: Controlled-X gate for qubits in multi-qubit register
    private ComplexQuantumGate ControlledX(int controlQubit, int targetQubit, int totalQubits) {
        int dimension = 1 << totalQubits;
        Complex[][] matrix = new Complex[dimension][dimension];
        Complex zero = Complex.ZERO;
        Complex one = Complex.ONE;

        for (int i = 0; i < dimension; i++) {
            // Check control bit
            boolean controlSet = ((i >> controlQubit) & 1) == 1;

            if (controlSet) {
                // Flip target bit
                int flipped = i ^ (1 << targetQubit);
                for (int j = 0; j < dimension; j++) {
                    matrix[i][j] = (j == flipped) ? one : zero;
                }
            } else {
                for (int j = 0; j < dimension; j++) {
                    matrix[i][j] = (j == i) ? one : zero;
                }
            }
        }

        return new ComplexQuantumGate(matrix);
    }

    // Helper: Hadamard on specific qubit
    private ComplexQuantumGate HadamardOnQubit(int targetQubit, int totalQubits) {
        int dimension = 1 << totalQubits;
        Complex[][] matrix = new Complex[dimension][dimension];
        Complex zero = Complex.ZERO;
        double invSqrt2 = 1.0 / Math.sqrt(2);

        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                matrix[i][j] = zero;
            }
        }

        for (int i = 0; i < dimension; i++) {
            int flipped = i ^ (1 << targetQubit);
            matrix[i][i] = new Complex(invSqrt2, 0);
            matrix[i][flipped] = new Complex(invSqrt2, 0);
        }

        return new ComplexQuantumGate(matrix);
    }

    // Helper: Measure given qubits, returns bits as integer (lowest bit is first qubit in array)
    private int measureQubits(ComplexQuantumRegister state, int[] qubits) {
        int dimension = state.size();
        double[] probabilities = new double[1 << qubits.length];

        // Sum probabilities of all basis states for each measurement outcome
        for (int basisIndex = 0; basisIndex < dimension; basisIndex++) {
            int measuredBits = 0;
            for (int i = 0; i < qubits.length; i++) {
                if (((basisIndex >> qubits[i]) & 1) == 1) {
                    measuredBits |= (1 << i);
                }
            }

            ComplexQuantumNumber amplitude = state.getAmplitude(basisIndex);
            double prob = amplitude.magnitudeSquared();
            probabilities[measuredBits] += prob;
        }

        // Randomly sample measurement outcome based on probabilities
        double r = random.nextDouble();
        double cumulative = 0.0;
        for (int outcome = 0; outcome < probabilities.length; outcome++) {
            cumulative += probabilities[outcome];
            if (r <= cumulative) {
                return outcome;
            }
        }

        // Fallback (should not happen if probabilities sum to 1)
        return probabilities.length - 1;
    }

 // Helper: Collapse state to subspace where measured qubits equal measuredBits
    private ComplexQuantumRegister collapseState(ComplexQuantumRegister state, int[] qubits, int measuredBits) {
        int dimension = state.size();
        ComplexQuantumRegister collapsed = new ComplexQuantumRegister(dimension);

        double norm = 0.0;
        for (int basisIndex = 0; basisIndex < dimension; basisIndex++) {
            boolean match = true;
            for (int i = 0; i < qubits.length; i++) {
                // Check if the bit at qubits[i] matches measuredBits at position i
                int bitValue = (basisIndex >> qubits[i]) & 1;
                int measuredBitValue = (measuredBits >> i) & 1;
                if (bitValue != measuredBitValue) {
                    match = false;
                    break;
                }
            }

            if (match) {
                ComplexQuantumNumber amp = state.getAmplitude(basisIndex);
                collapsed.setAmplitude(basisIndex, amp);
                norm += amp.magnitudeSquared();
            } else {
                // Set amplitude to zero vector (no components)
                collapsed.setAmplitude(basisIndex, new ComplexQuantumNumber());
            }
        }

        // Normalize collapsed state
        double normFactor = 1.0 / Math.sqrt(norm);
        for (int i = 0; i < dimension; i++) {
            ComplexQuantumNumber amp = collapsed.getAmplitude(i);
            ComplexQuantumNumber normalized = amp.multiply(new Complex(normFactor, 0));
            collapsed.setAmplitude(i, normalized);
        }

        return collapsed;
    }
}