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
package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.RepeatedTest;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.QuantumNumber;
import com.github.quantum_number.qubit.QuantumRegisterV1;

public class QuantumRegisterTest {

    private QuantumRegisterV1 reg;

    @BeforeEach
    public void setup() {
        reg = new QuantumRegisterV1(3);
    }

    @Test
    public void testRegisterInitialization() {
        assertEquals(3, reg.size(), "Register should be initialized with 3 qubits");
        for (int i = 0; i < reg.size(); i++) {
            QuantumNumber q = reg.getQubit(i);
            assertNotNull(q, "Each qubit should be initialized");
        }
    }

    @Test
    public void testSetAndGetQubit() {
        QuantumNumber customQubit = new QuantumNumber(
            0.1, 0.2,
            0.3, 0.4,
            0.5, 0.6,
            0.7, 0.8,
            0.9, 1.0,
            1.1, 1.2
        );
        reg.setQubit(1, customQubit);

        QuantumNumber result = reg.getQubit(1);
        assertEquals(customQubit.toString(), result.toString(), "Qubit should be updated correctly");
    }

    @Test
    public void testApplyGateUpdatesQubit() {
        QuantumNumber original = reg.getQubit(0);
        QuantumNumber gate = new QuantumNumber(
            0.5, 0.5,
            0.5, 0, 0.5, 0,
            0.5, 0, 0.5, 0,
            0.5, 0
        );

        reg.applyGate(0, gate);
        QuantumNumber updated = reg.getQubit(0);

        assertNotEquals(original.toString(), updated.toString(), "Qubit should change after gate application");
    }

    @Test
    public void testNormalizeAllDoesNotThrow() {
        assertDoesNotThrow(() -> reg.normalizeAll(), "Normalization should not throw on default qubits");
    }

    @Test
    public void testMeasureQubitReturnsValidState() {
        int outcome = reg.measureQubit(0);
        assertTrue(outcome == 0 || outcome == 1, "Measurement must be 0 or 1");
    }

    @RepeatedTest(50)
    public void testMeasureAllReturnsExpectedLength() {
        int[] outcomes = reg.measureAll();
        assertEquals(3, outcomes.length, "Measurement array should match number of qubits");
        for (int out : outcomes) {
            assertTrue(out == 0 || out == 1, "Each measurement result must be 0 or 1");
        }
    }

    @Test
    public void testToStringIncludesAllQubits() {
        String desc = reg.toString();
        for (int i = 0; i < reg.size(); i++) {
            assertTrue(desc.contains("Qubit " + i), "toString should include all qubits");
        }
    }

    @Test
    public void testInvalidRegisterSizeThrows() {
        assertThrows(IllegalArgumentException.class, () -> new QuantumRegisterV1(0));
    }
}
