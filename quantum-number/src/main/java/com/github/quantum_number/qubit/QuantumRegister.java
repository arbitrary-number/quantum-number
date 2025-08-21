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

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class QuantumRegister {
    private final List<QuantumNumber> qubits;
    private final Random rng = new Random();

    public QuantumRegister(int size) {
        if (size <= 0) throw new IllegalArgumentException("Register size must be > 0");
        this.qubits = new ArrayList<>(size);
        for (int i = 0; i < size; i++) {
            // Default each qubit to |0⟩ state
            qubits.add(new QuantumNumber(
                1, 0,
                1, 0,
                1, 0,
                0, 0,
                1, 0,
                1, 0
            ));
        }
    }

    public int size() {
        return qubits.size();
    }

    public QuantumNumber getQubit(int index) {
        return qubits.get(index);
    }

    public void setQubit(int index, QuantumNumber q) {
        qubits.set(index, q);
    }

    public void applyGate(int index, QuantumNumber gate) {
        QuantumNumber current = qubits.get(index);
        QuantumNumber updated = current.add(gate);
        qubits.set(index, updated);
    }

    public int measureQubit(int index) {
        return qubits.get(index).measure();
    }

    public int[] measureAll() {
        int[] result = new int[qubits.size()];
        for (int i = 0; i < qubits.size(); i++) {
            result[i] = measureQubit(i);
        }
        return result;
    }

    public void normalizeAll() {
        for (int i = 0; i < qubits.size(); i++) {
            QuantumNumber q = qubits.get(i);
            qubits.set(i, q.normalize());
        }
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("QuantumRegister {\n");
        for (int i = 0; i < qubits.size(); i++) {
            sb.append("  Qubit ").append(i).append(": ")
              .append(qubits.get(i).toString()).append("\n");
        }
        sb.append("}");
        return sb.toString();
    }
}
