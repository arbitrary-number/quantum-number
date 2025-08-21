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





        // Existing constructor and methods ...

        // Static factory methods for common gates

        public static ComplexQuantumGate hadamard() {
            double sqrt2inv = 1.0 / Math.sqrt(2);
            Complex h = new Complex(sqrt2inv, 0);

            Complex[][] hadamard = {
                {h, h},
                {h, h.negate()}
            };
            return new ComplexQuantumGate(hadamard);
        }

        public static ComplexQuantumGate pauliX() {
            Complex zero = Complex.ZERO;
            Complex one = Complex.ONE;

            Complex[][] x = {
                {zero, one},
                {one, zero}
            };
            return new ComplexQuantumGate(x);
        }

        public static ComplexQuantumGate pauliY() {
            Complex zero = Complex.ZERO;
            Complex i = new Complex(0, 1);
            Complex negI = new Complex(0, -1);

            Complex[][] y = {
                {zero, negI},
                {i, zero}
            };
            return new ComplexQuantumGate(y);
        }

        public static ComplexQuantumGate pauliZ() {
            Complex one = Complex.ONE;
            Complex minusOne = one.negate();
            Complex zero = Complex.ZERO;

            Complex[][] z = {
                {one, zero},
                {zero, minusOne}
            };
            return new ComplexQuantumGate(z);
        }

        public static ComplexQuantumGate phaseS() {
            Complex zero = Complex.ZERO;
            Complex one = Complex.ONE;
            Complex i = new Complex(0, 1);

            Complex[][] s = {
                {one, zero},
                {zero, i}
            };
            return new ComplexQuantumGate(s);
        }

        public static ComplexQuantumGate phaseT() {
            Complex zero = Complex.ZERO;
            Complex one = Complex.ONE;
            double angle = Math.PI / 4;
            Complex t = new Complex(Math.cos(angle), Math.sin(angle));

            Complex[][] T = {
                {one, zero},
                {zero, t}
            };
            return new ComplexQuantumGate(T);
        }

        // Controlled-NOT (CNOT) on 2 qubits - 4x4 matrix
        public static ComplexQuantumGate cnot() {
            Complex zero = Complex.ZERO;
            Complex one = Complex.ONE;

            Complex[][] cnot = {
                {one, zero, zero, zero},
                {zero, one, zero, zero},
                {zero, zero, zero, one},
                {zero, zero, one, zero}
            };
            return new ComplexQuantumGate(cnot);
        }

        // Toffoli gate (CCNOT) - 8x8 matrix
        public static ComplexQuantumGate toffoli() {
            Complex zero = Complex.ZERO;
            Complex one = Complex.ONE;

            Complex[][] toffoli = new Complex[8][8];
            // Initialize identity first
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

            return new ComplexQuantumGate(toffoli);
        }

        // Rotation around Z axis
        public static ComplexQuantumGate rotationZ(double theta) {
            Complex zero = Complex.ZERO;
            Complex expNeg = new Complex(Math.cos(-theta/2), Math.sin(-theta/2));
            Complex expPos = new Complex(Math.cos(theta/2), Math.sin(theta/2));

            Complex[][] rz = {
                {expNeg, zero},
                {zero, expPos}
            };
            return new ComplexQuantumGate(rz);
        }

        // You can similarly implement rotationX and rotationY...

        // Existing apply() method here...

        // --- New static factory methods for gates ---

        /** Swap gate (2 qubits) */
        public static ComplexQuantumGate swapGate() {
            Complex zero = Complex.ZERO;
            Complex one = Complex.ONE;

            Complex[][] swap = {
                {one, zero, zero, zero},
                {zero, zero, one, zero},
                {zero, one, zero, zero},
                {zero, zero, zero, one}
            };

            return new ComplexQuantumGate(swap);
        }

        /** Controlled-Z gate (2 qubits) */
        public static ComplexQuantumGate controlledZGate() {
            Complex zero = Complex.ZERO;
            Complex one = Complex.ONE;
            Complex minusOne = one.negate();

            Complex[][] cz = {
                {one, zero, zero, zero},
                {zero, one, zero, zero},
                {zero, zero, one, zero},
                {zero, zero, zero, minusOne}
            };

            return new ComplexQuantumGate(cz);
        }

        /**
         * Phase Shift gate on single qubit with arbitrary angle theta.
         * Matrix: [[1, 0], [0, exp(i*theta)]]
         */
        public static ComplexQuantumGate phaseShiftGate(double theta) {
            Complex zero = Complex.ZERO;
            Complex one = Complex.ONE;
            Complex phase = new Complex(Math.cos(theta), Math.sin(theta));

            Complex[][] phaseShift = {
                {one, zero},
                {zero, phase}
            };

            return new ComplexQuantumGate(phaseShift);
        }

        /** Fredkin gate (Controlled-Swap gate, 3 qubits, 8x8 matrix) */
        public static ComplexQuantumGate fredkinGate() {
            int dim = 8;
            Complex zero = Complex.ZERO;
            Complex one = Complex.ONE;

            // Initialize identity matrix
            Complex[][] fredkin = new Complex[dim][dim];
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    fredkin[i][j] = (i == j) ? one : zero;
                }
            }

            // Indices of the states where swap occurs if control qubit (bit 2) = 1:
            // Swap states |101⟩ (5) and |110⟩ (6)
            fredkin[5][5] = zero;
            fredkin[6][6] = zero;
            fredkin[5][6] = one;
            fredkin[6][5] = one;

            return new ComplexQuantumGate(fredkin);
        }

		public Complex[][] getMatrix() {
			return matrix;
		}

}
