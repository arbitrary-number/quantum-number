/*
 * Copyright (c) Arbitrary Number Project Team. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
package com.github.quantum_number.qubit;

import org.apache.commons.math3.complex.Complex;

import java.util.Arrays;
import java.util.Map;

/**
 * Represents a quantum register as a vector of ComplexQuantumNumber amplitudes.
 * Here, the register size is fixed to 4 amplitudes (like 2 qubits).
 */
public class ComplexQuantumRegister {

    //dimension = num qbits
    //private final int dimension;
    private final ComplexQuantumNumber[] amplitudes;
    //private Map<Integer, QuantumNumberComponent> indexToComponent; // index -> combined component
    //private Map<QuantumNumberComponent, Integer> componentToIndex; // combined component -> index
	private int size;

    /**
     * Initializes the register as a 2-qubit system (4 amplitudes).
     * Default state is |00⟩ with amplitude 1 on basis component 'a'.
     */
//    public ComplexQuantumRegister() {
//        this.dimension = 2;              // 2 qubits
//        this.size = 1 << dimension;      // size = 2^2 = 4
//        this.amplitudes = new ComplexQuantumNumber[size];
//
//        for (int i = 0; i < size; i++) {
//            amplitudes[i] = new ComplexQuantumNumber();
//        }
//
//        // Start in |00⟩ state, which maps to basis component 'a'
//        amplitudes[0].addComponent(QuantumNumberComponent.a, Complex.ONE);
//    }


//    public ComplexQuantumRegister(int dimension) {
//        this.dimension = dimension;
//        this.size = 1 << dimension; // 2^numQubits
//        this.amplitudes = new ComplexQuantumNumber[size];
//        for (int i = 0; i < size; i++) {
//            amplitudes[i] = new ComplexQuantumNumber();
//        }
//    }

//    public ComplexQuantumRegister(int numQubits) {
//        this.dimension = numQubits;
//        this.size = intPow(2, numQubits);
//        //new Integer(2).new Integer(numQubits)1 << numQubits; // 2^qubits
//        this.amplitudes = new ComplexQuantumNumber[size];
//        for (int i = 0; i < size; i++) {
//            amplitudes[i] = new ComplexQuantumNumber();
//        }
//    }

    // Private constructor to force usage of factory methods
    private ComplexQuantumRegister(int size) {
        this.size = size;
        this.amplitudes = new ComplexQuantumNumber[size];
        for (int i = 0; i < size; i++) {
            amplitudes[i] = new ComplexQuantumNumber();
        }
    }

    /** Create register given number of qubits */
    public static ComplexQuantumRegister ofQubits(int numQubits) {
        if (numQubits < 0) throw new IllegalArgumentException("Number of qubits cannot be negative");
        int size = 1 << numQubits;  // 2^numQubits
        return new ComplexQuantumRegister(size);
    }

    /** Create register given explicit dimension */
    public static ComplexQuantumRegister ofDimension(int dimension) {
        if (dimension <= 0) throw new IllegalArgumentException("Dimension must be positive");
        // dimension should be a power of two for qubit registers, but no enforcement here
        return new ComplexQuantumRegister(dimension);
    }

    /** Return number of qubits, assumes size is power of two */
    public int getNumQubits() {
        return (int)(Math.log(size) / Math.log(2));
    }


	public int getDimension() {
		return size;
	}

//    public int getNumQubitsFromDimension() {
//		return dimension;
//	}

    public static int intPow(int base, int exponent) {
        int result = 1;
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }
        return result;
    }


    public static ComplexQuantumRegister withZeroState(int qubitCount) {
        int dim = 1 << qubitCount;
        ComplexQuantumRegister reg = new ComplexQuantumRegister(dim);
        reg.getAmplitude(0).addComponent(QuantumNumberComponent.a, Complex.ONE);
        return reg;
    }

    // Tensor product of two registers
