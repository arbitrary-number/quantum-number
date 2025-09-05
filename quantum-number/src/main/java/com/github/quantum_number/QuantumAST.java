package com.github.quantum_number;

import java.util.ArrayList;
import java.util.List;

/**
 * QuantumAST - Quantum Abstract Syntax Tree
 * 
 * Represents symbolic mathematical expressions using Quantum Numbers as atomic units.
 * The AST structure preserves complete mathematical derivation and enables
 * symbolic computation with deferred evaluation (quantition).
 * 
 * AST Structure:
 * - Leaf nodes: Quantum Numbers (Minimum Units)
 * - Internal nodes: Operations or functions
 * - Root: Complete symbolic expression
 * 
 * @author Quantum Number Project Team
 * @version 1.0
 */
public class QuantumAST {
    
    /**
     * Node types in the Quantum AST
     */
    public enum NodeType {
        QUANTUM_NUMBER,    // Leaf node containing a Quantum Number
        OPERATION,         // Internal node representing an operation
        FUNCTION,          // Internal node representing a function
        VARIABLE,          // Symbolic variable
        CONSTANT           // Mathematical constant
    }
    
    /**
     * Operation types supported in Quantum AST
     */
    public enum OperationType {
        ADD,               // Addition
        SUBTRACT,          // Subtraction
        MULTIPLY,          // Multiplication
        DIVIDE,            // Division (including symbolic divide-by-zero)
        POWER,             // Exponentiation
        ROOT,              // Root extraction
        NEGATE             // Negation
    }
    
    /**
     * Function types supported in Quantum AST
     */
    public enum FunctionType {
        SIN,               // Sine
        COS,               // Cosine
        TAN,               // Tangent
        EXP,               // Exponential
        LOG,               // Natural logarithm
        SQRT,              // Square root
        ABS                // Absolute value
    }
    
    // Node properties
    private NodeType nodeType;
    private OperationType operationType;
    private FunctionType functionType;
    private SimpleQuantumNumber quantumValue;
    private String variableName;
    private List<QuantumAST> children;
    private QuantumAST parent;
    
    // Metadata for symbolic computation
    private boolean isEvaluated;
    private boolean isSimplified;
    private String derivationHistory;
    
    /**
     * Constructor for Quantum Number leaf nodes
     */
    public QuantumAST(NodeType nodeType, SimpleQuantumNumber value) {
        if (nodeType != NodeType.QUANTUM_NUMBER) {
            throw new IllegalArgumentException("This constructor is only for QUANTUM_NUMBER nodes");
        }
        this.nodeType = nodeType;
        this.quantumValue = new SimpleQuantumNumber(value);
        this.children = new ArrayList<>();
        this.isEvaluated = true; // Leaf nodes are already evaluated
        this.isSimplified = true;
        this.derivationHistory = "Quantum Number: " + value.toString();
    }
    
    /**
     * Constructor for operation nodes
     */
    public QuantumAST(OperationType operation, QuantumAST left, QuantumAST right) {
        this.nodeType = NodeType.OPERATION;
        this.operationType = operation;
        this.children = new ArrayList<>();
        this.children.add(left);
        this.children.add(right);
        
        // Set parent references
        left.parent = this;
        right.parent = this;
        
        this.isEvaluated = false;
        this.isSimplified = false;
        this.derivationHistory = "Operation: " + operation + "(" + left.derivationHistory + ", " + right.derivationHistory + ")";
    }
    
    /**
     * Constructor for unary operation nodes
     */
    public QuantumAST(OperationType operation, QuantumAST operand) {
        this.nodeType = NodeType.OPERATION;
        this.operationType = operation;
        this.children = new ArrayList<>();
        this.children.add(operand);
        
        // Set parent reference
        operand.parent = this;
        
        this.isEvaluated = false;
        this.isSimplified = false;
        this.derivationHistory = "Unary Operation: " + operation + "(" + operand.derivationHistory + ")";
    }
    
    /**
     * Constructor for function nodes
     */
    public QuantumAST(FunctionType function, QuantumAST argument) {
        this.nodeType = NodeType.FUNCTION;
        this.functionType = function;
        this.children = new ArrayList<>();
        this.children.add(argument);
        
        // Set parent reference
        argument.parent = this;
        
        this.isEvaluated = false;
        this.isSimplified = false;
        this.derivationHistory = "Function: " + function + "(" + argument.derivationHistory + ")";
    }
    
