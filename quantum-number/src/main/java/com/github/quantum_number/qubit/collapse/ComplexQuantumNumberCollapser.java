package com.github.quantum_number.qubit.collapse;

import java.util.Map;
import org.apache.commons.math3.complex.Complex;

import com.github.quantum_number.qubit.QuantumNumberComponent;

/**
 * Evaluates a ComplexQuantumExpression to a single Complex number,
 * given a mapping from QuantumNumberComponent symbols to Complex values.
 */
public class ComplexQuantumNumberCollapser {

    /**
     * Collapse the expression by evaluating all subtrees.
     *
     * @param expr the ComplexQuantumExpression tree
     * @param symbolTable map of QuantumNumberComponent to concrete Complex values
     * @return collapsed Complex value
     * @throws IllegalArgumentException if a symbol is missing from the map
     */
    public static Complex collapse(ComplexQuantumASTExpression expr, Map<QuantumNumberComponent, Complex> symbolTable) {
        if (expr instanceof ComplexQuantumASTExpression.Constant) {
            return ((ComplexQuantumASTExpression.Constant) expr).getValue();

        } else if (expr instanceof ComplexQuantumASTExpression.Symbol) {
            QuantumNumberComponent sym = ((ComplexQuantumASTExpression.Symbol) expr).getSymbol();
            if (!symbolTable.containsKey(sym)) {
                throw new IllegalArgumentException("Symbol " + sym + " not found in symbol table");
            }
            return symbolTable.get(sym);

        } else if (expr instanceof ComplexQuantumASTExpression.Add) {
            Complex left = collapse(((ComplexQuantumASTExpression.Add) expr).getLeft(), symbolTable);
            Complex right = collapse(((ComplexQuantumASTExpression.Add) expr).getRight(), symbolTable);
            return left.add(right);

        } else if (expr instanceof ComplexQuantumASTExpression.Sub) {
            Complex left = collapse(((ComplexQuantumASTExpression.Sub) expr).getLeft(), symbolTable);
            Complex right = collapse(((ComplexQuantumASTExpression.Sub) expr).getRight(), symbolTable);
            return left.subtract(right);

        } else if (expr instanceof ComplexQuantumASTExpression.Mul) {
            Complex left = collapse(((ComplexQuantumASTExpression.Mul) expr).getLeft(), symbolTable);
            Complex right = collapse(((ComplexQuantumASTExpression.Mul) expr).getRight(), symbolTable);
            return left.multiply(right);

        } else if (expr instanceof ComplexQuantumASTExpression.Div) {
            Complex numerator = collapse(((ComplexQuantumASTExpression.Div) expr).getNumerator(), symbolTable);
            Complex denominator = collapse(((ComplexQuantumASTExpression.Div) expr).getDenominator(), symbolTable);
            return numerator.divide(denominator);

        } else {
            throw new IllegalArgumentException("Unknown expression type: " + expr.getClass());
        }
    }
}
