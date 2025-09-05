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
import java.util.ArrayList;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Integration test suite that validates interoperability and consistency across all secp256k1 ECC versions.
 * Ensures that V2, V3, and V4 produce mathematically equivalent results while showcasing their unique features.
 */
@DisplayName("secp256k1 ECC Cross-Version Integration Tests")
public class Secp256k1ECCIntegrationTest {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCIntegrationTest.class.getName());

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
    @DisplayName("Mathematical Equivalence Across Versions")
    class MathematicalEquivalenceTest {

        @Test
        @DisplayName("G coordinate consistency across versions")
        void testGeneratorPointConsistency() {
            logger.info("🔍 Testing generator point consistency across versions");

            // All versions should have the same generator point
            Secp256k1ECCV2.Point gV2 = Secp256k1ECCV2.G;
            Secp256k1ECCV3.Point gV3 = Secp256k1ECCV3.G;
            Secp256k1ECCV4.Point gV4 = Secp256k1ECCV4.G;

            // Compare coordinates
            assertEquals(gV2.x, gV3.x, "V2 and V3 G.x should be identical");
            assertEquals(gV2.y, gV3.y, "V2 and V3 G.y should be identical");
            assertEquals(gV2.x, gV4.x, "V2 and V4 G.x should be identical");
            assertEquals(gV2.y, gV4.y, "V2 and V4 G.y should be identical");

            // All should be on curve
            assertTrue(Secp256k1ECCV2.isOnCurve(gV2), "V2 G should be on curve");
            assertTrue(gV3.isOnCurve(), "V3 G should be on curve");
            assertTrue(gV4.isOnCurve(), "V4 G should be on curve");

            logger.info("✅ Generator point consistency verified");
        }

        @Test
        @DisplayName("2G coordinate equivalence across versions")
        void test2GCoordinateEquivalence() {
            logger.info("🔍 Testing 2G coordinate equivalence across versions");

            Secp256k1ECCV2.Point g2V2 = Secp256k1ECCV2.calculate2G();
            Secp256k1ECCV3.Point g2V3 = Secp256k1ECCV3.calculate2G();
            Secp256k1ECCV4.Point g2V4 = Secp256k1ECCV4.calculate2G();

            // All versions should produce identical 2G coordinates
            assertEquals(g2V2.x, g2V3.x, "V2 and V3 2G.x should be identical");
            assertEquals(g2V2.y, g2V3.y, "V2 and V3 2G.y should be identical");
            assertEquals(g2V2.x, g2V4.x, "V2 and V4 2G.x should be identical");
            assertEquals(g2V2.y, g2V4.y, "V2 and V4 2G.y should be identical");

            // Verify 2G = G + G for all versions
            Secp256k1ECCV2.Point manual2GV2 = Secp256k1ECCV2.add(Secp256k1ECCV2.G, Secp256k1ECCV2.G);
            Secp256k1ECCV3.Point manual2GV3 = Secp256k1ECCV3.add(Secp256k1ECCV3.G, Secp256k1ECCV3.G);
            Secp256k1ECCV4.Point manual2GV4 = Secp256k1ECCV4.add(Secp256k1ECCV4.G, Secp256k1ECCV4.G);

            assertEquals(manual2GV2.x, g2V2.x, "V2 2G should equal G + G");
            assertEquals(manual2GV3.x, g2V3.x, "V3 2G should equal G + G");
            assertEquals(manual2GV4.x, g2V4.x, "V4 2G should equal G + G");

            logger.info("✅ 2G coordinate equivalence verified");
        }

        @Test
        @DisplayName("3G coordinate equivalence across versions")
        void test3GCoordinateEquivalence() {
            logger.info("🔍 Testing 3G coordinate equivalence across versions");

            Secp256k1ECCV2.Point g3V2 = Secp256k1ECCV2.calculate3G();
            Secp256k1ECCV3.Point g3V3 = Secp256k1ECCV3.calculate3G();
            Secp256k1ECCV4.Point g3V4 = Secp256k1ECCV4.calculate3G();

            // All versions should produce identical 3G coordinates
            assertEquals(g3V2.x, g3V3.x, "V2 and V3 3G.x should be identical");
            assertEquals(g3V2.y, g3V3.y, "V2 and V3 3G.y should be identical");
            assertEquals(g3V2.x, g3V4.x, "V2 and V4 3G.x should be identical");
            assertEquals(g3V2.y, g3V4.y, "V2 and V4 3G.y should be identical");

            // Verify 3G = 2G + G for all versions
            Secp256k1ECCV2.Point manual3GV2 = Secp256k1ECCV2.add(Secp256k1ECCV2.calculate2G(), Secp256k1ECCV2.G);
            Secp256k1ECCV3.Point manual3GV3 = Secp256k1ECCV3.add(Secp256k1ECCV3.calculate2G(), Secp256k1ECCV3.G);
            Secp256k1ECCV4.Point manual3GV4 = Secp256k1ECCV4.add(Secp256k1ECCV4.calculate2G(), Secp256k1ECCV4.G);

            assertEquals(manual3GV2.x, g3V2.x, "V2 3G should equal 2G + G");
            assertEquals(manual3GV3.x, g3V3.x, "V3 3G should equal 2G + G");
            assertEquals(manual3GV4.x, g3V4.x, "V4 3G should equal 2G + G");

            logger.info("✅ 3G coordinate equivalence verified");
        }

        @Test
        @DisplayName("Scalar multiplication consistency across versions")
        void testScalarMultiplicationConsistency() {
            logger.info("🔍 Testing scalar multiplication consistency across versions");

            BigInteger[] testScalars = {
                BigInteger.ONE,
                BigInteger.valueOf(5),
                BigInteger.valueOf(17),
                new BigInteger("123456789")
            };

            for (BigInteger scalar : testScalars) {
                Secp256k1ECCV2.Point resultV2 = Secp256k1ECCV2.multiply(scalar, Secp256k1ECCV2.G);
                Secp256k1ECCV3.Point resultV3 = Secp256k1ECCV3.multiply(scalar, Secp256k1ECCV3.G);
                Secp256k1ECCV4.Point resultV4 = Secp256k1ECCV4.multiply(scalar, Secp256k1ECCV4.G);

                // All versions should produce identical results
                assertEquals(resultV2.x, resultV3.x,
                    String.format("V2 and V3 should agree on scalar %s * G.x", scalar));
                assertEquals(resultV2.y, resultV3.y,
                    String.format("V2 and V3 should agree on scalar %s * G.y", scalar));
                assertEquals(resultV2.x, resultV4.x,
                    String.format("V2 and V4 should agree on scalar %s * G.x", scalar));
                assertEquals(resultV2.y, resultV4.y,
                    String.format("V2 and V4 should agree on scalar %s * G.y", scalar));

                // All results should be on curve
                assertTrue(Secp256k1ECCV2.isOnCurve(resultV2),
                    String.format("V2 result for scalar %s should be on curve", scalar));
                assertTrue(resultV3.isOnCurve(),
                    String.format("V3 result for scalar %s should be on curve", scalar));
                assertTrue(resultV4.isOnCurve(),
                    String.format("V4 result for scalar %s should be on curve", scalar));
            }

            logger.info("✅ Scalar multiplication consistency verified");
        }
    }

    @Nested
    @DisplayName("Version-Specific Feature Validation")
    class VersionSpecificFeaturesTest {

        @Test
        @DisplayName("V3 pre-computed points optimization")
        void testV3PrecomputedPoints() {
            logger.info("🔍 Testing V3 pre-computed points optimization");

            // V3 should use pre-computed points for small multiples
            Secp256k1ECCV3.Point g2_precomputed = Secp256k1ECCV3.calculate2G();
            Secp256k1ECCV3.Point g2_scalar = Secp256k1ECCV3.multiply(BigInteger.valueOf(2), Secp256k1ECCV3.G);

            assertEquals(g2_precomputed.x, g2_scalar.x, "Pre-computed 2G should equal scalar multiplication");
            assertEquals(g2_precomputed.y, g2_scalar.y, "Pre-computed 2G should equal scalar multiplication");

            // Test that pre-computed points are actually used (performance would show this)
            long startTime = System.nanoTime();
            for (int i = 0; i < 1000; i++) {
                Secp256k1ECCV3.multiply(BigInteger.valueOf(2), Secp256k1ECCV3.G);
            }
            long endTime = System.nanoTime();

            logger.info(String.format("V3 pre-computed 2G operations took %.2f μs total",
                (endTime - startTime) / 1000.0));

            logger.info("✅ V3 pre-computed points optimization verified");
        }

        @Test
        @DisplayName("V3 batch operations")
        void testV3BatchOperations() {
            logger.info("🔍 Testing V3 batch operations");

            List<BigInteger> scalars = List.of(
                BigInteger.ONE,
                BigInteger.valueOf(2),
                BigInteger.valueOf(3)
            );
            List<Secp256k1ECCV3.Point> points = List.of(
                Secp256k1ECCV3.G,
                Secp256k1ECCV3.G,
                Secp256k1ECCV3.G
            );

            List<Secp256k1ECCV3.Point> batchResults = Secp256k1ECCV3.batchMultiply(scalars, points);

            assertEquals(3, batchResults.size(), "Batch operation should return correct number of results");

            // Compare with individual operations
            for (int i = 0; i < scalars.size(); i++) {
                Secp256k1ECCV3.Point individual = Secp256k1ECCV3.multiply(scalars.get(i), points.get(i));
                assertEquals(individual.x, batchResults.get(i).x,
                    String.format("Batch and individual results should match for index %d", i));
                assertEquals(individual.y, batchResults.get(i).y,
                    String.format("Batch and individual results should match for index %d", i));
            }

            logger.info("✅ V3 batch operations verified");
        }

        @Test
        @DisplayName("V4 constant-time operations")
        void testV4ConstantTimeOperations() {
            logger.info("🔍 Testing V4 constant-time operations");

            // Test that operations take similar time regardless of input
            BigInteger smallScalar = BigInteger.valueOf(2);
            BigInteger largeScalar = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140", 16);

            long timeSmall = measureOperationTime(() ->
                Secp256k1ECCV4.multiply(smallScalar, Secp256k1ECCV4.G));
            long timeLarge = measureOperationTime(() ->
                Secp256k1ECCV4.multiply(largeScalar, Secp256k1ECCV4.G));

            double timeRatio = (double) Math.max(timeSmall, timeLarge) / Math.min(timeSmall, timeLarge);

            logger.info(String.format("V4 timing ratio (large/small): %.2f", timeRatio));

            // Constant-time operations should not vary too much (allowing for some system noise)
            assertTrue(timeRatio < 2.0, "V4 operations should be reasonably constant-time");

            logger.info("✅ V4 constant-time operations verified");
        }

        @Test
        @DisplayName("V4 secure random generation")
        void testV4SecureRandomGeneration() {
            logger.info("🔍 Testing V4 secure random generation");

            BigInteger scalar1 = Secp256k1ECCV4.generateSecureRandomScalar();
            BigInteger scalar2 = Secp256k1ECCV4.generateSecureRandomScalar();

            // Scalars should be different (very high probability)
            assertNotEquals(scalar1, scalar2, "Generated scalars should be different");

            // Scalars should be in valid range
            assertTrue(scalar1.compareTo(BigInteger.ONE) >= 0, "Scalar1 should be >= 1");
            assertTrue(scalar1.compareTo(Secp256k1ECCV4.N.subtract(BigInteger.ONE)) <= 0,
                "Scalar1 should be <= N-1");
            assertTrue(scalar2.compareTo(BigInteger.ONE) >= 0, "Scalar2 should be >= 1");
            assertTrue(scalar2.compareTo(Secp256k1ECCV4.N.subtract(BigInteger.ONE)) <= 0,
                "Scalar2 should be <= N-1");

            // Generated points should be valid
            Secp256k1ECCV4.Point randomPoint = Secp256k1ECCV4.generateSecureRandomPoint();
            assertTrue(Secp256k1ECCV4.validatePoint(randomPoint), "Random point should be valid");
            assertTrue(randomPoint.isOnCurve(), "Random point should be on curve");

            logger.info("✅ V4 secure random generation verified");
        }

        @Test
        @DisplayName("V4 secure batch operations")
        void testV4SecureBatchOperations() {
            logger.info("🔍 Testing V4 secure batch operations");

            BigInteger[] scalars = {
                BigInteger.ONE,
                BigInteger.valueOf(2),
                BigInteger.valueOf(3)
            };
            Secp256k1ECCV4.Point[] points = {
                Secp256k1ECCV4.G,
                Secp256k1ECCV4.G,
                Secp256k1ECCV4.G
            };

            Secp256k1ECCV4.Point[] secureBatchResults = Secp256k1ECCV4.secureBatchMultiply(scalars, points);

            assertEquals(3, secureBatchResults.length, "Secure batch should return correct number of results");

            // All results should be valid points
            for (int i = 0; i < secureBatchResults.length; i++) {
                assertTrue(Secp256k1ECCV4.validatePoint(secureBatchResults[i]),
                    String.format("Secure batch result %d should be valid", i));
                assertTrue(secureBatchResults[i].isOnCurve(),
                    String.format("Secure batch result %d should be on curve", i));
            }

            logger.info("✅ V4 secure batch operations verified");
        }
    }

    @Nested
    @DisplayName("Cross-Version Compatibility")
    class CrossVersionCompatibilityTest {

        @Test
        @DisplayName("Coordinate string representation compatibility")
        void testCoordinateStringRepresentation() {
            logger.info("🔍 Testing coordinate string representation compatibility");

            Secp256k1ECCV2.Point gV2 = Secp256k1ECCV2.G;
            Secp256k1ECCV3.Point gV3 = Secp256k1ECCV3.G;
            Secp256k1ECCV4.Point gV4 = Secp256k1ECCV4.G;

            // All versions should produce the same string representation for G
            String expectedGX = "0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
            String expectedGY = "0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8";

            assertTrue(gV2.toString().contains(expectedGX), "V2 G.x should be in string representation");
            assertTrue(gV2.toString().contains(expectedGY), "V2 G.y should be in string representation");
            assertTrue(gV3.toString().contains(expectedGX), "V3 G.x should be in string representation");
            assertTrue(gV3.toString().contains(expectedGY), "V3 G.y should be in string representation");
            assertTrue(gV4.toString().contains(expectedGX), "V4 G.x should be in string representation");
            assertTrue(gV4.toString().contains(expectedGY), "V4 G.y should be in string representation");

            logger.info("✅ Coordinate string representation compatibility verified");
        }

        @Test
        @DisplayName("Curve parameter consistency")
        void testCurveParameterConsistency() {
            logger.info("🔍 Testing curve parameter consistency across versions");

            // All versions should use identical curve parameters
            assertEquals(Secp256k1ECCV2.P, Secp256k1ECCV3.P, "P should be identical across versions");
            assertEquals(Secp256k1ECCV2.A, Secp256k1ECCV3.A, "A should be identical across versions");
            assertEquals(Secp256k1ECCV2.B, Secp256k1ECCV3.B, "B should be identical across versions");
            assertEquals(Secp256k1ECCV2.N, Secp256k1ECCV3.N, "N should be identical across versions");
            assertEquals(Secp256k1ECCV2.GX, Secp256k1ECCV3.GX, "GX should be identical across versions");
            assertEquals(Secp256k1ECCV2.GY, Secp256k1ECCV3.GY, "GY should be identical across versions");

            assertEquals(Secp256k1ECCV2.P, Secp256k1ECCV4.P, "P should be identical across versions");
            assertEquals(Secp256k1ECCV2.A, Secp256k1ECCV4.A, "A should be identical across versions");
            assertEquals(Secp256k1ECCV2.B, Secp256k1ECCV4.B, "B should be identical across versions");
            assertEquals(Secp256k1ECCV2.N, Secp256k1ECCV4.N, "N should be identical across versions");
            assertEquals(Secp256k1ECCV2.GX, Secp256k1ECCV4.GX, "GX should be identical across versions");
            assertEquals(Secp256k1ECCV2.GY, Secp256k1ECCV4.GY, "GY should be identical across versions");

            logger.info("✅ Curve parameter consistency verified");
        }

        @Test
        @DisplayName("Point validation consistency")
        void testPointValidationConsistency() {
            logger.info("🔍 Testing point validation consistency across versions");

            Secp256k1ECCV2.Point testPoint = new Secp256k1ECCV2.Point(BigInteger.ONE, BigInteger.TWO);
            Secp256k1ECCV3.Point testPointV3 = new Secp256k1ECCV3.Point(BigInteger.ONE, BigInteger.TWO);
            Secp256k1ECCV4.Point testPointV4 = new Secp256k1ECCV4.Point(BigInteger.ONE, BigInteger.TWO);

            boolean v2Valid = Secp256k1ECCV2.isOnCurve(testPoint);
            boolean v3Valid = testPointV3.isOnCurve();
            boolean v4Valid = testPointV4.isOnCurve();

            // All versions should agree on point validity
            assertEquals(v2Valid, v3Valid, "V2 and V3 should agree on point validity");
            assertEquals(v2Valid, v4Valid, "V2 and V4 should agree on point validity");

            // Test with generator point
            boolean v2GValid = Secp256k1ECCV2.isOnCurve(Secp256k1ECCV2.G);
            boolean v3GValid = Secp256k1ECCV3.G.isOnCurve();
            boolean v4GValid = Secp256k1ECCV4.G.isOnCurve();

            assertTrue(v2GValid, "V2 should validate generator point");
            assertTrue(v3GValid, "V3 should validate generator point");
            assertTrue(v4GValid, "V4 should validate generator point");

            logger.info("✅ Point validation consistency verified");
        }
    }

    @Nested
    @DisplayName("Performance Comparison Integration")
    class PerformanceComparisonTest {

        @Test
        @DisplayName("Integrated performance comparison")
        void testIntegratedPerformanceComparison() {
            logger.info("🔍 Running integrated performance comparison");

            // Run quick benchmark to compare versions
            assertDoesNotThrow(() -> Secp256k1ECCBenchmark.runQuickDemo(),
                "Quick benchmark demo should execute successfully");

            // Run full benchmark suite
            List<Secp256k1ECCBenchmark.BenchmarkResult> results =
                assertDoesNotThrow(() -> Secp256k1ECCBenchmark.runFullBenchmark(),
                    "Full benchmark should execute successfully");

            assertNotNull(results, "Benchmark results should not be null");
            assertFalse(results.isEmpty(), "Benchmark should produce results");

            // Verify we have results for all versions
            boolean hasV2Results = results.stream().anyMatch(r -> "V2".equals(r.version));
            boolean hasV3Results = results.stream().anyMatch(r -> "V3".equals(r.version));
            boolean hasV4Results = results.stream().anyMatch(r -> "V4".equals(r.version));

            assertTrue(hasV2Results, "Should have V2 benchmark results");
            assertTrue(hasV3Results, "Should have V3 benchmark results");
            assertTrue(hasV4Results, "Should have V4 benchmark results");

            // Print comparison table
            assertDoesNotThrow(() -> Secp256k1ECCBenchmark.printComparisonTable(results),
                "Comparison table should generate successfully");

            logger.info(String.format("✅ Performance comparison completed with %d benchmark results", results.size()));
        }
    }

    @Test
    @DisplayName("Complete cross-version integration test")
    void testCompleteCrossVersionIntegration() {
        logger.info("=== 🔬 Complete Cross-Version Integration Test ===");

        // Test mathematical equivalence
        testGeneratorPointConsistency();
        test2GCoordinateEquivalence();
        test3GCoordinateEquivalence();
        testScalarMultiplicationConsistency();

        // Test version-specific features
        testV3PrecomputedPoints();
        testV3BatchOperations();
        testV4ConstantTimeOperations();
        testV4SecureRandomGeneration();
        testV4SecureBatchOperations();

        // Test cross-version compatibility
        testCoordinateStringRepresentation();
        testCurveParameterConsistency();
        testPointValidationConsistency();

        // Test performance comparison
        testIntegratedPerformanceComparison();

        logger.info("✅ All cross-version integration tests passed");
        logger.info("🎉 secp256k1 ECC implementation suite is fully functional and consistent");
    }

    // ===== HELPER METHODS =====

    private long measureOperationTime(Runnable operation) {
        long startTime = System.nanoTime();
        operation.run();
        long endTime = System.nanoTime();
        return endTime - startTime;
    }
}
