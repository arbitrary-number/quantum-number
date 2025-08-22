package com.github.quantum_number.qubit.measurement;

import com.github.quantum_number.qubit.ComplexQuantumRegister;

public class ComplexMeasurementResult {

    private final ComplexQuantumRegister collapsedRegister;
    private final int measurementOutcome;

    public ComplexMeasurementResult(ComplexQuantumRegister collapsedRegister, int measurementOutcome) {
        this.collapsedRegister = collapsedRegister;
        this.measurementOutcome = measurementOutcome;
    }

    /**
     * Returns the quantum register after measurement collapse.
     */
    public ComplexQuantumRegister getCollapsedRegister() {
        return collapsedRegister;
    }

    /**
     * Returns the classical measurement outcome bits.
     *
     * This integer encodes the measured bits on the qubits (e.g., for 2 qubits: 0..3).
     */
    public int getMeasurementOutcome() {
        return measurementOutcome;
    }
}
