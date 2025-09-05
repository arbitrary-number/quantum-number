package com.github.quantum_number;

/**
 * Quantum Number System - Main Application
 * 
 * This application demonstrates the Quantum Number System implementation
 * in Java, providing a reference implementation for 256-bit symbolic
 * numeric computation.
 */
public class App 
{
    public static void main( String[] args )
    {
        System.out.println("=== Quantum Number System ===");
        System.out.println("256-bit Symbolic Numeric Computation");
        System.out.println();
        
        // Create sample Quantum Numbers
        SimpleQuantumNumber q1 = new SimpleQuantumNumber();
        q1.setOrdinal(0, 12345);  // a = 12345
        q1.setOrdinal(1, 67890);  // b = 67890
        q1.setOrdinal(2, 1);      // c = 1
        q1.setOrdinal(3, 1);      // d = 1
        q1.setOrdinal(4, 1);      // e = 1
        q1.setOrdinal(5, 1);      // f = 1
        q1.setOrdinal(6, 0);      // g = 0
        q1.setOrdinal(7, 0);      // h = 0
        q1.setOrdinal(8, 1);      // i = 1
        q1.setOrdinal(9, 1);      // j = 1
        q1.setOrdinal(10, 1);     // k = 1
        q1.setOrdinal(11, 1);     // l = 1
        q1.updateChecksum();
        
        SimpleQuantumNumber q2 = new SimpleQuantumNumber();
        q2.setOrdinal(0, 54321);  // a = 54321
        q2.setOrdinal(1, 98765);  // b = 98765
        q2.setOrdinal(2, 1);      // c = 1
        q2.setOrdinal(3, 1);      // d = 1
        q2.setOrdinal(4, 1);      // e = 1
        q2.setOrdinal(5, 1);      // f = 1
        q2.setOrdinal(6, 0);      // g = 0
        q2.setOrdinal(7, 0);      // h = 0
        q2.setOrdinal(8, 1);      // i = 1
        q2.setOrdinal(9, 1);      // j = 1
        q2.setOrdinal(10, 1);     // k = 1
        q2.setOrdinal(11, 1);     // l = 1
        q2.updateChecksum();
        
        System.out.println("Quantum Number 1:");
        System.out.println(q1.toString());
        System.out.println();
        
        System.out.println("Quantum Number 2:");
        System.out.println(q2.toString());
        System.out.println();
        
        // Demonstrate symbolic addition
        QuantumAST ast1 = new QuantumAST(QuantumAST.NodeType.QUANTUM_NUMBER, q1);
        QuantumAST ast2 = new QuantumAST(QuantumAST.NodeType.QUANTUM_NUMBER, q2);
        QuantumAST addResult = QuantumAST.createOperation(QuantumAST.OperationType.ADD, ast1, ast2);
        
        System.out.println("Symbolic Addition AST:");
        System.out.println(addResult.toString());
        System.out.println();
        
        // Demonstrate quantition (symbolic evaluation)
        System.out.println("Performing Quantition (Symbolic Evaluation)...");
        SimpleQuantumNumber result = addResult.quantition();
        System.out.println("Result:");
        System.out.println(result.toString());
        System.out.println();
        
        System.out.println("=== Quantum Number System Demo Complete ===");
    }
}
