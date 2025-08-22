package com.github.quantum_numer.qubit.test;

import org.apache.commons.math3.complex.Complex;
import org.junit.jupiter.api.Test;

import com.github.quantum_number.qubit.ComplexQuantumCircuit;
import com.github.quantum_number.qubit.ComplexQuantumGate;
import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.QuantumGateUtils;
import com.github.quantum_number.qubit.QuantumNumberComponent;

public class ComplexQuantumTeleportationTest {

	@Test
	public void testQuantumTeleportation() {
	    int nQubits = 3; // qubit 0: to teleport, 1 and 2: entangled pair

	    // Basis components and constants
	    QuantumNumberComponent a = QuantumNumberComponent.a;
	    Complex one = Complex.ONE;
	    Complex zero = Complex.ZERO;
	    double sqrt2inv = 1.0 / Math.sqrt(2);
	    Complex hVal = new Complex(sqrt2inv, 0);

	    // Hadamard gate (single qubit)
	    Complex[][] hadamard = {
	        {hVal, hVal},
	        {hVal, hVal.negate()}
	    };

	    // Pauli-X and Z gates (for correction)
	    Complex[][] xGate = {
	        {zero, one},
	        {one, zero}
	    };
	    Complex[][] zGate = {
	        {one, zero},
	        {zero, one.negate()}
	    };

	    // Prepare initial state |ψ⟩ to teleport on qubit 0, e.g. |+> = H|0>
	    ComplexQuantumRegister input = ComplexQuantumRegister.ofQubits(nQubits);
	    // For simplicity, start with |000> state (index 0)
	    ComplexQuantumNumber zeroState = new ComplexQuantumNumber();
	    zeroState.addComponent(a, one);
	    input.setAmplitude(0, zeroState);
	    for (int i = 1; i < input.size(); i++) {
	        input.setAmplitude(i, new ComplexQuantumNumber());
	    }

	    // Create circuit
	    ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();

	    // Step 1: Create EPR pair on qubits 1 and 2
	    circuit.addGate(new ComplexQuantumGate(
	        QuantumGateUtils.singleQubitGateOnNQubits(hadamard, nQubits, 1) // Hadamard on qubit 1
	    ));
	    circuit.addGate(new ComplexQuantumGate(
	        QuantumGateUtils.controlledGateOnNQubits(
	            xGate, nQubits, 1, 2 // CNOT with control=1, target=2
	        )
	    ));

	    // Step 2: Entangle qubit 0 (to teleport) with qubit 1
	    circuit.addGate(new ComplexQuantumGate(
	        QuantumGateUtils.controlledGateOnNQubits(
	            xGate, nQubits, 0, 1 // CNOT control=0, target=1
	        )
	    ));
	    circuit.addGate(new ComplexQuantumGate(
	        QuantumGateUtils.singleQubitGateOnNQubits(hadamard, nQubits, 0) // Hadamard on qubit 0
	    ));

	    // Run circuit
	    ComplexQuantumRegister afterCircuit = circuit.run(input);

	    // TODO: Implement measurement of qubits 0,1 and conditional corrections on qubit 2

	    // For now just print amplitudes
	    for (int i = 0; i < afterCircuit.size(); i++) {
	        System.out.println("Amplitude " + i + ": " + afterCircuit.getAmplitude(i));
	    }

	    // Further steps: simulate measurement, apply corrections and check final state on qubit 2 matches input
	}

}
