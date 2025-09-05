package com.github.quantum_number.v8;

import org.junit.Test;
import org.junit.Before;
import org.junit.After;
import static org.junit.Assert.*;

/**
 * Comprehensive unit tests for Secp256k1ECC implementation
 * using QuantumNumberV8's left pointer overflow system.
 *
 * Tests focus on correctness, performance, and edge cases in airgapped environments.
 *
 * @author Cline AI Assistant
 */
public class Secp256k1ECCTest {

    private static final int PERFORMANCE_ITERATIONS = 1000;
    private static final int WARMUP_ITERATIONS = 100;

    @Before
    public void setUp() {
        // Warm up the JVM and preload curve parameters
        for (int i = 0; i < WARMUP_ITERATIONS; i++) {
            Secp256k1ECC.ECCPoint p1 = Secp256k1ECC.createPointAtInfinity();
            Secp256k1ECC.ECCPoint p2 = Secp256k1ECC.SECP256K1_G;
            Secp256k1ECC.pointAdd(p1, p2);
        }
    }

    @After
    public void tearDown() {
        // Clean up any resources if needed
        System.gc();
    }

    // ===== BASIC MODULAR ARITHMETIC TESTS =====

    @Test
    public void testModularAddition() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
        QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(15);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        QuantumNumberV8 result = Secp256k1ECC.modAdd(a, b, modulus);
        QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

