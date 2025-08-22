package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumCircuit;
import com.github.quantum_number.qubit.ComplexQuantumGate;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.QuantumNumberComponent;

public class ComplexQuantumRegisterTest {

    private ComplexQuantumRegister register;

    @BeforeEach
    public void setup() {
        register = ComplexQuantumRegister.ofQubits(2);
    }


    private static final QuantumNumberComponent a = QuantumNumberComponent.a;


    @Test
    public void testRegisterTensorProductProducesExpectedAmplitudes2() {
        // Prepare register r1 = [1, 0]
        ComplexQuantumRegister r1 = ComplexQuantumRegister.ofQubits(2);
        ComplexQuantumNumber amp0 = new ComplexQuantumNumber();
        amp0.addComponent(a, Complex.ONE); // |0⟩
        r1.setAmplitude(0, amp0);
        r1.setAmplitude(1, new ComplexQuantumNumber()); // |1⟩ = 0

        // Prepare register r2 = [0, i]
        ComplexQuantumRegister r2 = ComplexQuantumRegister.ofQubits(2);
        ComplexQuantumNumber amp1 = new ComplexQuantumNumber();
        amp1.addComponent(a, new Complex(0, 1)); // i * |1⟩
        r2.setAmplitude(0, new ComplexQuantumNumber()); // |0⟩ = 0
        r2.setAmplitude(1, amp1);

        // Tensor product: r1 ⊗ r2
        ComplexQuantumRegister result = ComplexQuantumRegister.tensorProduct(r1, r2);

        // Expected size = 4 (2 qubits)
        assertEquals(16, result.size());

        // Expected amplitudes:
        // |00⟩ = 0
        assertEquals(0.0, result.getAmplitude(0).getCoefficient(a).abs(), 1e-10);

        // |01⟩ = 1 * i = i
        Complex amp01 = result.getAmplitude(1).getCoefficient(a);
        assertEquals(0.0, amp01.getReal(), 1e-10);
        assertEquals(1.0, amp01.getImaginary(), 1e-10);

        // |10⟩ = 0
        assertEquals(0.0, result.getAmplitude(2).getCoefficient(a).abs(), 1e-10);

        // |11⟩ = 0
        assertEquals(0.0, result.getAmplitude(3).getCoefficient(a).abs(), 1e-10);
    }

    @Test
    public void testRegisterTensorProductProducesExpectedAmplitudes() {
        // Prepare register r1 = [1, 0] (1 qubit)
        ComplexQuantumRegister r1 = ComplexQuantumRegister.ofQubits(1);
        ComplexQuantumNumber amp0 = new ComplexQuantumNumber();
        amp0.addComponent(a, Complex.ONE); // coefficient 1 for |0⟩
        r1.setAmplitude(0, amp0);
        r1.setAmplitude(1, new ComplexQuantumNumber()); // zero amplitude for |1⟩

        // Prepare register r2 = [0, i] (1 qubit)
        ComplexQuantumRegister r2 = ComplexQuantumRegister.ofQubits(1);
        ComplexQuantumNumber amp1 = new ComplexQuantumNumber();
        amp1.addComponent(a, new Complex(0, 1)); // coefficient i for |1⟩
        r2.setAmplitude(0, new ComplexQuantumNumber()); // zero amplitude for |0⟩
        r2.setAmplitude(1, amp1);

        // Tensor product: r1 ⊗ r2 (results in 2 qubits = 4 amplitudes)
        ComplexQuantumRegister result = ComplexQuantumRegister.tensorProduct(r1, r2);

        // Expected size = 4 (2 qubits)
        assertEquals(4, result.size());

        // Expected amplitudes:
        // |00⟩ = r1[0]*r2[0] = 1*0 = 0
        assertEquals(0.0, result.getAmplitude(0).getCoefficient(a).abs(), 1e-10);

        // |01⟩ = r1[0]*r2[1] = 1 * i = i
        Complex amp01 = result.getAmplitude(1).getCoefficient(a);
        assertEquals(0.0, amp01.getReal(), 1e-10);
        assertEquals(1.0, amp01.getImaginary(), 1e-10);

        // |10⟩ = r1[1]*r2[0] = 0 * 0 = 0
        assertEquals(0.0, result.getAmplitude(2).getCoefficient(a).abs(), 1e-10);

        // |11⟩ = r1[1]*r2[1] = 0 * i = 0
        assertEquals(0.0, result.getAmplitude(3).getCoefficient(a).abs(), 1e-10);
    }


}
