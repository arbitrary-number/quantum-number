package com.github.quantum_numer.qubit.test;

import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.Test;
import org.opentest4j.AssertionFailedError;

import com.github.quantum_number.qubit.ComplexQuantumNumber;
import com.github.quantum_number.qubit.ComplexQuantumNumberQFTStep2;
import com.github.quantum_number.qubit.ComplexQuantumRegister;
import com.github.quantum_number.qubit.ModularExponentiation;
import com.github.quantum_number.qubit.QuantumMeasurement;
import com.github.quantum_number.qubit.ShorStep3;

public class ShorFactorizationTest {

//	@Test
//    public void testFactorizationOf15() {
//	        int N = 15;               // Number to factor
//	        int a = 7;                // Chosen base coprime to N (7 and 15 are coprime)
//	        int nInputQubits = 4;     // Number of qubits for input register (x)
//	        int nOutputQubits = 4;    // Number of qubits for output register (y)
//
//	        // 1. Prepare initial superposition |x, 1>
//	        System.out.println("Step 1: Initializing quantum register to superposition |x⟩|1⟩...");
//	        int totalQubits = nInputQubits + nOutputQubits;
//	        int registerSize = 1 << totalQubits;
//	        ComplexQuantumRegister register = ComplexQuantumRegister.ofDimension(registerSize);
//
//	        double amplitude = 1.0d / ((double)Math.sqrt((1 << nInputQubits)));
//	        for (int x = 0; x < (1 << nInputQubits); x++) {
//	            int index = x * (1 << nOutputQubits) + 1; // y=1
//	            register.setAmplitude(index, ComplexQuantumNumber.fromDouble(amplitude));
//	        }
//
//	        register.normalize();
//	        System.out.println("Initial state prepared. Non-zero amplitudes:");
//	        for (int i = 0; i < register.size(); i++) {
//	            if (!register.getAmplitude(i).isZero()) {
//	                System.out.println("Index " + i + ": " + register.getAmplitude(i));
//	            }
//	        }
//
//	        // 2. Apply Modular Exponentiation
//	        System.out.println("\nStep 2: Applying modular exponentiation (a^x mod N)...");
//	        ModularExponentiation modExp = new ModularExponentiation(a, N, nInputQubits, nOutputQubits);
//	        ComplexQuantumRegister afterModExp = modExp.applyV2(register);
//
//	        System.out.println("After modular exponentiation. Non-zero amplitudes:");
//	        for (int i = 0; i < afterModExp.size(); i++) {
//	            System.out.println("amplitude " + i + ":" +
//	            		afterModExp.getAmplitude(i));
//	        }
//
//
//        // 3. Apply Quantum Fourier Transform on input register only (Step 2)
//        ComplexQuantumRegister afterQFT = ComplexQuantumNumberQFTStep2.applyQFT(afterModExp);
//
//        // 4. Simulate measurement of the input register (first nInputQubits)
//        int measuredIndex = QuantumMeasurement.measureFirstNQubits(
//        		afterQFT, nInputQubits);
//        System.out.println("Measured index (from first register): " + measuredIndex);
//
//        // 5. Estimate period r from measured value
//        int maxDenominator = 20;
//        System.out.println("nInputQubits: " + nInputQubits);
//        System.out.println("maxDenominator: " + maxDenominator);
//        System.out.println("Measured index (from first register): " +
//        		measuredIndex);
//        int estimatedPeriod = ShorStep3.estimatePeriod(measuredIndex,
//        		nInputQubits, maxDenominator);
//        System.out.println("Estimated period r: " + estimatedPeriod);
//
//        // 6. Attempt to find non-trivial factors from period r
//        assertTrue(estimatedPeriod > 1, "Estimated period should be > 1");
//
//        // Use gcd and factorization formulas:
//        int r = estimatedPeriod;
//        if (r % 2 == 1) {
//            // Period must be even for factoring
//            System.out.println("Period is odd; test inconclusive.");
//            return;
//        }
//
//        int candidate1 = gcd((int) Math.pow(a, r / 2) - 1, N);
//        int candidate2 = gcd((int) Math.pow(a, r / 2) + 1, N);
//
//        System.out.println("Possible factors: " + candidate1 + ", " + candidate2);
//
//        assertTrue(candidate1 > 1 && candidate1 < N, "Found a non-trivial factor");
//        assertTrue(candidate2 > 1 && candidate2 < N, "Found a non-trivial factor");
//    }

