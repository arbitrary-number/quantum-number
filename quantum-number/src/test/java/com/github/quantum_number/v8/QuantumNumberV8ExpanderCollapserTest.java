package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;

import java.math.BigDecimal;

import static org.junit.jupiter.api.Assertions.*;

public class QuantumNumberV8ExpanderCollapserTest {

    @Test
    public void testExpandAndCollapseSimpleValue() {
        BigDecimal original = new BigDecimal("12345.6789");
        QuantumNumberV8 qnum = QuantumNumberV8Expander.expand(original);
        BigDecimal collapsed = QuantumNumberV8Collapser.collapse(qnum);

        // Allow small delta due to scaling approximation (if any)
        BigDecimal delta = new BigDecimal("0.00000001");

        assertTrue(original.subtract(collapsed).abs().compareTo(delta) < 0,
            "Collapsed value should be close to original");
    }

    @Test
    public void testExpandAndCollapseInteger() {
        BigDecimal original = new BigDecimal("9876543210");
        QuantumNumberV8 qnum = QuantumNumberV8Expander.expand(original);
        BigDecimal collapsed = QuantumNumberV8Collapser.collapse(qnum);

        assertEquals(0, original.compareTo(collapsed),
            "Collapsed integer should equal original exactly");
    }

    @Test
    public void testExpandAndCollapseZero() {
        BigDecimal original = BigDecimal.ZERO;
        QuantumNumberV8 qnum = QuantumNumberV8Expander.expand(original);
        BigDecimal collapsed = QuantumNumberV8Collapser.collapse(qnum);

        assertTrue(collapsed.compareTo(BigDecimal.ZERO) == 0, "Zero value roundtrip");
    }

    @Test
    public void testExpandNegativeValue() {
        BigDecimal original = new BigDecimal("-123.456");
        QuantumNumberV8 qnum = QuantumNumberV8Expander.expand(original);
        BigDecimal collapsed = QuantumNumberV8Collapser.collapse(qnum);

        BigDecimal delta = new BigDecimal("0.00000001");

        assertTrue(original.subtract(collapsed).abs().compareTo(delta) < 0,
            "Negative value should roundtrip approximately");
    }
}