//    public static ComplexQuantumRegister tensorProduct(ComplexQuantumRegister r1, ComplexQuantumRegister r2) {
//        int totalQubits = r1.numQubits + r2.numQubits;
//        ComplexQuantumRegister result = new ComplexQuantumRegister(totalQubits);
//
//        for (int i = 0; i < r1.size; i++) {
//            QuantumNumberComponent comp1 = r1.getComponentForIndex(i);
//            ComplexQuantumNumber amp1 = r1.getAmplitude(i);
//
//            for (int j = 0; j < r2.size; j++) {
//                QuantumNumberComponent comp2 = r2.getComponentForIndex(j);
//                ComplexQuantumNumber amp2 = r2.getAmplitude(j);
//
//                // Combine components (tensor product of basis states)
//                QuantumNumberComponent combinedComp = QuantumNumberComponent.combine(comp1, comp2);
//
//                // Get index of combined component in result register
//                int combinedIndex = result.getIndexForComponent(combinedComp);
//
//                // Tensor product of amplitudes
//                ComplexQuantumNumber combinedAmp = ComplexQuantumNumber.tensorProduct(amp1, amp2);
//
//                // Set combined amplitude in result register
//                result.setAmplitude(combinedIndex, combinedAmp);
//            }
//        }
//        return result;
//    }

//    public static ComplexQuantumRegister tensorProduct(ComplexQuantumRegister r1, ComplexQuantumRegister r2) {
//        int size1 = r1.getDimension();
//        int size2 = r2.getDimension();
//        ComplexQuantumRegister result = new ComplexQuantumRegister(size1 + size2);
//
//        for (int i = 0; i < size1; i++) {
//            for (int j = 0; j < size2; j++) {
//                ComplexQuantumNumber amp1 = r1.getAmplitude(i);
//                ComplexQuantumNumber amp2 = r2.getAmplitude(j);
//
//                ComplexQuantumNumber combined = ComplexQuantumNumber.tensorProduct(amp1, amp2); // ✅ Use tensorProduct here
//                result.setAmplitude(i * size2 + j, combined);
//            }
//        }
//        return result;
//    }

    public static ComplexQuantumRegister tensorProduct(ComplexQuantumRegister r1, ComplexQuantumRegister r2) {
        int totalQubits = r1.getNumQubits() + r2.getNumQubits();
        ComplexQuantumRegister result = ComplexQuantumRegister.ofQubits(totalQubits);

        int size1 = r1.size();
        int size2 = r2.size();

        for (int i = 0; i < size1; i++) {
            ComplexQuantumNumber amp1 = r1.getAmplitude(i);
            for (int j = 0; j < size2; j++) {
                ComplexQuantumNumber amp2 = r2.getAmplitude(j);
                ComplexQuantumNumber combined = amp1.multiply(amp2);
                result.setAmplitude(i * size2 + j, combined);
            }
        }
        return result;
    }



//    public static ComplexQuantumRegister tensorProduct(ComplexQuantumRegister r1, ComplexQuantumRegister r2) {
//        int size1 = r1.size();
//        int size2 = r2.size();
//
//        ComplexQuantumRegister result = new ComplexQuantumRegister(size1 * size2);
//
//        for (int i = 0; i < size1; i++) {
//            ComplexQuantumNumber amp1 = r1.getAmplitude(i);
//
//            for (int j = 0; j < size2; j++) {
//                ComplexQuantumNumber amp2 = r2.getAmplitude(j);
//
//                ComplexQuantumNumber combined = ComplexQuantumNumber.tensorProduct(amp1, amp2);
//
//                result.setAmplitude(i * size2 + j, combined);
//            }
//        }
//
//        return result;
//    }





	// Add this method:
