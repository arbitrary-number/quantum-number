package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumGate;
import com.github.quantum_number.qubit.ComplexQuantumMeasurement;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.QuantumNumberComponent;
import com.github.quantum_number.qubit.measurement.ComplexMeasurementResult;

public class ComplexCompleteTeleportationV3Test {

	@Test
	public void testQuantumTeleportation() {
	    int nQubits = 3;

	    // Step 1: Create |+00> state to teleport
	    ComplexQuantumRegister input = ComplexQuantumRegister.ofQubits(nQubits);

	    ComplexQuantumNumber plusStateComponent  = new ComplexQuantumNumber();
	    plusStateComponent .addComponent(QuantumNumberComponent.a, Complex.ONE.divide(new Complex(Math.sqrt(2), 0)));

	    input.setAmplitude(0, plusStateComponent ); // |000⟩
	    input.setAmplitude(4, plusStateComponent ); // |100⟩
	    for (int i = 1; i < input.size(); i++) {
	        if (i != 4) input.setAmplitude(i, new ComplexQuantumNumber());
	    }

	    // Step 2: Create EPR pair between qubits 1 and 2
	    ComplexQuantumGate hadamard = ComplexQuantumGate.hadamard();
	    ComplexQuantumGate H1 = ComplexQuantumGate
	    		.singleQubitGateOnNQubitsV2(hadamard, nQubits, 1);
	    input = H1.apply(input);

	    ComplexQuantumGate cnot12 = ComplexQuantumGate.controlledXOnNQubits(nQubits, 1, 2);
	    input = cnot12.apply(input);

	    // Step 3: Entangle qubit 0 with qubit 1
	    ComplexQuantumGate cnot01 = ComplexQuantumGate.controlledXOnNQubits(nQubits, 0, 1);
	    input = cnot01.apply(input);

	    ComplexQuantumGate H0 = ComplexQuantumGate.singleQubitGateOnNQubits(hadamard, nQubits, 0);
	    input = H0.apply(input);

	    // Step 4: Measure qubits 0 and 1
	    ComplexMeasurementResult measurementResult =
	        ComplexQuantumMeasurement.measureQubitsComplex(input, new int[] {0, 1});
	    int measuredBits = measurementResult.getMeasurementOutcome();
	    ComplexQuantumRegister collapsedState = measurementResult.getCollapsedRegister();

	    System.out.println("Measurement on qubits 0 and 1: " + Integer.toBinaryString(measuredBits));

	    // Step 5: Apply correction to qubit 2
	    ComplexQuantumGate pauliX = ComplexQuantumGate.pauliX();
	    ComplexQuantumGate pauliZ = ComplexQuantumGate.pauliZ();

	    ComplexQuantumGate X2 = ComplexQuantumGate.singleQubitGateOnNQubitsV2(pauliX, nQubits, 2);
	    ComplexQuantumGate Z2 = ComplexQuantumGate.singleQubitGateOnNQubitsV2(pauliZ, nQubits, 2);

	    if ((measuredBits & 1) == 1) { // bit 0 = qubit 1
	        collapsedState = X2.apply(collapsedState);
	    }
	    if ((measuredBits & 2) == 2) { // bit 1 = qubit 0
	        collapsedState = Z2.apply(collapsedState);
	    }

	    // Step 6: Extract final state of qubit 2
	    ComplexQuantumNumber finalQubit2State = ComplexQuantumGate.extractQubitState(collapsedState, 2);
	    System.out.println("Final state on qubit 2: " + finalQubit2State);

	    // Step 7: Check fidelity with |+⟩
	    double expectedAmplitude = 1.0 / Math.sqrt(2);
	    double eps = 1e-10;

	    System.out.println("State before measurement:\n" + input);
	    System.out.println("State after measurement:\n" + collapsedState);

	    System.out.println("Expected |+>: (1/√2) ≈ " + expectedAmplitude);
	    System.out.println("Measured a: " + finalQubit2State.getComponent(QuantumNumberComponent.a));
	    System.out.println("Measured b: " + finalQubit2State.getComponent(QuantumNumberComponent.b));

	    double actualA = finalQubit2State.getComponent(QuantumNumberComponent.a).abs();
	    double actualB = finalQubit2State.getComponent(QuantumNumberComponent.b).abs();
	    double fidelity = Math.pow(actualA * expectedAmplitude + actualB * expectedAmplitude, 2);
	    System.out.println("Fidelity with |+⟩: " + fidelity);

	    assertEquals(expectedAmplitude, finalQubit2State.getComponent(QuantumNumberComponent.a).abs(), eps);
	    assertEquals(expectedAmplitude, finalQubit2State.getComponent(QuantumNumberComponent.b).abs(), eps);

	    Complex a = finalQubit2State.getComponent(QuantumNumberComponent.a);
	    Complex b = finalQubit2State.getComponent(QuantumNumberComponent.b);

	    Complex expected = new Complex(1/Math.sqrt(2), 0);
	    Complex inner = a.multiply(expected.conjugate()).add(b.multiply(expected.conjugate()));
	    double fidelity2 = inner.abs() * inner.abs();
	    System.out.println("Fidelity2 with |+⟩: " + fidelity2);
	}



}