        // 10 + 15 = 25, 25 mod 23 = 2
        assertEquals(2, evaluated.a1);
        assertTrue("Left pointer overflow chain should exist", result.left != null);
    }

    @Test
    public void testModularSubtraction() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
        QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(15);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        QuantumNumberV8 result = Secp256k1ECC.modSub(a, b, modulus);
        QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

        // 10 - 15 = -5, -5 mod 23 = 18
        assertEquals(18, evaluated.a1);
        assertTrue("Left pointer overflow chain should exist", result.left != null);
    }

    @Test
    public void testModularMultiplication() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(7);
        QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(8);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        QuantumNumberV8 result = Secp256k1ECC.modMul(a, b, modulus);
        QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

        // 7 * 8 = 56, 56 mod 23 = 10
        assertEquals(10, evaluated.a1);
        assertTrue("Left pointer overflow chain should exist", result.left != null);
    }

    @Test
    public void testModularInverse() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(7);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        QuantumNumberV8 inverse = Secp256k1ECC.modInverse(a, modulus);

        // Verify: 7 * inverse ≡ 1 mod 23
        QuantumNumberV8 product = Secp256k1ECC.modMul(a, inverse, modulus);
        QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(product, modulus);

        assertEquals(1, evaluated.a1);
    }

    // ===== POINT OPERATION TESTS =====

    @Test
    public void testPointAtInfinity() {
        Secp256k1ECC.ECCPoint infinity = Secp256k1ECC.createPointAtInfinity();
        assertTrue("Point should be at infinity", Secp256k1ECC.isPointAtInfinity(infinity));
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
    public void testPointDoubling() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint doubled = Secp256k1ECC.pointDouble(g);

        // Verify 2G is not infinity and not equal to G
        assertFalse("2G should not be infinity", Secp256k1ECC.isPointAtInfinity(doubled));
        assertFalse("2G should not equal G", Secp256k1ECC.pointsEqual(doubled, g));

        // Verify coordinates are properly reduced
        assertTrue("X coordinate should be validated", (doubled.pointFlags & Secp256k1ECC.POINT_VALIDATED) != 0);
        assertTrue("Y coordinate should be validated", (doubled.pointFlags & Secp256k1ECC.POINT_VALIDATED) != 0);
    }

    @Test
    public void testPointAdditionBasic() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint doubled = Secp256k1ECC.pointDouble(g);

        // G + G = 2G
        Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointAdd(g, g);
        assertTrue("G + G should equal 2G", Secp256k1ECC.pointsEqual(result, doubled));
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

        // G + (-G) = O (point at infinity)
        Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointAdd(g, negG);
        assertTrue("G + (-G) should be infinity", Secp256k1ECC.isPointAtInfinity(result));
    }

    // ===== SCALAR MULTIPLICATION TESTS =====

    @Test
    public void testScalarMultiplyByOne() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(1);

        Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);
        assertTrue("1 * G should equal G", Secp256k1ECC.pointsEqual(result, g));
    }

    @Test
    public void testScalarMultiplyByTwo() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(2);

        Secp256k1ECC.ECCPoint expected = Secp256k1ECC.pointDouble(g);
        Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);

        assertTrue("2 * G should equal 2G", Secp256k1ECC.pointsEqual(result, expected));
    }

    @Test
    public void testScalarMultiplyByZero() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(0);

        Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);
        assertTrue("0 * G should be infinity", Secp256k1ECC.isPointAtInfinity(result));
    }

    @Test
    public void testScalarMultiplyByThree() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(3);

        // 3G = 2G + G
        Secp256k1ECC.ECCPoint twoG = Secp256k1ECC.pointDouble(g);
        Secp256k1ECC.ECCPoint expected = Secp256k1ECC.pointAdd(twoG, g);
        Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);

        assertTrue("3 * G should equal 2G + G", Secp256k1ECC.pointsEqual(result, expected));
    }

    // ===== EDGE CASE TESTS =====

    @Test
    public void testPointAdditionWithItself() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();

        // P + P should equal 2P
        Secp256k1ECC.ECCPoint result1 = Secp256k1ECC.pointAdd(g, g);
        Secp256k1ECC.ECCPoint result2 = Secp256k1ECC.pointDouble(g);

        assertTrue("P + P should equal 2P", Secp256k1ECC.pointsEqual(result1, result2));
    }

    @Test
    public void testInfinityOperations() {
        Secp256k1ECC.ECCPoint inf1 = Secp256k1ECC.createPointAtInfinity();
        Secp256k1ECC.ECCPoint inf2 = Secp256k1ECC.createPointAtInfinity();

        // O + O = O
        Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointAdd(inf1, inf2);
        assertTrue("O + O should be O", Secp256k1ECC.isPointAtInfinity(result));

        // 2O = O
        Secp256k1ECC.ECCPoint doubled = Secp256k1ECC.pointDouble(inf1);
        assertTrue("2O should be O", Secp256k1ECC.isPointAtInfinity(doubled));
    }

    // ===== OVERFLOW CHAIN TESTS =====

    @Test
    public void testOverflowChainCreation() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(100);
        QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(200);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        QuantumNumberV8 result = Secp256k1ECC.modAdd(a, b, modulus);

        // Verify overflow chain exists
        assertNotNull("Overflow chain should exist", result.left);

        // Verify chain evaluation
        QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);
        assertNotNull("Evaluated result should not be null", evaluated);
    }

    @Test
    public void testOverflowChainTraversal() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
        QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

        // Create a chain of operations
        QuantumNumberV8 result1 = Secp256k1ECC.modAdd(a, a, modulus);  // 10 + 10 = 20
        QuantumNumberV8 result2 = Secp256k1ECC.modAdd(result1, a, modulus); // 20 + 10 = 30
        QuantumNumberV8 result3 = Secp256k1ECC.modMul(result2, a, modulus); // 30 * 10 = 300

        // Evaluate final result
        QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result3, modulus);

        // 300 mod 23 = 300 - 13*23 = 300 - 299 = 1
        assertEquals("Complex overflow chain should evaluate correctly", 1, evaluated.a1);
    }

    // ===== PERFORMANCE TESTS =====

    @Test
    public void testPointAdditionPerformance() {
        Secp256k1ECC.ECCPoint p1 = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint p2 = Secp256k1ECC.pointDouble(p1);

        long startTime = System.nanoTime();

        for (int i = 0; i < PERFORMANCE_ITERATIONS; i++) {
            Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointAdd(p1, p2);
            // Prevent optimization
            if (result == null) break;
        }

        long endTime = System.nanoTime();
        long duration = endTime - startTime;
        double avgTime = (double) duration / PERFORMANCE_ITERATIONS;

        System.out.printf("Point addition performance: %.2f ns per operation%n", avgTime);
        assertTrue("Performance should be reasonable", avgTime < 1000000); // Less than 1ms per operation
    }

    @Test
    public void testScalarMultiplicationPerformance() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(12345);

        long startTime = System.nanoTime();

        for (int i = 0; i < PERFORMANCE_ITERATIONS / 10; i++) { // Fewer iterations for scalar mul
            Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);
            // Prevent optimization
            if (result == null) break;
        }

        long endTime = System.nanoTime();
        long duration = endTime - startTime;
        double avgTime = (double) duration / (PERFORMANCE_ITERATIONS / 10);

        System.out.printf("Scalar multiplication performance: %.2f ns per operation%n", avgTime);
        assertTrue("Performance should be reasonable", avgTime < 50000000); // Less than 50ms per operation
    }

    @Test
    public void testModularArithmeticPerformance() {
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(123456789);
        QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(987654321);
        QuantumNumberV8 modulus = Secp256k1ECC.SECP256K1_P;

        long startTime = System.nanoTime();

        for (int i = 0; i < PERFORMANCE_ITERATIONS; i++) {
            QuantumNumberV8 result = Secp256k1ECC.modMul(a, b, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);
            // Prevent optimization
            if (evaluated == null) break;
        }

        long endTime = System.nanoTime();
        long duration = endTime - startTime;
        double avgTime = (double) duration / PERFORMANCE_ITERATIONS;

        System.out.printf("Modular multiplication performance: %.2f ns per operation%n", avgTime);
        assertTrue("Performance should be reasonable", avgTime < 100000); // Less than 100μs per operation
    }

    // ===== CORRECTNESS VERIFICATION TESTS =====

    @Test
    public void testCurveEquationVerification() {
        Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();

        // Verify G satisfies y² = x³ + 7 mod p
        QuantumNumberV8 y_squared = Secp256k1ECC.modMul(g.y, g.y, Secp256k1ECC.SECP256K1_P);
        QuantumNumberV8 x_cubed = Secp256k1ECC.modMul(
            Secp256k1ECC.modMul(g.x, g.x, Secp256k1ECC.SECP256K1_P),
            g.x,
            Secp256k1ECC.SECP256K1_P
        );
        QuantumNumberV8 right_side = Secp256k1ECC.modAdd(x_cubed,
            Secp256k1ECC.SECP256K1_B, Secp256k1ECC.SECP256K1_P);

        QuantumNumberV8 y_squared_eval = Secp256k1ECC.evaluateOverflowChain(y_squared, Secp256k1ECC.SECP256K1_P);
        QuantumNumberV8 right_side_eval = Secp256k1ECC.evaluateOverflowChain(right_side, Secp256k1ECC.SECP256K1_P);

        assertTrue("Generator point should satisfy curve equation",
            Secp256k1ECC.quantumEqual(y_squared_eval, right_side_eval));
    }

    @Test
    public void testPointAdditionResultVerification() {
        Secp256k1ECC.ECCPoint p1 = Secp256k1ECC.SECP256K1_G.copy();
        Secp256k1ECC.ECCPoint p2 = Secp256k1ECC.pointDouble(p1);

        Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointAdd(p1, p2);

        // Verify result satisfies curve equation
        QuantumNumberV8 y_squared = Secp256k1ECC.modMul(result.y, result.y, Secp256k1ECC.SECP256K1_P);
        QuantumNumberV8 x_cubed = Secp256k1ECC.modMul(
            Secp256k1ECC.modMul(result.x, result.x, Secp256k1ECC.SECP256K1_P),
            result.x,
            Secp256k1ECC.SECP256K1_P
        );
        QuantumNumberV8 right_side = Secp256k1ECC.modAdd(x_cubed,
            Secp256k1ECC.SECP256K1_B, Secp256k1ECC.SECP256K1_P);

        QuantumNumberV8 y_squared_eval = Secp256k1ECC.evaluateOverflowChain(y_squared, Secp256k1ECC.SECP256K1_P);
        QuantumNumberV8 right_side_eval = Secp256k1ECC.evaluateOverflowChain(right_side, Secp256k1ECC.SECP256K1_P);

        assertTrue("Point addition result should satisfy curve equation",
            Secp256k1ECC.quantumEqual(y_squared_eval, right_side_eval));
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

    // ===== MEMORY AND RESOURCE TESTS =====

    @Test
    public void testMemoryEfficiency() {
        // Test that overflow chains don't cause excessive memory usage
        QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(1000);
        QuantumNumberV8 modulus = Secp256k1ECC.SECP256K1_P;

        long initialMemory = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();

        // Perform many operations
        for (int i = 0; i < 1000; i++) {
            QuantumNumberV8 result = Secp256k1ECC.modMul(a, a, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);
            // Prevent optimization
            if (evaluated == null) break;
        }

        long finalMemory = Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory();
        long memoryIncrease = finalMemory - initialMemory;

        System.out.printf("Memory increase after 1000 operations: %d bytes%n", memoryIncrease);
        assertTrue("Memory usage should be reasonable", memoryIncrease < 10 * 1024 * 1024); // Less than 10MB
    }

    @Test
    public void testThreadSafety() {
        // Test that the implementation is thread-safe for single-user airgapped environment
        final Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
        final boolean[] results = new boolean[10];

        Thread[] threads = new Thread[10];
        for (int i = 0; i < 10; i++) {
            final int threadId = i;
            threads[i] = new Thread(() -> {
                try {
                    Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(
                        Secp256k1ECC.createQuantumNumber(threadId + 1), g);
                    results[threadId] = !Secp256k1ECC.isPointAtInfinity(result);
                } catch (Exception e) {
                    results[threadId] = false;
                }
            });
            threads[i].start();
        }

        // Wait for all threads to complete
        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                fail("Thread interrupted");
            }
        }

        // Verify all operations succeeded
        for (boolean result : results) {
            assertTrue("All scalar multiplications should succeed", result);
        }
    }

    // ===== INTEGRATION TESTS =====

    @Test
    public void testCompleteECDHKeyExchange() {
        // Alice's keypair
        QuantumNumberV8 alicePrivate = Secp256k1ECC.createQuantumNumber(0x123456789ABCDEF0L);
        Secp256k1ECC.ECCPoint alicePublic = Secp256k1ECC.scalarMultiply(alicePrivate,
            Secp256k1ECC.SECP256K1_G);

        // Bob's keypair
        QuantumNumberV8 bobPrivate = Secp256k1ECC.createQuantumNumber(0xFEDCBA9876543210L);
        Secp256k1ECC.ECCPoint bobPublic = Secp256k1ECC.scalarMultiply(bobPrivate,
            Secp256k1ECC.SECP256K1_G);

        // Shared secret computation
        Secp256k1ECC.ECCPoint aliceShared = Secp256k1ECC.scalarMultiply(alicePrivate, bobPublic);
        Secp256k1ECC.ECCPoint bobShared = Secp256k1ECC.scalarMultiply(bobPrivate, alicePublic);

        // Verify shared secrets match
        assertTrue("ECDH shared secrets should match",
            Secp256k1ECC.pointsEqual(aliceShared, bobShared));
    }

    @Test
    public void testOrderVerification() {
        // Verify that n*G = O (point at infinity)
        Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(
            Secp256k1ECC.SECP256K1_N, Secp256k1ECC.SECP256K1_G);

        assertTrue("n*G should equal point at infinity",
            Secp256k1ECC.isPointAtInfinity(result));
    }
}
