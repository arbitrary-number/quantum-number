package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.*;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.ModularExponentiation;

public class ModularExponentiationTest {

    @Test
    public void testModularExponentiation() {
        int a = 7;
        int N = 15;
        int nInputQubits = 3;   // 8 states
        int nOutputQubits = 4;  // 16 states for output register

        int totalQubits = nInputQubits + nOutputQubits;
        int size = 1 << totalQubits;

        // Initialize quantum register with amplitude 1 at |x=0, y=0>
        ComplexQuantumRegister register = ComplexQuantumRegister.ofDimension(size);
        for (int i = 0; i < size; i++) {
            register.setAmplitude(i, ComplexQuantumNumber.zero());
        }
        register.setAmplitude(0, ComplexQuantumNumber.fromComplex(new Complex(1, 0)));

        ModularExponentiation modExp = new ModularExponentiation(a, N, nInputQubits, nOutputQubits);
        ComplexQuantumRegister result = modExp.apply(register);

        // Check the amplitudes correspond to |x, a^x mod N>
        for (int x = 0; x < (1 << nInputQubits); x++) {
            int modExpVal = (int) (Math.pow(a, x) % N);

            int expectedIndex = x * (1 << nOutputQubits) + modExpVal;

            ComplexQuantumNumber amp = result.getAmplitude(expectedIndex);
            if (x == 0) {
                // Amplitude for |0,1> should be 1 (initial input was |0,0> but modular exponentiation on zero input yields something)
                // Actually, original amplitude was only on |0,0>, so only this one non-zero?
                // Let's just check non-zero amplitude exists only for x=0
                assertNotNull(amp);
                assertFalse(amp.isZero());
            } else {
                // All other amplitudes should be zero since input register had amplitude only at |0,0>
                assertTrue(amp == null || amp.isZero());
            }
        }
    }
}
