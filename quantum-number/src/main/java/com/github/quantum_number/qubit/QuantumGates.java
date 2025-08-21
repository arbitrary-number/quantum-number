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

public class QuantumGates {

    // Identity Gate (I): Leaves qubit unchanged
    public static QuantumNumber identity() {
        return new QuantumNumber(
            0, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0,
            0, 0
        );
    }

    // Pauli-X (NOT gate): Flips |0⟩ ↔ |1⟩
    public static QuantumNumber pauliX() {
        return new QuantumNumber(
            -1, 0, // α → -1
            0, 0, 0, 0,
             1, 0, // β → 1
            0, 0, 0, 0
        );
    }

    // Pauli-Z: Phase flip (|1⟩ → -|1⟩, |0⟩ unchanged)
    public static QuantumNumber pauliZ() {
        return new QuantumNumber(
            1, 0,
            0, 0, 0, 0,
           -1, 0,
            0, 0, 0, 0
        );
    }

    // Hadamard Gate: Creates superposition
    public static QuantumNumber hadamard() {
        double v = 1.0 / Math.sqrt(2);
        return new QuantumNumber(
            v, 0,   // α = 1/√2
            0, 0, 0, 0,
            v, 0,   // β = 1/√2
            0, 0, 0, 0
        );
    }

    // Phase Gate (S): Adds i phase to |1⟩
    public static QuantumNumber phase() {
        return new QuantumNumber(
            1, 0,
            0, 0, 0, 0,
            0, 1,
            0, 0, 0, 0
        );
    }

    // T Gate (π/4 phase)
    public static QuantumNumber tGate() {
        double cos = Math.cos(Math.PI / 4);
        double sin = Math.sin(Math.PI / 4);
        return new QuantumNumber(
            1, 0,
            0, 0, 0, 0,
            cos, sin,
            0, 0, 0, 0
        );
    }

    // Custom user-defined gate
    public static QuantumNumber custom(double[] realImagPairs) {
        if (realImagPairs.length != 12)
            throw new IllegalArgumentException("Expected 12 real-imaginary pairs for QuantumNumber");

        return new QuantumNumber(
            realImagPairs[0], realImagPairs[1],
            realImagPairs[2], realImagPairs[3],
            realImagPairs[4], realImagPairs[5],
            realImagPairs[6], realImagPairs[7],
            realImagPairs[8], realImagPairs[9],
            realImagPairs[10], realImagPairs[11]
        );
    }
}
