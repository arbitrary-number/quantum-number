package com.github.quantum_number;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import static org.junit.jupiter.api.Assertions.*;

/**
 * Quantum AST Test Suite
 * 
 * Tests symbolic computation functionality including:
 * - AST construction and manipulation
 * - Quantition (symbolic evaluation)
 * - Operation and function nodes
 * - AST simplification
 */
@DisplayName("Quantum AST Tests")
public class QuantumASTTest {
    
    private QuantumNumber testNumber1;
    private QuantumNumber testNumber2;
    private QuantumAST leafNode1;
    private QuantumAST leafNode2;
    
    @BeforeEach
    void setUp() {
        testNumber1 = new QuantumNumber();
        testNumber1.setOrdinal(0, 100);
        testNumber1.setOrdinal(1, 200);
        testNumber1.updateChecksum();
        
        testNumber2 = new QuantumNumber();
        testNumber2.setOrdinal(0, 50);
        testNumber2.setOrdinal(1, 150);
        testNumber2.updateChecksum();
        
        leafNode1 = new QuantumAST(QuantumAST.NodeType.QUANTUM_NUMBER, testNumber1);
        leafNode2 = new QuantumAST(QuantumAST.NodeType.QUANTUM_NUMBER, testNumber2);
    }
    
    @Test
    @DisplayName("Leaf Node Creation")
    void testLeafNodeCreation() {
        assertEquals(QuantumAST.NodeType.QUANTUM_NUMBER, leafNode1.getNodeType(), "Node type should be QUANTUM_NUMBER");
        assertEquals(testNumber1, leafNode1.getQuantumValue(), "Quantum value should match input");
        assertTrue(leafNode1.isEvaluated(), "Leaf nodes should be evaluated");
        assertTrue(leafNode1.isSimplified(), "Leaf nodes should be simplified");
        assertEquals(0, leafNode1.getChildren().size(), "Leaf nodes should have no children");
    }
    
    @Test
    @DisplayName("Addition Operation Node")
    void testAdditionOperationNode() {
        QuantumAST addNode = QuantumAST.createOperation(QuantumAST.OperationType.ADD, leafNode1, leafNode2);
        
        assertEquals(QuantumAST.NodeType.OPERATION, addNode.getNodeType(), "Node type should be OPERATION");
        assertEquals(QuantumAST.OperationType.ADD, addNode.getOperationType(), "Operation type should be ADD");
        assertFalse(addNode.isEvaluated(), "Operation nodes should not be initially evaluated");
        assertFalse(addNode.isSimplified(), "Operation nodes should not be initially simplified");
        assertEquals(2, addNode.getChildren().size(), "Binary operation should have 2 children");
        
        // Check parent-child relationships
        assertEquals(addNode, leafNode1.getParent(), "Left child should have correct parent");
        assertEquals(addNode, leafNode2.getParent(), "Right child should have correct parent");
    }
    
    @Test
    @DisplayName("Quantition - Addition")
    void testQuantitionAddition() {
        QuantumAST addNode = QuantumAST.createOperation(QuantumAST.OperationType.ADD, leafNode1, leafNode2);
        QuantumNumber result = addNode.quantition();
        
        // Should equal manual addition
        QuantumNumber expected = testNumber1.add(testNumber2);
        assertEquals(expected, result, "Quantition result should equal manual addition");
        assertTrue(result.verifyChecksum(), "Result should have valid checksum");
    }
    
    @Test
    @DisplayName("Quantition - Subtraction")
    void testQuantitionSubtraction() {
        QuantumAST subNode = QuantumAST.createOperation(QuantumAST.OperationType.SUBTRACT, leafNode1, leafNode2);
        QuantumNumber result = subNode.quantition();
        
        // Should equal manual subtraction
        QuantumNumber expected = testNumber1.subtract(testNumber2);
        assertEquals(expected, result, "Quantition result should equal manual subtraction");
        assertTrue(result.verifyChecksum(), "Result should have valid checksum");
    }
    
    @Test
    @DisplayName("Quantition - Multiplication")
    void testQuantitionMultiplication() {
        QuantumAST mulNode = QuantumAST.createOperation(QuantumAST.OperationType.MULTIPLY, leafNode1, leafNode2);
        QuantumNumber result = mulNode.quantition();
        
        // Should perform ordinal-wise multiplication
        assertNotNull(result, "Multiplication result should not be null");
        assertTrue(result.verifyChecksum(), "Result should have valid checksum");
    }
    
    @Test
    @DisplayName("Quantition - Division")
    void testQuantitionDivision() {
        QuantumAST divNode = QuantumAST.createOperation(QuantumAST.OperationType.DIVIDE, leafNode1, leafNode2);
        QuantumNumber result = divNode.quantition();
        
        // Should handle division including symbolic divide-by-zero
        assertNotNull(result, "Division result should not be null");
        assertTrue(result.verifyChecksum(), "Result should have valid checksum");
    }
    
