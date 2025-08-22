package com.github.quantum_number.qubit;

public class ComplexQuantumController {

    /**
     * Applies a gate only if a given classical bit condition is satisfied.
     * @param reg Quantum register
     * @param measuredBit The classical bit (from measurement)
     * @param expectedValue The value to match (0 or 1)
     * @param gate The quantum gate to apply
     * @return Modified register (new instance)
     */
    public static ComplexQuantumRegister applyConditionalGate(
        ComplexQuantumRegister reg,
        int measuredBit,
        int expectedValue,
        ComplexQuantumGate gate
    ) {
        if (measuredBit == expectedValue) {
            return gate.apply(reg);
        }
        return reg;
    }
}
