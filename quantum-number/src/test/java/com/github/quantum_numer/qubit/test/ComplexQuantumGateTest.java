package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumGate;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.QuantumNumberComponent;

public class ComplexQuantumGateTest {

    private static final double EPSILON = 1e-10;

//    @Test
//    public void testHadamardMatrix() {
//        ComplexQuantumGate H = ComplexQuantumGate.hadamard();
//        Complex[][] matrix = H.getMatrix();
//
//        assertEquals(2, matrix.length, "Hadamard should be 2x2");
//        assertEquals(2, matrix[0].length);
//
//        double invSqrt2 = 1 / Math.sqrt(2);
//
//        assertEquals(invSqrt2, matrix[0][0].getReal(), EPSILON);
//        assertEquals(0.0, matrix[0][0].getImaginary(), EPSILON);
//
//        assertEquals(invSqrt2, matrix[0][1].getReal(), EPSILON);
//        assertEquals(0.0, matrix[0][1].getImaginary(), EPSILON);
//
//        assertEquals(invSqrt2, matrix[1][0].getReal(), EPSILON);
//        assertEquals(0.0, matrix[1][0].getImaginary(), EPSILON);
//
//        assertEquals(-invSqrt2, matrix[1][1].getReal(), EPSILON);
//        assertEquals(0.0, matrix[1][1].getImaginary(), EPSILON);
//    }
//
//    @Test
//    public void testCNOTMatrix() {
//        ComplexQuantumGate cnot = ComplexQuantumGate.cnot();
//        Complex[][] matrix = cnot.getMatrix();
//
//        assertEquals(4, matrix.length, "CNOT should be 4x4");
//        assertEquals(4, matrix[0].length);
//
//        // Expected identity entries for |00> and |01>
//        assertEquals(Complex.ONE, matrix[0][0]);
//        assertEquals(Complex.ZERO, matrix[0][1]);
//        assertEquals(Complex.ZERO, matrix[1][0]);
//        assertEquals(Complex.ONE, matrix[1][1]);
//
//        // Expected swap entries for |10> and |11>
//        assertEquals(Complex.ZERO, matrix[2][2]);
//        assertEquals(Complex.ONE, matrix[2][3]);
//        assertEquals(Complex.ONE, matrix[3][2]);
//        assertEquals(Complex.ZERO, matrix[3][3]);
//    }
//
//    @Test
//    public void testApplyHadamardToZero() {
//        ComplexQuantumGate H = ComplexQuantumGate.hadamard();
//        ComplexQuantumRegister reg = ComplexQuantumRegister.withZeroState(1); // |0>
//
//        ComplexQuantumRegister result = ComplexQuantumRegister.applyGateOnQubit(reg, H, 0);
//
//        // Result should be (|0> + |1>) / sqrt(2)
//        ComplexQuantumNumber amp0 = result.getAmplitude(0);
//        ComplexQuantumNumber amp1 = result.getAmplitude(1);
//
//        Complex coef0 = amp0.getCoefficient(QuantumNumberComponent.a);
//        Complex coef1 = amp1.getCoefficient(QuantumNumberComponent.a);
//
//        double invSqrt2 = 1 / Math.sqrt(2);
//
//        assertEquals(invSqrt2, coef0.abs(), EPSILON, "|0> amplitude magnitude");
//        assertEquals(invSqrt2, coef1.abs(), EPSILON, "|1> amplitude magnitude");
//    }
//
    @Test
    public void testApplyCNOT() {
        ComplexQuantumGate cnot = ComplexQuantumGate.cnot();

        // Prepare |10> = amplitude 2 = 1, others 0 for 2 qubits
        ComplexQuantumRegister reg = ComplexQuantumRegister.ofQubits(2);
        ComplexQuantumNumber amp2 = new ComplexQuantumNumber();
        amp2.addComponent(QuantumNumberComponent.a, Complex.ONE);
        reg.setAmplitude(2, amp2);
        reg.setAmplitude(0, new ComplexQuantumNumber());
        reg.setAmplitude(1, new ComplexQuantumNumber());
        reg.setAmplitude(3, new ComplexQuantumNumber());

        System.out.println("reg = " + reg);
        System.out.println("cnot = " + cnot);
        ComplexQuantumRegister result = ComplexQuantumRegister.applyGateMatrix(reg, cnot);

        // Applying CNOT with control=0, target=1 flips |10> to |11>
        ComplexQuantumNumber resultAmp2 = result.getAmplitude(2);
        ComplexQuantumNumber resultAmp3 = result.getAmplitude(3);

        System.out.println("Result amplitudes:");
        for (int i = 0; i < result.size(); i++) {
            ComplexQuantumNumber amp = result.getAmplitude(i);
            System.out.println("|" + Integer.toBinaryString(i) + "⟩: " + amp.getCoefficient(QuantumNumberComponent.a));
        }

        assertEquals(0.0, resultAmp2.magnitudeSquared(), EPSILON, "Amplitude |10> should be 0");
        assertTrue(resultAmp3.magnitudeSquared() > 0, "Amplitude |11> should be nonzero");
    }
}
