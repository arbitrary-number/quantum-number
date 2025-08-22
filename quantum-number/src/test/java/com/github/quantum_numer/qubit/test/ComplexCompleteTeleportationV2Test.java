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

public class ComplexCompleteTeleportationV2Test {

//	@Test
//	public void testCompleteQuantumTeleportation() {
//	    int nQubits = 3;
//	    QuantumNumberComponent a = QuantumNumberComponent.a;
//
//	    // Prepare initial state on qubit 0 (example |+> state)
//	    ComplexQuantumRegister input = ComplexQuantumRegister.ofQubits(nQubits);
//	    ComplexQuantumNumber zeroState = new ComplexQuantumNumber();
//	    zeroState.addComponent(a, new Complex(1/Math.sqrt(2), 0)); // amplitude for |0>
//	    input.setAmplitude(0, zeroState);
//	    ComplexQuantumNumber oneState = new ComplexQuantumNumber();
//	    oneState.addComponent(a, new Complex(1/Math.sqrt(2), 0));  // amplitude for |1>
//	    input.setAmplitude(1, oneState);
//	    for (int i = 2; i < input.size(); i++) input.setAmplitude(i, new ComplexQuantumNumber());
//
//	    // Build circuit (Hadamard + CNOTs as before)
//	    ComplexQuantumCircuit circuit = new ComplexQuantumCircuit();
//	    // Create EPR pair on qubits 1 and 2
//	    circuit.addGate(new ComplexQuantumGate(
//	        QuantumGateUtils.singleQubitGateOnNQubits(QuantumGateUtils.hadamardMatrix(), nQubits, 1)
//	    ));
//	    circuit.addGate(new ComplexQuantumGate(
//	        QuantumGateUtils.controlledGateOnNQubits(QuantumGateUtils.xMatrix(), nQubits, 1, 2)
//	    ));
//	    // Entangle qubit 0 with qubit 1
//	    circuit.addGate(new ComplexQuantumGate(
//	        QuantumGateUtils.controlledGateOnNQubits(QuantumGateUtils.xMatrix(), nQubits, 0, 1)
//	    ));
//	    circuit.addGate(new ComplexQuantumGate(
//	        QuantumGateUtils.singleQubitGateOnNQubits(QuantumGateUtils.hadamardMatrix(), nQubits, 0)
//	    ));
//
//	    ComplexQuantumRegister state = circuit.run(input);
//
//	    // Simulate measurement on qubits 0 and 1
//	    int[] measurementResult = measureQubits(state, new int[]{0, 1});
//	    int m0 = measurementResult[0];
//	    int m1 = measurementResult[1];
//
//	    // Collapse state according to measurement result
//	    state = collapseState(state, new int[]{0,1}, measurementResult);
//
//	    // Apply conditional corrections on qubit 2
//	    if (m1 == 1) {
//	        state = new ComplexQuantumGate(QuantumGateUtils.singleQubitGateOnNQubits(QuantumGateUtils.xMatrix(), nQubits, 2)).apply(state);
//	    }
//	    if (m0 == 1) {
//	        state = new ComplexQuantumGate(QuantumGateUtils.singleQubitGateOnNQubits(QuantumGateUtils.zMatrix(), nQubits, 2)).apply(state);
//	    }
//
//	    // Extract qubit 2 state amplitudes and verify match original input on qubit 0
//	    // For simplicity, check amplitudes where qubits 0 and 1 = 00
//	    ComplexQuantumNumber finalQubit2State0 = state.getAmplitude(0); // |000⟩
//	    ComplexQuantumNumber finalQubit2State1 = state.getAmplitude(4); // |100⟩ (since qubit 2 is the 3rd qubit)
//
//	    // Check final qubit 2 matches initial superposition |+>
//	    double eps = 1e-10;
//	    assertEquals(zeroState.getCoefficient(a).abs(), finalQubit2State0.getCoefficient(a).abs(), eps);
//	    assertEquals(oneState.getCoefficient(a).abs(), finalQubit2State1.getCoefficient(a).abs(), eps);
//	}

