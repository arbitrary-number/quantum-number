package com.github.quantum_number.qubit.collapser.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.util.HashMap;
import java.util.Map;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.QuantumNumberComponent;
import com.github.quantum_number.qubit.collapse.ComplexQuantumASTExpression;
import com.github.quantum_number.qubit.collapse.ComplexQuantumNumberCollapser;

public class ComplexQuantumNumberCollapserTest {

    private static Map<QuantumNumberComponent, Complex> getSampleSymbolTable() {
        Map<QuantumNumberComponent, Complex> symbols = new HashMap<>();
        symbols.put(QuantumNumberComponent.a, new Complex(2, 0));     // 2
        symbols.put(QuantumNumberComponent.b, new Complex(0, 1));     // i
        symbols.put(QuantumNumberComponent.c, new Complex(1, 1));     // 1 + i
        return symbols;
    }

    @Test
    public void testConstant() {
        ComplexQuantumASTExpression expr = new ComplexQuantumASTExpression.Constant(new Complex(5, -3));
        Complex result = ComplexQuantumNumberCollapser.collapse(expr, new HashMap<>());
        assertEquals(new Complex(5, -3), result);
    }

    @Test
    public void testSymbol() {
        Map<QuantumNumberComponent, Complex> symbols = getSampleSymbolTable();
        ComplexQuantumASTExpression expr = new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.a);
        Complex result = ComplexQuantumNumberCollapser.collapse(expr, symbols);
        assertEquals(new Complex(2, 0), result);
    }

    @Test
    public void testAddition() {
        Map<QuantumNumberComponent, Complex> symbols = getSampleSymbolTable();
        ComplexQuantumASTExpression expr = new ComplexQuantumASTExpression.Add(
                new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.a),
                new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.b)
        );
        Complex result = ComplexQuantumNumberCollapser.collapse(expr, symbols);
        assertEquals(new Complex(2, 1), result);  // 2 + i
    }

    @Test
    public void testMultiplication() {
        Map<QuantumNumberComponent, Complex> symbols = getSampleSymbolTable();
        ComplexQuantumASTExpression expr = new ComplexQuantumASTExpression.Mul(
                new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.a),
                new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.b)
        );
        Complex result = ComplexQuantumNumberCollapser.collapse(expr, symbols);
        assertEquals(new Complex(0, 2), result);  // 2 * i
    }

    @Test
    public void testDivision() {
        Map<QuantumNumberComponent, Complex> symbols = getSampleSymbolTable();
        ComplexQuantumASTExpression expr = new ComplexQuantumASTExpression.Div(
                new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.b),   // i
                new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.c)    // 1 + i
        );
        Complex result = ComplexQuantumNumberCollapser.collapse(expr, symbols);
        Complex expected = new Complex(0.5, 0.5);  // i / (1 + i) = (1/2 + 1/2i)
        assertTrue(result.subtract(expected).abs() < 1e-10, "Expected " + expected + ", got " + result);
    }

    @Test
    public void testNestedExpression() {
        Map<QuantumNumberComponent, Complex> symbols = getSampleSymbolTable();

        // ((a + b) * c)
        ComplexQuantumASTExpression expr = new ComplexQuantumASTExpression.Mul(
                new ComplexQuantumASTExpression.Add(
                        new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.a),
                        new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.b)
                ),
                new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.c)
        );

        Complex result = ComplexQuantumNumberCollapser.collapse(expr, symbols);
        // (2 + i) * (1 + i) = 2 + 2i + i -1 = (1 + 3i)
        Complex expected = new Complex(1, 3);
        assertTrue(result.subtract(expected).abs() < 1e-10, "Expected " + expected + ", got " + result);
    }

    @Test
    public void testMissingSymbolThrows() {
        Map<QuantumNumberComponent, Complex> symbols = new HashMap<>();
        symbols.put(QuantumNumberComponent.a, new Complex(1, 0));

        ComplexQuantumASTExpression expr = new ComplexQuantumASTExpression.Symbol(QuantumNumberComponent.b);
        assertThrows(IllegalArgumentException.class, () -> {
            ComplexQuantumNumberCollapser.collapse(expr, symbols);
        });
    }
}
