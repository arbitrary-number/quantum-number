package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.RepeatedTest;
import java.math.BigInteger;
import java.util.logging.Logger;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Performance-focused test suite for Secp256k1ECCV5 ultra-high-performance implementation.
 * Focuses on speed measurements, throughput, and computational efficiency.
 */
@DisplayName("secp256k1 Ultra-High-Performance ECC Tests")
public class Secp256k1ECCV5Test {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCV5Test.class.getName());

    @BeforeEach
    void setupLogging() {
        ConsoleHandler handler = new ConsoleHandler();
        handler.setEncoding(StandardCharsets.UTF_8.toString());
        handler.setLevel(Level.ALL);
        logger.addHandler(handler);
        logger.setLevel(Level.ALL);
        logger.setUseParentHandlers(false);
    }

    @Test
    @DisplayName("Basic functionality verification")
    void testBasicFunctionality() {
        logger.info("🔍 Testing V5 basic functionality");

        Secp256k1ECCV5.Point g = Secp256k1ECCV5.G;
        Secp256k1ECCV5.Point g2 = Secp256k1ECCV5.calculate2G();
        Secp256k1ECCV5.Point g3 = Secp256k1ECCV5.calculate3G();

        // Verify coordinates are correct
        assertTrue(Secp256k1ECCV5.isOnCurve(g), "G should be on curve");
        assertTrue(Secp256k1ECCV5.isOnCurve(g2), "2G should be on curve");
        assertTrue(Secp256k1ECCV5.isOnCurve(g3), "3G should be on curve");

        // Verify 2G = G + G
        Secp256k1ECCV5.Point manual2G = Secp256k1ECCV5.add(g, g);
        assertEquals(manual2G.x, g2.x, "Manual 2G should equal pre-computed 2G");
        assertEquals(manual2G.y, g2.y, "Manual 2G should equal pre-computed 2G");

        logger.info("✅ V5 basic functionality verified");
    }

    @Test
    @DisplayName("Pre-computed points performance")
    void testPrecomputedPointsPerformance() {
        logger.info("⚡ Testing pre-computed points performance");

        long startTime = System.nanoTime();

        // Test accessing pre-computed points (should be near-instantaneous)
        for (int i = 1; i <= 15; i++) {
            Secp256k1ECCV5.Point point = Secp256k1ECCV5.multiply(BigInteger.valueOf(i), Secp256k1ECCV5.G);
            assertNotNull(point, "Point should not be null");
            assertTrue(Secp256k1ECCV5.isOnCurve(point), "Point should be on curve");
        }

        long endTime = System.nanoTime();
        double timeMs = (endTime - startTime) / 1_000_000.0;

        logger.info(String.format("📊 Pre-computed points access time: %.4f ms for 15 points", timeMs));
        logger.info(String.format("⚡ Average: %.4f μs per point", (timeMs * 1000) / 15));

        // Should be extremely fast (< 1ms total)
        assertTrue(timeMs < 1.0, "Pre-computed points should be accessed very quickly");

        logger.info("✅ Pre-computed points performance verified");
    }

    @Test
    @DisplayName("Large scalar multiplication performance")
    void testLargeScalarPerformance() {
        logger.info("🚀 Testing large scalar multiplication performance");

        // Test with large scalars
        BigInteger[] largeScalars = {
            new BigInteger("123456789012345678901234567890"),
            new BigInteger("987654321098765432109876543210"),
            new BigInteger("111111111111111111111111111111")
        };

        long totalTime = 0;

        for (BigInteger scalar : largeScalars) {
            long startTime = System.nanoTime();
            Secp256k1ECCV5.Point result = Secp256k1ECCV5.multiply(scalar, Secp256k1ECCV5.G);
            long endTime = System.nanoTime();

            long timeNs = endTime - startTime;
            totalTime += timeNs;

            assertNotNull(result, "Result should not be null");
            assertTrue(Secp256k1ECCV5.isOnCurve(result), "Result should be on curve");

            logger.info(String.format("🔢 Large scalar (%d bits): %.2f ms",
                scalar.bitLength(), timeNs / 1_000_000.0));
        }

        double avgTimeMs = (totalTime / 3) / 1_000_000.0;
        logger.info(String.format("📊 Average large scalar time: %.2f ms", avgTimeMs));

        // Should complete within reasonable time (< 100ms per operation)
        assertTrue(avgTimeMs < 100.0, "Large scalar multiplication should be reasonably fast");

        logger.info("✅ Large scalar performance verified");
    }

    @Test
    @DisplayName("Batch processing performance")
    void testBatchProcessingPerformance() {
        logger.info("📦 Testing batch processing performance");

        final int BATCH_SIZE = 1000;
        BigInteger[] scalars = new BigInteger[BATCH_SIZE];
        Secp256k1ECCV5.Point[] points = new Secp256k1ECCV5.Point[BATCH_SIZE];

        // Initialize test data
        for (int i = 0; i < BATCH_SIZE; i++) {
            scalars[i] = BigInteger.valueOf(i % 50 + 1); // Cycle through 1-50
            points[i] = Secp256k1ECCV5.G;
        }

        long startTime = System.nanoTime();
        Secp256k1ECCV5.Point[] results = Secp256k1ECCV5.batchMultiply(scalars, points);
        long endTime = System.nanoTime();

        double timeMs = (endTime - startTime) / 1_000_000.0;
        double avgTimePerOp = timeMs / BATCH_SIZE;

        logger.info(String.format("📊 Batch of %d operations: %.2f ms total", BATCH_SIZE, timeMs));
        logger.info(String.format("⚡ Average per operation: %.4f μs", avgTimePerOp * 1000));
        logger.info(String.format("🚀 Operations per second: %.0f", 1000.0 / avgTimePerOp));

        // Verify results
        assertEquals(BATCH_SIZE, results.length, "Should return correct number of results");
        for (int i = 0; i < Math.min(10, results.length); i++) { // Check first 10
            assertNotNull(results[i], "Result " + i + " should not be null");
            assertTrue(Secp256k1ECCV5.isOnCurve(results[i]), "Result " + i + " should be on curve");
        }

        // Should be very fast (< 500ms for 1000 operations)
        assertTrue(timeMs < 500.0, "Batch processing should be fast");

        logger.info("✅ Batch processing performance verified");
    }

    @RepeatedTest(5)
    @DisplayName("Stress test with random scalars")
    void testStressTestWithRandomScalars() {
        logger.info("🔥 Running stress test with random scalars");

        final int ITERATIONS = 1000;
        long totalTime = 0;
        int validResults = 0;

        for (int i = 0; i < ITERATIONS; i++) {
            // Generate random scalar (simulate real usage)
            BigInteger scalar = BigInteger.valueOf((int) (Math.random() * 1000) + 1);

            long startTime = System.nanoTime();
            Secp256k1ECCV5.Point result = Secp256k1ECCV5.multiply(scalar, Secp256k1ECCV5.G);
            long endTime = System.nanoTime();

            totalTime += (endTime - startTime);

            if (result != null && Secp256k1ECCV5.isOnCurve(result)) {
                validResults++;
            }
        }

        double avgTimeUs = (totalTime / (double) ITERATIONS) / 1000.0;
        double successRate = (validResults / (double) ITERATIONS) * 100.0;

        logger.info(String.format("📊 Stress test: %d iterations", ITERATIONS));
        logger.info(String.format("⏱️  Average time: %.2f μs per operation", avgTimeUs));
        logger.info(String.format("✅ Success rate: %.1f%%", successRate));
        logger.info(String.format("🚀 Throughput: %.0f ops/sec", 1_000_000.0 / avgTimeUs));

        // Should have 100% success rate and be fast
        assertEquals(100.0, successRate, 0.1, "Should have 100% success rate");
        assertTrue(avgTimeUs < 100.0, "Should be very fast (< 100μs per operation)");

        logger.info("✅ Stress test completed successfully");
    }

    @Test
    @DisplayName("Memory-efficient processing test")
    void testMemoryEfficientProcessing() {
        logger.info("💾 Testing memory-efficient processing");

        final int TOTAL_OPERATIONS = 5000;
        BigInteger[] scalars = new BigInteger[TOTAL_OPERATIONS];

        // Initialize scalars
        for (int i = 0; i < TOTAL_OPERATIONS; i++) {
            scalars[i] = BigInteger.valueOf(i % 100 + 1);
        }

        long startTime = System.nanoTime();
        Secp256k1ECCV5.processBatchEfficiently(scalars, Secp256k1ECCV5.G);
        long endTime = System.nanoTime();

        double timeMs = (endTime - startTime) / 1_000_000.0;
        double avgTimePerOp = timeMs / TOTAL_OPERATIONS;

        logger.info(String.format("💾 Memory-efficient processing: %d operations in %.2f ms", TOTAL_OPERATIONS, timeMs));
        logger.info(String.format("⚡ Average per operation: %.4f μs", avgTimePerOp * 1000));

        // Should be efficient (< 2000ms for 5000 operations)
        assertTrue(timeMs < 2000.0, "Memory-efficient processing should be reasonably fast");

        logger.info("✅ Memory-efficient processing verified");
    }

    @Test
    @DisplayName("Point caching effectiveness")
    void testPointCachingEffectiveness() {
        logger.info("🔄 Testing point caching effectiveness");

        // Test that repeated operations with same scalar are fast
        BigInteger scalar = BigInteger.valueOf(42);

        long startTime = System.nanoTime();
        for (int i = 0; i < 100; i++) {
            Secp256k1ECCV5.Point result = Secp256k1ECCV5.multiply(scalar, Secp256k1ECCV5.G);
            assertNotNull(result);
        }
        long endTime = System.nanoTime();

        double timeMs = (endTime - startTime) / 1_000_000.0;
        double avgTimePerOp = timeMs / 100;

        logger.info(String.format("🔄 Cached operations: 100 iterations in %.2f ms", timeMs));
        logger.info(String.format("⚡ Average per cached operation: %.4f μs", avgTimePerOp * 1000));

        // Should be very fast due to caching (< 50μs per operation)
        assertTrue(avgTimePerOp * 1000 < 50.0, "Cached operations should be very fast");

        logger.info("✅ Point caching effectiveness verified");
    }

    @Test
    @DisplayName("Ultra-fast coordinate demo")
    void testUltraFastCoordinateDemo() {
        logger.info("⚡ Testing ultra-fast coordinate demo");

        long startTime = System.nanoTime();
        Secp256k1ECCV5.logFastCoordinates();
        long endTime = System.nanoTime();

        double timeMs = (endTime - startTime) / 1_000_000.0;

        logger.info(String.format("⚡ Ultra-fast demo completed in %.4f ms", timeMs));

        // Should be very fast (< 10ms)
        assertTrue(timeMs < 10.0, "Coordinate demo should be very fast");

        logger.info("✅ Ultra-fast coordinate demo verified");
    }

    @Test
    @DisplayName("Performance regression test")
    void testPerformanceRegression() {
        logger.info("📈 Testing performance regression");

        // Baseline: simple scalar multiplication
        BigInteger scalar = BigInteger.valueOf(7);

        long[] times = new long[100];
        for (int i = 0; i < 100; i++) {
            long start = System.nanoTime();
            Secp256k1ECCV5.Point result = Secp256k1ECCV5.multiply(scalar, Secp256k1ECCV5.G);
            long end = System.nanoTime();
            times[i] = end - start;
            assertNotNull(result);
        }

        // Calculate statistics
        long sum = 0;
        long min = Long.MAX_VALUE;
        long max = Long.MIN_VALUE;

        for (long time : times) {
            sum += time;
            min = Math.min(min, time);
            max = Math.max(max, time);
        }

        double avgTimeUs = (sum / 100.0) / 1000.0;

        logger.info(String.format("📊 Performance stats:"));
        logger.info(String.format("   Average: %.2f μs", avgTimeUs));
        logger.info(String.format("   Min: %.2f μs", min / 1000.0));
        logger.info(String.format("   Max: %.2f μs", max / 1000.0));

        // Establish performance baseline - should be fast
        assertTrue(avgTimeUs < 20.0, "Average time should be under 20μs");
        assertTrue(max / 1000.0 < 50.0, "Max time should be under 50μs");

        logger.info("✅ Performance regression test passed");
    }

    @Test
    @DisplayName("Comprehensive V5 performance test")
    void testComprehensiveV5Performance() {
        logger.info("🎯 Running comprehensive V5 performance test");

        // Test 1: Basic operations
        long startTime = System.nanoTime();
        Secp256k1ECCV5.Point g = Secp256k1ECCV5.G;
        Secp256k1ECCV5.Point g2 = Secp256k1ECCV5.calculate2G();
        Secp256k1ECCV5.Point g3 = Secp256k1ECCV5.calculate3G();
        long basicTime = System.nanoTime() - startTime;

        // Test 2: Scalar multiplications
        startTime = System.nanoTime();
        for (int i = 1; i <= 10; i++) {
            Secp256k1ECCV5.multiply(BigInteger.valueOf(i), g);
        }
        long scalarTime = System.nanoTime() - startTime;

        // Test 3: Point additions
        startTime = System.nanoTime();
        for (int i = 0; i < 100; i++) {
            Secp256k1ECCV5.add(g, g2);
        }
        long additionTime = System.nanoTime() - startTime;

        // Calculate metrics
        double basicTimeMs = basicTime / 1_000_000.0;
        double scalarTimeMs = scalarTime / 1_000_000.0;
        double additionTimeMs = additionTime / 1_000_000.0;

        logger.info("🎯 V5 Performance Results:");
        logger.info(String.format("   Basic operations: %.4f ms", basicTimeMs));
        logger.info(String.format("   10 scalar mults: %.4f ms (%.2f μs each)", scalarTimeMs, (scalarTimeMs * 1000) / 10));
        logger.info(String.format("   100 additions: %.4f ms (%.2f μs each)", additionTimeMs, (additionTimeMs * 1000) / 100));

        // Performance assertions
        assertTrue(basicTimeMs < 1.0, "Basic operations should be very fast");
        assertTrue((scalarTimeMs * 1000) / 10 < 10.0, "Scalar multiplications should be fast");
        assertTrue((additionTimeMs * 1000) / 100 < 5.0, "Point additions should be very fast");

        logger.info("✅ Comprehensive V5 performance test passed");
    }
}
