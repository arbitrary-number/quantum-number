package com.github.quantum_number.qubit;

import java.util.ArrayList;
import java.util.List;

/**
 * Represents a quantum circuit as a sequence of ComplexQuantumGates.
 * Applies gates in order to an input quantum register.
 */
public class ComplexQuantumCircuit {

    private final List<ComplexQuantumGate> gates;

    public ComplexQuantumCircuit() {
        this.gates = new ArrayList<>();
    }

    /**
     * Add a gate to the circuit.
     * @param gate ComplexQuantumGate to add
     */
    public void addGate(ComplexQuantumGate gate) {
        if (gate == null) {
            throw new IllegalArgumentException("Gate cannot be null");
        }
        gates.add(gate);
    }

    /**
     * Applies all gates in sequence to the input register.
     * @param input Initial quantum register state
     * @return Final quantum register after all gates applied
     */
    public ComplexQuantumRegister run(ComplexQuantumRegister input) {
        ComplexQuantumRegister current = input;
        for (ComplexQuantumGate gate : gates) {
            current = gate.apply(current);
        }
        return current;
    }

    /**
     * Clears all gates in the circuit.
     */
    public void clear() {
        gates.clear();
    }

    /**
     * Returns the number of gates in the circuit.
     */
    public int size() {
        return gates.size();
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder("ComplexQuantumCircuit:\n");
        for (int i = 0; i < gates.size(); i++) {
            sb.append("Gate ").append(i).append(": ").append(gates.get(i)).append("\n");
        }
        return sb.toString();
    }
}
