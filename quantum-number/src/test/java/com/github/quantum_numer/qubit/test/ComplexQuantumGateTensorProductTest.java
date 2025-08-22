package com.github.quantum_numer.qubit.test;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumGate;

import org.apache.commons.math3.complex.Complex;

public class ComplexQuantumGateTensorProductTest {

    // Helper method to create a 2x2 identity matrix
    private Complex[][] identity2x2() {
        return new Complex[][] {
            {Complex.ONE, Complex.ZERO},
            {Complex.ZERO, Complex.ONE}
        };
    }

    // Helper method to create a 2x2 Hadamard matrix
    private Complex[][] hadamard2x2() {
        double invSqrt2 = 1.0 / Math.sqrt(2);
        return new Complex[][] {
            {new Complex(invSqrt2, 0), new Complex(invSqrt2, 0)},
            {new Complex(invSqrt2, 0), new Complex(-invSqrt2, 0)}
        };
    }

    @Test
    public void testKroneckerProduct() {
        Complex[][] A = identity2x2();
        Complex[][] B = hadamard2x2();

        Complex[][] expected = new Complex[4][4];
        // Manually calculate expected tensor product of I ⊗ H
        // Result should be:
        // [H 0]
        // [0 H]
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                expected[i][j] = Complex.ZERO;
            }
        }
        // Top-left 2x2 block = H
        expected[0][0] = new Complex(1/Math.sqrt(2), 0);
        expected[0][1] = new Complex(1/Math.sqrt(2), 0);
        expected[1][0] = new Complex(1/Math.sqrt(2), 0);
        expected[1][1] = new Complex(-1/Math.sqrt(2), 0);

        // Bottom-right 2x2 block = H
        expected[2][2] = new Complex(1/Math.sqrt(2), 0);
        expected[2][3] = new Complex(1/Math.sqrt(2), 0);
        expected[3][2] = new Complex(1/Math.sqrt(2), 0);
        expected[3][3] = new Complex(-1/Math.sqrt(2), 0);

        Complex[][] result = ComplexQuantumGate.kroneckerProduct(A, B);

        assertEquals(4, result.length);
        assertEquals(4, result[0].length);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                assertEquals(expected[i][j].getReal(), result[i][j].getReal(), 1e-10, "Mismatch at (" + i + "," + j + ") real");
                assertEquals(expected[i][j].getImaginary(), result[i][j].getImaginary(), 1e-10, "Mismatch at (" + i + "," + j + ") imag");
            }
        }
    }

    @Test
    public void testTensorProductGateQubitCountAndMatrixSize() {
        // Prepare gates
        ComplexQuantumGate I = new ComplexQuantumGate(identity2x2()); //, 1);
        ComplexQuantumGate H = new ComplexQuantumGate(hadamard2x2()); //, 1);

        // Tensor product H ⊗ I
        ComplexQuantumGate HI = ComplexQuantumGate.tensorProduct(H, I);

        // Should be a 2-qubit gate now
        assertEquals(2, HI.getNumQubits());

        // Matrix should be 4x4
        Complex[][] mat = HI.getMatrix();
        assertEquals(4, mat.length);
        assertEquals(4, mat[0].length);
    }
}