    /**
     * Constructor for variable nodes
     */
    public QuantumAST(String variableName) {
        this.nodeType = NodeType.VARIABLE;
        this.variableName = variableName;
        this.children = new ArrayList<>();
        this.isEvaluated = false;
        this.isSimplified = true; // Variables are already in simplest form
        this.derivationHistory = "Variable: " + variableName;
    }
    
    /**
     * Static factory method to create operation nodes
     */
    public static QuantumAST createOperation(OperationType operation, QuantumAST left, QuantumAST right) {
        return new QuantumAST(operation, left, right);
    }
    
    /**
     * Static factory method to create unary operation nodes
     */
    public static QuantumAST createUnaryOperation(OperationType operation, QuantumAST operand) {
        return new QuantumAST(operation, operand);
    }
    
    /**
     * Static factory method to create function nodes
     */
    public static QuantumAST createFunction(FunctionType function, QuantumAST argument) {
        return new QuantumAST(function, argument);
    }
    
    /**
     * Get the node type
     */
    public NodeType getNodeType() {
        return nodeType;
    }
    
    /**
     * Get the operation type (for operation nodes)
     */
    public OperationType getOperationType() {
        return operationType;
    }
    
    /**
     * Get the function type (for function nodes)
     */
    public FunctionType getFunctionType() {
        return functionType;
    }
    
    /**
     * Get the Quantum Number value (for leaf nodes)
     */
    public SimpleQuantumNumber getQuantumValue() {
        return quantumValue;
    }
    
    /**
     * Get the variable name (for variable nodes)
     */
    public String getVariableName() {
        return variableName;
    }
    
    /**
     * Get the children of this node
     */
    public List<QuantumAST> getChildren() {
        return new ArrayList<>(children);
    }
    
    /**
     * Get the parent of this node
     */
    public QuantumAST getParent() {
        return parent;
    }
    
    /**
     * Check if this AST has been evaluated
     */
    public boolean isEvaluated() {
        return isEvaluated;
    }
    
    /**
     * Check if this AST has been simplified
     */
    public boolean isSimplified() {
        return isSimplified;
    }
    
    /**
     * Get the derivation history
     */
    public String getDerivationHistory() {
        return derivationHistory;
    }
    
    /**
     * Quantition - Symbolic evaluation of the AST
     * This is the core process that converts symbolic expressions into Quantum Numbers
     */
    public SimpleQuantumNumber quantition() {
        if (isEvaluated && nodeType == NodeType.QUANTUM_NUMBER) {
            return new SimpleQuantumNumber(quantumValue);
        }
        
        switch (nodeType) {
            case QUANTUM_NUMBER:
                return new SimpleQuantumNumber(quantumValue);
                
            case OPERATION:
                return evaluateOperation();
                
            case FUNCTION:
                return evaluateFunction();
                
            case VARIABLE:
                throw new UnsupportedOperationException("Cannot evaluate variable without substitution: " + variableName);
                
            case CONSTANT:
                throw new UnsupportedOperationException("Constant evaluation not implemented");
                
            default:
                throw new IllegalStateException("Unknown node type: " + nodeType);
        }
    }
    
    /**
     * Evaluate operation nodes
     */
    private SimpleQuantumNumber evaluateOperation() {
        switch (operationType) {
            case ADD:
                if (children.size() != 2) {
                    throw new IllegalStateException("ADD operation requires exactly 2 operands");
                }
                SimpleQuantumNumber left = children.get(0).quantition();
                SimpleQuantumNumber right = children.get(1).quantition();
                return left.add(right);
                
            case SUBTRACT:
                if (children.size() != 2) {
                    throw new IllegalStateException("SUBTRACT operation requires exactly 2 operands");
                }
                left = children.get(0).quantition();
                right = children.get(1).quantition();
                return left.subtract(right);
                
            case MULTIPLY:
                if (children.size() != 2) {
                    throw new IllegalStateException("MULTIPLY operation requires exactly 2 operands");
                }
                left = children.get(0).quantition();
                right = children.get(1).quantition();
                return multiplyQuantumNumbers(left, right);
                
            case DIVIDE:
                if (children.size() != 2) {
                    throw new IllegalStateException("DIVIDE operation requires exactly 2 operands");
                }
                left = children.get(0).quantition();
                right = children.get(1).quantition();
                return divideQuantumNumbers(left, right);
                
            case NEGATE:
                if (children.size() != 1) {
                    throw new IllegalStateException("NEGATE operation requires exactly 1 operand");
                }
                SimpleQuantumNumber operand = children.get(0).quantition();
                return operand.negate();
                
            case POWER:
            case ROOT:
                throw new UnsupportedOperationException("Operation not yet implemented: " + operationType);
                
            default:
                throw new IllegalStateException("Unknown operation type: " + operationType);
        }
    }
    
