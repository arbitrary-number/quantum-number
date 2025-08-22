package com.github.quantum_number.qubit;

import java.util.Arrays;

import org.apache.commons.math3.complex.Complex;

public class ComplexQuantumGate {

    @Override
	public String toString() {
		return "ComplexQuantumGate [matrix=" + Arrays.toString(matrix) + ", dimension=" + dimension + "]";
	}

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

    public static Complex[][] kroneckerProduct(Complex[][] A, Complex[][] B) {
        int aRows = A.length;
        int aCols = A[0].length;
        int bRows = B.length;
        int bCols = B[0].length;

        Complex[][] result = new Complex[aRows * bRows][aCols * bCols];

        for (int i = 0; i < aRows; i++) {
            for (int j = 0; j < aCols; j++) {
                for (int k = 0; k < bRows; k++) {
                    for (int l = 0; l < bCols; l++) {
                        // result block at [i*bRows + k][j*bCols + l]
                        result[i * bRows + k][j * bCols + l] = A[i][j].multiply(B[k][l]);
                    }
                }
            }
        }
        return result;
    }


    public static ComplexQuantumGate tensorProduct(ComplexQuantumGate g1, ComplexQuantumGate g2) {
        //int newQubits = g1.getNumQubits() + g2.getNumQubits();

        // Perform Kronecker product of matrices:
        Complex[][] newMatrix = kroneckerProduct(g1.getMatrix(), g2.getMatrix());

        return new ComplexQuantumGate(newMatrix);
    }

    /**
     * Apply the gate to a ComplexQuantumRegister and return a new register as result.
     */
//    public ComplexQuantumRegister apply(ComplexQuantumRegister input) {
//        if (input.getDimension() != dimension) {
//            throw new IllegalArgumentException("Input register dimension mismatch");
//        }
//
//        ComplexQuantumRegister output = ComplexQuantumRegister.ofDimension(dimension);
//
//        // Apply matrix multiplication: output = matrix * input
//        for (int row = 0; row < dimension; row++) {
//            ComplexQuantumNumber resultVector = new ComplexQuantumNumber();
//
//            for (int col = 0; col < dimension; col++) {
//                // matrix[row][col] is Complex scalar
//                Complex scalar = matrix[row][col];
//
//                // input.getAmplitude(col) returns ComplexQuantumNumber (the vector amplitude at index col)
//                ComplexQuantumNumber inputVector = input.getAmplitude(col);
//
//                // Multiply input vector by scalar
//                ComplexQuantumNumber scaledVector = inputVector.multiply(scalar);
//
//                // Sum into resultVector
//                resultVector = resultVector.add(scaledVector);
//            }
//
//            output.setAmplitude(row, resultVector);
//        }
//
//        return output;
//    }


	public ComplexQuantumRegister apply(ComplexQuantumRegister current) {
		return ComplexQuantumGate.apply(this, current);
	}

    public static ComplexQuantumRegister apply(ComplexQuantumGate gate, ComplexQuantumRegister reg) {
        int size = reg.size(); // e.g. 4 for 2 qubits
        ComplexQuantumRegister output =
        		ComplexQuantumRegister.ofDimension(size);

        for (int i = 0; i < size; i++) {
            ComplexQuantumNumber newAmplitude = new ComplexQuantumNumber();

            for (int j = 0; j < size; j++) {
                Complex gateElement = gate.getMatrix()[i][j];
                ComplexQuantumNumber ampJ = reg.getAmplitude(j);
                ComplexQuantumNumber term = ampJ.multiply(gateElement);
                newAmplitude.addComponents(term);
            }

            output.setAmplitude(i, newAmplitude);
        }

        return output;
    }
//
//    public static ComplexQuantumGate tensorProduct(ComplexQuantumGate g1, ComplexQuantumGate g2) {
//        int dim1 = g1.getDimension();
//        int dim2 = g2.getDimension();
//        int newDim = dim1 * dim2;
//
//        Complex[][] result = new Complex[newDim][newDim];
//
//        for (int i = 0; i < dim1; i++) {
//            for (int j = 0; j < dim1; j++) {
//                Complex scalar = g1.matrix[i][j];
//                for (int k = 0; k < dim2; k++) {
//                    for (int l = 0; l < dim2; l++) {
//                        result[i * dim2 + k][j * dim2 + l] = scalar.multiply(g2.matrix[k][l]);
//                    }
//                }
//            }
//        }
//        return new ComplexQuantumGate(result);
//    }

    public int getNumQubits() {
        int dim = this.dimension; // or similar method returning matrix size (rows)
        return (int)(Math.log(dim) / Math.log(2));
    }

    public static int intPow(int base, int exponent) {
        int result = 1;
        for (int i = 0; i < exponent; i++) {
            result *= base;
        }
        return result;
    }

