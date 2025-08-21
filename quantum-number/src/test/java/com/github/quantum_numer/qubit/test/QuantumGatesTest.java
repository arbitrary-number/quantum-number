package com.github.quantum_numer.qubit.test;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.QuantumGates;
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
import com.github.quantum_number.qubit.QuantumNumber;
import com.github.quantum_number.qubit.QuantumRegister;

public class QuantumGatesTest {

    @Test
    public void testIdentityGatePreservesQubit() {
        QuantumRegister reg = new QuantumRegister(1);
        QuantumNumber original = reg.getQubit(0);

        reg.applyGate(0, QuantumGates.identity());
        QuantumNumber result = reg.getQubit(0);

        assertEquals(original.toString(), result.toString(), "Identity gate should leave qubit unchanged");
    }

    @Test
    public void testPauliXFlipsState() {
        QuantumRegister reg = new QuantumRegister(1); // default |0⟩
        reg.applyGate(0, QuantumGates.pauliX());
        reg.normalizeAll();
        int result = reg.measureQubit(0);
        assertEquals(1, result, "Pauli-X gate should flip |0⟩ to |1⟩");
    }

    @Test
    public void testPauliZDoesNotChangeMeasurement() {
        QuantumRegister reg = new QuantumRegister(1); // default |0⟩
        reg.applyGate(0, QuantumGates.pauliZ());
        reg.normalizeAll();
        int result = reg.measureQubit(0);
        assertEquals(0, result, "Pauli-Z should not flip |0⟩ to |1⟩");
    }

    @Test
    public void testHadamardCreatesSuperposition() {
        QuantumRegister reg = new QuantumRegister(1);
        reg.applyGate(0, QuantumGates.hadamard());
        reg.normalizeAll();

        double prob = reg.getQubit(0).measureProbability();
        assertTrue(prob > 0.4 && prob < 0.6, "Hadamard should create ~50% chance of |0⟩");
    }

    @Test
    public void testPhaseGateDoesNotChangeProbability() {
        QuantumRegister reg = new QuantumRegister(1);
        reg.applyGate(0, QuantumGates.phase());
        reg.normalizeAll();

        double prob = reg.getQubit(0).measureProbability();
        assertEquals(1.0, prob, 0.001, "Phase gate should not change |0⟩ measurement probability");
    }

    @Test
    public void testTGateAppliesPhaseToQubit() {
        QuantumRegister reg = new QuantumRegister(1);
        reg.applyGate(0, QuantumGates.tGate());
        reg.normalizeAll();

        double prob = reg.getQubit(0).measureProbability();
        assertEquals(1.0, prob, 0.001, "T gate should not affect |0⟩ measurement of |0⟩ state");
    }

    @Test
    public void testCustomGateBuildsCorrectly() {
        double[] raw = {
            0.1, 0.2,
            0.3, 0.4,
            0.5, 0.6,
            0.7, 0.8,
            0.9, 1.0,
            1.1, 1.2
        };

        QuantumNumber gate = QuantumGates.custom(raw);
        assertNotNull(gate);
        assertTrue(gate.toString().contains("0.10 + 0.20i"), "Gate should reflect custom value");
    }

    @Test
    public void testInvalidCustomGateThrows() {
        double[] bad = { 0.1, 0.2, 0.3 }; // too short
        assertThrows(IllegalArgumentException.class, () -> QuantumGates.custom(bad));
    }
}