    /**
     * Evaluate function nodes
     */
    private SimpleQuantumNumber evaluateFunction() {
        if (children.size() != 1) {
            throw new IllegalStateException("Function requires exactly 1 argument");
        }
        
        SimpleQuantumNumber argument = children.get(0).quantition();
        
        switch (functionType) {
            case ABS:
                return absoluteValue(argument);
                
            case SQRT:
                return squareRoot(argument);
                
            case SIN:
            case COS:
            case TAN:
            case EXP:
            case LOG:
                throw new UnsupportedOperationException("Function not yet implemented: " + functionType);
                
            default:
                throw new IllegalStateException("Unknown function type: " + functionType);
        }
    }
    
    /**
     * Multiply two Quantum Numbers (simplified implementation)
     */
    private SimpleQuantumNumber multiplyQuantumNumbers(SimpleQuantumNumber a, SimpleQuantumNumber b) {
        // This is a simplified implementation
        // In a full implementation, this would handle the complex ordinal relationships
        SimpleQuantumNumber result = new SimpleQuantumNumber();
        
        for (int i = 0; i < SimpleQuantumNumber.NUM_ORDINALS; i++) {
            int valueA = a.getSignedOrdinal(i);
            int valueB = b.getSignedOrdinal(i);
            long product = (long) valueA * valueB;
            
            // Handle overflow by clamping
            if (product > SimpleQuantumNumber.ORDINAL_MAX) product = SimpleQuantumNumber.ORDINAL_MAX;
            if (product < SimpleQuantumNumber.ORDINAL_MIN) product = SimpleQuantumNumber.ORDINAL_MIN;
            
            result.setOrdinal(i, Math.abs((int) product));
            result.setSign(i, product < 0);
        }
        
        result.updateChecksum();
        return result;
    }
    
    /**
     * Divide two Quantum Numbers (with symbolic divide-by-zero handling)
     */
    private SimpleQuantumNumber divideQuantumNumbers(SimpleQuantumNumber a, SimpleQuantumNumber b) {
        // Check for division by zero
        if (b.isZero()) {
            // In the Quantum Number system, division by zero is handled symbolically
            // For this reference implementation, we create a special representation
            SimpleQuantumNumber result = new SimpleQuantumNumber();
            result.setOrdinal(0, 1);      // a = 1
            result.setOrdinal(1, 0);      // b = 0 (represents division by zero)
            result.setOrdinal(2, 1);      // c = 1
            // Set other ordinals to represent symbolic division by zero
            for (int i = 3; i < SimpleQuantumNumber.NUM_ORDINALS; i++) {
                result.setOrdinal(i, 1);
            }
            result.updateChecksum();
            return result;
        }
        
        // Simplified division implementation
        SimpleQuantumNumber result = new SimpleQuantumNumber();
        
        for (int i = 0; i < SimpleQuantumNumber.NUM_ORDINALS; i++) {
            int valueA = a.getSignedOrdinal(i);
            int valueB = b.getSignedOrdinal(i);
            
            if (valueB == 0) {
                // Handle individual ordinal division by zero symbolically
                result.setOrdinal(i, 1);
                result.setSign(i, false);
            } else {
                int quotient = valueA / valueB;
                result.setOrdinal(i, Math.abs(quotient));
                result.setSign(i, quotient < 0);
            }
        }
        
        result.updateChecksum();
        return result;
    }
    
    /**
     * Absolute value of a Quantum Number
     */
    private SimpleQuantumNumber absoluteValue(SimpleQuantumNumber q) {
        SimpleQuantumNumber result = new SimpleQuantumNumber(q);
        
        // Set all signs to positive
        for (int i = 0; i < SimpleQuantumNumber.NUM_ORDINALS; i++) {
            result.setSign(i, false);
        }
        
        result.updateChecksum();
        return result;
    }
    
    /**
     * Square root of a Quantum Number (simplified implementation)
     */
    private SimpleQuantumNumber squareRoot(SimpleQuantumNumber q) {
        // This is a very simplified implementation
        // A full implementation would handle the complex mathematical relationships
        SimpleQuantumNumber result = new SimpleQuantumNumber();
        
        for (int i = 0; i < SimpleQuantumNumber.NUM_ORDINALS; i++) {
            int value = q.getOrdinal(i);
            int sqrt = (int) Math.sqrt(value);
            result.setOrdinal(i, sqrt);
            result.setSign(i, false); // Square root is always positive
        }
        
        result.updateChecksum();
        return result;
    }
    
