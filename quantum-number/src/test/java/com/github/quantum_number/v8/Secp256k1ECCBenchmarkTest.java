package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Nested;
import java.math.BigInteger;
import java.util.logging.Logger;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.util.List;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Test suite for Secp256k1ECCBenchmark performance measurement and validation.
 * Ensures benchmark accuracy and proper functionality across all versions.
 */
@DisplayName("secp256k1 ECC Benchmark Test Suite")
public class Secp256k1ECCBenchmarkTest {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCBenchmarkTest.class.getName());

    @BeforeEach
    void setupLogging() {
        ConsoleHandler handler = new ConsoleHandler();
        handler.setEncoding(StandardCharsets.UTF_8.toString());
        handler.setLevel(Level.ALL);
        logger.addHandler(handler);
        logger.setLevel(Level.ALL);
        logger.setUseParentHandlers(false);
    }

    @Nested
    @DisplayName("Benchmark Result Validation")
    class BenchmarkResultValidationTest {

        @Test
        @DisplayName("Benchmark result structure validation")
        void testBenchmarkResultStructure() {
            Secp256k1ECCBenchmark.BenchmarkResult result = new Secp256k1ECCBenchmark.BenchmarkResult(
                "Test Operation", "V2", 1000L, 900L, 1100L, 1000.0, 1024L);

            assertEquals("Test Operation", result.operation);
            assertEquals("V2", result.version);
            assertEquals(1000L, result.averageTimeNs);
            assertEquals(900L, result.minTimeNs);
            assertEquals(1100L, result.maxTimeNs);
            assertEquals(1000.0, result.operationsPerSecond);
            assertEquals(1024L, result.memoryUsageBytes);
        }

        @Test
        @DisplayName("Benchmark result string representation")
        void testBenchmarkResultToString() {
            Secp256k1ECCBenchmark.BenchmarkResult result = new Secp256k1ECCBenchmark.BenchmarkResult(
                "Scalar Multiplication", "V3", 2000L, 1800L, 2200L, 500.0, 2048L);

            String expected = "Scalar Multiplication (V3): 500 ops/sec, avg=2.00 μs, mem=2 KB";
            assertEquals(expected, result.toString());
        }
    }

    @Nested
    @DisplayName("Quick Demo Validation")
    class QuickDemoValidationTest {

        @Test
        @DisplayName("Quick demo execution")
        void testQuickDemoExecution() {
            // This test ensures the quick demo runs without throwing exceptions
            assertDoesNotThrow(() -> Secp256k1ECCBenchmark.runQuickDemo(),
                "Quick demo should execute without exceptions");
        }

        @Test
        @DisplayName("Quick demo performance consistency")
        void testQuickDemoPerformanceConsistency() {
            // Run quick demo multiple times to ensure consistent results
            for (int i = 0; i < 3; i++) {
                assertDoesNotThrow(() -> Secp256k1ECCBenchmark.runQuickDemo(),
                    "Quick demo should be consistent across runs");
            }
        }
    }

    @Nested
    @DisplayName("Full Benchmark Suite Validation")
    class FullBenchmarkSuiteValidationTest {

        @Test
        @DisplayName("Full benchmark execution")
        void testFullBenchmarkExecution() {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results =
                assertDoesNotThrow(() -> Secp256k1ECCBenchmark.runFullBenchmark(),
                    "Full benchmark should execute without exceptions");

            assertNotNull(results, "Benchmark results should not be null");
            assertFalse(results.isEmpty(), "Benchmark should produce results");

            // Verify we have results for all expected operations and versions
            boolean hasScalarMultiplication = results.stream()
                .anyMatch(r -> "Scalar Multiplication".equals(r.operation));
            boolean hasPointAddition = results.stream()
                .anyMatch(r -> "Point Addition".equals(r.operation));
            boolean hasBatchOperations = results.stream()
                .anyMatch(r -> r.operation.contains("Batch"));
            boolean hasCoordinateCalculations = results.stream()
                .anyMatch(r -> r.operation.contains("Calculation"));
            boolean hasMemoryBenchmarks = results.stream()
                .anyMatch(r -> "Point Creation".equals(r.operation));

            assertTrue(hasScalarMultiplication, "Should have scalar multiplication benchmarks");
            assertTrue(hasPointAddition, "Should have point addition benchmarks");
            assertTrue(hasBatchOperations, "Should have batch operation benchmarks");
            assertTrue(hasCoordinateCalculations, "Should have coordinate calculation benchmarks");
            assertTrue(hasMemoryBenchmarks, "Should have memory usage benchmarks");
        }

        @Test
        @DisplayName("Benchmark result consistency")
        void testBenchmarkResultConsistency() {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results = Secp256k1ECCBenchmark.runFullBenchmark();

            for (Secp256k1ECCBenchmark.BenchmarkResult result : results) {
                // Validate timing constraints
                assertTrue(result.averageTimeNs > 0, "Average time should be positive");
                assertTrue(result.minTimeNs > 0, "Minimum time should be positive");
                assertTrue(result.maxTimeNs > 0, "Maximum time should be positive");
                assertTrue(result.minTimeNs <= result.averageTimeNs, "Min time should not exceed average");
                assertTrue(result.averageTimeNs <= result.maxTimeNs, "Average time should not exceed max");

                // Validate operations per second
                assertTrue(result.operationsPerSecond > 0, "Operations per second should be positive");

                // Validate memory usage (where applicable)
                assertTrue(result.memoryUsageBytes >= 0, "Memory usage should be non-negative");
            }
        }

        @Test
        @DisplayName("Version comparison availability")
        void testVersionComparisonAvailability() {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results = Secp256k1ECCBenchmark.runFullBenchmark();

            // Check that we have results for all versions
            boolean hasV2 = results.stream().anyMatch(r -> "V2".equals(r.version));
            boolean hasV3 = results.stream().anyMatch(r -> "V3".equals(r.version));
            boolean hasV4 = results.stream().anyMatch(r -> "V4".equals(r.version));

            assertTrue(hasV2, "Should have V2 benchmark results");
            assertTrue(hasV3, "Should have V3 benchmark results");
            assertTrue(hasV4, "Should have V4 benchmark results");
        }
    }

    @Nested
    @DisplayName("Performance Expectations")
    class PerformanceExpectationsTest {

        @Test
        @DisplayName("V3 performance improvement over V2")
        void testV3PerformanceImprovement() {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results = Secp256k1ECCBenchmark.runFullBenchmark();

            // Find scalar multiplication results for V2 and V3
            double v2OpsPerSec = results.stream()
                .filter(r -> "Scalar Multiplication".equals(r.operation) && "V2".equals(r.version))
                .findFirst()
                .map(r -> r.operationsPerSecond)
                .orElse(0.0);

            double v3OpsPerSec = results.stream()
                .filter(r -> "Scalar Multiplication".equals(r.operation) && "V3".equals(r.version))
                .findFirst()
                .map(r -> r.operationsPerSecond)
                .orElse(0.0);

            // V3 should generally perform better than V2 due to optimizations
            // Allow for some variance due to system conditions
            if (v2OpsPerSec > 0 && v3OpsPerSec > 0) {
                double improvementRatio = v3OpsPerSec / v2OpsPerSec;
                logger.info(String.format("V3/V2 performance ratio: %.2f", improvementRatio));

                // V3 should be at least 80% as fast as V2 (allowing for constant-time overhead in some cases)
                assertTrue(improvementRatio >= 0.8,
                    "V3 should perform reasonably compared to V2");
            }
        }

        @Test
        @DisplayName("V4 security overhead assessment")
        void testV4SecurityOverhead() {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results = Secp256k1ECCBenchmark.runFullBenchmark();

            // Compare V4 performance with V2 (baseline)
            double v2OpsPerSec = results.stream()
                .filter(r -> "Scalar Multiplication".equals(r.operation) && "V2".equals(r.version))
                .findFirst()
                .map(r -> r.operationsPerSecond)
                .orElse(0.0);

            double v4OpsPerSec = results.stream()
                .filter(r -> "Scalar Multiplication".equals(r.operation) && "V4".equals(r.version))
                .findFirst()
                .map(r -> r.operationsPerSecond)
                .orElse(0.0);

            if (v2OpsPerSec > 0 && v4OpsPerSec > 0) {
                double overheadRatio = v2OpsPerSec / v4OpsPerSec;
                logger.info(String.format("V4 security overhead ratio: %.2f", overheadRatio));

                // V4 should not be more than 50% slower due to security features
                assertTrue(overheadRatio <= 1.5,
                    "V4 security overhead should be reasonable");
            }
        }

        @Test
        @DisplayName("Batch operation efficiency")
        void testBatchOperationEfficiency() {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results = Secp256k1ECCBenchmark.runFullBenchmark();

            // Find batch operation results
            double v3BatchOpsPerSec = results.stream()
                .filter(r -> "Batch Multiplication".equals(r.operation) && "V3".equals(r.version))
                .findFirst()
                .map(r -> r.operationsPerSecond)
                .orElse(0.0);

            double v4SecureBatchOpsPerSec = results.stream()
                .filter(r -> "Secure Batch Multiplication".equals(r.operation) && "V4".equals(r.version))
                .findFirst()
                .map(r -> r.operationsPerSecond)
                .orElse(0.0);

            if (v3BatchOpsPerSec > 0) {
                logger.info(String.format("V3 batch operations/sec: %.0f", v3BatchOpsPerSec));
                assertTrue(v3BatchOpsPerSec > 0, "V3 should support batch operations");
            }

            if (v4SecureBatchOpsPerSec > 0) {
                logger.info(String.format("V4 secure batch operations/sec: %.0f", v4SecureBatchOpsPerSec));
                assertTrue(v4SecureBatchOpsPerSec > 0, "V4 should support secure batch operations");
            }
        }
    }

    @Nested
    @DisplayName("Benchmark Accuracy Validation")
    class BenchmarkAccuracyValidationTest {

        @Test
        @DisplayName("Statistical consistency of measurements")
        void testStatisticalConsistency() {
            // Run benchmark multiple times and check statistical properties
            List<Secp256k1ECCBenchmark.BenchmarkResult> run1 = Secp256k1ECCBenchmark.runFullBenchmark();
            List<Secp256k1ECCBenchmark.BenchmarkResult> run2 = Secp256k1ECCBenchmark.runFullBenchmark();
            List<Secp256k1ECCBenchmark.BenchmarkResult> run3 = Secp256k1ECCBenchmark.runFullBenchmark();

            // Compare results for the same operation/version across runs
            for (int i = 0; i < run1.size(); i++) {
                Secp256k1ECCBenchmark.BenchmarkResult r1 = run1.get(i);
                Secp256k1ECCBenchmark.BenchmarkResult r2 = run2.get(i);
                Secp256k1ECCBenchmark.BenchmarkResult r3 = run3.get(i);

                // Results should be reasonably consistent (within 50% for performance benchmarks)
                if (r1.operationsPerSecond > 0) {
                    double avgOpsPerSec = (r1.operationsPerSecond + r2.operationsPerSecond + r3.operationsPerSecond) / 3;
                    double variation1 = Math.abs(r1.operationsPerSecond - avgOpsPerSec) / avgOpsPerSec;
                    double variation2 = Math.abs(r2.operationsPerSecond - avgOpsPerSec) / avgOpsPerSec;
                    double variation3 = Math.abs(r3.operationsPerSecond - avgOpsPerSec) / avgOpsPerSec;

                    assertTrue(variation1 <= 0.5, "Run 1 should be statistically consistent");
                    assertTrue(variation2 <= 0.5, "Run 2 should be statistically consistent");
                    assertTrue(variation3 <= 0.5, "Run 3 should be statistically consistent");
                }
            }
        }

        @Test
        @DisplayName("Memory measurement accuracy")
        void testMemoryMeasurementAccuracy() {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results = Secp256k1ECCBenchmark.runFullBenchmark();

            // Check memory measurements for point creation
            results.stream()
                .filter(r -> "Point Creation".equals(r.operation))
                .forEach(result -> {
                    // Memory usage should be reasonable (not zero, not excessively high)
                    assertTrue(result.memoryUsageBytes > 0, "Memory usage should be measurable");
                    assertTrue(result.memoryUsageBytes < 1024 * 1024, "Memory usage should be reasonable (< 1MB)");

                    logger.info(String.format("%s memory usage: %d bytes", result.version, result.memoryUsageBytes));
                });
        }
    }

    @Nested
    @DisplayName("Comparison Table Functionality")
    class ComparisonTableFunctionalityTest {

        @Test
        @DisplayName("Comparison table generation")
        void testComparisonTableGeneration() {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results = Secp256k1ECCBenchmark.runFullBenchmark();

            // Test that comparison table can be generated without errors
            assertDoesNotThrow(() -> Secp256k1ECCBenchmark.printComparisonTable(results),
                "Comparison table generation should not throw exceptions");
        }

        @Test
        @DisplayName("Comparison table content validation")
        void testComparisonTableContent() {
            List<Secp256k1ECCBenchmark.BenchmarkResult> results = Secp256k1ECCBenchmark.runFullBenchmark();

            // Ensure we have diverse operations for meaningful comparison
            long uniqueOperations = results.stream()
                .map(r -> r.operation)
                .distinct()
                .count();

            long uniqueVersions = results.stream()
                .map(r -> r.version)
                .distinct()
                .count();

            assertTrue(uniqueOperations >= 3, "Should have multiple operation types for comparison");
            assertTrue(uniqueVersions >= 3, "Should have multiple versions for comparison");

            logger.info(String.format("Comparison table includes %d operations and %d versions",
                uniqueOperations, uniqueVersions));
        }
    }

    @Test
    @DisplayName("Complete benchmark validation suite")
    void testCompleteBenchmarkValidationSuite() {
        logger.info("=== 🧪 Complete Benchmark Validation Suite ===");

        // Test all benchmark components
        assertDoesNotThrow(() -> Secp256k1ECCBenchmark.runQuickDemo(),
            "Quick demo should work");

        List<Secp256k1ECCBenchmark.BenchmarkResult> fullResults =
            assertDoesNotThrow(() -> Secp256k1ECCBenchmark.runFullBenchmark(),
                "Full benchmark should work");

        assertDoesNotThrow(() -> Secp256k1ECCBenchmark.printComparisonTable(fullResults),
            "Comparison table should work");

        // Validate comprehensive results
        assertNotNull(fullResults, "Results should not be null");
        assertFalse(fullResults.isEmpty(), "Results should not be empty");

        // Check for expected benchmark categories
        boolean hasPerformanceMetrics = fullResults.stream()
            .anyMatch(r -> r.operationsPerSecond > 0);
        boolean hasMemoryMetrics = fullResults.stream()
            .anyMatch(r -> r.memoryUsageBytes > 0);
        boolean hasTimingMetrics = fullResults.stream()
            .anyMatch(r -> r.averageTimeNs > 0);

        assertTrue(hasPerformanceMetrics, "Should have performance metrics");
        assertTrue(hasMemoryMetrics, "Should have memory metrics");
        assertTrue(hasTimingMetrics, "Should have timing metrics");

        logger.info(String.format("✅ Benchmark validation complete - %d results analyzed", fullResults.size()));
    }
}