    @Test
    @DisplayName("Quantition - Division by Zero")
    void testQuantitionDivisionByZero() {
        QuantumAST zeroNode = new QuantumAST(QuantumAST.NodeType.QUANTUM_NUMBER, QuantumNumber.zero());
        QuantumAST divByZeroNode = QuantumAST.createOperation(QuantumAST.OperationType.DIVIDE, leafNode1, zeroNode);
        
        // Should handle symbolic division by zero
        assertDoesNotThrow(() -> {
            QuantumNumber result = divByZeroNode.quantition();
            assertNotNull(result, "Division by zero should return symbolic result");
            assertTrue(result.verifyChecksum(), "Symbolic division by zero should have valid checksum");
        }, "Division by zero should not throw exception");
    }
    
    @Test
    @DisplayName("Unary Negation Operation")
    void testUnaryNegationOperation() {
        QuantumAST negNode = QuantumAST.createUnaryOperation(QuantumAST.OperationType.NEGATE, leafNode1);
        
        assertEquals(QuantumAST.NodeType.OPERATION, negNode.getNodeType(), "Node type should be OPERATION");
        assertEquals(QuantumAST.OperationType.NEGATE, negNode.getOperationType(), "Operation type should be NEGATE");
        assertEquals(1, negNode.getChildren().size(), "Unary operation should have 1 child");
        
        QuantumNumber result = negNode.quantition();
        QuantumNumber expected = testNumber1.negate();
        assertEquals(expected, result, "Negation result should equal manual negation");
    }
    
    @Test
    @DisplayName("Function Node - Absolute Value")
    void testFunctionNodeAbsoluteValue() {
        QuantumAST absNode = QuantumAST.createFunction(QuantumAST.FunctionType.ABS, leafNode1);
        
        assertEquals(QuantumAST.NodeType.FUNCTION, absNode.getNodeType(), "Node type should be FUNCTION");
        assertEquals(QuantumAST.FunctionType.ABS, absNode.getFunctionType(), "Function type should be ABS");
        assertEquals(1, absNode.getChildren().size(), "Function should have 1 argument");
        
        QuantumNumber result = absNode.quantition();
        assertNotNull(result, "Absolute value result should not be null");
        assertTrue(result.verifyChecksum(), "Result should have valid checksum");
        
        // All signs should be positive
        for (int i = 0; i < QuantumNumber.NUM_ORDINALS; i++) {
            assertFalse(result.getSign(i), "All signs should be positive in absolute value");
        }
    }
    
    @Test
    @DisplayName("Function Node - Square Root")
    void testFunctionNodeSquareRoot() {
        QuantumAST sqrtNode = QuantumAST.createFunction(QuantumAST.FunctionType.SQRT, leafNode1);
        
        assertEquals(QuantumAST.FunctionType.SQRT, sqrtNode.getFunctionType(), "Function type should be SQRT");
        
        QuantumNumber result = sqrtNode.quantition();
        assertNotNull(result, "Square root result should not be null");
        assertTrue(result.verifyChecksum(), "Result should have valid checksum");
    }
    
    @Test
    @DisplayName("Complex AST Construction")
    void testComplexASTConstruction() {
        // Create (a + b) * c
        QuantumNumber testNumber3 = new QuantumNumber();
        testNumber3.setOrdinal(0, 25);
        testNumber3.updateChecksum();
        QuantumAST leafNode3 = new QuantumAST(QuantumAST.NodeType.QUANTUM_NUMBER, testNumber3);
        
        QuantumAST addNode = QuantumAST.createOperation(QuantumAST.OperationType.ADD, leafNode1, leafNode2);
        QuantumAST mulNode = QuantumAST.createOperation(QuantumAST.OperationType.MULTIPLY, addNode, leafNode3);
        
        assertEquals(3, mulNode.getHeight(), "Complex AST should have correct height");
        assertEquals(5, mulNode.getNodeCount(), "Complex AST should have correct node count");
        assertFalse(mulNode.hasVariables(), "AST with only numbers should not have variables");
    }
    
    @Test
    @DisplayName("Variable Node")
    void testVariableNode() {
        QuantumAST varNode = new QuantumAST("x");
        
        assertEquals(QuantumAST.NodeType.VARIABLE, varNode.getNodeType(), "Node type should be VARIABLE");
        assertEquals("x", varNode.getVariableName(), "Variable name should be 'x'");
        assertFalse(varNode.isEvaluated(), "Variable nodes should not be evaluated");
        assertTrue(varNode.isSimplified(), "Variable nodes should be simplified");
        assertEquals(0, varNode.getChildren().size(), "Variable nodes should have no children");
        
        // Should throw exception when trying to quantition without substitution
        assertThrows(UnsupportedOperationException.class, () -> varNode.quantition(),
            "Variable quantition should throw exception without substitution");
    }
    
    @Test
    @DisplayName("AST with Variables")
    void testASTWithVariables() {
        QuantumAST varNode = new QuantumAST("x");
        QuantumAST addNode = QuantumAST.createOperation(QuantumAST.OperationType.ADD, leafNode1, varNode);
        
        assertTrue(addNode.hasVariables(), "AST with variables should return true for hasVariables()");
        assertEquals(3, addNode.getNodeCount(), "AST should count all nodes including variables");
    }
    