	@Test
	public void testQuantumTeleportation() {
	    int nQubits = 3; // qubit 0: to teleport, 1 and 2: entangled pair

	    // Create initial |+> state on qubit 0 (to teleport)
	    ComplexQuantumRegister input = ComplexQuantumRegister.ofQubits(nQubits);

	    // |+> = (|0> + |1>) / sqrt(2)
	    ComplexQuantumNumber plusState = new ComplexQuantumNumber();
	    plusState.addComponent(QuantumNumberComponent.a, Complex.ONE.divide(new Complex(Math.sqrt(2), 0)));
	    plusState.addComponent(QuantumNumberComponent.b, Complex.ONE.divide(new Complex(Math.sqrt(2), 0)));

	    // Set amplitude for |0..0> state with qubit 0 in superposition
	    input.setAmplitude(0, plusState); // This is a simplification; in reality, |+00> state index 0 or 1?

	    // Actually, |+00> = (|000> + |100>)/sqrt(2)
	    // So set amplitudes 0 and 4 accordingly (0b000=0, 0b100=4)
	    input.setAmplitude(0, new ComplexQuantumNumber());
	    input.setAmplitude(4, new ComplexQuantumNumber());
	    ComplexQuantumNumber amp = new ComplexQuantumNumber();
	    amp.addComponent(QuantumNumberComponent.a, Complex.ONE.divide(new Complex(Math.sqrt(2), 0)));
	    input.setAmplitude(0, amp);
	    input.setAmplitude(4, amp);

	    // The rest of amplitudes zero
	    for (int i = 1; i < input.size(); i++) {
	        if (i != 4) input.setAmplitude(i, new ComplexQuantumNumber());
	    }

	    // Define gates
	    ComplexQuantumGate hadamard = ComplexQuantumGate.hadamard();
	    ComplexQuantumGate pauliX = ComplexQuantumGate.pauliX();
	    ComplexQuantumGate pauliZ = ComplexQuantumGate.pauliZ();



	    // Step 1: Create EPR pair on qubits 1 and 2
	    ComplexQuantumGate H1 = singleQubitGateOnNQubits(hadamard, nQubits, 1);
	    input = H1.apply(input);

	    ComplexQuantumGate cnot12 = controlledXOnNQubits(nQubits, 1, 2);
	    input = cnot12.apply(input);

	    // Step 2: Entangle qubit 0 with qubit 1
	    ComplexQuantumGate cnot01 = controlledXOnNQubits(nQubits, 0, 1);
	    input = cnot01.apply(input);

	    ComplexQuantumGate H0 = singleQubitGateOnNQubits(hadamard, nQubits, 0);
	    input = H0.apply(input);

	    // Step 3: Measurement on qubits 0 and 1
	    ComplexMeasurementResult measurementResult = ComplexQuantumMeasurement.
	    		measureQubitsComplex(input, new int[] {0,1});
		int measuredBits = measurementResult.getMeasurementOutcome();
	    // For testing, print measurement result
	    System.out.println("Measurement on qubits 0 and 1: " + Integer.toBinaryString(measuredBits));

	    // Step 4: Apply corrections on qubit 2 based on measurement outcome
	    ComplexQuantumGate X2 = singleQubitGateOnNQubits(pauliX, nQubits, 2);
	    ComplexQuantumGate Z2 = singleQubitGateOnNQubits(pauliZ, nQubits, 2);

	    if ((measuredBits & 1) == 1) { // bit 0 (qubit 1)
	        input = X2.apply(input);
	    }
	    if ((measuredBits & 2) == 2) { // bit 1 (qubit 0)
	        input = Z2.apply(input);
	    }

	    // Step 5: Verify final qubit 2 matches initial state |+>
	    // Extract reduced state of qubit 2
	    ComplexQuantumNumber finalQubit2State = extractQubitState(input, 2);

	    System.out.println("Final state on qubit 2: " + finalQubit2State);

	    // Assert or check fidelity here (not implemented)

	 // Step 3: Measurement on qubits 0 and 1
	    ComplexQuantumRegister originalInput = measurementResult.getCollapsedRegister();

	    System.out.println("Measurement on qubits 0 and 1: " + Integer.toBinaryString(measuredBits));

	    if ((measuredBits & 1) == 1) { // bit 0 (qubit 1)
	    	originalInput = X2.apply(originalInput);
	    }
	    if ((measuredBits & 2) == 2) { // bit 1 (qubit 0)
	    	originalInput = Z2.apply(originalInput);
	    }

	    // Step 5: Verify final qubit 2 matches initial state |+>
	    double expectedAmplitude = 1.0 / Math.sqrt(2);
	    finalQubit2State = extractQubitState(originalInput, 2);

	    double eps = 1e-10;
	    assertEquals(expectedAmplitude, finalQubit2State.getComponent(QuantumNumberComponent.a).abs(), eps);
	    assertEquals(expectedAmplitude, finalQubit2State.getComponent(QuantumNumberComponent.b).abs(), eps);

	}

