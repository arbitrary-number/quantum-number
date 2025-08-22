package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.*;

import java.math.BigInteger;
import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.*;

public class ShorFactorizationTestV1 {

    @Test
    public void testFactorizationOf15() {
        int N = 15;  // Composite to factor
        int nInputQubits = 4;   // Enough to cover range > N²
        int nOutputQubits = 4;
        int totalQubits = nInputQubits + nOutputQubits;
        int maxDenominator = 50;  // Arbitrary safety limit

        // 1. Choose a random a ∈ [2, N-1] such that gcd(a, N)=1
        int a = 2;
        assertEquals(1, BigInteger.valueOf(a).gcd(BigInteger.valueOf(N)).intValue(),
                "Base 'a' must be coprime to N");

        // 2. Step 1: Initialize registers and apply modular exponentiation (emulated)
        ComplexQuantumRegister reg = ComplexQuantumNumberQFT.initializeRegisters(nInputQubits, nOutputQubits);
        // Apply modular exponentiation: |x, 1> → |x, a^x mod N>
        // This is a stub simulation for N=15, a=2
        for (int x = 0; x < (1 << nInputQubits); x++) {
            ComplexQuantumNumber amp = reg.getAmplitude(x * (1 << nOutputQubits));
            if (amp == null || amp.getComponent(QuantumNumberComponent.a) == null) continue;
            // Only operate on non-zero amplitudes
            int y = BigInteger.valueOf(a).pow(x).mod(BigInteger.valueOf(N)).intValue();
            int outIndex = x * (1 << nOutputQubits) + y;
            reg.setAmplitude(outIndex, amp);  // Move amplitude to the correct output
            reg.setAmplitude(x * (1 << nOutputQubits), ComplexQuantumNumber.zero());
        }
        reg.normalize();

        // 3. Step 2: QFT on input register
        reg = ComplexQuantumNumberQFTStep2.applyQFT(reg);

        // 4. Step 3: Measure and estimate period
        int measured = QuantumMeasurement.measure(reg);
        int r = ShorStep3.estimatePeriod(measured, nInputQubits, maxDenominator);

        System.out.println("Measured index: " + measured);
        System.out.println("Estimated period r: " + r);

        assertTrue(r > 0 && r <= maxDenominator, "Failed to estimate valid period");

        // 5. Classical fallback: If r is odd or a^(r/2) ≡ ±1 (mod N), trial fails
        assertTrue(r % 2 == 0, "Period must be even");

        int apow = BigInteger.valueOf(a)
            .modPow(BigInteger.valueOf(r / 2), BigInteger.valueOf(N))
            .intValue();
        assertNotEquals(N - 1, apow, "a^(r/2) mod N should not be -1");

        // Compute factors: gcd(a^(r/2) ± 1, N)
        int factor1 = BigInteger.valueOf(apow - 1).gcd(BigInteger.valueOf(N)).intValue();
        int factor2 = BigInteger.valueOf(apow + 1).gcd(BigInteger.valueOf(N)).intValue();

        System.out.println("Found factors: " + factor1 + ", " + factor2);

        // Ensure non-trivial factors
        assertTrue(factor1 > 1 && factor1 < N, "factor1 is trivial");
        assertTrue(factor2 > 1 && factor2 < N, "factor2 is trivial");
        assertEquals(N, factor1 * factor2, "factors do not multiply to N");
    }
}
