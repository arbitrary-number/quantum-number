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

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.RepeatedTest;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.QuantumNumber;

public class QuantumNumberTest {

    private QuantumNumber zeroState;
    private QuantumNumber oneState;
    private QuantumNumber testGate;

    @BeforeEach
    public void setUp() {
        // |0⟩-like QuantumNumber
        zeroState = new QuantumNumber(
            1, 0,
            1, 0,
            1, 0,
            0, 0,  // β = 0
            1, 0,
            1, 0
        );

        // |1⟩-like QuantumNumber
        oneState = new QuantumNumber(
            0, 0,
            1, 0,
            1, 0,
            1, 0,  // β = 1
            1, 0,
            1, 0
        );

        // Example gate (simple non-zero transformation)
        testGate = new QuantumNumber(
            0.5, 0.5,
            0.5, 0.0,
            0.5, 0.0,
            0.5, 0.0,
            0.5, 0.0,
            0.5, 0.0
        );
    }

    @Test
    public void testAdditionIsCommutative() {
        QuantumNumber result1 = zeroState.add(testGate);
        QuantumNumber result2 = testGate.add(zeroState);
        assertEquals(result1.toString(), result2.toString(), "Addition should be commutative");
    }

    @Test
    public void testInverseOperation() {
        QuantumNumber inverse = testGate.inverse();
        QuantumNumber neutral = testGate.add(inverse);
        assertEquals("((0.00 + 0.00i) / ((0.00 + 0.00i) / (0.00 + 0.00i))) ÷ ((0.00 + 0.00i) / ((0.00 + 0.00i) / (0.00 + 0.00i)))",
                     neutral.toString(), "Gate + Inverse should return to neutral state");
    }

    @Test
    public void testSubtractionEquivalentToAddingInverse() {
        QuantumNumber result1 = zeroState.subtract(testGate);
        QuantumNumber result2 = zeroState.add(testGate.inverse());
        assertEquals(result1.toString(), result2.toString(), "Subtraction should be same as adding inverse");
    }

    @Test
    public void testMeasurementProbabilityOfZeroStateCloseTo1() {
        double p0 = zeroState.measureProbability();
        assertTrue(p0 > 0.99, "Probability of measuring |0⟩ should be near 1.0");
    }

    @Test
    public void testMeasurementProbabilityOfOneStateCloseTo0() {
        double p0 = oneState.measureProbability();
        assertTrue(p0 < 0.01, "Probability of measuring |0⟩ in |1⟩ state should be near 0.0");
    }

    @RepeatedTest(100)
    public void testMeasurementStatisticsOnZeroState() {
        int outcome = zeroState.measure();
        assertEquals(0, outcome, "Repeated measurements of |0⟩ state should always yield 0");
    }

    @RepeatedTest(100)
    public void testMeasurementStatisticsOnOneState() {
        int outcome = oneState.measure();
        assertEquals(1, outcome, "Repeated measurements of |1⟩ state should always yield 1");
    }

    @Test
    public void testToStringFormatting() {
        String repr = zeroState.toString();
        assertTrue(repr.contains("+"), "String representation should include complex formatting");
        assertTrue(repr.contains("i"), "String representation should include imaginary unit");
    }

    @Test
    public void testMeasurementEdgeCaseWithZeroAmplitude() {
        QuantumNumber weirdState = new QuantumNumber(
            0, 0, 0, 0, 0, 0,  // α = 0
            0, 0, 0, 0, 0, 0   // β = 0 (fully zeroed)
        );
        double prob = weirdState.measureProbability();
        assertFalse(Double.isNaN(prob), "Probability should be defined even if amplitudes are zero");
    }

    @Test
    public void testAddGateTwiceIncreasesProbability() {
        QuantumNumber after1 = zeroState.add(testGate);
        QuantumNumber after2 = after1.add(testGate);

        double p0_1 = after1.measureProbability();
        double p0_2 = after2.measureProbability();

        assertTrue(p0_2 < p0_1, "Repeated gate application should reduce |0⟩ probability");
    }
}
