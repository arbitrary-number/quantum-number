package com.github.quantum_number.qubit.collapse;

import org.apache.commons.math3.complex.Complex;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.QuantumNumberComponent;

public class ComplexQuantumNumberCollapserV2 {

    private final ComplexQuantumNumber quantumNumber;

    public ComplexQuantumNumberCollapserV2(ComplexQuantumNumber quantumNumber) {
        this.quantumNumber = quantumNumber;
    }

    /**
     * Recursively collapse the expression to a single Complex number.
     */
    public Complex collapse(ComplexQuantumASTExpression expr) {
        if (expr instanceof ComplexQuantumASTExpression.Constant c) {
            return c.value();
        } else if (expr instanceof ComplexQuantumASTExpression.Symbol s) {
            QuantumNumberComponent symbol = s.getSymbol();
            Complex val = quantumNumber.getComponent(symbol);
            if (val == null) {
                // If the symbol is not found, treat as zero
                return Complex.ZERO;
            }
            return val;
        } else if (expr instanceof ComplexQuantumASTExpression.Add add) {
            Complex left = collapse(add.left());
            Complex right = collapse(add.right());
            return left.add(right);
        } else if (expr instanceof ComplexQuantumASTExpression.Sub sub) {
            Complex left = collapse(sub.left());
            Complex right = collapse(sub.right());
            return left.subtract(right);
        } else if (expr instanceof ComplexQuantumASTExpression.Mul mul) {
            Complex left = collapse(mul.left());
            Complex right = collapse(mul.right());
            return left.multiply(right);
        } else if (expr instanceof ComplexQuantumASTExpression.Div div) {
            Complex numerator = collapse(div.numerator());
            Complex denominator = collapse(div.denominator());
            if (denominator.equals(Complex.ZERO)) {
                throw new ArithmeticException("Division by zero in quantum number collapse");
            }
            return numerator.divide(denominator);
        } else {
            throw new IllegalArgumentException("Unknown expression type: " + expr.getClass());
        }
    }

	public static Complex collapse(ComplexQuantumNumber num1) {
		ComplexQuantumASTExpression convert =
				ComplexQuantumASTConverter.convert(num1);
		ComplexQuantumNumberCollapserV2 new1 = new
				ComplexQuantumNumberCollapserV2(num1);
		return new1.collapse(convert);
	}
}
