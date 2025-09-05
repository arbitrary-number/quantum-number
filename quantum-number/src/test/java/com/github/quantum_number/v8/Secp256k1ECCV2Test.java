package com.github.quantum_number.v8;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import java.math.BigInteger;
import java.util.logging.Logger;
import java.util.logging.ConsoleHandler;
import java.util.logging.Level;
import java.nio.charset.StandardCharsets;

import static org.junit.jupiter.api.Assertions.*;

/**
 * JUnit 5 test demonstrating secp256k1 elliptic curve coordinates G, 2G, and 3G.
 * Uses UTF-8 encoding for mathematical symbols and formulas in logging.
 */
@DisplayName("secp256k1 Elliptic Curve Coordinate Tests")
public class Secp256k1ECCV2Test {

    private static final Logger logger = Logger.getLogger(Secp256k1ECCV2Test.class.getName());

    @BeforeEach
    void setupLogging() {
        // Configure UTF-8 encoding for console output
        ConsoleHandler handler = new ConsoleHandler();
        handler.setEncoding(StandardCharsets.UTF_8.toString());
        handler.setLevel(Level.ALL);
        logger.addHandler(handler);
        logger.setLevel(Level.ALL);
        logger.setUseParentHandlers(false);
    }

    @Test
    @DisplayName("Generator Point G is on secp256k1 curve")
    void testGeneratorPointOnCurve() {
        logger.info("🔢 Testing Generator Point G ∈ secp256k1");

        Secp256k1ECCV2.Point g = Secp256k1ECCV2.G;
        assertNotNull(g, "Generator point should not be null");
        assertFalse(g.infinity, "Generator point should not be infinity");

        boolean onCurve = Secp256k1ECCV2.isOnCurve(g);
        logger.info(String.format("G = %s", g));
        logger.info(String.format("G ∈ E: %s", onCurve ? "✓" : "✗"));
        assertTrue(onCurve, "Generator point G should be on the secp256k1 curve");
    }

    @Test
    @DisplayName("Calculate and verify 2G = G + G coordinates")
    void testCalculate2G() {
        logger.info("🔢 Testing 2G = G + G coordinate calculation");

        Secp256k1ECCV2.Point g = Secp256k1ECCV2.G;
        Secp256k1ECCV2.Point g2 = Secp256k1ECCV2.calculate2G();

        assertNotNull(g2, "2G should not be null");
        assertFalse(g2.infinity, "2G should not be infinity");

        // Verify 2G is on curve
        boolean onCurve = Secp256k1ECCV2.isOnCurve(g2);
        logger.info(String.format("G = %s", g));
        logger.info(String.format("2G = G + G = %s", g2));
        logger.info(String.format("2G ∈ E: %s", onCurve ? "✓" : "✗"));
        assertTrue(onCurve, "Point 2G should be on the secp256k1 curve");

        // Verify 2G = 2 * G using scalar multiplication
        Secp256k1ECCV2.Point g2_scalar = Secp256k1ECCV2.multiply(BigInteger.valueOf(2), g);
        assertEquals(g2.x, g2_scalar.x, "2G x-coordinate should match scalar multiplication");
        assertEquals(g2.y, g2_scalar.y, "2G y-coordinate should match scalar multiplication");
        logger.info("✓ 2G verified via scalar multiplication");
    }

    @Test
    @DisplayName("Calculate and verify 3G = 2G + G coordinates")
    void testCalculate3G() {
        logger.info("🔢 Testing 3G = 2G + G coordinate calculation");

        Secp256k1ECCV2.Point g = Secp256k1ECCV2.G;
        Secp256k1ECCV2.Point g2 = Secp256k1ECCV2.calculate2G();
        Secp256k1ECCV2.Point g3 = Secp256k1ECCV2.calculate3G();

        assertNotNull(g3, "3G should not be null");
        assertFalse(g3.infinity, "3G should not be infinity");

        // Verify 3G is on curve
        boolean onCurve = Secp256k1ECCV2.isOnCurve(g3);
        logger.info(String.format("G = %s", g));
        logger.info(String.format("2G = %s", g2));
        logger.info(String.format("3G = 2G + G = %s", g3));
        logger.info(String.format("3G ∈ E: %s", onCurve ? "✓" : "✗"));
        assertTrue(onCurve, "Point 3G should be on the secp256k1 curve");

        // Verify 3G = 3 * G using scalar multiplication
        Secp256k1ECCV2.Point g3_scalar = Secp256k1ECCV2.multiply(BigInteger.valueOf(3), g);
        assertEquals(g3.x, g3_scalar.x, "3G x-coordinate should match scalar multiplication");
        assertEquals(g3.y, g3_scalar.y, "3G y-coordinate should match scalar multiplication");
        logger.info("✓ 3G verified via scalar multiplication");
    }

    @Test
    @DisplayName("Verify point addition and scalar multiplication consistency")
    void testPointArithmeticConsistency() {
        logger.info("🔢 Testing point arithmetic consistency");

        Secp256k1ECCV2.Point g = Secp256k1ECCV2.G;

        // Test: 3G = G + G + G
        Secp256k1ECCV2.Point g3_manual = Secp256k1ECCV2.add(Secp256k1ECCV2.add(g, g), g);
        Secp256k1ECCV2.Point g3_scalar = Secp256k1ECCV2.multiply(BigInteger.valueOf(3), g);

        assertEquals(g3_manual.x, g3_scalar.x, "Manual addition should equal scalar multiplication for 3G");
        assertEquals(g3_manual.y, g3_scalar.y, "Manual addition should equal scalar multiplication for 3G");

        logger.info(String.format("G + G + G = %s", g3_manual));
        logger.info(String.format("3 × G = %s", g3_scalar));
        logger.info("✓ Point addition and scalar multiplication are consistent");
    }

    @Test
    @DisplayName("Demonstrate coordinate progression G → 2G → 3G")
    void testCoordinateProgression() {
        logger.info("🔢 Demonstrating secp256k1 coordinate progression");

        Secp256k1ECCV2.Point g = Secp256k1ECCV2.G;
        Secp256k1ECCV2.Point g2 = Secp256k1ECCV2.calculate2G();
        Secp256k1ECCV2.Point g3 = Secp256k1ECCV2.calculate3G();

        logger.info("=== secp256k1 Generator Point Progression ===");
        logger.info(String.format("G₁ = %s", g));
        logger.info(String.format("G₂ = 2G₁ = %s", g2));
        logger.info(String.format("G₃ = 3G₁ = %s", g3));

        // Verify all points are distinct
        assertNotEquals(g.x, g2.x, "G and 2G should have different x-coordinates");
        assertNotEquals(g.x, g3.x, "G and 3G should have different x-coordinates");
        assertNotEquals(g2.x, g3.x, "2G and 3G should have different x-coordinates");

        logger.info("✓ All generator multiples have distinct coordinates");
    }

    @Test
    @DisplayName("Log complete coordinate demonstration")
    void testLogCoordinates() {
        logger.info("🔢 Running complete coordinate demonstration");
        Secp256k1ECCV2.logCoordinates();
        logger.info("✓ Coordinate demonstration completed");
    }
}
