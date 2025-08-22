package com.github.quantum_number.qubit.collapse;

import org.apache.commons.math3.complex.Complex;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.QuantumNumberComponent;

public class ComplexQuantumASTConverter {

    /**
     * Convert ComplexQuantumNumber with components a-f into a nested ComplexQuantumASTExpression
     * following the structure:
     * ((±a) / ((±b) / (±c))) / ((±d) / ((±e) / (±f)))
     */
    public static ComplexQuantumASTExpression convert(ComplexQuantumNumber number) {

        // For brevity
        Complex a = getCoeff(number, QuantumNumberComponent.a);
        Complex b = getCoeff(number, QuantumNumberComponent.b);
        Complex c = getCoeff(number, QuantumNumberComponent.c);
        Complex d = getCoeff(number, QuantumNumberComponent.d);
        Complex e = getCoeff(number, QuantumNumberComponent.e);
        Complex f = getCoeff(number, QuantumNumberComponent.f);

        // Build expressions for each component as Constants
        ComplexQuantumASTExpression term1 = new ComplexQuantumASTExpression.Constant(a);
        ComplexQuantumASTExpression term2 = new ComplexQuantumASTExpression.Constant(b);
        ComplexQuantumASTExpression term3 = new ComplexQuantumASTExpression.Constant(c);
        ComplexQuantumASTExpression term4 = new ComplexQuantumASTExpression.Constant(d);
        ComplexQuantumASTExpression term5 = new ComplexQuantumASTExpression.Constant(e);
        ComplexQuantumASTExpression term6 = new ComplexQuantumASTExpression.Constant(f);

        // Build nested divisions:
        // denominator1 = term2 / term3
        ComplexQuantumASTExpression denom1 = new ComplexQuantumASTExpression.Div(term2, term3);
        // leftFraction = term1 / denom1
        ComplexQuantumASTExpression leftFraction = new ComplexQuantumASTExpression.Div(term1, denom1);

        // denominator2 = term5 / term6
        ComplexQuantumASTExpression denom2 = new ComplexQuantumASTExpression.Div(term5, term6);
        // rightFraction = term4 / denom2
        ComplexQuantumASTExpression rightFraction = new ComplexQuantumASTExpression.Div(term4, denom2);

        // Final expression = leftFraction / rightFraction
        return new ComplexQuantumASTExpression.Div(leftFraction, rightFraction);
    }

    // Helper: get coefficient or zero if absent
    static Complex getCoeff(ComplexQuantumNumber number, QuantumNumberComponent comp) {
        Complex val = number.getComponent(comp);
        return val != null ? val : Complex.ZERO;
    }
}
