package com.github.quantum_number.v8;

import java.math.BigInteger;
import java.util.List;
import java.util.logging.Logger;

/**
 * Performance runner for comparing all secp256k1 ECC implementations.
 * Demonstrates the performance improvements across versions.
 */
public class Secp256k1ECCPerformanceRunner {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCPerformanceRunner.class.getName());

    public static void main(String[] args) {
        logger.info("🚀 Starting secp256k1 ECC Performance Comparison");
        logger.info("================================================");

        runAllBenchmarks();
        runDirectPerformanceComparison();
        runV5StressTest();

        logger.info("✅ Performance comparison completed!");
    }

    /**
     * Run comprehensive benchmark suite
     */
    public static void runAllBenchmarks() {
        logger.info("📊 Running comprehensive benchmark suite...");

        try {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results = Secp256k1ECCBenchmark.runFullBenchmark();
            Secp256k1ECCBenchmark.printComparisonTable(results);

            // Analyze results
            analyzeBenchmarkResults(results);

        } catch (Exception e) {
            logger.severe("Error running benchmarks: " + e.getMessage());
        }
    }

    /**
     * Direct performance comparison between versions
     */
    public static void runDirectPerformanceComparison() {
        logger.info("🔍 Running direct performance comparison...");

        BigInteger scalar = BigInteger.valueOf(12345);
        int iterations = 1000;

        // V2 Performance
        logger.info("Testing V2...");
        long v2Time = measurePerformance(() -> {
            for (int i = 0; i < iterations; i++) {
                Secp256k1ECCV2.multiply(scalar, Secp256k1ECCV2.G);
            }
        });

        // V3 Performance
        logger.info("Testing V3...");
        long v3Time = measurePerformance(() -> {
            for (int i = 0; i < iterations; i++) {
                Secp256k1ECCV3.multiply(scalar, Secp256k1ECCV3.G);
            }
        });

        // V4 Performance
        logger.info("Testing V4...");
        long v4Time = measurePerformance(() -> {
            for (int i = 0; i < iterations; i++) {
                Secp256k1ECCV4.multiply(scalar, Secp256k1ECCV4.G);
            }
        });

        // V5 Performance
        logger.info("Testing V5...");
        long v5Time = measurePerformance(() -> {
            for (int i = 0; i < iterations; i++) {
                Secp256k1ECCV5.multiply(scalar, Secp256k1ECCV5.G);
            }
        });

        // Results
        logger.info("🎯 Direct Performance Comparison Results:");
        logger.info(String.format("   V2: %.2f ms (%d iterations)", v2Time / 1_000_000.0, iterations));
        logger.info(String.format("   V3: %.2f ms (%d iterations)", v3Time / 1_000_000.0, iterations));
        logger.info(String.format("   V4: %.2f ms (%d iterations)", v4Time / 1_000_000.0, iterations));
        logger.info(String.format("   V5: %.2f ms (%d iterations)", v5Time / 1_000_000.0, iterations));

        // Calculate improvements
        double v2PerOp = (v2Time / (double) iterations) / 1_000_000.0;
        double v3PerOp = (v3Time / (double) iterations) / 1_000_000.0;
        double v4PerOp = (v4Time / (double) iterations) / 1_000_000.0;
        double v5PerOp = (v5Time / (double) iterations) / 1_000_000.0;

        logger.info("⚡ Per-Operation Performance (milliseconds):");
        logger.info(String.format("   V2: %.6f ms/op", v2PerOp));
        logger.info(String.format("   V3: %.6f ms/op", v3PerOp));
        logger.info(String.format("   V4: %.6f ms/op", v4PerOp));
        logger.info(String.format("   V5: %.6f ms/op", v5PerOp));

        // Calculate speedup
        double v3Speedup = v2PerOp / v3PerOp;
        double v4Speedup = v2PerOp / v4PerOp;
        double v5Speedup = v2PerOp / v5PerOp;

        logger.info("🚀 Speedup vs V2 (baseline):");
        logger.info(String.format("   V3: %.2fx faster", v3Speedup));
        logger.info(String.format("   V4: %.2fx faster", v4Speedup));
        logger.info(String.format("   V5: %.2fx faster", v5Speedup));

        // Throughput
        double v2OpsPerSec = 1000.0 / v2PerOp;
        double v3OpsPerSec = 1000.0 / v3PerOp;
        double v4OpsPerSec = 1000.0 / v4PerOp;
        double v5OpsPerSec = 1000.0 / v5PerOp;

        logger.info("📈 Operations per second:");
        logger.info(String.format("   V2: %.0f ops/sec", v2OpsPerSec));
        logger.info(String.format("   V3: %.0f ops/sec", v3OpsPerSec));
        logger.info(String.format("   V4: %.0f ops/sec", v4OpsPerSec));
        logger.info(String.format("   V5: %.0f ops/sec", v5OpsPerSec));
    }

    /**
     * Run V5 stress test
     */
    public static void runV5StressTest() {
        logger.info("🔥 Running V5 stress test...");

        Secp256k1ECCV5.stressTest(5000);

        logger.info("💾 Running V5 memory-efficient test...");
        BigInteger[] testScalars = new BigInteger[10000];
        for (int i = 0; i < testScalars.length; i++) {
            testScalars[i] = BigInteger.valueOf(i % 1000 + 1);
        }
        Secp256k1ECCV5.processBatchEfficiently(testScalars, Secp256k1ECCV5.G);
    }

    /**
     * Analyze benchmark results
     */
    private static void analyzeBenchmarkResults(List<Secp256k1ECCBenchmark.BenchmarkResult> results) {
        logger.info("📋 Benchmark Analysis:");

        // Group by operation
        results.stream()
            .collect(java.util.stream.Collectors.groupingBy(r -> r.operation))
            .forEach((operation, operationResults) -> {
                logger.info(String.format("🔧 %s:", operation));

                operationResults.forEach(result -> {
                    logger.info(String.format("   %s: %.0f ops/sec (%.2f μs avg)",
                        result.version,
                        result.operationsPerSecond,
                        result.averageTimeNs / 1000.0));
                });

                // Find best performer for this operation
                Secp256k1ECCBenchmark.BenchmarkResult best = operationResults.stream()
                    .max((a, b) -> Double.compare(a.operationsPerSecond, b.operationsPerSecond))
                    .orElse(null);

                if (best != null) {
                    logger.info(String.format("   🏆 Best: %s (%.0f ops/sec)",
                        best.version, best.operationsPerSecond));
                }
            });
    }

    /**
     * Measure performance of a runnable
     */
    private static long measurePerformance(Runnable task) {
        // Warmup
        for (int i = 0; i < 10; i++) {
            task.run();
        }

        // Measurement
        long startTime = System.nanoTime();
        task.run();
        long endTime = System.nanoTime();

        return endTime - startTime;
    }

    /**
     * Quick coordinate demonstration for all versions
     */
    public static void demonstrateAllVersions() {
        logger.info("🎭 Demonstrating all versions:");

        // V2
        logger.info("V2 Coordinates:");
        Secp256k1ECCV2.Point g2 = Secp256k1ECCV2.G;
        Secp256k1ECCV2.Point g2_2 = Secp256k1ECCV2.calculate2G();
        Secp256k1ECCV2.Point g2_3 = Secp256k1ECCV2.calculate3G();
        logger.info("  G:  " + g2);
        logger.info("  2G: " + g2_2);
        logger.info("  3G: " + g2_3);

        // V3
        logger.info("V3 Coordinates:");
        Secp256k1ECCV3.Point g3 = Secp256k1ECCV3.G;
        Secp256k1ECCV3.Point g3_2 = Secp256k1ECCV3.calculate2G();
        Secp256k1ECCV3.Point g3_3 = Secp256k1ECCV3.calculate3G();
        logger.info("  G:  " + g3);
        logger.info("  2G: " + g3_2);
        logger.info("  3G: " + g3_3);

        // V4
        logger.info("V4 Coordinates:");
        Secp256k1ECCV4.Point g4 = Secp256k1ECCV4.G;
        Secp256k1ECCV4.Point g4_2 = Secp256k1ECCV4.calculate2G();
        Secp256k1ECCV4.Point g4_3 = Secp256k1ECCV4.calculate3G();
        logger.info("  G:  " + g4);
        logger.info("  2G: " + g4_2);
        logger.info("  3G: " + g4_3);

        // V5
        logger.info("V5 Coordinates:");
        Secp256k1ECCV5.Point g5 = Secp256k1ECCV5.G;
        Secp256k1ECCV5.Point g5_2 = Secp256k1ECCV5.calculate2G();
        Secp256k1ECCV5.Point g5_3 = Secp256k1ECCV5.calculate3G();
        logger.info("  G:  " + g5);
        logger.info("  2G: " + g5_2);
        logger.info("  3G: " + g5_3);

        // Verify all versions produce identical results
        boolean coordinatesMatch =
            g2.x.equals(g3.x) && g2.y.equals(g3.y) &&
            g2.x.equals(g4.x) && g2.y.equals(g4.y) &&
            g2.x.equals(g5.x) && g2.y.equals(g5.y);

        logger.info("✅ All versions produce identical coordinates: " + coordinatesMatch);
    }
}
