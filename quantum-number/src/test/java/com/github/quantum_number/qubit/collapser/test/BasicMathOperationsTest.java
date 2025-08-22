package com.github.quantum_number.qubit.collapser.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.QuantumNumberComponent;
import com.github.quantum_number.qubit.collapse.ComplexQuantumNumberCollapserV2;

public class BasicMathOperationsTest {

    private ComplexQuantumNumber num1;
    private ComplexQuantumNumber num2;
    private ComplexQuantumNumberCollapserV2 collapser;

    @BeforeEach
    public void setup() {

        // Setup num1 with:
        // a = 4, b-f = 1,
        num1 = new ComplexQuantumNumber();
        num1.addComponent(QuantumNumberComponent.a, new Complex(4, 0));
        num1.addComponent(QuantumNumberComponent.b, new Complex(1, 0));
        num1.addComponent(QuantumNumberComponent.c, new Complex(1, 0));
        num1.addComponent(QuantumNumberComponent.d, new Complex(1, 0));
        num1.addComponent(QuantumNumberComponent.e, new Complex(1, 0));
        num1.addComponent(QuantumNumberComponent.f, new Complex(1, 0));

        // Setup num2 similarly with a = 2
        num2 = new ComplexQuantumNumber();
        num2.addComponent(QuantumNumberComponent.a, new Complex(2, 0));
        num2.addComponent(QuantumNumberComponent.b, new Complex(1, 0));
        num2.addComponent(QuantumNumberComponent.c, new Complex(1, 0));
        num2.addComponent(QuantumNumberComponent.d, new Complex(1, 0));
        num2.addComponent(QuantumNumberComponent.e, new Complex(1, 0));
        num2.addComponent(QuantumNumberComponent.f, new Complex(1, 0));
    }

    @Test
    public void testCollapseNum1() {
        Complex collapsed = ComplexQuantumNumberCollapserV2.collapse(num1);
        // Since g-l are zero, imaginary parts zero, collapse roughly equals a=4
        assertEquals(new Complex(4, 0), collapsed);
    }

    @Test
    public void testCollapseNum2() {
        Complex collapsed = ComplexQuantumNumberCollapserV2.collapse(num2);
        assertEquals(new Complex(2, 0), collapsed);
    }

//    @Test
//    public void testAdditionCollapse() {
//        // Create new number by adding component-wise (just a for this test)
//        ComplexQuantumNumber sum = new ComplexQuantumNumber();
//
//        for (QuantumNumberComponent comp : QuantumNumberComponent.values()) {
//            Complex val1 = num1.getComponent(comp);
//            Complex val2 = num2.getComponent(comp);
//            val1 = val1 == null ? Complex.ZERO : val1;
//            val2 = val2 == null ? Complex.ZERO : val2;
//
//            sum.addComponent(comp, val1.add(val2));
//        }
//
//        Complex collapsedSum = collapser.collapse(sum);
//
//        // Expected a = 6 (4 + 2), others 2 (1 + 1), g-l = 0
//        assertEquals(new Complex(6, 0), collapsedSum);
//    }

    @Test
    public void testMultiplicationCollapse() {
        // For simplicity multiply just component a of num1 and num2

        ComplexQuantumNumber product = new ComplexQuantumNumber();

        // a = 4 * 2 = 8, rest b-f = 1*1=1
        product.addComponent(QuantumNumberComponent.a,
                num1.getComponent(QuantumNumberComponent.a).multiply(num2.getComponent(QuantumNumberComponent.a)));
        for (QuantumNumberComponent comp : new QuantumNumberComponent[] {
                QuantumNumberComponent.b, QuantumNumberComponent.c, QuantumNumberComponent.d,
                QuantumNumberComponent.e, QuantumNumberComponent.f }) {
            product.addComponent(comp,
                    num1.getComponent(comp).multiply(num2.getComponent(comp)));
        }

        Complex collapsedProduct = ComplexQuantumNumberCollapserV2.collapse(product);

        // Expected approx 8 because a dominates, others being 1 with complex fraction gives value near a
        // Adjust assertion based on your exact collapse logic:
        assertTrue(collapsedProduct.getReal() > 7.9 && collapsedProduct.getReal() < 8.1);
        assertEquals(0, collapsedProduct.getImaginary(), 1e-12);
    }
}
