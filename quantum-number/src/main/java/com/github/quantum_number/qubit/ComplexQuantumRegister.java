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

/**
 * Represents a quantum register as a vector of ComplexQuantumNumber amplitudes.
 * Here, the register size is fixed to 4 amplitudes (like 2 qubits).
 */
public class ComplexQuantumRegister {

    private final int dimension;
    private final ComplexQuantumNumber[] amplitudes;

    /**
     * Initializes the register with 4 amplitudes.
     * Default state is |0⟩ with amplitude 1 on basis component 'a' for amplitude index 0.
     */
    public ComplexQuantumRegister() {
        amplitudes = new ComplexQuantumNumber[4];
        for (int i = 0; i < amplitudes.length; i++) {
            amplitudes[i] = new ComplexQuantumNumber();
        }
        // Set amplitude[0] to basis |a⟩ with coefficient 1
        amplitudes[0].addComponent(QuantumNumberComponent.a, Complex.ONE);
        this.dimension = 1;
    }


    public ComplexQuantumRegister(int dimension) {
        this.dimension = dimension;
        this.amplitudes = new ComplexQuantumNumber[dimension];
        for (int i = 0; i < dimension; i++) {
            this.amplitudes[i] = new ComplexQuantumNumber();
        }
    }

    public static ComplexQuantumRegister withZeroState(int qubitCount) {
        int dim = 1 << qubitCount;
        ComplexQuantumRegister reg = new ComplexQuantumRegister(dim);
        reg.getAmplitude(0).addComponent(QuantumNumberComponent.a, Complex.ONE);
        return reg;
    }

    // Add this method:
    public int getDimension() {
        return this.dimension;
    }

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
}
