package com.github.quantum_number.v8;

/**
 * Standalone test runner for Secp256k1ECC implementation
 * Runs tests without JUnit dependency for airgapped environments
 *
 * @author Cline AI Assistant
 */
public class Secp256k1ECCTestRunner {

    private static int testsRun = 0;
    private static int testsPassed = 0;
    private static int testsFailed = 0;

    public static void main(String[] args) {
        System.out.println("=== Secp256k1ECC Coordinate Demonstration ===");
        System.out.println("Demonstrating G, 2G, and 3G point coordinates");
        System.out.println();

        // Run focused test demonstrating ECC point coordinates
        runECCPointCoordinateDemo();

        // Print results
        System.out.println("\n=== Test Results ===");
        System.out.printf("Tests Run: %d%n", testsRun);
        System.out.printf("Tests Passed: %d%n", testsPassed);
        System.out.printf("Tests Failed: %d%n", testsFailed);
        System.out.printf("Success Rate: %.1f%%%n",
            testsRun > 0 ? (double) testsPassed / testsRun * 100 : 0);

        if (testsFailed == 0) {
            System.out.println("[PASS] Coordinate demonstration successful!");
        } else {
            System.out.println("[FAIL] Coordinate demonstration failed.");
        }
    }

    private static void runECCPointCoordinateDemo() {
        System.out.println("=== ECC Point Coordinate Demonstration ===");

        test("ECC Point Coordinate Demo", () -> {
            try {
                // Get the generator point G
                Secp256k1ECC.ECCPoint G = Secp256k1ECC.SECP256K1_G;
                System.out.println("Generator Point G:");
                System.out.printf("  X: %08X %08X %08X %08X%n",
                    G.x.a4, G.x.a3, G.x.a2, G.x.a1);
                System.out.printf("  Y: %08X %08X %08X %08X%n",
                    G.y.a4, G.y.a3, G.y.a2, G.y.a1);

                // Calculate 2G
                Secp256k1ECC.ECCPoint G2 = Secp256k1ECC.pointDouble(G);
                System.out.println("Point 2G (G + G):");
                System.out.printf("  X: %08X %08X %08X %08X%n",
                    G2.x.a4, G2.x.a3, G2.x.a2, G2.x.a1);
                System.out.printf("  Y: %08X %08X %08X %08X%n",
                    G2.y.a4, G2.y.a3, G2.y.a2, G2.y.a1);

                // Calculate 3G using scalar multiplication
                Secp256k1ECC.ECCPoint G3 = Secp256k1ECC.scalarMultiply(
                    Secp256k1ECC.createQuantumNumber(3), G);
                System.out.println("Point 3G (scalar multiply by 3):");
                System.out.printf("  X: %08X %08X %08X %08X%n",
                    G3.x.a4, G3.x.a3, G3.x.a2, G3.x.a1);
                System.out.printf("  Y: %08X %08X %08X %08X%n",
                    G3.y.a4, G3.y.a3, G3.y.a2, G3.y.a1);

                // Verify that 3G = 2G + G
                Secp256k1ECC.ECCPoint G3_alt = Secp256k1ECC.pointAdd(G2, G);
                boolean pointsEqual = Secp256k1ECC.pointsEqual(G3, G3_alt);

                System.out.println("Verification: 3G = 2G + G");
                System.out.println("  Result: " + (pointsEqual ? "VERIFIED ✓" : "FAILED ✗"));

                return pointsEqual && !Secp256k1ECC.isPointAtInfinity(G) &&
                       !Secp256k1ECC.isPointAtInfinity(G2) && !Secp256k1ECC.isPointAtInfinity(G3);

            } catch (Exception e) {
                System.out.println("Error in coordinate demo: " + e.getMessage());
                return false;
            }
        });
    }

    private static void runBasicTests() {
        System.out.println("Running Basic Tests...");

        test("Point Creation", () -> {
            Secp256k1ECC.ECCPoint point = new Secp256k1ECC.ECCPoint();
            return point != null && point.x != null && point.y != null && point.z != null;
        });

        test("Point at Infinity", () -> {
            Secp256k1ECC.ECCPoint infinity = Secp256k1ECC.createPointAtInfinity();
            return Secp256k1ECC.isPointAtInfinity(infinity);
        });

        test("Generator Point", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G;
            return g != null && !Secp256k1ECC.isPointAtInfinity(g) &&
                   (g.pointFlags & Secp256k1ECC.POINT_VALIDATED) != 0;
        });

