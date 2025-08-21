package com.github.quantum_number.qubit;

import org.apache.commons.math3.complex.Complex;
import java.util.Arrays;

public class ComplexQuantumGate {

    private final Complex[][] matrix;
    private final int dimension;

    // Constructor accepts square matrix
    public ComplexQuantumGate(Complex[][] matrix) {
        if (matrix == null || matrix.length == 0 || matrix.length != matrix[0].length) {
            throw new IllegalArgumentException("Matrix must be non-empty square matrix");
        }
        this.dimension = matrix.length;
        this.matrix = new Complex[dimension][dimension];
        for (int i = 0; i < dimension; i++) {
            if (matrix[i].length != dimension) {
                throw new IllegalArgumentException("Matrix rows must have same length as columns");
            }
            for (int j = 0; j < dimension; j++) {
                this.matrix[i][j] = matrix[i][j];
            }
        }
    }

    // Apply gate to a ComplexQuantumRegister and return a new ComplexQuantumRegister
    public ComplexQuantumRegister apply(ComplexQuantumRegister input) {
        if (input.getDimension() != this.dimension) {
            throw new IllegalArgumentException("Input register dimension does not match gate dimension");
        }

        ComplexQuantumRegister result = new ComplexQuantumRegister(dimension);

        for (int row = 0; row < dimension; row++) {
            Complex sum = Complex.ZERO;
            for (int col = 0; col < dimension; col++) {
                Complex amplitude = input.getAmplitude(col);
                sum = sum.add(matrix[row][col].multiply(amplitude));
            }
            result.setAmplitude(row, sum);
        }
        return result;
    }

    // Optional: check unitarity of matrix (U * U† = I)
    public boolean isUnitary(double tolerance) {
        // TODO: implement
        return true;
    }

    // Getters
    public Complex[][] getMatrix() {
        // Return defensive copy or immutable view as needed
        Complex[][] copy = new Complex[dimension][dimension];
        for (int i = 0; i < dimension; i++) {
            copy[i] = Arrays.copyOf(matrix[i], dimension);
        }
        return copy;
    }

    public int getDimension() {
        return dimension;
    }
}
