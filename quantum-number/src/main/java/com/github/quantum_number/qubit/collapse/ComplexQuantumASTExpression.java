package com.github.quantum_number.qubit.collapse;

import org.apache.commons.math3.complex.Complex;

import com.github.quantum_number.qubit.QuantumNumberComponent;

public interface ComplexQuantumASTExpression {

    class Symbol implements ComplexQuantumASTExpression {
        private final QuantumNumberComponent symbol;

        public Symbol(QuantumNumberComponent symbol) {
            this.symbol = symbol;
        }

        public QuantumNumberComponent getSymbol() {
            return symbol;
        }

        @Override
        public String toString() {
            return symbol.toString();
        }
    }

    class Constant implements ComplexQuantumASTExpression {
        private final Complex value;

        public Constant(Complex value) {
            this.value = value;
        }

        public Complex getValue() {
            return value;
        }

        public Complex value() {
            return value;
        }

        @Override
        public String toString() {
            return value.toString();
        }
    }

    class Add implements ComplexQuantumASTExpression {
        private final ComplexQuantumASTExpression left;
        private final ComplexQuantumASTExpression right;

        public Add(ComplexQuantumASTExpression left, ComplexQuantumASTExpression right) {
            this.left = left;
            this.right = right;
        }

        public ComplexQuantumASTExpression getLeft() {
            return left;
        }

        public ComplexQuantumASTExpression getRight() {
            return right;
        }

        public ComplexQuantumASTExpression right() {
            return right;
        }

        @Override
        public String toString() {
            return "(" + left + " + " + right + ")";
        }

		public ComplexQuantumASTExpression left() {
            return left;
		}
    }

    class Sub implements ComplexQuantumASTExpression {
        private final ComplexQuantumASTExpression left;
        private final ComplexQuantumASTExpression right;

        public Sub(ComplexQuantumASTExpression left, ComplexQuantumASTExpression right) {
            this.left = left;
            this.right = right;
        }

        public ComplexQuantumASTExpression getLeft() {
            return left;
        }

        public ComplexQuantumASTExpression getRight() {
            return right;
        }

        public ComplexQuantumASTExpression left() {
            return left;
        }

        public ComplexQuantumASTExpression right() {
            return right;
        }

        @Override
        public String toString() {
            return "(" + left + " - " + right + ")";
        }
    }

    class Mul implements ComplexQuantumASTExpression {
        private final ComplexQuantumASTExpression left;
        private final ComplexQuantumASTExpression right;

        public Mul(ComplexQuantumASTExpression left, ComplexQuantumASTExpression right) {
            this.left = left;
            this.right = right;
        }

        public ComplexQuantumASTExpression getLeft() {
            return left;
        }

        public ComplexQuantumASTExpression getRight() {
            return right;
        }

        public ComplexQuantumASTExpression left() {
            return left;
        }

        public ComplexQuantumASTExpression right() {
            return right;
        }

        @Override
        public String toString() {
            return "(" + left + " * " + right + ")";
        }
    }

    class Div implements ComplexQuantumASTExpression {
        private final ComplexQuantumASTExpression numerator;
        private final ComplexQuantumASTExpression denominator;

        public Div(ComplexQuantumASTExpression numerator, ComplexQuantumASTExpression denominator) {
            this.numerator = numerator;
            this.denominator = denominator;
        }

        public ComplexQuantumASTExpression getNumerator() {
            return numerator;
        }

        public ComplexQuantumASTExpression getDenominator() {
            return denominator;
        }

        public ComplexQuantumASTExpression numerator() {
            return numerator;
        }

        public ComplexQuantumASTExpression denominator() {
            return denominator;
        }

        @Override
        public String toString() {
            return "(" + numerator + " / " + denominator + ")";
        }
    }
}
