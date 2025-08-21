package com.github.quantum_number.qubit;

import org.apache.commons.math3.complex.Complex;

public class ComplexQuantumGate {

    private final Complex[][] matrix;  // Gate represented as NxN matrix of Complex scalars
    private final int dimension;

    public ComplexQuantumGate(Complex[][] matrix) {
        if (matrix == null || matrix.length == 0 || matrix.length != matrix[0].length) {
            throw new IllegalArgumentException("Matrix must be square and non-empty");
        }
        this.dimension = matrix.length;
        this.matrix = new Complex[dimension][dimension];
        for (int i = 0; i < dimension; i++) {
            if (matrix[i].length != dimension)
                throw new IllegalArgumentException("Matrix rows must have same length as dimension");
            for (int j = 0; j < dimension; j++) {
                this.matrix[i][j] = matrix[i][j];
            }
        }
    }

    public int getDimension() {
        return dimension;
    }

    /**
     * Apply the gate to a ComplexQuantumRegister and return a new register as result.
     */
    public ComplexQuantumRegister apply(ComplexQuantumRegister input) {
        if (input.getDimension() != dimension) {
            throw new IllegalArgumentException("Input register dimension mismatch");
        }

        ComplexQuantumRegister output = new ComplexQuantumRegister(dimension);

        // Apply matrix multiplication: output = matrix * input
        for (int row = 0; row < dimension; row++) {
            ComplexQuantumNumber resultVector = new ComplexQuantumNumber();

            for (int col = 0; col < dimension; col++) {
                // matrix[row][col] is Complex scalar
                Complex scalar = matrix[row][col];

                // input.getAmplitude(col) returns ComplexQuantumNumber (the vector amplitude at index col)
                ComplexQuantumNumber inputVector = input.getAmplitude(col);

                // Multiply input vector by scalar
                ComplexQuantumNumber scaledVector = inputVector.multiply(scalar);

                // Sum into resultVector
                resultVector = resultVector.add(scaledVector);
            }

            output.setAmplitude(row, resultVector);
        }

        return output;
    }
}
