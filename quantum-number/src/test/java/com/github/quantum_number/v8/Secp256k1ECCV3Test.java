package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Nested;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;
import java.math.BigInteger;
import java.util.logging.Logger;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.util.List;
import java.util.ArrayList;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Comprehensive JUnit 5 test suite for Secp256k1ECCV3 enhanced implementation.
 * Tests performance optimizations, batch operations, and advanced validation.
 */
@DisplayName("secp256k1 Enhanced ECC Implementation Tests")
public class Secp256k1ECCV3Test {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCV3Test.class.getName());

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
    @DisplayName("Point Class Enhancements")
    class PointEnhancementsTest {

        @Test
        @DisplayName("Point equality and hashCode consistency")
        void testPointEqualityAndHashCode() {
            Secp256k1ECCV3.Point p1 = new Secp256k1ECCV3.Point(BigInteger.ONE, BigInteger.TWO);
            Secp256k1ECCV3.Point p2 = new Secp256k1ECCV3.Point(BigInteger.ONE, BigInteger.TWO);
            Secp256k1ECCV3.Point p3 = new Secp256k1ECCV3.Point(BigInteger.TWO, BigInteger.ONE);

            assertEquals(p1, p2, "Equal points should be equal");
            assertEquals(p1.hashCode(), p2.hashCode(), "Equal points should have equal hash codes");
            assertNotEquals(p1, p3, "Different points should not be equal");
        }

        @Test
        @DisplayName("Point isOnCurve method")
        void testPointIsOnCurve() {
            Secp256k1ECCV3.Point g = Secp256k1ECCV3.G;
            Secp256k1ECCV3.Point infinity = new Secp256k1ECCV3.Point(true);

            assertTrue(g.isOnCurve(), "Generator point should be on curve");
            assertTrue(infinity.isOnCurve(), "Point at infinity should be on curve");

            // Test invalid point
            Secp256k1ECCV3.Point invalid = new Secp256k1ECCV3.Point(BigInteger.ONE, BigInteger.ONE);
            assertFalse(invalid.isOnCurve(), "Invalid point should not be on curve");
        }

        @Test
        @DisplayName("Compressed point representation")
        void testCompressedRepresentation() {
            Secp256k1ECCV3.Point g = Secp256k1ECCV3.G;
            byte[] compressed = g.toCompressedBytes();

            assertEquals(33, compressed.length, "Compressed representation should be 33 bytes");
            assertTrue(compressed[0] == 0x02 || compressed[0] == 0x03, "First byte should indicate even/odd y");

            // Test infinity
            Secp256k1ECCV3.Point infinity = new Secp256k1ECCV3.Point(true);
            byte[] infinityCompressed = infinity.toCompressedBytes();
            assertEquals(1, infinityCompressed.length, "Infinity should compress to 1 byte");
            assertEquals(0x00, infinityCompressed[0], "Infinity should be represented as 0x00");
        }
    }

    @Nested
    @DisplayName("Pre-computed Points")
    class PrecomputedPointsTest {

        @Test
        @DisplayName("Pre-computed points are correctly calculated")
        void testPrecomputedPoints() {
            Secp256k1ECCV3.Point g = Secp256k1ECCV3.G;
            Secp256k1ECCV3.Point g2 = Secp256k1ECCV3.calculate2G();
            Secp256k1ECCV3.Point g3 = Secp256k1ECCV3.calculate3G();
            Secp256k1ECCV3.Point g4 = Secp256k1ECCV3.calculate4G();

            // Verify 2G = G + G
            assertEquals(Secp256k1ECCV3.add(g, g), g2, "2G should equal G + G");

            // Verify 3G = 2G + G
            assertEquals(Secp256k1ECCV3.add(g2, g), g3, "3G should equal 2G + G");

            // Verify 4G = 3G + G
            assertEquals(Secp256k1ECCV3.add(g3, g), g4, "4G should equal 3G + G");
        }

        @Test
        @DisplayName("Scalar multiplication uses pre-computed points")
        void testScalarMultiplicationOptimization() {
            Secp256k1ECCV3.Point g = Secp256k1ECCV3.G;

            // Test small multiples use pre-computed points
            assertEquals(Secp256k1ECCV3.G, Secp256k1ECCV3.multiply(BigInteger.ONE, g));
            assertEquals(Secp256k1ECCV3.calculate2G(), Secp256k1ECCV3.multiply(BigInteger.valueOf(2), g));
            assertEquals(Secp256k1ECCV3.calculate3G(), Secp256k1ECCV3.multiply(BigInteger.valueOf(3), g));
            assertEquals(Secp256k1ECCV3.calculate4G(), Secp256k1ECCV3.multiply(BigInteger.valueOf(4), g));
        }
    }

    @Nested
    @DisplayName("Batch Operations")
    class BatchOperationsTest {

        @Test
        @DisplayName("Batch scalar multiplication")
        void testBatchScalarMultiplication() {
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

            List<Secp256k1ECCV3.Point> results = Secp256k1ECCV3.batchMultiply(scalars, points);

            assertEquals(3, results.size(), "Should return same number of results as inputs");
            assertEquals(Secp256k1ECCV3.G, results.get(0), "1 * G should equal G");
            assertEquals(Secp256k1ECCV3.calculate2G(), results.get(1), "2 * G should equal 2G");
            assertEquals(Secp256k1ECCV3.calculate3G(), results.get(2), "3 * G should equal 3G");
        }

        @Test
        @DisplayName("Batch operation input validation")
        void testBatchOperationValidation() {
            List<BigInteger> scalars = List.of(BigInteger.ONE, BigInteger.valueOf(2));
            List<Secp256k1ECCV3.Point> points = List.of(Secp256k1ECCV3.G); // Different size

            assertThrows(IllegalArgumentException.class,
                () -> Secp256k1ECCV3.batchMultiply(scalars, points),
                "Should throw exception for mismatched input sizes");
        }
    }

    @Nested
    @DisplayName("Sequence Generation")
    class SequenceGenerationTest {

        @Test
        @DisplayName("Generate point sequence")
        void testGeneratePointSequence() {
            List<Secp256k1ECCV3.Point> sequence = Secp256k1ECCV3.generatePointSequence(3);

            assertEquals(4, sequence.size(), "Sequence should include 0G through 3G");

            // Verify sequence: 0G, 1G, 2G, 3G
            assertTrue(sequence.get(0).infinity, "0G should be point at infinity");
            assertEquals(Secp256k1ECCV3.G, sequence.get(1), "1G should equal G");
            assertEquals(Secp256k1ECCV3.calculate2G(), sequence.get(2), "2G should be correct");
            assertEquals(Secp256k1ECCV3.calculate3G(), sequence.get(3), "3G should be correct");
        }

        @Test
        @DisplayName("Validate point sequence")
        void testValidateSequence() {
            List<Secp256k1ECCV3.Point> validSequence = Secp256k1ECCV3.generatePointSequence(5);
            assertTrue(Secp256k1ECCV3.validateSequence(validSequence), "Valid sequence should pass validation");

            // Create invalid sequence
            List<Secp256k1ECCV3.Point> invalidSequence = new ArrayList<>(validSequence);
            invalidSequence.set(2, new Secp256k1ECCV3.Point(BigInteger.ONE, BigInteger.ONE)); // Invalid point
            assertFalse(Secp256k1ECCV3.validateSequence(invalidSequence), "Invalid sequence should fail validation");
        }
    }

    @Nested
    @DisplayName("Advanced Arithmetic")
    class AdvancedArithmeticTest {

        @Test
        @DisplayName("Point addition associativity")
        void testPointAdditionAssociativity() {
            Secp256k1ECCV3.Point g = Secp256k1ECCV3.G;
            Secp256k1ECCV3.Point g2 = Secp256k1ECCV3.calculate2G();
            Secp256k1ECCV3.Point g3 = Secp256k1ECCV3.calculate3G();

            // Test (G + 2G) + 3G = G + (2G + 3G)
            Secp256k1ECCV3.Point left = Secp256k1ECCV3.add(Secp256k1ECCV3.add(g, g2), g3);
            Secp256k1ECCV3.Point right = Secp256k1ECCV3.add(g, Secp256k1ECCV3.add(g2, g3));

            assertEquals(left, right, "Point addition should be associative");
        }

        @Test
        @DisplayName("Scalar multiplication distributivity")
        void testScalarMultiplicationDistributivity() {
            Secp256k1ECCV3.Point g = Secp256k1ECCV3.G;

            // Test 5G = G + 2G + 3G + 4G + G
            Secp256k1ECCV3.Point manual = Secp256k1ECCV3.add(
                Secp256k1ECCV3.add(
                    Secp256k1ECCV3.add(
                        Secp256k1ECCV3.add(g, Secp256k1ECCV3.calculate2G()),
                        Secp256k1ECCV3.calculate3G()
                    ),
                    Secp256k1ECCV3.calculate4G()
                ),
                g
            );

            Secp256k1ECCV3.Point scalar = Secp256k1ECCV3.multiply(BigInteger.valueOf(5), g);

            assertEquals(manual, scalar, "Manual addition should equal scalar multiplication");
        }

        @Test
        @DisplayName("Verify arithmetic properties")
        void testVerifyArithmetic() {
            boolean result = Secp256k1ECCV3.verifyArithmetic();
            assertTrue(result, "All arithmetic properties should hold");
        }
    }

    @Nested
    @DisplayName("Performance and Logging")
    class PerformanceAndLoggingTest {

        @Test
        @DisplayName("Comprehensive coordinate logging")
        void testComprehensiveCoordinateLogging() {
            // This test mainly ensures the logging method doesn't throw exceptions
            assertDoesNotThrow(() -> Secp256k1ECCV3.logComprehensiveCoordinates(),
                "Comprehensive logging should not throw exceptions");
        }

        @ParameterizedTest
        @ValueSource(ints = {1, 2, 3, 4, 5})
        @DisplayName("Performance test for scalar multiplication")
        void testScalarMultiplicationPerformance(int multiplier) {
            Secp256k1ECCV3.Point g = Secp256k1ECCV3.G;

            long startTime = System.nanoTime();
            Secp256k1ECCV3.Point result = Secp256k1ECCV3.multiply(BigInteger.valueOf(multiplier), g);
            long endTime = System.nanoTime();

            double durationMs = (endTime - startTime) / 1_000_000.0;

            assertNotNull(result, "Result should not be null");
            assertTrue(result.isOnCurve(), "Result should be on curve");
            assertTrue(durationMs < 100.0, "Scalar multiplication should complete within 100ms");

            logger.info(String.format("Scalar multiplication %d * G took %.3f ms", multiplier, durationMs));
        }

        @Test
        @DisplayName("Batch operation performance")
        void testBatchOperationPerformance() {
            List<BigInteger> scalars = List.of(
                BigInteger.valueOf(1),
                BigInteger.valueOf(2),
                BigInteger.valueOf(3),
                BigInteger.valueOf(4),
                BigInteger.valueOf(5)
            );
            List<Secp256k1ECCV3.Point> points = List.of(
                Secp256k1ECCV3.G,
                Secp256k1ECCV3.G,
                Secp256k1ECCV3.G,
                Secp256k1ECCV3.G,
                Secp256k1ECCV3.G
            );

            long startTime = System.nanoTime();
            List<Secp256k1ECCV3.Point> results = Secp256k1ECCV3.batchMultiply(scalars, points);
            long endTime = System.nanoTime();

            double durationMs = (endTime - startTime) / 1_000_000.0;

            assertEquals(5, results.size(), "Should return correct number of results");
            assertTrue(durationMs < 500.0, "Batch operation should complete within 500ms");

            logger.info(String.format("Batch multiplication took %.3f ms", durationMs));
        }
    }

    @Nested
    @DisplayName("Edge Cases and Error Handling")
    class EdgeCasesTest {

        @Test
        @DisplayName("Zero scalar multiplication")
        void testZeroScalarMultiplication() {
            Secp256k1ECCV3.Point result = Secp256k1ECCV3.multiply(BigInteger.ZERO, Secp256k1ECCV3.G);
            assertTrue(result.infinity, "Zero scalar should result in point at infinity");
        }

        @Test
        @DisplayName("Identity element (infinity) operations")
        void testInfinityOperations() {
            Secp256k1ECCV3.Point infinity = new Secp256k1ECCV3.Point(true);
            Secp256k1ECCV3.Point g = Secp256k1ECCV3.G;

            // Infinity + G = G
            assertEquals(g, Secp256k1ECCV3.add(infinity, g));

            // G + Infinity = G
            assertEquals(g, Secp256k1ECCV3.add(g, infinity));

            // Infinity + Infinity = Infinity
            assertEquals(infinity, Secp256k1ECCV3.add(infinity, infinity));
        }

        @Test
        @DisplayName("Large scalar multiplication")
        void testLargeScalarMultiplication() {
            BigInteger largeScalar = new BigInteger("123456789012345678901234567890");
            Secp256k1ECCV3.Point result = Secp256k1ECCV3.multiply(largeScalar, Secp256k1ECCV3.G);

            assertNotNull(result, "Large scalar multiplication should succeed");
            assertTrue(result.isOnCurve(), "Result should be on curve");
            assertFalse(result.infinity, "Result should not be infinity for large scalar");
        }
    }

    @Test
    @DisplayName("Complete integration test")
    void testCompleteIntegration() {
        logger.info("=== 🔬 Complete Secp256k1ECCV3 Integration Test ===");

        // Test all major features
        Secp256k1ECCV3.Point g = Secp256k1ECCV3.G;
        Secp256k1ECCV3.Point g2 = Secp256k1ECCV3.calculate2G();
        Secp256k1ECCV3.Point g3 = Secp256k1ECCV3.calculate3G();

        // Verify coordinates
        assertTrue(Secp256k1ECCV3.isOnCurve(g), "G should be on curve");
        assertTrue(Secp256k1ECCV3.isOnCurve(g2), "2G should be on curve");
        assertTrue(Secp256k1ECCV3.isOnCurve(g3), "3G should be on curve");

        // Test sequence generation
        List<Secp256k1ECCV3.Point> sequence = Secp256k1ECCV3.generatePointSequence(3);
        assertTrue(Secp256k1ECCV3.validateSequence(sequence), "Generated sequence should be valid");

        // Test batch operations
        List<BigInteger> scalars = List.of(BigInteger.ONE, BigInteger.valueOf(2), BigInteger.valueOf(3));
        List<Secp256k1ECCV3.Point> points = List.of(g, g, g);
        List<Secp256k1ECCV3.Point> batchResults = Secp256k1ECCV3.batchMultiply(scalars, points);
        assertEquals(3, batchResults.size(), "Batch operation should return correct number of results");

        // Test arithmetic verification
        assertTrue(Secp256k1ECCV3.verifyArithmetic(), "Arithmetic properties should hold");

        logger.info("✅ All integration tests passed");
    }
}
