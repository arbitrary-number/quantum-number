package com.github.quantum_number.v8;

import java.math.BigInteger;

/**
 * Simple performance demonstration for secp256k1 ECC V5
 */
public class Secp256k1ECCPerformanceDemo {

    public static void main(String[] args) {
        System.out.println("=== Secp256k1 ECC V5 Performance Demo ===");

        // Test coordinate calculation
        Secp256k1ECCV5.Point g = Secp256k1ECCV5.G;
        Secp256k1ECCV5.Point g2 = Secp256k1ECCV5.calculate2G();
        Secp256k1ECCV5.Point g3 = Secp256k1ECCV5.calculate3G();

        System.out.println("G:  " + g);
        System.out.println("2G: " + g2);
        System.out.println("3G: " + g3);

        // Performance test
        System.out.println("\n=== Performance Test ===");
        long startTime = System.nanoTime();

        int iterations = 10000;
        for (int i = 0; i < iterations; i++) {
            BigInteger scalar = BigInteger.valueOf(i % 100 + 1);
            Secp256k1ECCV5.Point result = Secp256k1ECCV5.multiply(scalar, Secp256k1ECCV5.G);
        }

        long endTime = System.nanoTime();
        double totalTimeMs = (endTime - startTime) / 1_000_000.0;
        double avgTimePerOp = totalTimeMs / iterations;

        System.out.println("Iterations: " + iterations);
        System.out.println("Total time: " + String.format("%.2f ms", totalTimeMs));
        System.out.println("Average per operation: " + String.format("%.4f μs", avgTimePerOp * 1000));
        System.out.println("Operations per second: " + String.format("%.0f", 1000.0 / avgTimePerOp));

        System.out.println("\n=== Demo completed successfully! ===");
    }
}
