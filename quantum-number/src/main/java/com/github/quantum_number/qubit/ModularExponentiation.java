package com.github.quantum_number.qubit;

import java.math.BigInteger;

/**
 * Simulates modular exponentiation in Shor's algorithm:
 * Maps |x, y⟩ → |x, y * a^x mod N⟩
 *
 * Here, we assume the second register initially holds |1⟩,
 * so it becomes |a^x mod N⟩ after the operation.
 */
public class ModularExponentiation {

    private final int a;  // base
    private final int N;  // modulus
    private final int nInputQubits;
    private final int nOutputQubits;
    private final int totalQubits;

    public ModularExponentiation(int a, int N, int nInputQubits, int nOutputQubits) {
        this.a = a;
        this.N = N;
        this.nInputQubits = nInputQubits;
        this.nOutputQubits = nOutputQubits;
        this.totalQubits = nInputQubits + nOutputQubits;
    }

    /**
     * Applies modular exponentiation on the input quantum register.
     *
     * @param register Quantum register |x, y⟩ where y is initially 1 for all x.
     * @return New quantum register with modular exponentiation applied.
     */
//    public ComplexQuantumRegister apply(ComplexQuantumRegister register) {
//        int inputSize = 1 << nInputQubits;
//        int outputSize = 1 << nOutputQubits;
//
//        ComplexQuantumRegister result = ComplexQuantumRegister.ofDimension(register.size());
//        ComplexQuantumRegister.clearRegister(register);
//
//        for (int x = 0; x < inputSize; x++) {
//            // Compute a^x mod N
//            int modExp = BigInteger.valueOf(a).modPow(BigInteger.valueOf(x), BigInteger.valueOf(N)).intValue();
//
//            // For y initially = 1, we place amplitude at |x, modExp⟩
//            int originalIndex = x * outputSize;  // y=0 (assuming initial y=1 is |0⟩? If y=1 is |1⟩ adjust accordingly)
//            ComplexQuantumNumber amplitude = register.getAmplitude(originalIndex);
//            if (amplitude != null && !amplitude.isZero()) {
//                int targetIndex = x * outputSize + modExp;
//                result.setAmplitude(targetIndex, amplitude);
//            }
//        }
//
//        result.normalize();
//        return result;
//    }
    /**
     * Improved modular exponentiation:
     * Assumes initial state is |x, y=1⟩.
     */
    public ComplexQuantumRegister applyV2(ComplexQuantumRegister register) {
        int inputSize = 1 << nInputQubits;
        int outputSize = 1 << nOutputQubits;
        int yInit = 1;

        System.out.println("\n[ModularExponentiation.applyV2] Starting modular exponentiation");
        System.out.println(" - a: " + a + ", N: " + N);
        System.out.println(" - Input qubits: " + nInputQubits + ", Output qubits: " + nOutputQubits);
        System.out.println(" - Input size: " + inputSize + ", Output size: " + outputSize);
        System.out.println(" - Total register size: " + register.size());

        ComplexQuantumRegister result = ComplexQuantumRegister.ofDimension(register.size());
        ComplexQuantumRegister.clearRegister(result);
        System.out.println(" - Cleared result register to zero");

        int processed = 0;

        for (int x = 0; x < inputSize; x++) {
            int modExp = BigInteger.valueOf(a).modPow(BigInteger.valueOf(x), BigInteger.valueOf(N)).intValue();
            int originalIndex = x * outputSize + yInit;
            ComplexQuantumNumber amplitude = register.getAmplitude(originalIndex);

            System.out.printf("   [x=%d] a^x mod N = %d | originalIndex = %d | amplitude = %s\n",
                    x, modExp, originalIndex, amplitude);

            if (amplitude != null && !amplitude.isZero()) {
                int targetIndex = x * outputSize + modExp;
                result.setAmplitude(targetIndex, amplitude);
                System.out.printf("   --> Setting result[%d] = %s\n", targetIndex, amplitude);
                processed++;
            }
        }

        System.out.println(" - Processed non-zero amplitudes: " + processed);
        if (processed == 0) {
            System.out.println(" ⚠️ Warning: No amplitudes were transferred. Check yInit or input state.");
        }

        System.out.println("\n - Result register amplitudes before normalization:");
        for (int i = 0; i < result.size(); i++) {
            ComplexQuantumNumber amp = result.getAmplitude(i);
            if (amp != null && !amp.isZero()) {
                System.out.println("   Index " + i + ": " + amp);
            }
        }

        result.normalize();

        System.out.println(" - Normalized result register.");
        return result;
    }

    public ComplexQuantumRegister apply(ComplexQuantumRegister register) {
        int inputSize = 1 << nInputQubits;
        int outputSize = 1 << nOutputQubits;

        System.out.println("ModularExponentiation.apply called");
        System.out.println("Input register size: " + register.size());
        System.out.println("Input qubits: " + nInputQubits + ", Output qubits: " + nOutputQubits);
        System.out.println("Total qubits: " + totalQubits);
        System.out.println("Input size (2^nInputQubits): " + inputSize);
        System.out.println("Output size (2^nOutputQubits): " + outputSize);

        ComplexQuantumRegister result = ComplexQuantumRegister.ofDimension(register.size());
        ComplexQuantumRegister.clearRegister(result);
        System.out.println("Initialized result register with zero amplitudes");

        for (int x = 0; x < inputSize; x++) {
            // Compute a^x mod N
            int modExp = BigInteger.valueOf(a).modPow(BigInteger.valueOf(x), BigInteger.valueOf(N)).intValue();
            System.out.println("x = " + x + ", a^x mod N = " + modExp);

            int originalIndex = x * outputSize;  // y=0 index (assuming initial y=1 is |0⟩)
            ComplexQuantumNumber amplitude = register.getAmplitude(originalIndex);
            System.out.println("Original index: " + originalIndex + ", amplitude: " + amplitude);

            if (amplitude != null && !amplitude.isZero()) {
                int targetIndex = x * outputSize + modExp;
                System.out.println("Setting amplitude at target index: " + targetIndex + " with amplitude: " + amplitude);
                result.setAmplitude(targetIndex, amplitude);
            } else {
                System.out.println("Amplitude at original index is zero or null, skipping");
            }
        }

        System.out.println("Before normalization, result register amplitudes:");
        for (int i = 0; i < result.size(); i++) {
            System.out.println("Index " + i + ": " + result.getAmplitude(i));
        }

        result.normalize();

        System.out.println("After normalization, result register amplitudes:");
        for (int i = 0; i < result.size(); i++) {
            System.out.println("Index " + i + ": " + result.getAmplitude(i));
        }

        return result;
    }

}
