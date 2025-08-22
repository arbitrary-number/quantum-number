package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.*;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.*;

public class ComplexQuantumMeasurementControllerTest {

    @Test
    public void testMeasureQubitCollapsesState() {
        // Prepare 2-qubit register in |00> + |10> normalized (amplitudes 1/sqrt(2))
        int nQubits = 2;
        ComplexQuantumRegister reg = ComplexQuantumRegister.ofQubits(nQubits);

        ComplexQuantumNumber amp0 = new ComplexQuantumNumber();
        amp0.addComponent(QuantumNumberComponent.a, new Complex(1/Math.sqrt(2), 0));
        ComplexQuantumNumber amp2 = new ComplexQuantumNumber();
        amp2.addComponent(QuantumNumberComponent.a, new Complex(1/Math.sqrt(2), 0));

        reg.setAmplitude(0, amp0);
        reg.setAmplitude(1, new ComplexQuantumNumber());
        reg.setAmplitude(2, amp2);
        reg.setAmplitude(3, new ComplexQuantumNumber());

        // Measure qubit 0 (LSB)
        int result = ComplexQuantumMeasurement.measureQubit(reg, 0);

        // Result should be 0 or 1
        assertTrue(result == 0 || result == 1);

        // After measurement, all amplitudes where qubit0 != result should be zero
        for (int i = 0; i < reg.size(); i++) {
            int bit = (i >> 0) & 1;
            if (bit != result) {
                assertEquals(0.0, reg.getAmplitude(i).normSquared(), 1e-10, "Amplitude should be 0 after collapse");
            }
        }

        // Register should remain normalized
        double norm = 0.0;
        for (int i = 0; i < reg.size(); i++) {
            norm += reg.getAmplitude(i).normSquared();
        }
        assertEquals(1.0, norm, 1e-10);
    }

    @Test
    public void testApplyConditionalGate() {
        // Prepare 1-qubit |0> state
        int nQubits = 1;
        ComplexQuantumRegister reg = ComplexQuantumRegister.ofQubits(nQubits);

        ComplexQuantumNumber amp0 = new ComplexQuantumNumber();
        amp0.addComponent(QuantumNumberComponent.a, Complex.ONE);
        reg.setAmplitude(0, amp0);
        reg.setAmplitude(1, new ComplexQuantumNumber());

        // Define Pauli-X gate
        Complex[][] xMatrix = {
            {Complex.ZERO, Complex.ONE},
            {Complex.ONE, Complex.ZERO}
        };
        ComplexQuantumGate xGate = new ComplexQuantumGate(xMatrix);

        // Apply X gate conditionally if measuredBit == 1 (should NOT apply)
        ComplexQuantumRegister regAfter = ComplexQuantumController.applyConditionalGate(reg, 0, 1, xGate);
        assertEquals(reg.getAmplitude(0).getCoefficient(QuantumNumberComponent.a), regAfter.getAmplitude(0).getCoefficient(QuantumNumberComponent.a));
        assertEquals(reg.getAmplitude(1).getCoefficient(QuantumNumberComponent.a), regAfter.getAmplitude(1).getCoefficient(QuantumNumberComponent.a));

        // Apply X gate conditionally if measuredBit == 0 (should apply)
        regAfter = ComplexQuantumController.applyConditionalGate(reg, 0, 0, xGate);
        // Now amplitude |1> should be 1, |0> should be 0 after X gate on |0>
        assertEquals(0.0, regAfter.getAmplitude(0).normSquared(), 1e-10);
        assertEquals(1.0, regAfter.getAmplitude(1).normSquared(), 1e-10);
    }
}
