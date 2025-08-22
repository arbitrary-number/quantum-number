package com.github.quantum_number.qubit.collapser.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.QuantumNumberComponent;
import com.github.quantum_number.qubit.collapse.ComplexQuantumASTConverter;
import com.github.quantum_number.qubit.collapse.ComplexQuantumASTExpression;

public class ComplexQuantumASTConverterTest {

    @Test
    public void testConvert() {
        // Create a ComplexQuantumNumber with a=1+0i, b=2+0i, ..., f=6+0i
        ComplexQuantumNumber number = new ComplexQuantumNumber();
        number.addComponent(QuantumNumberComponent.a, new Complex(1, 0));
        number.addComponent(QuantumNumberComponent.b, new Complex(2, 0));
        number.addComponent(QuantumNumberComponent.c, new Complex(3, 0));
        number.addComponent(QuantumNumberComponent.d, new Complex(4, 0));
        number.addComponent(QuantumNumberComponent.e, new Complex(5, 0));
        number.addComponent(QuantumNumberComponent.f, new Complex(6, 0));

        // Convert to AST expression
        ComplexQuantumASTExpression expr = ComplexQuantumASTConverter.convert(number);

        // The expression should represent:
        // ((1) / ((2) / (3))) / ((4) / ((5) / (6)))

        // We can check structure by casting and checking nested parts:

        assertTrue(expr instanceof ComplexQuantumASTExpression.Div);
        ComplexQuantumASTExpression.Div topDiv = (ComplexQuantumASTExpression.Div) expr;

        // Left side of top division: (1) / ((2)/(3))
        ComplexQuantumASTExpression left = topDiv.numerator();
        ComplexQuantumASTExpression right = topDiv.denominator();

        assertTrue(left instanceof ComplexQuantumASTExpression.Div);
        ComplexQuantumASTExpression.Div leftDiv = (ComplexQuantumASTExpression.Div) left;

        // leftDiv numerator should be Constant(1)
        assertTrue(leftDiv.numerator() instanceof ComplexQuantumASTExpression.Constant);
        assertEquals(new Complex(1, 0), ((ComplexQuantumASTExpression.Constant)leftDiv.numerator()).value());

        // leftDiv denominator should be (2)/(3)
        assertTrue(leftDiv.denominator() instanceof ComplexQuantumASTExpression.Div);
        ComplexQuantumASTExpression.Div leftDenomDiv = (ComplexQuantumASTExpression.Div) leftDiv.denominator();
        assertEquals(new Complex(2, 0), ((ComplexQuantumASTExpression.Constant)leftDenomDiv.numerator()).value());
        assertEquals(new Complex(3, 0), ((ComplexQuantumASTExpression.Constant)leftDenomDiv.denominator()).value());

        // Right side of top division: (4) / ((5)/(6))
        assertTrue(right instanceof ComplexQuantumASTExpression.Div);
        ComplexQuantumASTExpression.Div rightDiv = (ComplexQuantumASTExpression.Div) right;

        // rightDiv numerator should be Constant(4)
        assertTrue(rightDiv.numerator() instanceof ComplexQuantumASTExpression.Constant);
        assertEquals(new Complex(4, 0), ((ComplexQuantumASTExpression.Constant)rightDiv.numerator()).value());

        // rightDiv denominator should be (5)/(6)
        assertTrue(rightDiv.denominator() instanceof ComplexQuantumASTExpression.Div);
        ComplexQuantumASTExpression.Div rightDenomDiv = (ComplexQuantumASTExpression.Div) rightDiv.denominator();
        assertEquals(new Complex(5, 0), ((ComplexQuantumASTExpression.Constant)rightDenomDiv.numerator()).value());
        assertEquals(new Complex(6, 0), ((ComplexQuantumASTExpression.Constant)rightDenomDiv.denominator()).value());
    }
}
