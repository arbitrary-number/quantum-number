package com.github.quantum_number.v8;

import java.math.BigInteger;
import java.util.logging.Logger;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

/**
 * Performance benchmark suite for comparing secp256k1 ECC implementations across versions.
 * Measures execution times, memory usage, and scalability of different approaches.
 */
public class Secp256k1ECCBenchmark {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCBenchmark.class.getName());

    // Benchmark configuration
    private static final int WARMUP_ITERATIONS = 100;
    private static final int MEASUREMENT_ITERATIONS = 1000;
    private static final int BATCH_SIZE = 100;

    /**
     * Benchmark result data structure
     */
    public static class BenchmarkResult {
        public final String operation;
        public final String version;
        public final long averageTimeNs;
        public final long minTimeNs;
        public final long maxTimeNs;
        public final double operationsPerSecond;
        public final long memoryUsageBytes;

        public BenchmarkResult(String operation, String version, long averageTimeNs,
                             long minTimeNs, long maxTimeNs, double operationsPerSecond,
                             long memoryUsageBytes) {
            this.operation = operation;
            this.version = version;
            this.averageTimeNs = averageTimeNs;
            this.minTimeNs = minTimeNs;
            this.maxTimeNs = maxTimeNs;
            this.operationsPerSecond = operationsPerSecond;
            this.memoryUsageBytes = memoryUsageBytes;
        }

        @Override
        public String toString() {
            return String.format("%s (%s): %.2f ops/sec, avg=%.2f μs, mem=%d KB",
                operation, version, operationsPerSecond,
                averageTimeNs / 1000.0, memoryUsageBytes / 1024);
        }
    }

    /**
     * Run comprehensive benchmark suite
     */
    public static List<BenchmarkResult> runFullBenchmark() {
        List<BenchmarkResult> results = new ArrayList<>();

        logger.info("=== 🚀 Starting secp256k1 ECC Benchmark Suite ===");

        // Scalar multiplication benchmarks
        results.addAll(runScalarMultiplicationBenchmarks());

        // Point addition benchmarks
        results.addAll(runPointAdditionBenchmarks());

        // Batch operation benchmarks
        results.addAll(runBatchOperationBenchmarks());

        // Coordinate calculation benchmarks
        results.addAll(runCoordinateCalculationBenchmarks());

        // Memory usage benchmarks
        results.addAll(runMemoryUsageBenchmarks());

        logger.info("=== 📊 Benchmark Results Summary ===");
        for (BenchmarkResult result : results) {
            logger.info(result.toString());
        }

        return results;
    }

    /**
     * Benchmark scalar multiplication across versions
     */
    private static List<BenchmarkResult> runScalarMultiplicationBenchmarks() {
        List<BenchmarkResult> results = new ArrayList<>();
        BigInteger[] testScalars = {
            BigInteger.ONE,
            BigInteger.valueOf(12345),
            new BigInteger("12345678901234567890"),
            new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140", 16)
        };

        logger.info("🔢 Benchmarking Scalar Multiplication...");

        for (BigInteger scalar : testScalars) {
            // V2 Benchmark
            BenchmarkResult v2Result = benchmarkScalarMultiplication(
                "Scalar Multiplication", "V2", scalar, Secp256k1ECCV2.G);
            results.add(v2Result);

            // V3 Benchmark
            BenchmarkResult v3Result = benchmarkScalarMultiplication(
                "Scalar Multiplication", "V3", scalar, Secp256k1ECCV3.G);
            results.add(v3Result);

            // V4 Benchmark
            BenchmarkResult v4Result = benchmarkScalarMultiplication(
                "Scalar Multiplication", "V4", scalar, Secp256k1ECCV4.G);
            results.add(v4Result);
        }

        return results;
    }

    /**
     * Benchmark point addition across versions
     */
    private static List<BenchmarkResult> runPointAdditionBenchmarks() {
        List<BenchmarkResult> results = new ArrayList<>();

        logger.info("➕ Benchmarking Point Addition...");

        // V2 Benchmark
        BenchmarkResult v2Result = benchmarkPointAddition("Point Addition", "V2",
            Secp256k1ECCV2.G, Secp256k1ECCV2.calculate2G());
        results.add(v2Result);

        // V3 Benchmark
        BenchmarkResult v3Result = benchmarkPointAddition("Point Addition", "V3",
            Secp256k1ECCV3.G, Secp256k1ECCV3.calculate2G());
        results.add(v3Result);

        // V4 Benchmark
        BenchmarkResult v4Result = benchmarkPointAddition("Point Addition", "V4",
            Secp256k1ECCV4.G, Secp256k1ECCV4.calculate2G());
        results.add(v4Result);

        return results;
    }

    /**
     * Benchmark batch operations (V3 and V4 only)
     */
    private static List<BenchmarkResult> runBatchOperationBenchmarks() {
        List<BenchmarkResult> results = new ArrayList<>();

        logger.info("📦 Benchmarking Batch Operations...");

        BigInteger[] scalars = new BigInteger[BATCH_SIZE];
        Secp256k1ECCV3.Point[] pointsV3 = new Secp256k1ECCV3.Point[BATCH_SIZE];
        Secp256k1ECCV4.Point[] pointsV4 = new Secp256k1ECCV4.Point[BATCH_SIZE];

        // Initialize test data
        for (int i = 0; i < BATCH_SIZE; i++) {
            scalars[i] = BigInteger.valueOf(i + 1);
            pointsV3[i] = Secp256k1ECCV3.G;
            pointsV4[i] = Secp256k1ECCV4.G;
        }

        // V3 Batch Benchmark
        BenchmarkResult v3BatchResult = benchmarkBatchMultiplication(
            "Batch Multiplication", "V3", scalars, pointsV3);
        results.add(v3BatchResult);

        // V4 Secure Batch Benchmark
        BenchmarkResult v4BatchResult = benchmarkSecureBatchMultiplication(
            "Secure Batch Multiplication", "V4", scalars, pointsV4);
        results.add(v4BatchResult);

        return results;
    }

    /**
     * Benchmark coordinate calculations
     */
    private static List<BenchmarkResult> runCoordinateCalculationBenchmarks() {
        List<BenchmarkResult> results = new ArrayList<>();

        logger.info("📐 Benchmarking Coordinate Calculations...");

        // 2G Calculation
        results.add(benchmarkCoordinateCalculation("2G Calculation", "V2",
            () -> Secp256k1ECCV2.calculate2G()));
        results.add(benchmarkCoordinateCalculation("2G Calculation", "V3",
            () -> Secp256k1ECCV3.calculate2G()));
        results.add(benchmarkCoordinateCalculation("2G Calculation", "V4",
            () -> Secp256k1ECCV4.calculate2G()));

        // 3G Calculation
        results.add(benchmarkCoordinateCalculation("3G Calculation", "V2",
            () -> Secp256k1ECCV2.calculate3G()));
        results.add(benchmarkCoordinateCalculation("3G Calculation", "V3",
            () -> Secp256k1ECCV3.calculate3G()));
        results.add(benchmarkCoordinateCalculation("3G Calculation", "V4",
            () -> Secp256k1ECCV4.calculate3G()));

        return results;
    }

    /**
     * Benchmark memory usage
     */
    private static List<BenchmarkResult> runMemoryUsageBenchmarks() {
        List<BenchmarkResult> results = new ArrayList<>();

        logger.info("💾 Benchmarking Memory Usage...");

        // Memory usage for point creation
        results.add(benchmarkMemoryUsage("Point Creation", "V2",
            () -> new Secp256k1ECCV2.Point(BigInteger.ONE, BigInteger.TWO)));
        results.add(benchmarkMemoryUsage("Point Creation", "V3",
            () -> new Secp256k1ECCV3.Point(BigInteger.ONE, BigInteger.TWO)));
        results.add(benchmarkMemoryUsage("Point Creation", "V4",
            () -> new Secp256k1ECCV4.Point(BigInteger.ONE, BigInteger.TWO)));

        return results;
    }

    // ===== INDIVIDUAL BENCHMARK METHODS =====

    private static BenchmarkResult benchmarkScalarMultiplication(String operation, String version,
                                                                 BigInteger scalar, Object point) {
        long[] times = new long[MEASUREMENT_ITERATIONS];

        // Warmup
        for (int i = 0; i < WARMUP_ITERATIONS; i++) {
            performScalarMultiplication(version, scalar, point);
        }

        // Measurement
        for (int i = 0; i < MEASUREMENT_ITERATIONS; i++) {
            long start = System.nanoTime();
            performScalarMultiplication(version, scalar, point);
            long end = System.nanoTime();
            times[i] = end - start;
        }

        return calculateBenchmarkResult(operation, version, times);
    }

    private static BenchmarkResult benchmarkPointAddition(String operation, String version,
                                                        Object p1, Object p2) {
        long[] times = new long[MEASUREMENT_ITERATIONS];

        // Warmup
        for (int i = 0; i < WARMUP_ITERATIONS; i++) {
            performPointAddition(version, p1, p2);
        }

        // Measurement
        for (int i = 0; i < MEASUREMENT_ITERATIONS; i++) {
            long start = System.nanoTime();
            performPointAddition(version, p1, p2);
            long end = System.nanoTime();
            times[i] = end - start;
        }

        return calculateBenchmarkResult(operation, version, times);
    }

    private static BenchmarkResult benchmarkBatchMultiplication(String operation, String version,
                                                               BigInteger[] scalars, Object[] points) {
        long[] times = new long[Math.max(1, MEASUREMENT_ITERATIONS / 10)]; // Fewer iterations for batch ops

        // Warmup
        for (int i = 0; i < Math.max(1, WARMUP_ITERATIONS / 10); i++) {
            performBatchMultiplication(version, scalars, points);
        }

        // Measurement
        for (int i = 0; i < times.length; i++) {
            long start = System.nanoTime();
            performBatchMultiplication(version, scalars, points);
            long end = System.nanoTime();
            times[i] = end - start;
        }

        return calculateBenchmarkResult(operation, version, times);
    }

    private static BenchmarkResult benchmarkSecureBatchMultiplication(String operation, String version,
                                                                     BigInteger[] scalars, Secp256k1ECCV4.Point[] points) {
        long[] times = new long[Math.max(1, MEASUREMENT_ITERATIONS / 10)];

        // Warmup
        for (int i = 0; i < Math.max(1, WARMUP_ITERATIONS / 10); i++) {
            Secp256k1ECCV4.secureBatchMultiply(scalars, points);
        }

        // Measurement
        for (int i = 0; i < times.length; i++) {
            long start = System.nanoTime();
            Secp256k1ECCV4.secureBatchMultiply(scalars, points);
            long end = System.nanoTime();
            times[i] = end - start;
        }

        return calculateBenchmarkResult(operation, version, times);
    }

    private static BenchmarkResult benchmarkCoordinateCalculation(String operation, String version, Runnable calculation) {
        long[] times = new long[MEASUREMENT_ITERATIONS];

        // Warmup
        for (int i = 0; i < WARMUP_ITERATIONS; i++) {
            calculation.run();
        }

        // Measurement
        for (int i = 0; i < MEASUREMENT_ITERATIONS; i++) {
            long start = System.nanoTime();
            calculation.run();
            long end = System.nanoTime();
            times[i] = end - start;
        }

        return calculateBenchmarkResult(operation, version, times);
    }

    private static BenchmarkResult benchmarkMemoryUsage(String operation, String version, Runnable creation) {
        Runtime runtime = Runtime.getRuntime();
        long[] memoryUsages = new long[MEASUREMENT_ITERATIONS];

        // Warmup
        for (int i = 0; i < WARMUP_ITERATIONS; i++) {
            creation.run();
        }

        // Measurement
        for (int i = 0; i < MEASUREMENT_ITERATIONS; i++) {
            runtime.gc(); // Suggest garbage collection
            long before = runtime.totalMemory() - runtime.freeMemory();

            creation.run();

            long after = runtime.totalMemory() - runtime.freeMemory();
            memoryUsages[i] = after - before;
        }

        // Calculate average memory usage
        long totalMemory = 0;
        for (long mem : memoryUsages) {
            totalMemory += mem;
        }
        long averageMemory = totalMemory / memoryUsages.length;

        return new BenchmarkResult(operation, version, 0, 0, 0, 0, averageMemory);
    }

    // ===== HELPER METHODS =====

    private static void performScalarMultiplication(String version, BigInteger scalar, Object point) {
        switch (version) {
            case "V2":
                Secp256k1ECCV2.multiply(scalar, (Secp256k1ECCV2.Point) point);
                break;
            case "V3":
                Secp256k1ECCV3.multiply(scalar, (Secp256k1ECCV3.Point) point);
                break;
            case "V4":
                Secp256k1ECCV4.multiply(scalar, (Secp256k1ECCV4.Point) point);
                break;
        }
    }

    private static void performPointAddition(String version, Object p1, Object p2) {
        switch (version) {
            case "V2":
                Secp256k1ECCV2.add((Secp256k1ECCV2.Point) p1, (Secp256k1ECCV2.Point) p2);
                break;
            case "V3":
                Secp256k1ECCV3.add((Secp256k1ECCV3.Point) p1, (Secp256k1ECCV3.Point) p2);
                break;
            case "V4":
                Secp256k1ECCV4.add((Secp256k1ECCV4.Point) p1, (Secp256k1ECCV4.Point) p2);
                break;
        }
    }

    private static void performBatchMultiplication(String version, BigInteger[] scalars, Object[] points) {
        if ("V3".equals(version)) {
            @SuppressWarnings("unchecked")
            List<Secp256k1ECCV3.Point> pointList = (List<Secp256k1ECCV3.Point>) (List<?>) List.of(points);
            Secp256k1ECCV3.batchMultiply(List.of(scalars), pointList);
        }
    }

    private static BenchmarkResult calculateBenchmarkResult(String operation, String version, long[] times) {
        // Calculate statistics
        long totalTime = 0;
        long minTime = Long.MAX_VALUE;
        long maxTime = Long.MIN_VALUE;

        for (long time : times) {
            totalTime += time;
            minTime = Math.min(minTime, time);
            maxTime = Math.max(maxTime, time);
        }

        long averageTime = totalTime / times.length;
        double operationsPerSecond = 1_000_000_000.0 / averageTime;

        return new BenchmarkResult(operation, version, averageTime, minTime, maxTime,
                                 operationsPerSecond, 0);
    }

    /**
     * Print benchmark comparison table
     */
    public static void printComparisonTable(List<BenchmarkResult> results) {
        logger.info("=== 📈 Performance Comparison Table ===");
        logger.info(String.format("%-25s %-8s %-12s %-12s %-12s",
            "Operation", "Version", "Ops/sec", "Avg Time(μs)", "Memory(KB)"));
        logger.info("─".repeat(80));

        String currentOperation = "";
        for (BenchmarkResult result : results) {
            if (!currentOperation.equals(result.operation)) {
                if (!currentOperation.isEmpty()) {
                    logger.info(""); // Empty line between operation groups
                }
                currentOperation = result.operation;
            }

            logger.info(String.format("%-25s %-8s %-12.0f %-12.2f %-12d",
                result.operation,
                result.version,
                result.operationsPerSecond,
                result.averageTimeNs / 1000.0,
                result.memoryUsageBytes / 1024));
        }
    }

    /**
     * Run quick benchmark demo
     */
    public static void runQuickDemo() {
        logger.info("=== ⚡ Quick Benchmark Demo ===");

        // Quick scalar multiplication test
        BigInteger scalar = BigInteger.valueOf(12345);

        long startV2 = System.nanoTime();
        Secp256k1ECCV2.multiply(scalar, Secp256k1ECCV2.G);
        long timeV2 = System.nanoTime() - startV2;

        long startV3 = System.nanoTime();
        Secp256k1ECCV3.multiply(scalar, Secp256k1ECCV3.G);
        long timeV3 = System.nanoTime() - startV3;

        long startV4 = System.nanoTime();
        Secp256k1ECCV4.multiply(scalar, Secp256k1ECCV4.G);
        long timeV4 = System.nanoTime() - startV4;

        logger.info(String.format("V2: %.2f μs", timeV2 / 1000.0));
        logger.info(String.format("V3: %.2f μs", timeV3 / 1000.0));
        logger.info(String.format("V4: %.2f μs", timeV4 / 1000.0));

        double speedupV3 = (double) timeV2 / timeV3;
        double speedupV4 = (double) timeV2 / timeV4;

        logger.info(String.format("V3 speedup: %.2fx", speedupV3));
        logger.info(String.format("V4 speedup: %.2fx", speedupV4));
    }
}