//    public int getDimension() {
//        return this.dimension;
//    }

    /**
     * Gets the amplitude at the specified index.
     * @param index must be in [0, 3]
     * @return ComplexQuantumNumber amplitude
     * @throws IndexOutOfBoundsException if index is invalid
     */
    public ComplexQuantumNumber getAmplitude(int index) {
        if (index < 0 || index >= amplitudes.length) {
            throw new IndexOutOfBoundsException("Amplitude index out of range");
        }
        return amplitudes[index];
    }

    /**
     * Sets the amplitude at the specified index.
     * @param index must be in [0, 3]
     * @param amplitude must not be null
     * @throws IndexOutOfBoundsException if index is invalid
     * @throws IllegalArgumentException if amplitude is null
     */
    public void setAmplitude(int index, ComplexQuantumNumber amplitude) {
        if (index < 0 || index >= amplitudes.length) {
            throw new IndexOutOfBoundsException("Amplitude index out of range");
        }
        if (amplitude == null) {
            throw new IllegalArgumentException("Amplitude cannot be null");
        }
        amplitudes[index] = amplitude;
    }

    /**
     * Normalize the register so that sum of magnitude squared of amplitudes is 1.
     * If the register is zero, throws ArithmeticException.
     */
    public void normalize() {
        double sumSquares = Arrays.stream(amplitudes)
                .mapToDouble(ComplexQuantumNumber::magnitudeSquared)
                .sum();

        if (sumSquares == 0.0) {
            throw new ArithmeticException("Cannot normalize zero vector quantum register");
        }

        double normFactor = 1.0 / Math.sqrt(sumSquares);
        for (int i = 0; i < amplitudes.length; i++) {
            amplitudes[i].scale(normFactor);
        }
    }

    public static ComplexQuantumRegister collapseState(ComplexQuantumRegister state, int[] qubits, int measuredBits) {
        int dimension = state.size();
        ComplexQuantumRegister collapsed = new ComplexQuantumRegister(dimension);

        double norm = 0.0;
        for (int basisIndex = 0; basisIndex < dimension; basisIndex++) {
            boolean match = true;
            for (int i = 0; i < qubits.length; i++) {
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
                collapsed.setAmplitude(basisIndex, new ComplexQuantumNumber());
            }
        }

        if (norm == 0) {
            throw new IllegalStateException("Collapse norm is zero, invalid measurement or state");
        }

        double normFactor = 1.0 / Math.sqrt(norm);
        for (int i = 0; i < dimension; i++) {
            ComplexQuantumNumber amp = collapsed.getAmplitude(i);
            ComplexQuantumNumber normalized = amp.multiply(new Complex(normFactor, 0));
            collapsed.setAmplitude(i, normalized);
        }

        return collapsed;
    }

    /**
     * Returns the number of amplitudes (register size).
     */
    public int size() {
        return amplitudes.length;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("ComplexQuantumRegister:\n");
        for (int i = 0; i < amplitudes.length; i++) {
            sb.append("Amplitude ").append(i).append(": ").append(amplitudes[i]).append("\n");
        }
        return sb.toString();
    }

	public static ComplexQuantumRegister applyCorrections(ComplexQuantumRegister collapsedState, int measuredBits,
			int targetQubit) {
        ComplexQuantumRegister corrected = collapsedState;

        // If measured bit 1 (second measured qubit) is 1 → apply X gate on target qubit
        if (((measuredBits >> 1) & 1) == 1) {
            corrected = applyGateOnOnlyOneQubit(corrected, ComplexQuantumGate.pauliX(), targetQubit);
        }

        // If measured bit 0 (first measured qubit) is 1 → apply Z gate on target qubit
        if (((measuredBits >> 0) & 1) == 1) {
            corrected = applyGateOnOnlyOneQubit(corrected, ComplexQuantumGate.pauliZ(), targetQubit);
        }

        return corrected;
    }

	public static ComplexQuantumRegister applyGateMatrix(
		    ComplexQuantumRegister input,
		    ComplexQuantumGate gate
		) {
		    int size = input.size();
		    ComplexQuantumRegister output = new ComplexQuantumRegister(size);

		    Complex[][] matrix = gate.getMatrix();
		    if (matrix.length != size || matrix[0].length != size) {
		        throw new IllegalArgumentException("Gate matrix size must match register size");
		    }

		    for (int i = 0; i < size; i++) {
		        ComplexQuantumNumber result = new ComplexQuantumNumber();
		        for (int j = 0; j < size; j++) {
		            ComplexQuantumNumber amp = input.getAmplitude(j);
		            Complex factor = matrix[i][j];
		            ComplexQuantumNumber term = amp.multiply(factor);
		            result.addComponents(term);
		        }
		        output.setAmplitude(i, result);
		    }

		    return output;
		}


    public static ComplexQuantumRegister applyGateOnOnlyOneQubit(
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
            System.out.println("Processing basis index: " + basisIndex + " with amplitude: " + amplitude);

            for (int gateRow = 0; gateRow < 2; gateRow++) {
                int bit = (basisIndex >> targetQubit) & 1;
                System.out.println("  Gate row: " + gateRow + ", target bit: " + bit);

                Complex gateElement = gate.getMatrix()[gateRow][bit];
                ComplexQuantumNumber multiplied = amplitude.multiply(gateElement);

                int outputIndex;
                if (bit == gateRow) {
                    outputIndex = basisIndex;
                    System.out.println("    Bit matches gate row. Output index = " + outputIndex);
                } else {
                    outputIndex = basisIndex ^ (1 << targetQubit);
                    System.out.println("    Bit does not match. Flipping target qubit.");
                    System.out.println("    Output index = " + outputIndex);
                }

                System.out.println("    Gate element: " + gateElement);
                System.out.println("    Multiplied result: " + multiplied);
                output.getAmplitude(outputIndex).addComponents(multiplied);
                System.out.println("    Added to output index " + outputIndex + ": " + output.getAmplitude(outputIndex));
            }
        }


