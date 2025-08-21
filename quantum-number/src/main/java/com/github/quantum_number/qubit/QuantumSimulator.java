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

public class QuantumSimulator {
    public static void main(String[] args) {
        // Initialize a |0⟩-like state using QuantumNumber
        QuantumNumber psi = new QuantumNumber(
            1, 0,   // a + gi
            1, 0,   // b + hi
            1, 0,   // c + ii
            0, 0,   // d + ji (represents β = 0)
            1, 0,   // e + ki
            1, 0    // f + li
        );

        System.out.println("Initial state (ψ):");
        System.out.println(psi);

        // Apply a "gate" (quantition transformation)
        QuantumNumber gate = new QuantumNumber(
            0.5, 0.5,
            0.5, 0.0,
            0.5, 0.0,
            0.5, 0.0,
            0.5, 0.0,
            0.5, 0.0
        );

        QuantumNumber psiPrime = psi.add(gate);
        System.out.println("\nAfter gate (ψ'):");
        System.out.println(psiPrime);

        // Simulated measurement
        int outcome = psiPrime.measure();
        System.out.println("\nMeasurement outcome: |" + outcome + "⟩");
    }
}