    /**
     * Simplify the AST by applying algebraic rules
     */
    public QuantumAST simplify() {
        if (isSimplified) {
            return this;
        }
        
        // Recursively simplify children first
        for (QuantumAST child : children) {
            child.simplify();
        }
        
        // Apply simplification rules based on node type
        switch (nodeType) {
            case OPERATION:
                return simplifyOperation();
                
            case FUNCTION:
                return simplifyFunction();
                
            default:
                this.isSimplified = true;
                return this;
        }
    }
    
    /**
     * Simplify operation nodes
     */
    private QuantumAST simplifyOperation() {
        switch (operationType) {
            case ADD:
                // x + 0 = x
                if (children.get(1).nodeType == NodeType.QUANTUM_NUMBER && 
                    children.get(1).quantumValue.isZero()) {
                    return children.get(0);
                }
                if (children.get(0).nodeType == NodeType.QUANTUM_NUMBER && 
                    children.get(0).quantumValue.isZero()) {
                    return children.get(1);
                }
                break;
                
            case MULTIPLY:
                // x * 1 = x
                if (children.get(1).nodeType == NodeType.QUANTUM_NUMBER) {
                    SimpleQuantumNumber one = SimpleQuantumNumber.one();
                    if (children.get(1).quantumValue.equals(one)) {
                        return children.get(0);
                    }
                }
                if (children.get(0).nodeType == NodeType.QUANTUM_NUMBER) {
                    SimpleQuantumNumber one = SimpleQuantumNumber.one();
                    if (children.get(0).quantumValue.equals(one)) {
                        return children.get(1);
                    }
                }
                // x * 0 = 0
                if (children.get(1).nodeType == NodeType.QUANTUM_NUMBER && 
                    children.get(1).quantumValue.isZero()) {
                    return new QuantumAST(NodeType.QUANTUM_NUMBER, SimpleQuantumNumber.zero());
                }
                if (children.get(0).nodeType == NodeType.QUANTUM_NUMBER && 
                    children.get(0).quantumValue.isZero()) {
                    return new QuantumAST(NodeType.QUANTUM_NUMBER, SimpleQuantumNumber.zero());
                }
                break;
        }
        
        this.isSimplified = true;
        return this;
    }
    
    /**
     * Simplify function nodes
     */
    private QuantumAST simplifyFunction() {
        // Function-specific simplification rules would go here
        this.isSimplified = true;
        return this;
    }
    
    /**
     * String representation of the AST
     */
    @Override
    public String toString() {
        return toString(0);
    }
    
    /**
     * String representation with indentation for tree structure
     */
    private String toString(int depth) {
        StringBuilder sb = new StringBuilder();
        String indent = "  ".repeat(depth);
        
        sb.append(indent);
        
        switch (nodeType) {
            case QUANTUM_NUMBER:
                sb.append("QuantumNumber: ").append(quantumValue.toString().replaceAll("\n", "\n" + indent));
                break;
                
            case OPERATION:
                sb.append("Operation: ").append(operationType);
                for (QuantumAST child : children) {
                    sb.append("\n").append(child.toString(depth + 1));
                }
                break;
                
            case FUNCTION:
                sb.append("Function: ").append(functionType);
                for (QuantumAST child : children) {
                    sb.append("\n").append(child.toString(depth + 1));
                }
                break;
                
            case VARIABLE:
                sb.append("Variable: ").append(variableName);
                break;
                
            case CONSTANT:
                sb.append("Constant: [not implemented]");
                break;
        }
        
        return sb.toString();
    }
    
    /**
     * Get the height of the AST
     */
    public int getHeight() {
        if (children.isEmpty()) {
            return 1;
        }
        
        int maxChildHeight = 0;
        for (QuantumAST child : children) {
            maxChildHeight = Math.max(maxChildHeight, child.getHeight());
        }
        
        return maxChildHeight + 1;
    }
    
    /**
     * Get the number of nodes in the AST
     */
    public int getNodeCount() {
        int count = 1; // This node
        
        for (QuantumAST child : children) {
            count += child.getNodeCount();
        }
        
        return count;
    }
    
    /**
     * Check if the AST contains any variables
     */
    public boolean hasVariables() {
        if (nodeType == NodeType.VARIABLE) {
            return true;
        }
        
        for (QuantumAST child : children) {
            if (child.hasVariables()) {
                return true;
            }
        }
        
        return false;
    }
}