//        for (int basisIndex = 0; basisIndex < dim; basisIndex++) {
//            ComplexQuantumNumber amplitude = input.getAmplitude(basisIndex);
//
//            // For each element of the gate matrix (2x2)
//            for (int gateRow = 0; gateRow < 2; gateRow++) {
//                // Calculate which basis state this gateRow corresponds to for the target qubit
//                int bit = (basisIndex >> targetQubit) & 1;
//
//                if (bit == gateRow) {
//                    // If bits match, just copy amplitude * gate element
//                    int outputIndex = basisIndex;
//                    Complex gateElement = gate.getMatrix()[gateRow][bit];
//                    ComplexQuantumNumber multiplied = amplitude.multiply(gateElement);
//                    output.getAmplitude(outputIndex).addComponents(multiplied);
//                } else {
//                    // Flip the bit at targetQubit position to get new output index
//                    int outputIndex = basisIndex ^ (1 << targetQubit);
//                    Complex gateElement = gate.getMatrix()[gateRow][bit];
//                    ComplexQuantumNumber multiplied = amplitude.multiply(gateElement);
//                    output.getAmplitude(outputIndex).addComponents(multiplied);
//                }
//            }
//        }

        return output;
    }

	public static ComplexQuantumRegister reduceToSingleQubit(ComplexQuantumRegister correctedState,
			int qubitIndex, int totalQubits) {
        int size = 1 << totalQubits;   // 2^n total states
        int newSize = 2;               // single qubit -> 2 states

        ComplexQuantumRegister reduced = new ComplexQuantumRegister(newSize);

        for (int i = 0; i < size; i++) {
            int bit = (i >> qubitIndex) & 1;  // extract the qubitIndex-th bit from i
            ComplexQuantumNumber amplitude = correctedState.getAmplitude(i);

            // Add amplitude to the reduced register's bit-th position
            ComplexQuantumNumber current = reduced.getAmplitude(bit);
            current.addComponents(amplitude); // assuming addComponents merges components correctly
        }

        return reduced;
	}

	public static ComplexQuantumRegister measureQubits(ComplexQuantumRegister originalReg, int[] qubitsToMeasure) {
	    int nQubits = (int)(Math.log(originalReg.size()) / Math.log(2));
	    int measuredBitsCount = qubitsToMeasure.length;
	    int dim = originalReg.size();

	    double[] probabilities = new double[1 << measuredBitsCount];

	    // Step 1: Calculate probabilities for each measurement outcome
	    for (int i = 0; i < dim; i++) {
	        int outcome = 0;
	        for (int b = 0; b < measuredBitsCount; b++) {
	            int qubit = qubitsToMeasure[b];
	            int bitVal = (i >> (nQubits - 1 - qubit)) & 1;
	            outcome |= (bitVal << (measuredBitsCount - 1 - b));
	        }
	        ComplexQuantumNumber amp = originalReg.getAmplitude(i);
	        double ampSquared = amp.normSquared();
	        probabilities[outcome] += ampSquared;
	    }

	    // Step 2: Randomly select measurement outcome based on probabilities
	    double r = Math.random();
	    double cumProb = 0;
	    int measuredOutcome = 0;
	    for (int i = 0; i < probabilities.length; i++) {
	        cumProb += probabilities[i];
	        if (r <= cumProb) {
	            measuredOutcome = i;
	            break;
	        }
	    }

	    // Step 3: Create a new collapsed register consistent with measurement
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
	            ComplexQuantumNumber amp = originalReg.getAmplitude(i);
	            normFactor += amp.normSquared();
	            collapsed.setAmplitude(i, amp);
	        } else {
	            collapsed.setAmplitude(i, new ComplexQuantumNumber());
	        }
	    }

	    // Step 4: Normalize the collapsed register
	    double invSqrt = 1.0 / Math.sqrt(normFactor);
	    for (int i = 0; i < dim; i++) {
	        ComplexQuantumNumber amp = collapsed.getAmplitude(i);
	        collapsed.setAmplitude(i, amp.multiply(new Complex(invSqrt, 0d)));
	    }

	    // Step 5: You can optionally return measurement outcome along with register
	    // But here just return the collapsed register
	    return collapsed;
	}

}