	// Helper: Embed a single-qubit gate on n qubits targeting qubit targetQubit
    // Qubit indexing from left or right? Assuming qubit 0 is most significant (leftmost)
    ComplexQuantumGate singleQubitGateOnNQubits(ComplexQuantumGate gate, int totalQubits, int targetQubit) {
        ComplexQuantumGate result = ComplexQuantumGate.identityOfQubits(1);
        for (int i = 0; i < totalQubits; i++) {
            if (i == targetQubit) {
                result = ComplexQuantumGate.tensorProduct(result, gate);
            } else {
                result = ComplexQuantumGate.tensorProduct(result, ComplexQuantumGate.identityOfQubits(1));
            }
        }
        return result;
    }

    // Helper: Controlled-X (CNOT) gate on qubits control and target in n-qubit system
    ComplexQuantumGate controlledXOnNQubits(int nQubits, int control, int target) {
        int dim = 1 << nQubits;
        Complex[][] matrix = new Complex[dim][dim];
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                matrix[i][j] = Complex.ZERO;
            }
        }

        for (int i = 0; i < dim; i++) {
            int controlBit = (i >> (nQubits - 1 - control)) & 1;
            int targetBit = (i >> (nQubits - 1 - target)) & 1;
            int flippedTargetBit = targetBit ^ controlBit;
            int j = i;
            if (targetBit != flippedTargetBit) {
                j = i ^ (1 << (nQubits - 1 - target));
            }
            matrix[j][i] = Complex.ONE;
        }

        return new ComplexQuantumGate(matrix);
    }


	// Helper to extract reduced qubit state for qubitIndex (simple extraction ignoring full density matrix)
	private ComplexQuantumNumber extractQubitState(ComplexQuantumRegister reg, int qubitIndex) {
	    int nQubits = (int)(Math.log(reg.size()) / Math.log(2));
	    int dim = reg.size();

	    // The state of one qubit in superposition can be extracted by summing amplitudes where other qubits fixed
	    // For simplicity, sum amplitudes for qubitIndex = 0 and 1 separately

	    ComplexQuantumNumber zeroAmp = new ComplexQuantumNumber();
	    ComplexQuantumNumber oneAmp = new ComplexQuantumNumber();

	    for (int i = 0; i < dim; i++) {
	        int bit = (i >> (nQubits - 1 - qubitIndex)) & 1;
	        ComplexQuantumNumber amp = reg.getAmplitude(i);
	        if (bit == 0) {
	            zeroAmp = zeroAmp.add(amp);
	        } else {
	            oneAmp = oneAmp.add(amp);
	        }
	    }

	    // Return superposition alpha|0> + beta|1> as ComplexQuantumNumber with components a and b
	    ComplexQuantumNumber state = new ComplexQuantumNumber();
	    state.addComponent(QuantumNumberComponent.a, zeroAmp.getComponent(QuantumNumberComponent.a));
	    state.addComponent(QuantumNumberComponent.b, oneAmp.getComponent(QuantumNumberComponent.a));

	    return state;
	}



}
