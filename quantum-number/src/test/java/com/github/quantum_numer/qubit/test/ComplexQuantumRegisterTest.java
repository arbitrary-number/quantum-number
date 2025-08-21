package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.Complex;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.QuantumNumberComponent;

public class ComplexQuantumRegisterTest {

    private ComplexQuantumRegister register;

    @BeforeEach
    public void setup() {
        register = new ComplexQuantumRegister();
    }

    @Test
    public void testDefaultStateIsZeroZero() {
        ComplexQuantumNumber amp0 = register.getAmplitude(0);
        assertTrue(amp0.hasComponent(QuantumNumberComponent.a), "Amplitude 0 should contain basis 'a'");
        assertEquals(Complex.ONE, amp0.getCoefficient(QuantumNumberComponent.a), "Coefficient should be 1+0i");

        for (int i = 1; i < 4; i++) {
            ComplexQuantumNumber amp = register.getAmplitude(i);
            assertTrue(amp.isZero(), "Amplitude at index " + i + " should be zero");
        }
    }

    @Test
    public void testSetAndGetAmplitude() {
        ComplexQuantumNumber amp = new ComplexQuantumNumber();
        amp.addComponent(QuantumNumberComponent.b, new Complex(0.5, 0.5));

        register.setAmplitude(2, amp);
        ComplexQuantumNumber retrieved = register.getAmplitude(2);

        assertEquals(amp, retrieved, "Set and retrieved amplitude should be equal");
    }

    @Test
    public void testNormalize() {
        ComplexQuantumNumber amp0 = new ComplexQuantumNumber();
        amp0.addComponent(QuantumNumberComponent.a, new Complex(3, 0));
        ComplexQuantumNumber amp1 = new ComplexQuantumNumber();
        amp1.addComponent(QuantumNumberComponent.a, new Complex(4, 0));

        register.setAmplitude(0, amp0);
        register.setAmplitude(1, amp1);
        register.setAmplitude(2, new ComplexQuantumNumber());
        register.setAmplitude(3, new ComplexQuantumNumber());

        register.normalize();

        double sumSquares = 0.0;
        for (int i = 0; i < 4; i++) {
            sumSquares += register.getAmplitude(i).magnitudeSquared();
        }
        assertEquals(1.0, sumSquares, 1e-12, "Sum of squares after normalization should be 1");

        ComplexQuantumNumber normAmp0 = register.getAmplitude(0);
        Complex coeff0 = normAmp0.getCoefficient(QuantumNumberComponent.a);
        assertEquals(0.6, coeff0.getReal(), 1e-12);
        assertEquals(0.0, coeff0.getImaginary(), 1e-12);

        ComplexQuantumNumber normAmp1 = register.getAmplitude(1);
        Complex coeff1 = normAmp1.getCoefficient(QuantumNumberComponent.a);
        assertEquals(0.8, coeff1.getReal(), 1e-12);
        assertEquals(0.0, coeff1.getImaginary(), 1e-12);
    }

    @Test
    public void testGetAmplitudeInvalidIndex() {
        assertThrows(IndexOutOfBoundsException.class, () -> register.getAmplitude(-1));
        assertThrows(IndexOutOfBoundsException.class, () -> register.getAmplitude(4));
    }

    @Test
    public void testSetAmplitudeInvalidIndex() {
        ComplexQuantumNumber amp = new ComplexQuantumNumber();
        amp.addComponent(QuantumNumberComponent.a, Complex.ONE);
        assertThrows(IndexOutOfBoundsException.class, () -> register.setAmplitude(-1, amp));
        assertThrows(IndexOutOfBoundsException.class, () -> register.setAmplitude(4, amp));
    }

    @Test
    public void testSetAmplitudeNull() {
        assertThrows(IllegalArgumentException.class, () -> register.setAmplitude(0, null));
    }
}
