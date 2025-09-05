package com.github.quantum_number.v8;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

public class QuantumNumberV8MathTest {

    @Test
    void testMultiplyBySmallFactor_zeroFactor() {
        long[] comp = {1, 2, 3, 4};
        long[] result = QuantumNumberV8Math.multiplyBySmallFactor(comp, 0);
        assertArrayEquals(new long[]{0, 0, 0, 0}, result);
    }

    @Test
    void testMultiplyBySmallFactor_factorOne() {
        long[] comp = {1, 2, 3, 4};
        long[] result = QuantumNumberV8Math.multiplyBySmallFactor(comp, 1);
        assertArrayEquals(comp, result);
    }

    @Test
    void testMultiplyBySmallFactor_simple() {
        // Multiply by 2
        long[] comp = {0xFFFFFFFFFFFFFFFFL, 0, 0, 0};
        long[] expected = {0xFFFFFFFFFFFFFFFEL, 1, 0, 0}; // 0xFFFFFFFFFFFFFFFF * 2 = 0x1FFFFFFFFFFFFFFFE (low 64 bits and carry)
        long[] result = QuantumNumberV8Math.multiplyBySmallFactor(comp, 2);
        assertArrayEquals(expected, result);
    }

    @Test
    void testMultiplyBySmallFactor_carryPropagation() {
        // Setup component to cause carry propagation
        long[] comp = {0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFL, 0, 0};
        long[] expected = {
            0xFFFFFFFFFFFFFFFEL, 0xFFFFFFFFFFFFFFFFL,
            1, 0
        };
        // Explanation:
        // comp * 2 = 0x1FFFFFFFFFFFFFFFE + 0x1FFFFFFFFFFFFFFFE<<64 = will propagate carry to third word
        long[] result = QuantumNumberV8Math.multiplyBySmallFactor(comp, 2);
        assertArrayEquals(expected, result);
    }

    @Test
    void testMultiplyB() {
        QuantumNumberV8 q = new QuantumNumberV8();
        q.b1 = 1; q.b2 = 0; q.b3 = 0; q.b4 = 0;
        QuantumNumberV8Math.multiplyB(q, 3);
        assertEquals(3L, q.b1);
        assertEquals(0L, q.b2);
        assertEquals(0L, q.b3);
        assertEquals(0L, q.b4);
    }

    @Test
    void testMultiplyD() {
        QuantumNumberV8 q = new QuantumNumberV8();
        q.d1 = 0xFFFFFFFFFFFFFFFFL;
        q.d2 = 0L;
        q.d3 = 0L;
        q.d4 = 0L;
        QuantumNumberV8Math.multiplyD(q, 2);
        assertEquals(0xFFFFFFFFFFFFFFFEL, q.d1);
        assertEquals(1L, q.d2);
        assertEquals(0L, q.d3);
        assertEquals(0L, q.d4);
    }

    @Test
    void testMultiplyF() {
        QuantumNumberV8 q = new QuantumNumberV8();
        q.f1 = 0x8000000000000000L; // Large number, test shifts and multiply
        q.f2 = 0L;
        q.f3 = 0L;
        q.f4 = 0L;
        QuantumNumberV8Math.multiplyF(q, 2);
        assertEquals(0L, q.f1);
        assertEquals(1L, q.f2);
        assertEquals(0L, q.f3);
        assertEquals(0L, q.f4);
    }

}
