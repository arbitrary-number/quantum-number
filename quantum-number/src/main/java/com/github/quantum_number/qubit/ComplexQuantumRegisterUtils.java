package com.github.quantum_number.qubit;
import java.util.HashMap;
import java.util.Map;

public class ComplexQuantumRegisterUtils {

    /**
     * Extracts the Quantum Number qubit state from a ComplexQuantumRegister.
     * For the targetQubit, returns a map:
     *     0 → amplitude components where that qubit is |0⟩
     *     1 → amplitude components where that qubit is |1⟩
     *
     * Symbolic components (a–l) are preserved.
     */
    public static Map<Integer, ComplexQuantumNumber> extractQuantumNumberQubitState(
            ComplexQuantumRegister register, int targetQubit, int nQubits) {

        Map<Integer, ComplexQuantumNumber> stateMap = new HashMap<>();
        stateMap.put(0, new ComplexQuantumNumber());
        stateMap.put(1, new ComplexQuantumNumber());

        for (int index = 0; index < register.size(); index++) {
            // Read target qubit bit from basis index
            int bit = (index >> (nQubits - 1 - targetQubit)) & 1;

            // Get amplitude for basis index (could include symbolic components like a, b, etc.)
            ComplexQuantumNumber amplitude = register.getAmplitude(index);

            // Accumulate (symbolic components are preserved)
            stateMap.get(bit).addComponents(amplitude);
        }

        return stateMap;
    }
}