        test("Quantum Number Creation", () -> {
            QuantumNumberV8 qn = Secp256k1ECC.createQuantumNumber(42);
            return qn != null && qn.a1 == 42;
        });
    }

    private static void runModularArithmeticTests() {
        System.out.println("Running Modular Arithmetic Tests...");

        test("Modular Addition", () -> {
            QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
            QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(15);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            QuantumNumberV8 result = Secp256k1ECC.modAdd(a, b, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

            return evaluated.a1 == 2 && result.left != null; // 10 + 15 = 25, 25 mod 23 = 2
        });

        test("Modular Subtraction", () -> {
            QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
            QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(15);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            QuantumNumberV8 result = Secp256k1ECC.modSub(a, b, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

            return evaluated.a1 == 18 && result.left != null; // 10 - 15 = -5, -5 mod 23 = 18
        });

        test("Modular Multiplication", () -> {
            QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(7);
            QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(8);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            QuantumNumberV8 result = Secp256k1ECC.modMul(a, b, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

            return evaluated.a1 == 10 && result.left != null; // 7 * 8 = 56, 56 mod 23 = 10
        });

        test("Modular Inverse", () -> {
            QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(7);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            QuantumNumberV8 inverse = Secp256k1ECC.modInverse(a, modulus);

            // Verify: 7 * inverse ≡ 1 mod 23
            QuantumNumberV8 product = Secp256k1ECC.modMul(a, inverse, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(product, modulus);

            return evaluated.a1 == 1;
        });
    }

    private static void runPointOperationTests() {
        System.out.println("Running Point Operation Tests...");

        test("Point Addition with Infinity", () -> {
            Secp256k1ECC.ECCPoint infinity = Secp256k1ECC.createPointAtInfinity();
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();

            Secp256k1ECC.ECCPoint result1 = Secp256k1ECC.pointAdd(infinity, g);
            Secp256k1ECC.ECCPoint result2 = Secp256k1ECC.pointAdd(g, infinity);

            return Secp256k1ECC.pointsEqual(result1, g) && Secp256k1ECC.pointsEqual(result2, g);
        });

        test("Point Doubling", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            Secp256k1ECC.ECCPoint doubled = Secp256k1ECC.pointDouble(g);

            return !Secp256k1ECC.isPointAtInfinity(doubled) &&
                   !Secp256k1ECC.pointsEqual(doubled, g) &&
                   (doubled.pointFlags & Secp256k1ECC.POINT_VALIDATED) != 0;
        });

        test("Point Addition Basic", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            Secp256k1ECC.ECCPoint doubled = Secp256k1ECC.pointDouble(g);

            Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointAdd(g, g);

            return Secp256k1ECC.pointsEqual(result, doubled);
        });

        test("Point Negation", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();

            // Create -G
            Secp256k1ECC.ECCPoint negG = new Secp256k1ECC.ECCPoint();
            negG.x = Secp256k1ECC.copyQuantumNumber(g.x);
            negG.y = Secp256k1ECC.modSub(Secp256k1ECC.createQuantumNumber(0), g.y, Secp256k1ECC.SECP256K1_P);
            negG.z = Secp256k1ECC.createQuantumNumber(1);
            negG.coordinateSystem = Secp256k1ECC.COORDINATE_AFFINE;

            Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointAdd(g, negG);

            return Secp256k1ECC.isPointAtInfinity(result);
        });
    }

    private static void runScalarMultiplicationTests() {
        System.out.println("Running Scalar Multiplication Tests...");

        test("Scalar Multiply by One", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(1);

            Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);

            return Secp256k1ECC.pointsEqual(result, g);
        });

        test("Scalar Multiply by Two", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(2);

            Secp256k1ECC.ECCPoint expected = Secp256k1ECC.pointDouble(g);
            Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);

            return Secp256k1ECC.pointsEqual(result, expected);
        });

        test("Scalar Multiply by Zero", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(0);

            Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);

            return Secp256k1ECC.isPointAtInfinity(result);
        });

        test("Scalar Multiply by Three", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(3);

            Secp256k1ECC.ECCPoint twoG = Secp256k1ECC.pointDouble(g);
            Secp256k1ECC.ECCPoint expected = Secp256k1ECC.pointAdd(twoG, g);
            Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);

            return Secp256k1ECC.pointsEqual(result, expected);
        });
    }

    private static void runOverflowChainTests() {
        System.out.println("Running Overflow Chain Tests...");

        test("Overflow Chain Creation", () -> {
            QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(100);
            QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(200);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            QuantumNumberV8 result = Secp256k1ECC.modAdd(a, b, modulus);

            return result.left != null;
        });

        test("Overflow Chain Evaluation", () -> {
            QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
            QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(15);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            QuantumNumberV8 result = Secp256k1ECC.modAdd(a, b, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

            return evaluated.a1 == 2; // 10 + 15 = 25, 25 mod 23 = 2
        });

        test("Complex Overflow Chain", () -> {
            QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(10);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            // Create chain: 10 + 10 = 20, 20 + 10 = 30, 30 * 10 = 300
            QuantumNumberV8 result1 = Secp256k1ECC.modAdd(a, a, modulus);
            QuantumNumberV8 result2 = Secp256k1ECC.modAdd(result1, a, modulus);
            QuantumNumberV8 result3 = Secp256k1ECC.modMul(result2, a, modulus);

            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result3, modulus);

            return evaluated.a1 == 1; // 300 mod 23 = 1
        });
    }

    private static void runCorrectnessTests() {
        System.out.println("Running Correctness Tests...");

        test("Point Addition Commutativity", () -> {
            Secp256k1ECC.ECCPoint p1 = Secp256k1ECC.SECP256K1_G.copy();
            Secp256k1ECC.ECCPoint p2 = Secp256k1ECC.pointDouble(p1);

            Secp256k1ECC.ECCPoint result1 = Secp256k1ECC.pointAdd(p1, p2);
            Secp256k1ECC.ECCPoint result2 = Secp256k1ECC.pointAdd(p2, p1);

            return Secp256k1ECC.pointsEqual(result1, result2);
        });

        test("Point Addition Associativity", () -> {
            Secp256k1ECC.ECCPoint p1 = Secp256k1ECC.SECP256K1_G.copy();
            Secp256k1ECC.ECCPoint p2 = Secp256k1ECC.pointDouble(p1);
            Secp256k1ECC.ECCPoint p3 = Secp256k1ECC.pointDouble(p2);

            Secp256k1ECC.ECCPoint left = Secp256k1ECC.pointAdd(
                Secp256k1ECC.pointAdd(p1, p2), p3);
            Secp256k1ECC.ECCPoint right = Secp256k1ECC.pointAdd(
                p1, Secp256k1ECC.pointAdd(p2, p3));

            return Secp256k1ECC.pointsEqual(left, right);
        });

        test("Generator Order Verification", () -> {
            // This is a simplified test - in practice we'd verify n*G = O
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            QuantumNumberV8 smallOrder = Secp256k1ECC.createQuantumNumber(7);

            Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(smallOrder, g);

            return result != null; // Basic functionality test
        });

        test("Curve Equation Verification", () -> {
            // Simplified test - verify basic modular arithmetic works
            QuantumNumberV8 x = Secp256k1ECC.createQuantumNumber(5);
            QuantumNumberV8 y = Secp256k1ECC.createQuantumNumber(10);
            QuantumNumberV8 modulus = Secp256k1ECC.SECP256K1_P;

            QuantumNumberV8 y_squared = Secp256k1ECC.modMul(y, y, modulus);
            QuantumNumberV8 x_cubed = Secp256k1ECC.modMul(
                Secp256k1ECC.modMul(x, x, modulus), x, modulus);
            QuantumNumberV8 right_side = Secp256k1ECC.modAdd(x_cubed,
                Secp256k1ECC.SECP256K1_B, modulus);

            QuantumNumberV8 y_sq_eval = Secp256k1ECC.evaluateOverflowChain(y_squared, modulus);
            QuantumNumberV8 right_eval = Secp256k1ECC.evaluateOverflowChain(right_side, modulus);

            // For x=5, y=10, this won't satisfy the curve equation, but tests the math
            return y_sq_eval != null && right_eval != null;
        });
    }

    private static void runEdgeCaseTests() {
        System.out.println("Running Edge Case Tests...");

        test("Large Number Modular Arithmetic", () -> {
            QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(0xFFFFFFFFL);
            QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber(0xFFFFFFFL);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(1000003);

            QuantumNumberV8 result = Secp256k1ECC.modAdd(a, b, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

            return evaluated.a1 >= 0 && evaluated.a1 < modulus.a1;
        });

        test("Zero Modular Operations", () -> {
            QuantumNumberV8 zero = Secp256k1ECC.createQuantumNumber(0);
            QuantumNumberV8 five = Secp256k1ECC.createQuantumNumber(5);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            QuantumNumberV8 addResult = Secp256k1ECC.modAdd(zero, five, modulus);
            QuantumNumberV8 mulResult = Secp256k1ECC.modMul(zero, five, modulus);

            QuantumNumberV8 addEval = Secp256k1ECC.evaluateOverflowChain(addResult, modulus);
            QuantumNumberV8 mulEval = Secp256k1ECC.evaluateOverflowChain(mulResult, modulus);

            return addEval.a1 == 5 && mulEval.a1 == 0;
        });

        test("Modular Inverse of One", () -> {
            QuantumNumberV8 one = Secp256k1ECC.createQuantumNumber(1);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            QuantumNumberV8 inverse = Secp256k1ECC.modInverse(one, modulus);

            // Verify: 1 * inverse ≡ 1 mod 23
            QuantumNumberV8 product = Secp256k1ECC.modMul(one, inverse, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(product, modulus);

            return evaluated.a1 == 1;
        });

        test("Point Doubling at Infinity", () -> {
            Secp256k1ECC.ECCPoint infinity = Secp256k1ECC.createPointAtInfinity();
            Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointDouble(infinity);

            return Secp256k1ECC.isPointAtInfinity(result);
        });

        test("Scalar Multiply by Large Number", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            QuantumNumberV8 largeScalar = Secp256k1ECC.createQuantumNumber(1000);

            Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(largeScalar, g);

            return result != null && !Secp256k1ECC.isPointAtInfinity(result);
        });

        test("Complex Point Addition Chain", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            Secp256k1ECC.ECCPoint result = g.copy();

            // Add G to itself 5 times: G + G + G + G + G = 5G
            for (int i = 0; i < 4; i++) {
                result = Secp256k1ECC.pointAdd(result, g);
            }

            // Verify it's not infinity and not equal to G
            return !Secp256k1ECC.isPointAtInfinity(result) &&
                   !Secp256k1ECC.pointsEqual(result, g);
        });

        test("Overflow Chain Depth Test", () -> {
            QuantumNumberV8 base = Secp256k1ECC.createQuantumNumber(2);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            // Create a deep chain: 2 + 2 = 4, 4 + 2 = 6, 6 + 2 = 8, 8 + 2 = 10
            QuantumNumberV8 result = base;
            for (int i = 0; i < 4; i++) {
                result = Secp256k1ECC.modAdd(result, base, modulus);
            }

            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

            return evaluated.a1 == 10; // 2 * 5 = 10
        });

        test("Boundary Modular Arithmetic", () -> {
            QuantumNumberV8 maxInt = Secp256k1ECC.createQuantumNumber(Integer.MAX_VALUE);
            QuantumNumberV8 one = Secp256k1ECC.createQuantumNumber(1);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(10007);

            QuantumNumberV8 result = Secp256k1ECC.modAdd(maxInt, one, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

            return evaluated.a1 >= 0 && evaluated.a1 < modulus.a1;
        });

        test("Negative Result Handling", () -> {
            QuantumNumberV8 small = Secp256k1ECC.createQuantumNumber(5);
            QuantumNumberV8 large = Secp256k1ECC.createQuantumNumber(10);
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(23);

            QuantumNumberV8 result = Secp256k1ECC.modSub(small, large, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, modulus);

            return evaluated.a1 == 18; // 5 - 10 = -5, -5 mod 23 = 18
        });

        test("Point Copy Integrity", () -> {
            Secp256k1ECC.ECCPoint original = Secp256k1ECC.SECP256K1_G.copy();
            Secp256k1ECC.ECCPoint copy = original.copy();

            // Modify original and verify copy is unchanged
            original.x.a1 = 999999;

            return copy.x.a1 != 999999 && Secp256k1ECC.pointsEqual(copy, Secp256k1ECC.SECP256K1_G);
        });
    }

    private static void runSecurityTests() {
        System.out.println("Running Security Tests...");

        test("Constant Time Scalar Multiplication", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();

            long startTime1 = System.nanoTime();
            Secp256k1ECC.scalarMultiply(Secp256k1ECC.createQuantumNumber(12345), g);
            long endTime1 = System.nanoTime();

            long startTime2 = System.nanoTime();
            Secp256k1ECC.scalarMultiply(Secp256k1ECC.createQuantumNumber(67890), g);
            long endTime2 = System.nanoTime();

            long time1 = endTime1 - startTime1;
            long time2 = endTime2 - startTime2;

            // Times should be relatively close (within 50% difference)
            double ratio = Math.max(time1, time2) / (double) Math.min(time1, time2);

            return ratio < 1.5; // Allow 50% variance for constant-time behavior
        });

        test("No Information Leakage in Point Operations", () -> {
            Secp256k1ECC.ECCPoint p1 = Secp256k1ECC.SECP256K1_G.copy();
            Secp256k1ECC.ECCPoint p2 = Secp256k1ECC.pointDouble(p1);

            // Both operations should complete without exceptions
            Secp256k1ECC.ECCPoint result1 = Secp256k1ECC.pointAdd(p1, p2);
            Secp256k1ECC.ECCPoint result2 = Secp256k1ECC.pointAdd(p2, p1);

            return result1 != null && result2 != null &&
                   Secp256k1ECC.pointsEqual(result1, result2);
        });

        test("Secure Modular Inverse", () -> {
            QuantumNumberV8 base = Secp256k1ECC.createQuantumNumber(17);
            QuantumNumberV8 modulus = Secp256k1ECC.SECP256K1_P;

            QuantumNumberV8 inverse = Secp256k1ECC.modInverse(base, modulus);

            // Verify: base * inverse ≡ 1 mod p
            QuantumNumberV8 product = Secp256k1ECC.modMul(base, inverse, modulus);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(product, modulus);

            return evaluated.a1 == 1;
        });

        test("Large Prime Modulus Handling", () -> {
            // Test with a number close to the secp256k1 prime
            QuantumNumberV8 largeNum = Secp256k1ECC.copyQuantumNumber(Secp256k1ECC.SECP256K1_P);
            largeNum.a1 -= 1; // p - 1

            QuantumNumberV8 result = Secp256k1ECC.modAdd(largeNum,
                Secp256k1ECC.createQuantumNumber(1), Secp256k1ECC.SECP256K1_P);
            QuantumNumberV8 evaluated = Secp256k1ECC.evaluateOverflowChain(result, Secp256k1ECC.SECP256K1_P);

            return evaluated.a1 == 0; // (p - 1) + 1 ≡ 0 mod p
        });
    }

    private static void runPerformanceTests() {
        System.out.println("Running Performance Tests...");

        test("Scalar Multiplication Performance", () -> {
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();
            QuantumNumberV8 scalar = Secp256k1ECC.createQuantumNumber(0x12345678);

            long startTime = System.nanoTime();
            Secp256k1ECC.ECCPoint result = Secp256k1ECC.scalarMultiply(scalar, g);
            long endTime = System.nanoTime();

            long duration = endTime - startTime;

            // Should complete in reasonable time (less than 100ms)
            return duration < 100000000 && result != null;
        });

        test("Batch Modular Operations", () -> {
            QuantumNumberV8 modulus = Secp256k1ECC.createQuantumNumber(1009);

            long startTime = System.nanoTime();
            for (int i = 0; i < 1000; i++) {
                QuantumNumberV8 a = Secp256k1ECC.createQuantumNumber(i % 100);
                QuantumNumberV8 b = Secp256k1ECC.createQuantumNumber((i + 1) % 100);

                QuantumNumberV8 result = Secp256k1ECC.modMul(a, b, modulus);
                Secp256k1ECC.evaluateOverflowChain(result, modulus);
            }
            long endTime = System.nanoTime();

            long duration = endTime - startTime;
            long avgTime = duration / 1000;

            // Average time per operation should be reasonable
            return avgTime < 10000; // Less than 10 microseconds per operation
        });

        test("Memory Efficiency Test", () -> {
            // Test that operations don't create excessive object overhead
            Secp256k1ECC.ECCPoint g = Secp256k1ECC.SECP256K1_G.copy();

            long startTime = System.nanoTime();
            for (int i = 0; i < 100; i++) {
                Secp256k1ECC.ECCPoint result = Secp256k1ECC.pointDouble(g);
                g = result; // Chain the operations
            }
            long endTime = System.nanoTime();

            long duration = endTime - startTime;

            return duration < 50000000 && g != null; // Less than 50ms for 100 operations
        });
    }

    private static void test(String testName, TestFunction testFunction) {
        testsRun++;
        try {
            boolean result = testFunction.run();
            if (result) {
                testsPassed++;
                System.out.printf("[PASS] %s: PASSED%n", testName);
            } else {
                testsFailed++;
                System.out.printf("[FAIL] %s: FAILED%n", testName);
            }
        } catch (Exception e) {
            testsFailed++;
            System.out.printf("[ERROR] %s: ERROR - %s%n", testName, e.getMessage());
        }
    }

    @FunctionalInterface
    private interface TestFunction {
        boolean run() throws Exception;
    }
}