	@Test
    public void testFactorizationOf15() {
	        int N = 15;
	        int[] candidates = null;
	        int tries = 1000;
	        int j = 0;
	        outer:
	        for (j = 0;j<tries;j++) {
	        	inner:
		        for (int i=0;i<2;i++) {
		        	boolean success = candidates == null ? false :
		        		candidates[i] > 1 && candidates[i] < N;
		        	if (!success) {
		        		try {
		        			candidates = findFactors(N);
		        		} catch (AssertionFailedError e) {
		        			// Shor is quantum and fails x % of the time
		        			continue outer;
		        		}
	        			// Shor is quantum and fails x % of the time
		        		continue outer;
		        	}
		        }
	        	break outer;
	        }
	        int candidate1 = candidates[0];
	        int candidate2 = candidates[1];

        System.out.println("Possible factors: " + candidate1 + ", " + candidate2);

        assertTrue(candidate1 > 1 && candidate1 < N, "Found a non-trivial factor");
        assertTrue(candidate2 > 1 && candidate2 < N, "Found a non-trivial factor");

        System.out.println("Succeeded after " + j + " tries");
    }

	public int[] findFactors(int N) {
		//int a = 7;                // Chosen base coprime to N (7 and 15 are coprime)

		int a = 2 + (int)(Math.random() * (N-2));
		while (gcd(a, N) != 1) {
		    a = 2 + (int)(Math.random() * (N-2));
		}

		int nInputQubits = 4;     // Number of qubits for input register (x)
        int nOutputQubits = 4;    // Number of qubits for output register (y)

        // 1. Prepare initial superposition |x, 1>
        System.out.println("Step 1: Initializing quantum register to superposition |x⟩|1⟩...");
        int totalQubits = nInputQubits + nOutputQubits;
        int registerSize = 1 << totalQubits;
        ComplexQuantumRegister register = ComplexQuantumRegister.ofDimension(registerSize);

        double amplitude = 1.0d / ((double)Math.sqrt((1 << nInputQubits)));
        for (int x = 0; x < (1 << nInputQubits); x++) {
            int index = x * (1 << nOutputQubits) + 1; // y=1
            register.setAmplitude(index, ComplexQuantumNumber.fromDouble(amplitude));
        }

        register.normalize();
        System.out.println("Initial state prepared. Non-zero amplitudes:");
        for (int i = 0; i < register.size(); i++) {
            if (!register.getAmplitude(i).isZero()) {
                System.out.println("Index " + i + ": " + register.getAmplitude(i));
            }
        }

        // 2. Apply Modular Exponentiation
        System.out.println("\nStep 2: Applying modular exponentiation (a^x mod N)...");
        ModularExponentiation modExp = new ModularExponentiation(a, N, nInputQubits, nOutputQubits);
        ComplexQuantumRegister afterModExp = modExp.applyV2(register);

        System.out.println("After modular exponentiation. Non-zero amplitudes:");
        for (int i = 0; i < afterModExp.size(); i++) {
            System.out.println("amplitude " + i + ":" +
            		afterModExp.getAmplitude(i));
        }


    // 3. Apply Quantum Fourier Transform on input register only (Step 2)
    ComplexQuantumRegister afterQFT = ComplexQuantumNumberQFTStep2.applyQFT(afterModExp);

    // 4. Simulate measurement of the input register (first nInputQubits)
    int measuredIndex = QuantumMeasurement.measureFirstNQubits(
    		afterQFT, nInputQubits);
    System.out.println("Measured index (from first register): " + measuredIndex);

    // 5. Estimate period r from measured value
    int maxDenominator = 20;
    System.out.println("nInputQubits: " + nInputQubits);
    System.out.println("maxDenominator: " + maxDenominator);
    System.out.println("Measured index (from first register): " +
    		measuredIndex);
    int estimatedPeriod = ShorStep3.estimatePeriod(measuredIndex,
    		nInputQubits, maxDenominator);
    System.out.println("Estimated period r: " + estimatedPeriod);

    // 6. Attempt to find non-trivial factors from period r
    assertTrue(estimatedPeriod > 1, "Estimated period should be > 1");

    // Use gcd and factorization formulas:
    int r = estimatedPeriod;
    if (r % 2 == 1) {
        // Period must be even for factoring
        System.out.println("Period is odd; test inconclusive.");
        return null;
    }

    int candidate1 = gcd((int) Math.pow(a, r / 2) - 1, N);
    int candidate2 = gcd((int) Math.pow(a, r / 2) + 1, N);

    System.out.println("Possible factors: " + candidate1 + ", " + candidate2);
    return new int[] {candidate1, candidate2};
	}

//    @Test
//    public void testFactorizationOf15() {
//        int N = 15;               // Number to factor
//        int a = 7;                // Chosen base coprime to N (7 and 15 are coprime)
//        int nInputQubits = 4;     // Number of qubits for input register (x)
//        int nOutputQubits = 4;    // Number of qubits for output register (y)
//
//        // 1. Prepare initial superposition |x, 1>
//        int totalQubits = nInputQubits + nOutputQubits;
//        int registerSize = 1 << totalQubits;
//        ComplexQuantumRegister register = ComplexQuantumRegister.ofDimension(registerSize);
//
//        // Initialize amplitudes to uniform superposition over x, with y=1 (index y=1)
//        double amplitude = 1.0 / Math.sqrt(1 << nInputQubits);
//        for (int x = 0; x < (1 << nInputQubits); x++) {
//            int index = x * (1 << nOutputQubits) + 1; // y=1
//            register.setAmplitude(index, ComplexQuantumNumber.fromDouble(amplitude));
//        }
//
//        register.normalize();
//
//        // 2. Apply Modular Exponentiation (|x, y=1> → |x, y * a^x mod N>)
//        ModularExponentiation modExp = new ModularExponentiation(a, N, nInputQubits, nOutputQubits);
//        ComplexQuantumRegister afterModExp = modExp.apply(register);
//
//        // 3. Apply Quantum Fourier Transform on input register only (Step 2)
//        ComplexQuantumRegister afterQFT = ComplexQuantumNumberQFTStep2.applyQFT(afterModExp);
//
//        // 4. Simulate measurement of the input register (first nInputQubits)
//        int measuredIndex = QuantumMeasurement.measureFirstNQubits(
//        		afterQFT, nInputQubits);
//        System.out.println("Measured index (from first register): " + measuredIndex);
//
//        // 5. Estimate period r from measured value
//        int maxDenominator = 20;
//        int estimatedPeriod = ShorStep3.estimatePeriod(measuredIndex, nInputQubits, maxDenominator);
//        System.out.println("Estimated period r: " + estimatedPeriod);
//
//        // 6. Attempt to find non-trivial factors from period r
//        assertTrue(estimatedPeriod > 1, "Estimated period should be > 1");
//
//        // Use gcd and factorization formulas:
//        int r = estimatedPeriod;
//        if (r % 2 == 1) {
//            // Period must be even for factoring
//            System.out.println("Period is odd; test inconclusive.");
//            return;
//        }
//
//        int candidate1 = gcd((int) Math.pow(a, r / 2) - 1, N);
//        int candidate2 = gcd((int) Math.pow(a, r / 2) + 1, N);
//
//        System.out.println("Possible factors: " + candidate1 + ", " + candidate2);
//
//        assertTrue(candidate1 > 1 && candidate1 < N, "Found a non-trivial factor");
//        assertTrue(candidate2 > 1 && candidate2 < N, "Found a non-trivial factor");
//    }

    private int gcd(int a, int b) {
        if (b == 0) return a;
        return gcd(b, a % b);
    }
}
