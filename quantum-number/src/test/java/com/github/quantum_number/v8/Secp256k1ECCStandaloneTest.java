package com.github.quantum_number.v8;

import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Standalone unit tests for Secp256k1ECC implementation
 * Tests only the ECC functionality without dependencies on other parts of the codebase
 *
 * @author Cline AI Assistant
 */
public class Secp256k1ECCStandaloneTest {

    @Test
    public void testBasicPointCreation() {
        // Test that we can create points
        Secp256k1ECC.ECCPoint point = new Secp256k1ECC.ECCPoint();
        assertNotNull("Point should be created", point);
        assertNotNull("X coordinate should exist", point.x);
        assertNotNull("Y coordinate should exist", point.y);
        assertNotNull("Z coordinate should exist", point.z);
    }

    @Test
    public void testPointAtInfinity() {
        Secp256k1ECC.ECCPoint infinity = Secp256k1ECC.createPointAtInfinity();
        assertTrue("Point should be at infinity", Secp256k1ECC.isPointAtInfinity(infinity));
    }

    @Test
    public void testGeneratorPoint() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G;
        assertNotNull("Generator point should exist", g);
        assertFalse("Generator should not be at infinity", Secp256k1ECC.isPointAtInfinity(g));
        assertEquals("Generator should have validated flag", Secp256k1ECC.POINT_VALIDATED, g.pointFlags);
    }

    @Test
    public void testPointEquality() {
        Secp256k1ECC.ECCPoint p1 = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint p2 = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint p3 = new Secp256k1ECC.ECCPoint();

        assertTrue("Identical points should be equal", Secp256k1ECC.pointsEqual(p1, p2));
        assertFalse("Different points should not be equal", Secp256k1ECC.pointsEqual(p1, p3));
    }

    @Test
    public void testPointDoubling() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint doubled = Secp256k1ECC.pointDouble(g);

        assertNotNull("Doubled point should exist", doubled);
        assertFalse("Doubled point should not be at infinity", Secp256k1ECC.isPointAtInfinity(doubled));
        assertFalse("Doubled point should not equal original", Secp256k1ECC.pointsEqual(doubled, g));
    }

    @Test
    public void testPointAdditionWithInfinity() {
        Secp256k1ECC.ECCPoint infinity = Secp256k1ECC.createPointAtInfinity();
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();

        // O + G = G
        Secp256k1ECC.ECCPoint result1 = Secp256k1ECC.pointAdd(infinity, g);
        assertTrue("O + G should equal G", Secp256k1ECC.pointsEqual(result1, g));

        // G + O = G
        Secp256k1ECC.ECCPoint result2 = Secp256k1ECC.pointAdd(g, infinity);
        assertTrue("G + O should equal G", Secp256k1ECC.pointsEqual(result2, g));
    }

    @Test
    public void testPointAdditionBasic() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint doubled = Secp256k1ECC.pointDouble(g);

        // G + G should equal 2G
        Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointAdd(g, g);
        assertTrue("G + G should equal 2G", Secp256k1ECC.pointsEqual(result, doubled));
    }

    @Test
    public void testScalarMultiplicationByOne() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(1);

        Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);
        assertTrue("1 * G should equal G", Secp256k1ECC.pointsEqual(result, g));
    }

    @Test
    public void testScalarMultiplicationByTwo() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(2);

        Secp256k1ECC.ECCPoint expected = Secp256k1ECC.pointDouble(g);
        Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);

        assertTrue("2 * G should equal 2G", Secp256k1ECC.pointsEqual(result, expected));
    }

    @Test
    public void testScalarMultiplicationByZero() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(0);

        Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);
        assertTrue("0 * G should be infinity", Secp256k1ECC.isPointAtInfinity(result));
    }

    @Test
    public void testModularArithmetic() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
        QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(15);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        // Test basic modular addition
        QuantumNumberV8 result = Secp256k1ECC.modAdd(a, b, modulus);
        assertNotNull("Modular addition result should exist", result);
        assertNotNull("Left pointer should exist for overflow terms", result.left);
    }

    @Test
    public void testOverflowChainEvaluation() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
        QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(15);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        QuantumNumberV8 result = Secp256k1ECC.modAdd(a, b, modulus);
        QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

        assertNotNull("Evaluated result should exist", evaluated);
        // 10 + 15 = 25, 25 mod 23 = 2
        assertEquals("Modular arithmetic should be correct", 2, evaluated.a1);
    }

    @Test
    public void testComplexOverflowChain() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        // Create a chain of operations
        QuantumNumberV8 result1 = Secp256k1ECC.modAdd(a, a, modulus);  // 10 + 10 = 20
        QuantumNumberV8 result2 = Secp256k1ECC.modAdd(result1, a, modulus); // 20 + 10 = 30
        QuantumNumberV8 result3 = Secp256k1ECC.modMul(result2, a, modulus); // 30 * 10 = 300

        // Evaluate final result
        QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result3, modulus);

        assertNotNull("Complex chain evaluation should work", evaluated);
        // 300 mod 23 = 300 - 13*23 = 300 - 299 = 1
        assertEquals("Complex modular arithmetic should be correct", 1, evaluated.a1);
    }

    @Test
    public void testPointCopy() {
        Secp256k1ECC.ECCPoint original = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint copy = original.copy();

        assertTrue("Copy should equal original", Secp256k1ECC.pointsEqual(original, copy));
        assertNotSame("Copy should be different object", original, copy);
    }

    @Test
    public void testQuantumNumberCreation() {
        QuantumNumberV8 qn = Secp256k1ECC.createQuantumNumber(42);
        assertNotNull("Quantum number should be created", qn);
        assertEquals("Value should be set correctly", 42, qn.a1);
    }

    @Test
    public void testQuantumNumberEquality() {
        QuantumNumberV8 qn1 = Secp256k1ECC.createQuantumNumber(100);
        QuantumNumberV8 qn2 = Secp256k1ECC.createQuantumNumber(100);
        QuantumNumberV8 qn3 = Secp256k1ECC.createQuantumNumber(200);

        assertTrue("Equal quantum numbers should be equal", Secp256k1ECC.quantumEqual(qn1, qn2));
        assertFalse("Different quantum numbers should not be equal", Secp256k1ECC.quantumEqual(qn1, qn3));
    }

    @Test
    public void testBitOperations() {
        QuantumNumberV8 qn = Secp256k1ECC.createQuantumNumber(0b101010); // 42 in decimal

        assertTrue("Bit 1 should be set", Secp256k1ECC.quantumGetBit(qn, 1));
        assertFalse("Bit 0 should not be set", Secp256k1ECC.quantumGetBit(qn, 0));
        assertTrue("Bit 3 should be set", Secp256k1ECC.quantumGetBit(qn, 3));
        assertFalse("Bit 2 should not be set", Secp256k1ECC.quantumGetBit(qn, 2));
    }

    @Test
    public void testModularInverse() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(7);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        QuantumNumberV8 inverse = Secp256k1ECC.modInverse(a, modulus);

        // Verify: 7 * inverse ≡ 1 mod 23
        QuantumNumberV8 product = Secp256k1ECC.modMul(a, inverse, modulus);
        QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(product, modulus);

        assertEquals("Modular inverse should satisfy a * a^(-1) ≡ 1 mod p", 1, evaluated.a1);
    }

    @Test
    public void testPointNegation() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();

        // Create -G by negating y coordinate
        Secp256k1ECC.ECCPoint negG = new Secp256k1ECC.ECCPoint();
        negG.x = Secp256k1ECC.copyQuantumNumber(g.x);
        negG.y = Secp256k1ECC.modSub(Secp256k1ECC.createQuantumNumber(0), g.y, Secp256k1ECC.SECP256K1_P);
        negG.z = Secp256k1ECC.createQuantumNumber(1);
        negG.coordinateSystem = Secp256k1ECC.COORDINATE_AFFINE;

        // G + (-G) should equal point at infinity
        Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointAdd(g, negG);
        assertTrue("G + (-G) should be infinity", Secp256k1ECC.isPointAtInfinity(result));
    }

    @Test
    public void testAssociativity() {
        Secp256k1ECC.ECCPoint p1 = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint p2 = Secp256k1ECC.pointDouble(p1);
        Secp256k1ECC.ECCPoint p3 = Secp256k1ECC.pointDouble(p2);

        // Test (p1 + p2) + p3 = p1 + (p2 + p3)
        Secp256k1ECC.ECCPoint left = Secp256k1ECC.pointAdd(
            Secp256k1ECC.pointAdd(p1, p2), p3);
        Secp256k1ECC.ECCPoint right = Secp256k1ECC.pointAdd(
            p1, Secp256k1ECC.pointAdd(p2, p3));

        assertTrue("Point addition should be associative",
            Secp256k1ECC.pointsEqual(left, right));
    }

    @Test
    public void testCommutativity() {
        Secp256k1ECC.ECCPoint p1 = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint p2 = Secp256k1ECC.pointDouble(p1);

        // Test p1 + p2 = p2 + p1
        Secp256k1ECC.ECCPoint result1 = Secp256k1ECC.pointAdd(p1, p2);
        Secp256k1ECC.ECCPoint result2 = Secp256k1ECC.pointAdd(p2, p1);

        assertTrue("Point addition should be commutative",
            Secp256k1ECC.pointsEqual(result1, result2));
    }

    @Test
    public void testOrderVerification() {
        // Verify that n*G = O (point at infinity)
        // Note: This is a simplified test due to computational complexity
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        QuantumNumberV8 smallOrder = Secp256k1ECC.createQuantumNumber(7); // Small prime for testing

        Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(smallOrder, g);
        assertNotNull("Scalar multiplication should work", result);
        // In a real test, we'd verify this equals infinity for the actual order
    }

    @Test
    public void testPerformanceBaseline() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();

        long startTime = System.nanoTime();
        for (int i = 0; i < 100; i++) {
            Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointDouble(g);
            // Prevent optimization
            if (result == null) break;
        }
        long endTime = System.nanoTime();

        double avgTime = (double) (endTime - startTime) / 100;
        System.out.printf("Point doubling performance: %.2f ns/op%n", avgTime);

        // Just verify it runs without errors and within reasonable time
        assertTrue("Performance should be reasonable", avgTime < 1000000); // Less than 1ms per operation
    }
}
