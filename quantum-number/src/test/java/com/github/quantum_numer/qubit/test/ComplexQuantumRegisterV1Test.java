package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumCircuit;
import com.github.quantum_number.qubit.ComplexQuantumGate;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.QuantumNumberComponent;

public class ComplexQuantumRegisterV1Test {

    private ComplexQuantumRegister register;

    @BeforeEach
    public void setup() {
        register = ComplexQuantumRegister.ofQubits(2);
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
    public void testQuantumRegisterInitialization() {
        ComplexQuantumRegister register = ComplexQuantumRegister.ofDimension(2);
        ComplexQuantumNumber amplitude = new ComplexQuantumNumber();
        amplitude.addComponent(QuantumNumberComponent.a, Complex.ONE);

        register.setAmplitude(0, amplitude);

        assertEquals(Complex.ONE, register.getAmplitude(0).getCoefficient(QuantumNumberComponent.a));
        assertEquals(0.0, register.getAmplitude(1).magnitudeSquared(), 1e-10);
    }


    private static final QuantumNumberComponent a = QuantumNumberComponent.a;


//    @Test
//    public void testRegisterTensorProductProducesExpectedAmplitudes() {
//        // Prepare register r1 = [1, 0]
//        ComplexQuantumRegister r1 = new ComplexQuantumRegister(2);
//        ComplexQuantumNumber amp0 = new ComplexQuantumNumber();
//        amp0.addComponent(a, Complex.ONE); // |0⟩
//        r1.setAmplitude(0, amp0);
//        r1.setAmplitude(1, new ComplexQuantumNumber()); // |1⟩ = 0
//
//        // Prepare register r2 = [0, i]
//        ComplexQuantumRegister r2 = new ComplexQuantumRegister(2);
//        ComplexQuantumNumber amp1 = new ComplexQuantumNumber();
//        amp1.addComponent(a, new Complex(0, 1)); // i * |1⟩
//        r2.setAmplitude(0, new ComplexQuantumNumber()); // |0⟩ = 0
//        r2.setAmplitude(1, amp1);
//
//        // Tensor product: r1 ⊗ r2
//        ComplexQuantumRegister result = ComplexQuantumRegister.tensorProduct(r1, r2);
//
//        // Expected size = 4 (2 qubits)
//        assertEquals(4, result.size());
//
//        // Expected amplitudes:
//        // |00⟩ = 0
//        assertEquals(0.0, result.getAmplitude(0).getCoefficient(a).abs(), 1e-10);
//
//        // |01⟩ = 1 * i = i
//        Complex amp01 = result.getAmplitude(1).getCoefficient(a);
//        assertEquals(0.0, amp01.getReal(), 1e-10);
//        assertEquals(1.0, amp01.getImaginary(), 1e-10);
//
//        // |10⟩ = 0
//        assertEquals(0.0, result.getAmplitude(2).getCoefficient(a).abs(), 1e-10);
//
//        // |11⟩ = 0
//        assertEquals(0.0, result.getAmplitude(3).getCoefficient(a).abs(), 1e-10);
//    }

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
//
//    @Test
//    public void testRegisterTensorProductProducesExpectedAmplitudes2() {
//        ComplexQuantumRegister r1 = new ComplexQuantumRegister(2);
//        ComplexQuantumNumber amp0 = new ComplexQuantumNumber();
//        amp0.addComponent(QuantumNumberComponent.a, Complex.ONE);
//        r1.setAmplitude(0, amp0);
//
//        ComplexQuantumRegister r2 = new ComplexQuantumRegister(2);
//        ComplexQuantumNumber amp1 = new ComplexQuantumNumber();
//        amp1.addComponent(QuantumNumberComponent.a, new Complex(0, 1)); // i
//        r2.setAmplitude(1, amp1);
//
//        ComplexQuantumRegister result = ComplexQuantumRegister.tensorProduct(r1, r2);
//
//        System.out.println("Tensor product result:");
//        for (int i = 0; i < result.size(); i++) {
//            ComplexQuantumNumber amp = result.getAmplitude(i);
//            System.out.printf("Index %d: %s%n", i, amp);
//        }
//
//        Complex amp01 = result.getAmplitude(1).getCoefficient(QuantumNumberComponent.combine(
//            QuantumNumberComponent.a, QuantumNumberComponent.a
//        ));
//
//        assertEquals(0.0, amp01.getReal(), 1e-10);
//        assertEquals(1.0, amp01.getImaginary(), 1e-10);
//    }

//    @Test
//    public void testTeleportationWithMeasurementAndCorrections() {
//        // Step 1: Prepare the unknown qubit |ψ⟩ = α|0⟩ + β|1⟩
//        Complex alpha = new Complex(1 / Math.sqrt(3), 0);
//        Complex beta = new Complex(Math.sqrt(2.0 / 3.0), 0);
//
//        // Prepare |ψ⟩ state in 1-qubit register
//        ComplexQuantumRegister psi = new ComplexQuantumRegister(2);
//        ComplexQuantumNumber psi0 = new ComplexQuantumNumber();
//        psi0.addComponent(a, alpha);
//        ComplexQuantumNumber psi1 = new ComplexQuantumNumber();
//        psi1.addComponent(a, beta);
//        psi.setAmplitude(0, psi0);
//        psi.setAmplitude(1, psi1);
//
//        // Step 2: Prepare entangled Bell pair |Φ+⟩ between qubits 1 and 2
//        ComplexQuantumRegister bell = prepareBellPair();
//
//        // Step 3: Initialize 3-qubit register |ψ⟩⊗|Φ+⟩
//        ComplexQuantumRegister input = ComplexQuantumRegister.tensorProduct(psi, bell);
//
//        // Step 4: Build multi-qubit gates
//
//        // Identity gate 2x2
//        ComplexQuantumGate I = ComplexQuantumGate.identity(2);
//
//        // Build 3-qubit Hadamard on qubit 0: H ⊗ I ⊗ I
//        ComplexQuantumGate H0 = ComplexQuantumGate.tensorProduct(
//            ComplexQuantumGate.hadamard(),
//            ComplexQuantumGate.tensorProduct(I, I)
//        );
//
//        // Build 3-qubit CNOT with control=0 target=1:
//        // This is CNOT ⊗ I
//        ComplexQuantumGate CNOT_01 = ComplexQuantumGate.tensorProduct(
//            ComplexQuantumGate.cnot(),
//            I
//        );
//
//        // Step 5: Apply gates: CNOT(0->1) then Hadamard(0)
//        ComplexQuantumRegister afterCNOT = CNOT_01.apply(input);
//        ComplexQuantumRegister afterHadamard = H0.apply(afterCNOT);
//
//        // Step 6: Measure qubits 0 and 1
//        int measuredBits = measureQubits(afterHadamard, new int[]{0, 1});
//
//        // Step 7: Collapse state based on measurement outcome
//        ComplexQuantumRegister collapsedState =
//        		ComplexQuantumRegister.collapseState(afterHadamard, new int[]{0, 1}, measuredBits);
//
//        // Step 8: Apply corrections on qubit 2 depending on measurement outcome
//        ComplexQuantumRegister correctedState =
//        		ComplexQuantumRegister.applyCorrections(collapsedState, measuredBits, 2);
//
//        // Step 9: Extract qubit 2 reduced state (single qubit)
//        ComplexQuantumRegister teleportedQubit =
//        		ComplexQuantumRegister.reduceToSingleQubit(correctedState, 2, 3);
//
//        // Step 10: Verify teleportedQubit amplitudes ≈ psi amplitudes
//        for (int i = 0; i < 2; i++) {
//            ComplexQuantumNumber teleportedAmp = teleportedQubit.getAmplitude(i);
//            ComplexQuantumNumber originalAmp = psi.getAmplitude(i);
//
//            Complex teleportedCoeff = teleportedAmp.getCoefficient(a);
//            Complex originalCoeff = originalAmp.getCoefficient(a);
//
//            System.out.printf("Teleported amplitude |%d⟩: %s%n", i, teleportedCoeff);
//            System.out.printf("Original amplitude |%d⟩: %s%n", i, originalCoeff);
//
//            assertEquals(originalCoeff.getReal(), teleportedCoeff.getReal(), 1e-6, "Real parts differ at amplitude " + i);
//            assertEquals(originalCoeff.getImaginary(), teleportedCoeff.getImaginary(), 1e-6, "Imag parts differ at amplitude " + i);
//        }
//    }
//
//    @Test
//    public void testTeleportationWithMeasurementAndCorrections() {
//        // Step 1: Prepare the unknown qubit |ψ⟩ = α|0⟩ + β|1⟩ (1 qubit)
//        Complex alpha = new Complex(1 / Math.sqrt(3), 0);
//        Complex beta = new Complex(Math.sqrt(2.0 / 3.0), 0);
//
//        ComplexQuantumRegister psi = new ComplexQuantumRegister(1); // 1 qubit!
//        ComplexQuantumNumber psi0 = new ComplexQuantumNumber();
//        psi0.addComponent(a, alpha);
//        ComplexQuantumNumber psi1 = new ComplexQuantumNumber();
//        psi1.addComponent(a, beta);
//        psi.setAmplitude(0, psi0);
//        psi.setAmplitude(1, psi1);
//
//        // Step 2: Prepare entangled Bell pair |Φ+⟩ between qubits 1 and 2 (2 qubits)
//        ComplexQuantumRegister bell = prepareBellPair(); // should return 2 qubits register
//
//        // Step 3: Initialize 3-qubit register |ψ⟩⊗|Φ+⟩ (1 + 2 qubits = 3 qubits)
//        ComplexQuantumRegister input = ComplexQuantumRegister.tensorProduct(psi, bell);
//
//
//        // Step 4: Build multi-qubit gates
//
//        // Identity gate for 1 qubit (2x2)
//        ComplexQuantumGate I = ComplexQuantumGate.identity(1);
//
//        // Hadamard gate for 1 qubit
//        ComplexQuantumGate H = ComplexQuantumGate.hadamard();
//
//        // Build 3-qubit Hadamard on qubit 0: H ⊗ I ⊗ I
//        ComplexQuantumGate H0 = ComplexQuantumGate.tensorProduct(
//            H,
//            ComplexQuantumGate.tensorProduct(I, I)
//        );
//
//        // Build 3-qubit CNOT with control=0 target=1: CNOT ⊗ I
//        ComplexQuantumGate CNOT_01 = ComplexQuantumGate.tensorProduct(
//            ComplexQuantumGate.cnot(),
//            I
//        );
//
//        System.out.println("Input register qubits: " + input.getNumQubits());
//        System.out.println("Hadamard gate qubits: " + H0.getNumQubits());
//        System.out.println("CNOT gate qubits: " + CNOT_01.getNumQubits());
//
//
//        // Step 5: Apply gates: CNOT(0->1) then Hadamard(0)
//        ComplexQuantumRegister afterCNOT = CNOT_01.apply(input);
//        ComplexQuantumRegister afterHadamard = H0.apply(afterCNOT);
//
//        // Step 6: Measure qubits 0 and 1
//        int measuredBits = measureQubits(afterHadamard, new int[]{0, 1});
//
//        // Step 7: Collapse state based on measurement outcome
//        ComplexQuantumRegister collapsedState =
//            ComplexQuantumRegister.collapseState(afterHadamard, new int[]{0, 1}, measuredBits);
//
//        // Step 8: Apply corrections on qubit 2 depending on measurement outcome
//        ComplexQuantumRegister correctedState =
//            ComplexQuantumRegister.applyCorrections(collapsedState, measuredBits, 2);
//
//        // Step 9: Extract qubit 2 reduced state (single qubit)
//        ComplexQuantumRegister teleportedQubit =
//            ComplexQuantumRegister.reduceToSingleQubit(correctedState, 2, 3);
//
//        // Step 10: Verify teleportedQubit amplitudes ≈ psi amplitudes
//        for (int i = 0; i < 2; i++) {
//            ComplexQuantumNumber teleportedAmp = teleportedQubit.getAmplitude(i);
//            ComplexQuantumNumber originalAmp = psi.getAmplitude(i);
//
//            Complex teleportedCoeff = teleportedAmp.getCoefficient(a);
//            Complex originalCoeff = originalAmp.getCoefficient(a);
//
//            System.out.printf("Teleported amplitude |%d⟩: %s%n", i, teleportedCoeff);
//            System.out.printf("Original amplitude |%d⟩: %s%n", i, originalCoeff);
//
//            assertEquals(originalCoeff.getReal(), teleportedCoeff.getReal(), 1e-6, "Real parts differ at amplitude " + i);
//            assertEquals(originalCoeff.getImaginary(), teleportedCoeff.getImaginary(), 1e-6, "Imag parts differ at amplitude " + i);
//        }
//    }

    @Test
    public void testTeleportationWithMeasurementAndCorrections() {
        // Step 1: Prepare the unknown qubit |ψ⟩ = α|0⟩ + β|1⟩ (1 qubit)
        Complex alpha = new Complex(1.0 / Math.sqrt(3), 0);  // make sure division is floating point
        Complex beta = new Complex(Math.sqrt(2.0 / 3.0), 0);

        ComplexQuantumRegister psi = ComplexQuantumRegister.ofQubits(1); // 1 qubit
        ComplexQuantumNumber psi0 = new ComplexQuantumNumber();
        psi0.addComponent(a, alpha);
        ComplexQuantumNumber psi1 = new ComplexQuantumNumber();
        psi1.addComponent(a, beta);
        psi.setAmplitude(0, psi0);
        psi.setAmplitude(1, psi1);

        // Step 2: Prepare entangled Bell pair |Φ+⟩ between qubits 1 and 2 (2 qubits)
        ComplexQuantumRegister bell = prepareBellPair(); // returns 2-qubit register

        // Step 3: Initialize 3-qubit register |ψ⟩⊗|Φ+⟩ (1 + 2 = 3 qubits)
        ComplexQuantumRegister input = ComplexQuantumRegister.tensorProduct(psi, bell);

        // Optional debug: calculate qubits from register size
        int inputQubits = (int)(Math.log(input.size()) / Math.log(2));
        System.out.println("Input register qubits: " + inputQubits);

        // Step 4: Build multi-qubit gates

        // Identity gate for 1 qubit (2x2)
        ComplexQuantumGate I = ComplexQuantumGate.identityOfQubits(1);

        // Hadamard gate for 1 qubit
        ComplexQuantumGate H = ComplexQuantumGate.hadamard();

        // Build 3-qubit Hadamard on qubit 0: H ⊗ I ⊗ I
        ComplexQuantumGate H0 = ComplexQuantumGate.tensorProduct(
            H,
            ComplexQuantumGate.tensorProduct(I, I)
        );

        // Build 3-qubit CNOT with control=0 target=1: CNOT ⊗ I
        ComplexQuantumGate CNOT_01 = ComplexQuantumGate.tensorProduct(
            ComplexQuantumGate.cnot(),
            I
        );

        // Optional debug: calculate qubits from gate matrix size
        int H0Qubits = (int)(Math.log(H0.getDimension()) / Math.log(2));
        int CNOTQubits = (int)(Math.log(CNOT_01.getDimension()) / Math.log(2));
        System.out.println("Hadamard gate qubits: " + H0Qubits);
        System.out.println("CNOT gate qubits: " + CNOTQubits);

        // Step 5: Apply gates: CNOT(0->1) then Hadamard(0)
        ComplexQuantumRegister afterCNOT = CNOT_01.apply(input);
        ComplexQuantumRegister afterHadamard = H0.apply(afterCNOT);

        // Step 6: Measure qubits 0 and 1
        int measuredBits = measureQubits(afterHadamard, new int[]{0, 1});

        // Step 7: Collapse state based on measurement outcome
        ComplexQuantumRegister collapsedState =
            ComplexQuantumRegister.collapseState(afterHadamard, new int[]{0, 1}, measuredBits);

        // Step 8: Apply corrections on qubit 2 depending on measurement outcome
        ComplexQuantumRegister correctedState =
            ComplexQuantumRegister.applyCorrections(collapsedState, measuredBits, 2);

        // Step 9: Extract qubit 2 reduced state (single qubit)
        ComplexQuantumRegister teleportedQubit =
            ComplexQuantumRegister.reduceToSingleQubit(correctedState, 2, 3);

        // Step 10: Verify teleportedQubit amplitudes ≈ psi amplitudes
        for (int i = 0; i < 2; i++) {
            ComplexQuantumNumber teleportedAmp = teleportedQubit.getAmplitude(i);
            ComplexQuantumNumber originalAmp = psi.getAmplitude(i);

            Complex teleportedCoeff = teleportedAmp.getCoefficient(a);
            Complex originalCoeff = originalAmp.getCoefficient(a);

            System.out.printf("Teleported amplitude |%d⟩: %s%n", i, teleportedCoeff);
            System.out.printf("Original amplitude |%d⟩: %s%n", i, originalCoeff);

            assertEquals(originalCoeff.getReal(), teleportedCoeff.getReal(), 1e-6, "Real parts differ at amplitude " + i);
            assertEquals(originalCoeff.getImaginary(), teleportedCoeff.getImaginary(), 1e-6, "Imag parts differ at amplitude " + i);
        }
    }



    private ComplexQuantumRegister prepareBellPair() {
        // Bell state |Φ+> = (|00> + |11>) / sqrt(2)
        ComplexQuantumRegister bell = ComplexQuantumRegister.ofDimension(4); // 2 qubits -> 4 amplitudes

        ComplexQuantumNumber amp00 = new ComplexQuantumNumber();
        amp00.addComponent(a, new Complex(1 / Math.sqrt(2), 0));

        ComplexQuantumNumber amp11 = new ComplexQuantumNumber();
        amp11.addComponent(a, new Complex(1 / Math.sqrt(2), 0));

        bell.setAmplitude(0, amp00); // |00>
        bell.setAmplitude(3, amp11); // |11>

        // Other amplitudes are zero by default
        for (int i = 1; i < 3; i++) {
            bell.setAmplitude(i, new ComplexQuantumNumber());
        }

        return bell;
    }

    /**
     * Simulate measurement on specified qubits of the register.
     * Returns an integer representing the measured bits combined (e.g. 0,1,2,3 for 2 qubits).
     */
    private int measureQubits(ComplexQuantumRegister register, int[] qubitIndices) {
        int totalStates = register.size();
        double[] probabilities = new double[totalStates];

        // Calculate probability for each basis state
        for (int i = 0; i < totalStates; i++) {
            ComplexQuantumNumber amp = register.getAmplitude(i);
            double prob = 0;
            // Sum squared magnitudes of all components in amp
            for (Complex c : amp.getComponents().values()) {
                prob += c.abs() * c.abs();
            }
            probabilities[i] = prob;
        }

        // Calculate probabilities for each measurement outcome of the specified qubits
        int outcomes = 1 << qubitIndices.length;
        double[] outcomeProbs = new double[outcomes];

        for (int state = 0; state < totalStates; state++) {
            int outcome = 0;
            for (int q = 0; q < qubitIndices.length; q++) {
                int qubitIndex = qubitIndices[q];
                // Extract bit at position qubitIndex from state index
                int bit = (state >> qubitIndex) & 1;
                outcome |= (bit << q);
            }
            outcomeProbs[outcome] += probabilities[state];
        }

        // Randomly choose outcome according to probabilities
        double r = Math.random();
        double cumulative = 0;
        for (int i = 0; i < outcomes; i++) {
            cumulative += outcomeProbs[i];
            if (r <= cumulative) {
                return i;
            }
        }
        // Fallback
        return outcomes - 1;
    }




}
