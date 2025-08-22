package com.github.quantum_number.qubit.collapser.test;

import static org.junit.jupiter.api.Assertions.assertTrue;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.QuantumNumberComponent;
import com.github.quantum_number.qubit.collapse.ComplexQuantumASTExpression;
import com.github.quantum_number.qubit.collapse.ComplexQuantumASTConverter;

public class ComplexQuantumASTConverterTestV1 {

    @Test
    public void testConvertNestedExpression() {
        ComplexQuantumNumber number = new ComplexQuantumNumber();

        // Assign distinct real values for a-l for easy identification
        number.addComponent(QuantumNumberComponent.a, new Complex(1, 0));
        number.addComponent(QuantumNumberComponent.b, new Complex(2, 0));
        number.addComponent(QuantumNumberComponent.c, new Complex(3, 0));
        number.addComponent(QuantumNumberComponent.d, new Complex(4, 0));
        number.addComponent(QuantumNumberComponent.e, new Complex(5, 0));
        number.addComponent(QuantumNumberComponent.f, new Complex(6, 0));

        number.addComponent(QuantumNumberComponent.g, new Complex(0, 1));

        // Convert to expression
        ComplexQuantumASTExpression expr = ComplexQuantumASTConverter.convert(number);

        // The root should be a Div expression
        assertTrue(expr instanceof ComplexQuantumASTExpression.Div, "Root should be a Div");

        ComplexQuantumASTExpression.Div rootDiv = (ComplexQuantumASTExpression.Div) expr;

        // Left and right of rootDiv
        ComplexQuantumASTExpression leftFraction = rootDiv.getNumerator();
        ComplexQuantumASTExpression rightFraction = rootDiv.getDenominator();

        // Left fraction = (a+g i) / ((b+h i)/(c+i i))
        assertTrue(leftFraction instanceof ComplexQuantumASTExpression.Div, "Left fraction should be Div");

        ComplexQuantumASTExpression.Div leftDiv = (ComplexQuantumASTExpression.Div) leftFraction;

        // Numerator of leftDiv is (a + g i)
        ComplexQuantumASTExpression leftNumerator = leftDiv.getNumerator();
        assertTrue(leftNumerator instanceof ComplexQuantumASTExpression.Add, "Left numerator should be Add");

        // Denominator of leftDiv is (b + h i) / (c + i i)
        ComplexQuantumASTExpression leftDenominator = leftDiv.getDenominator();
        assertTrue(leftDenominator instanceof ComplexQuantumASTExpression.Div, "Left denominator should be Div");

        ComplexQuantumASTExpression.Div leftDenDiv = (ComplexQuantumASTExpression.Div) leftDenominator;

        // Numerator of leftDenDiv is (b + h i)
        ComplexQuantumASTExpression leftDenNumerator = leftDenDiv.getNumerator();
        assertTrue(leftDenNumerator instanceof ComplexQuantumASTExpression.Add, "Left denominator numerator should be Add");

        // Denominator of leftDenDiv is (c + i i)
        ComplexQuantumASTExpression leftDenDenominator = leftDenDiv.getDenominator();
        assertTrue(leftDenDenominator instanceof ComplexQuantumASTExpression.Add, "Left denominator denominator should be Add");

        // Right fraction = (d + j i) / ((e + k i) / (f + l i))
        assertTrue(rightFraction instanceof ComplexQuantumASTExpression.Div, "Right fraction should be Div");

        ComplexQuantumASTExpression.Div rightDiv = (ComplexQuantumASTExpression.Div) rightFraction;

        ComplexQuantumASTExpression rightNumerator = rightDiv.getNumerator();
        assertTrue(rightNumerator instanceof ComplexQuantumASTExpression.Add, "Right numerator should be Add");

        ComplexQuantumASTExpression rightDenominator = rightDiv.getDenominator();
        assertTrue(rightDenominator instanceof ComplexQuantumASTExpression.Div, "Right denominator should be Div");

        ComplexQuantumASTExpression.Div rightDenDiv = (ComplexQuantumASTExpression.Div) rightDenominator;

        ComplexQuantumASTExpression rightDenNumerator = rightDenDiv.getNumerator();
        assertTrue(rightDenNumerator instanceof ComplexQuantumASTExpression.Add, "Right denominator numerator should be Add");

        ComplexQuantumASTExpression rightDenDenominator = rightDenDiv.getDenominator();
        assertTrue(rightDenDenominator instanceof ComplexQuantumASTExpression.Add, "Right denominator denominator should be Add");

        // Spot-check leaf symbols inside one Add on left numerator (a + g i)
        ComplexQuantumASTExpression.Add add = (ComplexQuantumASTExpression.Add) leftNumerator;

        // One side should be symbol 'a', other should be multiplication of g * I
        boolean hasSymbolA = containsSymbol(add, QuantumNumberComponent.a);
        boolean hasImagMul = containsImagMul(add, QuantumNumberComponent.g);

        assertTrue(hasSymbolA, "Left numerator should contain symbol 'a'");
        assertTrue(hasImagMul, "Left numerator should contain imaginary multiplication with 'g'");

        // Similarly, spot-check right numerator contains symbol 'd'
        ComplexQuantumASTExpression.Add rightAdd = (ComplexQuantumASTExpression.Add) rightNumerator;
        assertTrue(containsSymbol(rightAdd, QuantumNumberComponent.d), "Right numerator should contain symbol 'd'");
    }

    // Helper method to recursively check for a symbol in the expression tree
    private boolean containsSymbol(ComplexQuantumASTExpression expr, QuantumNumberComponent symbol) {
        if (expr instanceof ComplexQuantumASTExpression.Symbol s) {
            return s.getSymbol().equals(symbol);
        } else if (expr instanceof ComplexQuantumASTExpression.Add a) {
            return containsSymbol(a.getLeft(), symbol) || containsSymbol(a.getRight(), symbol);
        } else if (expr instanceof ComplexQuantumASTExpression.Sub s) {
            return containsSymbol(s.getLeft(), symbol) || containsSymbol(s.getRight(), symbol);
        } else if (expr instanceof ComplexQuantumASTExpression.Mul m) {
            return containsSymbol(m.getLeft(), symbol) || containsSymbol(m.getRight(), symbol);
        } else if (expr instanceof ComplexQuantumASTExpression.Div d) {
            return containsSymbol(d.getNumerator(), symbol) || containsSymbol(d.getDenominator(), symbol);
        }
        return false;
    }

    // Helper method to check if expression contains multiplication of a symbol and the imaginary unit
    private boolean containsImagMul(ComplexQuantumASTExpression expr, QuantumNumberComponent symbol) {
        if (expr instanceof ComplexQuantumASTExpression.Mul m) {
            boolean leftIsConstantI = m.getLeft() instanceof ComplexQuantumASTExpression.Constant c
                    && c.getValue().equals(new Complex(0,1));
            boolean rightIsSymbol = m.getRight() instanceof ComplexQuantumASTExpression.Symbol s
                    && s.getSymbol().equals(symbol);

            boolean rightIsConstantI = m.getRight() instanceof ComplexQuantumASTExpression.Constant c2
                    && c2.getValue().equals(new Complex(0,1));
            boolean leftIsSymbol = m.getLeft() instanceof ComplexQuantumASTExpression.Symbol s2
                    && s2.getSymbol().equals(symbol);

            return (leftIsConstantI && rightIsSymbol) || (rightIsConstantI && leftIsSymbol);
        } else if (expr instanceof ComplexQuantumASTExpression.Add a) {
            return containsImagMul(a.getLeft(), symbol) || containsImagMul(a.getRight(), symbol);
        }
        return false;
    }
}