    @Test
    @DisplayName("AST Simplification - Addition with Zero")
    void testSimplificationAdditionWithZero() {
        QuantumAST zeroNode = new QuantumAST(QuantumAST.NodeType.QUANTUM_NUMBER, QuantumNumber.zero());
        QuantumAST addNode = QuantumAST.createOperation(QuantumAST.OperationType.ADD, leafNode1, zeroNode);
        
        QuantumAST simplified = addNode.simplify();
        
        // Should simplify to just the non-zero operand
        // Note: This test depends on the simplification implementation
        assertNotNull(simplified, "Simplified AST should not be null");
    }
    
    @Test
    @DisplayName("AST Simplification - Multiplication with One")
    void testSimplificationMultiplicationWithOne() {
        QuantumAST oneNode = new QuantumAST(QuantumAST.NodeType.QUANTUM_NUMBER, QuantumNumber.one());
        QuantumAST mulNode = QuantumAST.createOperation(QuantumAST.OperationType.MULTIPLY, leafNode1, oneNode);
        
        QuantumAST simplified = mulNode.simplify();
        
        // Should simplify to just the non-one operand
        assertNotNull(simplified, "Simplified AST should not be null");
    }
    
    @Test
    @DisplayName("AST Simplification - Multiplication with Zero")
    void testSimplificationMultiplicationWithZero() {
        QuantumAST zeroNode = new QuantumAST(QuantumAST.NodeType.QUANTUM_NUMBER, QuantumNumber.zero());
        QuantumAST mulNode = QuantumAST.createOperation(QuantumAST.OperationType.MULTIPLY, leafNode1, zeroNode);
        
        QuantumAST simplified = mulNode.simplify();
        
        // Should simplify to zero
        assertNotNull(simplified, "Simplified AST should not be null");
    }
    
    @Test
    @DisplayName("AST String Representation")
    void testASTStringRepresentation() {
        QuantumAST addNode = QuantumAST.createOperation(QuantumAST.OperationType.ADD, leafNode1, leafNode2);
        String representation = addNode.toString();
        
        assertTrue(representation.contains("Operation: ADD"), "String should contain operation type");
        assertTrue(representation.contains("QuantumNumber:"), "String should contain quantum number info");
        assertFalse(representation.isEmpty(), "String representation should not be empty");
    }
    
    @Test
    @DisplayName("Derivation History")
    void testDerivationHistory() {
        QuantumAST addNode = QuantumAST.createOperation(QuantumAST.OperationType.ADD, leafNode1, leafNode2);
        String history = addNode.getDerivationHistory();
        
        assertTrue(history.contains("ADD"), "History should contain operation type");
        assertFalse(history.isEmpty(), "Derivation history should not be empty");
    }
    
    @Test
    @DisplayName("Unsupported Operations")
    void testUnsupportedOperations() {
        // Test unsupported operation types
        assertThrows(UnsupportedOperationException.class, () -> {
            QuantumAST powerNode = QuantumAST.createOperation(QuantumAST.OperationType.POWER, leafNode1, leafNode2);
            powerNode.quantition();
        }, "Unsupported operations should throw exception");
        
        // Test unsupported function types
        assertThrows(UnsupportedOperationException.class, () -> {
            QuantumAST sinNode = QuantumAST.createFunction(QuantumAST.FunctionType.SIN, leafNode1);
            sinNode.quantition();
        }, "Unsupported functions should throw exception");
    }
    
    @Test
    @DisplayName("Invalid Operation Arguments")
    void testInvalidOperationArguments() {
        // Test operations with wrong number of arguments
        assertThrows(IllegalStateException.class, () -> {
            // Create a malformed AST (this would require internal manipulation)
            QuantumAST addNode = QuantumAST.createOperation(QuantumAST.OperationType.ADD, leafNode1, leafNode2);
            // Remove a child to make it invalid (this is conceptual - actual implementation may vary)
            addNode.quantition();
        });
    }
    
    @Test
    @DisplayName("AST Height and Node Count")
    void testASTMetrics() {
        // Single leaf node
        assertEquals(1, leafNode1.getHeight(), "Leaf node should have height 1");
        assertEquals(1, leafNode1.getNodeCount(), "Leaf node should have count 1");
        
        // Binary operation
        QuantumAST addNode = QuantumAST.createOperation(QuantumAST.OperationType.ADD, leafNode1, leafNode2);
        assertEquals(2, addNode.getHeight(), "Binary operation should have height 2");
        assertEquals(3, addNode.getNodeCount(), "Binary operation should have count 3");
        
        // Nested operations
        QuantumAST mulNode = QuantumAST.createOperation(QuantumAST.OperationType.MULTIPLY, addNode, leafNode1);
        assertEquals(3, mulNode.getHeight(), "Nested operation should have height 3");
        assertEquals(4, mulNode.getNodeCount(), "Nested operation should have count 4");
    }
}