    public static ComplexQuantumGate identityOfDimension(int dim) {
        Complex[][] id = new Complex[dim][dim];
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                id[i][j] = (i == j) ? Complex.ONE : Complex.ZERO;
            }
        }
        return new ComplexQuantumGate(id);
    }

    public static ComplexQuantumGate identityOfQubits(int numQubits) {
        int dim = 1 << numQubits; // 2^numQubits
        Complex[][] id = new Complex[dim][dim];
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                id[i][j] = (i == j) ? Complex.ONE : Complex.ZERO;
            }
        }
        return new ComplexQuantumGate(id);
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

        public static ComplexQuantumGate singleQubitGateOnNQubits(ComplexQuantumGate gate, int nQubits, int targetQubit) {
            // Build identity gates for qubits before and after target
            ComplexQuantumGate left = identityOfQubits(targetQubit);
            ComplexQuantumGate right = identityOfQubits(nQubits - targetQubit - 1);

            // Tensor product: left ⊗ gate ⊗ right
            return tensorProduct(tensorProduct(left, gate), right);
        }

        public static ComplexQuantumGate controlledXOnNQubits(int nQubits, int control, int target) {
            int dim = 1 << nQubits;
            Complex[][] matrix = new Complex[dim][dim];
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    matrix[i][j] = Complex.ZERO;
                }
            }

            for (int i = 0; i < dim; i++) {
                int controlBit = (i >> (nQubits - 1 - control)) & 1;
                int targetBit = (i >> (nQubits - 1 - target)) & 1;
                int flippedTargetBit = targetBit ^ controlBit;
                int j = i;
                if (targetBit != flippedTargetBit) {
                    j = i ^ (1 << (nQubits - 1 - target));
                }
                matrix[j][i] = Complex.ONE;
            }

            return new ComplexQuantumGate(matrix);
        }

    	// Helper to extract reduced qubit state for qubitIndex (simple extraction ignoring full density matrix)
        public static ComplexQuantumNumber extractQubitState(ComplexQuantumRegister reg, int qubitIndex) {
    	    int nQubits = (int)(Math.log(reg.size()) / Math.log(2));
    	    int dim = reg.size();

    	    // The state of one qubit in superposition can be extracted by summing amplitudes where other qubits fixed
    	    // For simplicity, sum amplitudes for qubitIndex = 0 and 1 separately

    	    ComplexQuantumNumber zeroAmp = new ComplexQuantumNumber();
    	    ComplexQuantumNumber oneAmp = new ComplexQuantumNumber();

    	    for (int i = 0; i < dim; i++) {
    	        int bit = (i >> (nQubits - 1 - qubitIndex)) & 1;
    	        ComplexQuantumNumber amp = reg.getAmplitude(i);
    	        if (bit == 0) {
    	            zeroAmp = zeroAmp.add(amp);
    	        } else {
    	            oneAmp = oneAmp.add(amp);
    	        }
    	    }

    	    // Return superposition alpha|0> + beta|1> as ComplexQuantumNumber with components a and b
    	    ComplexQuantumNumber state = new ComplexQuantumNumber();
    	    state.addComponent(QuantumNumberComponent.a, zeroAmp.getComponent(QuantumNumberComponent.a));
    	    state.addComponent(QuantumNumberComponent.b, oneAmp.getComponent(QuantumNumberComponent.a));

    	    return state;
    	}

        public static ComplexQuantumGate singleQubitGateOnNQubitsV2(ComplexQuantumGate gate, int totalQubits, int targetQubit) {
            ComplexQuantumGate result = ComplexQuantumGate.identityOfQubits(1);
            for (int i = 0; i < totalQubits; i++) {
                if (i == targetQubit) {
                    result = ComplexQuantumGate.tensorProduct(result, gate);
                } else {
                    result = ComplexQuantumGate.tensorProduct(result, ComplexQuantumGate.identityOfQubits(1));
                }
            }
            return result;
        }

		public Complex[][] getMatrix() {
			return matrix;
		}

		public static ComplexQuantumGate controlledPhaseGateOnNQubits(int nQubits, int controlQubit, int targetQubit, double theta) {
		    int dim = 1 << nQubits;
		    Complex[][] matrix = new Complex[dim][dim];
		    Complex phase = new Complex(Math.cos(theta), Math.sin(theta));

		    // Initialize as identity matrix
		    for (int i = 0; i < dim; i++) {
		        for (int j = 0; j < dim; j++) {
		            matrix[i][j] = (i == j) ? Complex.ONE : Complex.ZERO;
		        }
		    }

		    // Apply phase only if control and target bits are 1
		    for (int i = 0; i < dim; i++) {
		        int controlBit = (i >> (nQubits - 1 - controlQubit)) & 1;
		        int targetBit = (i >> (nQubits - 1 - targetQubit)) & 1;
		        if (controlBit == 1 && targetBit == 1) {
		            matrix[i][i] = phase;
		        }
		    }

		    return new ComplexQuantumGate(matrix);
		}

		public static ComplexQuantumGate rotationX(double theta) {
		    Complex cos = new Complex(Math.cos(theta / 2), 0);
		    Complex iSin = new Complex(0, -Math.sin(theta / 2));

		    Complex[][] matrix = {
		        {cos, iSin},
		        {iSin, cos}
		    };
		    return new ComplexQuantumGate(matrix);
		}

		public static ComplexQuantumGate rotationY(double theta) {
		    Complex cos = new Complex(Math.cos(theta / 2), 0);
		    Complex sin = new Complex(Math.sin(theta / 2), 0);

		    Complex[][] matrix = {
		        {cos, sin.negate()},
		        {sin, cos}
		    };
		    return new ComplexQuantumGate(matrix);
		}

		private Complex[][] deepCopyMatrix(Complex[][] input) {
		    int dim = input.length;
		    Complex[][] copy = new Complex[dim][dim];
		    for (int i = 0; i < dim; i++) {
		        System.arraycopy(input[i], 0, copy[i], 0, dim);
		    }
		    return copy;
		}


}
