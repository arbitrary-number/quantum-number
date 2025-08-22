package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.*;

import com.github.quantum_number.qubit.*;
import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class ComplexQuantumGateV1Test {

    private QuantumNumberComponent a;
    private Complex one = new Complex(1, 0);
    private Complex zero = Complex.ZERO;

    private ComplexQuantumRegister input;

    @BeforeEach
    public void setup() {
        // Simple basis
        a = QuantumNumberComponent.a;

        // |ψ⟩ = [1, 0]
        input = new ComplexQuantumRegister(2);
        ComplexQuantumNumber state0 = new ComplexQuantumNumber();
        state0.addComponent(a, one);

        input.setAmplitude(0, state0); // |0⟩ state
        input.setAmplitude(1, new ComplexQuantumNumber()); // |1⟩ is zero
    }

    @Test
    public void testIdentityGate() {
        Complex[][] identity = {
            {one, zero},
            {zero, one}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(identity);
        ComplexQuantumRegister result = gate.apply(input);

        assertEquals(input.getAmplitude(0), result.getAmplitude(0));
        assertEquals(input.getAmplitude(1), result.getAmplitude(1));
    }

    @Test
    public void testXGate() {
        // Pauli-X gate
        Complex[][] xGate = {
            {zero, one},
            {one, zero}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(xGate);
        ComplexQuantumRegister result = gate.apply(input);

        assertEquals(input.getAmplitude(0), result.getAmplitude(1));
        assertTrue(result.getAmplitude(0).isZero());
    }

    @Test
    public void testHadamardGate() {
        double sqrt2inv = 1.0 / Math.sqrt(2);
        Complex h = new Complex(sqrt2inv, 0);

        Complex[][] hadamard = {
            {h, h},
            {h, h.negate()}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(hadamard);
        ComplexQuantumRegister result = gate.apply(input);

        ComplexQuantumNumber out0 = result.getAmplitude(0);
        ComplexQuantumNumber out1 = result.getAmplitude(1);

        Complex expected = new Complex(sqrt2inv, 0);
        assertEquals(expected.getReal(), out0.getCoefficient(a).getReal(), 1e-10);
        assertEquals(expected.getImaginary(), out0.getCoefficient(a).getImaginary(), 1e-10);
    }

    @Test
    public void testInvalidDimensions() {
        Complex[][] badMatrix = {
            {one, zero}
            // Not square
        };

        assertThrows(IllegalArgumentException.class, () -> new ComplexQuantumGate(badMatrix));
    }

    @Test
    public void testMismatchedInputSize() {
        Complex[][] matrix = {
            {one, zero},
            {zero, one}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(matrix);

        ComplexQuantumRegister smallRegister = new ComplexQuantumRegister(1);

        assertThrows(IllegalArgumentException.class, () -> gate.apply(smallRegister));
    }

    @Test
    public void testYGate() {
        Complex i = Complex.I;
        Complex minusI = i.negate();

        Complex[][] yGate = {
            {zero, minusI},
            {i, zero}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(yGate);
        ComplexQuantumRegister result = gate.apply(input);

        Complex expected = Complex.I; // Pauli-Y on |0⟩ → i|1⟩
        assertTrue(result.getAmplitude(0).isZero());
        assertComplexEquals(expected, result.getAmplitude(1).getCoefficient(a), 1e-10);
    }

    @Test
    public void testZGate() {
        Complex[][] zGate = {
            {one, zero},
            {zero, one.negate()}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(zGate);
        ComplexQuantumRegister result = gate.apply(input);

        // Should remain |0⟩ unchanged
        assertEquals(input.getAmplitude(0), result.getAmplitude(0));
        assertTrue(result.getAmplitude(1).isZero());
    }

    @Test
    public void testPhaseGate() {
        Complex i = Complex.I;

        Complex[][] sGate = {
            {one, zero},
            {zero, i}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(sGate);
        ComplexQuantumRegister result = gate.apply(input);

        // Since |0⟩ → |0⟩ under phase gate
        assertEquals(input.getAmplitude(0), result.getAmplitude(0));
        assertTrue(result.getAmplitude(1).isZero());
    }

    @Test
    public void testCNOTGate() {
        // Setup 2-qubit register: |10⟩ (control = 1, target = 0)
        ComplexQuantumRegister twoQubitInput = new ComplexQuantumRegister(4);
        ComplexQuantumNumber state = new ComplexQuantumNumber();
        state.addComponent(a, one);
        twoQubitInput.setAmplitude(2, state); // |10⟩ → index 2

        // CNOT Gate (4x4)
        Complex[][] cnot = {
            {one, zero, zero, zero},
            {zero, one, zero, zero},
            {zero, zero, zero, one},
            {zero, zero, one, zero}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(cnot);
        ComplexQuantumRegister result = gate.apply(twoQubitInput);

        // CNOT on |10⟩ → |11⟩ → index 3
        assertTrue(result.getAmplitude(2).isZero());
        assertEquals(one, result.getAmplitude(3).getCoefficient(a));
    }

    private void assertComplexEquals(Complex expected, Complex actual, double delta) {
        assertEquals(expected.getReal(), actual.getReal(), delta, "Real parts differ");
        assertEquals(expected.getImaginary(), actual.getImaginary(), delta, "Imaginary parts differ");
    }

    @Test
    public void testToffoliGate() {
        Complex one = Complex.ONE;
        Complex zero = Complex.ZERO;

        // 3-qubit register → size 8
        ComplexQuantumRegister threeQubitInput = new ComplexQuantumRegister(8);

        // Set |110⟩ state (index 6) with coefficient 1 on basis component 'a'
        ComplexQuantumNumber state110 = new ComplexQuantumNumber();
        state110.addComponent(a, one);
        threeQubitInput.setAmplitude(6, state110);

        // Toffoli gate (8x8 matrix)
        Complex[][] toffoli = new Complex[8][8];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                toffoli[i][j] = (i == j) ? one : zero;
            }
        }
        // Flip |110⟩ <-> |111⟩
        toffoli[6][6] = zero;
        toffoli[7][7] = zero;
        toffoli[6][7] = one;
        toffoli[7][6] = one;

        ComplexQuantumGate gate = new ComplexQuantumGate(toffoli);
        ComplexQuantumRegister result = gate.apply(threeQubitInput);

        // |110⟩ → |111⟩
        assertTrue(result.getAmplitude(6).isZero());
        assertEquals(one, result.getAmplitude(7).getCoefficient(a));
    }

    @Test
    public void testPhaseTGate() {
        Complex one = Complex.ONE;
        Complex zero = Complex.ZERO;
        double angle = Math.PI / 4;
        Complex t = new Complex(Math.cos(angle), Math.sin(angle));

        Complex[][] tGate = {
            {one, zero},
            {zero, t}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(tGate);
        ComplexQuantumRegister result = gate.apply(input);

        // |0⟩ unchanged
        assertEquals(input.getAmplitude(0), result.getAmplitude(0));
        // |1⟩ zero amplitude → still zero
        assertTrue(result.getAmplitude(1).isZero());
    }

    @Test
    public void testRotationZGate() {
        Complex one = Complex.ONE;
        Complex zero = Complex.ZERO;
        double theta = Math.PI / 2;

        Complex expNeg = new Complex(Math.cos(-theta / 2), Math.sin(-theta / 2));
        Complex expPos = new Complex(Math.cos(theta / 2), Math.sin(theta / 2));

        Complex[][] rz = {
            {expNeg, zero},
            {zero, expPos}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(rz);
        ComplexQuantumRegister result = gate.apply(input);

        ComplexQuantumNumber out0 = result.getAmplitude(0);
        Complex expected0 = expNeg;

        assertComplexEquals(expected0, out0.getCoefficient(a), 1e-10);

        // |1⟩ amplitude zero → remains zero
        assertTrue(result.getAmplitude(1).isZero());
    }

    @Test
    public void testApplyGateOnZeroVector() {
        Complex zero = Complex.ZERO;
        ComplexQuantumRegister zeroRegister = new ComplexQuantumRegister(2);
        // Overwrite amplitudes to zero vector
        zeroRegister.setAmplitude(0, new ComplexQuantumNumber());
        zeroRegister.setAmplitude(1, new ComplexQuantumNumber());

        ComplexQuantumGate gate = ComplexQuantumGate.pauliX(); // or any gate
        ComplexQuantumRegister result = gate.apply(zeroRegister);

        assertTrue(result.getAmplitude(0).isZero());
        assertTrue(result.getAmplitude(1).isZero());
    }

    @Test
    public void testNormalizationAfterGate() {
        // Start with normalized input (|0> state)
        ComplexQuantumGate hadamard = ComplexQuantumGate.hadamard();
        ComplexQuantumRegister result = hadamard.apply(input);

        // Should be normalized after application? Let's normalize explicitly
        result.normalize();

        double sumSquares = 0.0;
        for (int i = 0; i < result.size(); i++) {
            sumSquares += result.getAmplitude(i).magnitudeSquared();
        }
        assertEquals(1.0, sumSquares, 1e-10);
    }

    @Test
    public void testSwapGate() {
        // 2-qubit register: |01⟩ (index 1)
        ComplexQuantumRegister input2 = new ComplexQuantumRegister(4);
        ComplexQuantumNumber state = new ComplexQuantumNumber();
        state.addComponent(a, one);
        input2.setAmplitude(1, state);

        ComplexQuantumGate swap = ComplexQuantumGate.swapGate();
        ComplexQuantumRegister result = swap.apply(input2);

        // Swap |01⟩ → |10⟩ (index 2)
        assertTrue(result.getAmplitude(1).isZero());
        assertEquals(one, result.getAmplitude(2).getCoefficient(a));
    }

    @Test
    public void testControlledZGate() {
        // 2-qubit register |11⟩ (index 3)
        ComplexQuantumRegister input2 = new ComplexQuantumRegister(4);
        ComplexQuantumNumber state = new ComplexQuantumNumber();
        state.addComponent(a, one);
        input2.setAmplitude(3, state);

        ComplexQuantumGate cz = ComplexQuantumGate.controlledZGate();
        ComplexQuantumRegister result = cz.apply(input2);

        // The amplitude at |11⟩ should be multiplied by -1
        Complex expected = one.negate();
        assertComplexEquals(expected, result.getAmplitude(3).getCoefficient(a), 1e-10);

        // Other amplitudes remain zero
        for (int i = 0; i < 3; i++) {
            assertTrue(result.getAmplitude(i).isZero());
        }
    }

    @Test
    public void testPhaseShiftGate() {
        double theta = Math.PI / 3;  // 60 degrees
        Complex phase = new Complex(Math.cos(theta), Math.sin(theta));

        ComplexQuantumGate phaseShift = ComplexQuantumGate.phaseShiftGate(theta);
        ComplexQuantumRegister result = phaseShift.apply(input);

        // |0⟩ unchanged
        assertEquals(input.getAmplitude(0), result.getAmplitude(0));

        // |1⟩ zero amplitude → remains zero
        assertTrue(result.getAmplitude(1).isZero());

        // Now test on |1> state
        ComplexQuantumRegister inputOne = new ComplexQuantumRegister(2);
        ComplexQuantumNumber stateOne = new ComplexQuantumNumber();
        stateOne.addComponent(a, one);
        inputOne.setAmplitude(1, stateOne);

        ComplexQuantumRegister resultOne = phaseShift.apply(inputOne);

        ComplexQuantumNumber out1 = resultOne.getAmplitude(1);
        assertComplexEquals(phase, out1.getCoefficient(a), 1e-10);

        // |0⟩ amplitude zero → remains zero
        assertTrue(resultOne.getAmplitude(0).isZero());
    }

    @Test
    public void testFredkinGate() {
        // 3-qubit register, index 5 corresponds to |101>
        ComplexQuantumRegister input3 = new ComplexQuantumRegister(8);
        ComplexQuantumNumber state = new ComplexQuantumNumber();
        state.addComponent(a, one);
        input3.setAmplitude(5, state);

        ComplexQuantumGate fredkin = ComplexQuantumGate.fredkinGate();
        ComplexQuantumRegister result = fredkin.apply(input3);

        // Fredkin swaps qubits 2 and 3 if qubit 1 is 1
        // So |101> (index 5) swaps to |110> (index 6)
        assertTrue(result.getAmplitude(5).isZero());
        assertEquals(one, result.getAmplitude(6).getCoefficient(a));
    }

    @Test
    public void testApplyGateToLargerRegisterThrows() {
        Complex[][] xGate = {
            {zero, one},
            {one, zero}
        };

        ComplexQuantumGate gate = new ComplexQuantumGate(xGate);

        ComplexQuantumRegister largeRegister = new ComplexQuantumRegister(4); // size 4 (2 qubits)

        // Applying a 2x2 gate to 4-dimensional register should throw IllegalArgumentException
        assertThrows(IllegalArgumentException.class, () -> gate.apply(largeRegister));
    }



}
