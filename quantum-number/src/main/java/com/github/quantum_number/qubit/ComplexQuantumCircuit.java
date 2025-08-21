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

    /**
     * Insert a gate at the specified index.
     * @param index position to insert the gate
     * @param gate gate to insert
     */
    public void insertGate(int index, ComplexQuantumGate gate) {
        if (gate == null) {
            throw new IllegalArgumentException("Gate cannot be null");
        }
        if (index < 0 || index > gates.size()) {
            throw new IndexOutOfBoundsException("Index out of bounds: " + index);
        }
        gates.add(index, gate);
    }

    /**
     * Remove the gate at the specified index.
     * @param index index of gate to remove
     * @return the removed gate
     */
    public ComplexQuantumGate removeGate(int index) {
        if (index < 0 || index >= gates.size()) {
            throw new IndexOutOfBoundsException("Index out of bounds: " + index);
        }
        return gates.remove(index);
    }

    /**
     * Get the gate at the specified index.
     * @param index index of gate
     * @return gate at the index
     */
    public ComplexQuantumGate getGate(int index) {
        if (index < 0 || index >= gates.size()) {
            throw new IndexOutOfBoundsException("Index out of bounds: " + index);
        }
        return gates.get(index);
    }


    /**
     * Applies all gates in sequence to the input register,
     * returning an array of intermediate registers after each gate.
     * @param input Initial quantum register state
     * @return List of intermediate quantum registers after each gate
     */
    public List<ComplexQuantumRegister> runStepwise(ComplexQuantumRegister input) {
        List<ComplexQuantumRegister> states = new ArrayList<>();
        ComplexQuantumRegister current = input;
        for (ComplexQuantumGate gate : gates) {
            current = gate.apply(current);
            states.add(current);
        }
        return states;
    }


//    @Override
//    public String toString() {
//        StringBuilder sb = new StringBuilder("ComplexQuantumCircuit with ")
//            .append(gates.size())
//            .append(" gates:\n");
//        for (int i = 0; i < gates.size(); i++) {
//            sb.append("Gate ").append(i).append(":\n");
//            sb.append(gates.get(i).toString()).append("\n");
//        }
//        return sb.toString();
//    }
}
