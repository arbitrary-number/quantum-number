/**
 * Quantix OS - Advanced Mathematical Scheduler Test
 * 
 * Test program demonstrating the enhanced mathematical scheduling system
 * with AST-based process management and symbolic computation optimization.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 * 
 * Date: August 19, 2025
 */

#include "../kernel/quantix_scheduler.h"
#include "../kernel/quantum_number.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test helper functions
static ast_node_t* create_test_ast_simple_addition(void);
static ast_node_t* create_test_ast_complex_expression(void);
static ast_node_t* create_test_ast_symbolic_computation(void);
static void test_quantix_scheduler_init(void);
static void test_quantix_process_creation(void);
static void test_ast_evaluation(void);
static void test_mathematical_scheduling(void);
static void test_symbolic_computation(void);
static void test_process_priority_calculation(void);
static void test_ast_complexity_calculation(void);
static void test_symbolic_stack_operations(void);
static void print_test_results(const char *test_name, int passed, int total);

// Global test statistics
static int total_tests = 0;
static int passed_tests = 0;

/**
 * Main test function
 */
int main(void) {
    printf("=== Quantix OS Advanced Mathematical Scheduler Test ===\n\n");
    
    // Initialize memory management (simplified for testing)
    if (qm_init(16 * 1024 * 1024) != QM_SUCCESS) {
        printf("ERROR: Failed to initialize memory management\n");
        return 1;
    }
    
    // Run test suite
    test_quantix_scheduler_init();
    test_quantix_process_creation();
    test_ast_evaluation();
    test_mathematical_scheduling();
    test_symbolic_computation();
    test_process_priority_calculation();
    test_ast_complexity_calculation();
    test_symbolic_stack_operations();
    
    // Print final results
    printf("\n=== Test Results Summary ===\n");
    printf("Total Tests: %d\n", total_tests);
    printf("Passed Tests: %d\n", passed_tests);
    printf("Failed Tests: %d\n", total_tests - passed_tests);
    printf("Success Rate: %.1f%%\n", 
           total_tests > 0 ? (100.0 * passed_tests / total_tests) : 0.0);
    
    if (passed_tests == total_tests) {
        printf("🎉 ALL TESTS PASSED! 🎉\n");
        return 0;
    } else {
        printf("❌ Some tests failed.\n");
        return 1;
    }
}

/**
 * Test Quantix scheduler initialization
 */
static void test_quantix_scheduler_init(void) {
    printf("Testing Quantix Scheduler Initialization...\n");
    int test_passed = 0;
    int test_total = 3;
    
    // Test 1: Initialize scheduler
    if (quantix_scheduler_init() == QUANTIX_SUCCESS) {
        test_passed++;
        printf("  ✓ Scheduler initialization successful\n");
    } else {
        printf("  ✗ Scheduler initialization failed\n");
    }
    
    // Test 2: Get scheduler state
    quantix_scheduler_state_t *state = quantix_get_scheduler_state();
    if (state != NULL) {
        test_passed++;
        printf("  ✓ Scheduler state retrieval successful\n");
    } else {
        printf("  ✗ Scheduler state retrieval failed\n");
    }
    
    // Test 3: Verify initial state
    if (state && state->mathematical_load == 0 && state->symbolic_load == 0) {
        test_passed++;
        printf("  ✓ Initial scheduler state is correct\n");
    } else {
        printf("  ✗ Initial scheduler state is incorrect\n");
    }
    
    print_test_results("Quantix Scheduler Initialization", test_passed, test_total);
}

/**
 * Test Quantix process creation
 */
static void test_quantix_process_creation(void) {
    printf("\nTesting Quantix Process Creation...\n");
    int test_passed = 0;
    int test_total = 5;
    
    // Create test AST
    ast_node_t *test_ast = create_test_ast_simple_addition();
    
    // Test 1: Create mathematical process
    quantix_process_t *math_process = quantix_create_process(
        "MathProcess", test_ast, QP_PRIORITY_NORMAL, QP_FLAG_MATHEMATICAL);
    
    if (math_process != NULL) {
        test_passed++;
        printf("  ✓ Mathematical process creation successful\n");
    } else {
        printf("  ✗ Mathematical process creation failed\n");
    }
    
    // Test 2: Create symbolic process
    quantix_process_t *symbolic_process = quantix_create_process(
        "SymbolicProcess", test_ast, QP_PRIORITY_HIGH, QP_FLAG_SYMBOLIC);
    
    if (symbolic_process != NULL) {
        test_passed++;
        printf("  ✓ Symbolic process creation successful\n");
    } else {
        printf("  ✗ Symbolic process creation failed\n");
    }
    
    // Test 3: Verify process properties
    if (math_process && math_process->pid > 0 && 
        strcmp(math_process->name, "MathProcess") == 0) {
        test_passed++;
        printf("  ✓ Process properties are correct\n");
    } else {
        printf("  ✗ Process properties are incorrect\n");
    }
    
    // Test 4: Start mathematical process
    if (math_process && quantix_start_process(math_process) == QUANTIX_SUCCESS) {
        test_passed++;
        printf("  ✓ Mathematical process start successful\n");
    } else {
        printf("  ✗ Mathematical process start failed\n");
    }
    
    // Test 5: Start symbolic process
    if (symbolic_process && quantix_start_process(symbolic_process) == QUANTIX_SUCCESS) {
        test_passed++;
        printf("  ✓ Symbolic process start successful\n");
    } else {
        printf("  ✗ Symbolic process start failed\n");
    }
    
    print_test_results("Quantix Process Creation", test_passed, test_total);
}

/**
 * Test AST evaluation
 */
static void test_ast_evaluation(void) {
    printf("\nTesting AST Evaluation...\n");
    int test_passed = 0;
    int test_total = 4;
    
    // Create test process
    ast_node_t *test_ast = create_test_ast_simple_addition();
    quantix_process_t *process = quantix_create_process(
        "ASTTestProcess", test_ast, QP_PRIORITY_NORMAL, QP_FLAG_MATHEMATICAL);
    
    if (!process) {
        printf("  ✗ Failed to create test process for AST evaluation\n");
        print_test_results("AST Evaluation", 0, test_total);
        return;
    }
    
    quantix_start_process(process);
    
    // Test 1: Evaluate simple AST node
    if (quantix_evaluate_ast_node(process, test_ast) == QUANTIX_SUCCESS) {
        test_passed++;
        printf("  ✓ AST node evaluation successful\n");
    } else {
        printf("  ✗ AST node evaluation failed\n");
    }
    
    // Test 2: Check if result is on stack
    if (process->symbolic_stack_pointer > 0) {
        test_passed++;
        printf("  ✓ Result pushed to symbolic stack\n");
    } else {
        printf("  ✗ No result on symbolic stack\n");
    }
    
    // Test 3: Verify evaluation statistics
    if (process->math_metrics.ast_nodes_evaluated > 0) {
        test_passed++;
        printf("  ✓ Evaluation statistics updated\n");
    } else {
        printf("  ✗ Evaluation statistics not updated\n");
    }
    
    // Test 4: Test complex AST evaluation
    ast_node_t *complex_ast = create_test_ast_complex_expression();
    if (quantix_evaluate_ast_node(process, complex_ast) == QUANTIX_SUCCESS) {
        test_passed++;
        printf("  ✓ Complex AST evaluation successful\n");
    } else {
        printf("  ✗ Complex AST evaluation failed\n");
    }
    
    print_test_results("AST Evaluation", test_passed, test_total);
}

/**
 * Test mathematical scheduling
 */
static void test_mathematical_scheduling(void) {
    printf("\nTesting Mathematical Scheduling...\n");
    int test_passed = 0;
    int test_total = 3;
    
    // Test 1: Schedule mathematical process
    quantix_process_t *scheduled = quantix_schedule_mathematical_process();
    if (scheduled != NULL) {
        test_passed++;
        printf("  ✓ Mathematical process scheduling successful\n");
    } else {
        printf("  ✗ Mathematical process scheduling failed\n");
    }
    
    // Test 2: Schedule symbolic process
    quantix_process_t *symbolic_scheduled = quantix_schedule_symbolic_process();
    if (symbolic_scheduled != NULL) {
        test_passed++;
        printf("  ✓ Symbolic process scheduling successful\n");
    } else {
        printf("  ✗ Symbolic process scheduling failed\n");
    }
    
    // Test 3: Verify scheduling statistics
    quantix_scheduler_state_t *state = quantix_get_scheduler_state();
    if (state && (state->mathematical_operations_scheduled > 0 || 
                  state->symbolic_computations_scheduled > 0)) {
        test_passed++;
        printf("  ✓ Scheduling statistics updated\n");
    } else {
        printf("  ✗ Scheduling statistics not updated\n");
    }
    
    print_test_results("Mathematical Scheduling", test_passed, test_total);
}

/**
 * Test symbolic computation
 */
static void test_symbolic_computation(void) {
    printf("\nTesting Symbolic Computation...\n");
    int test_passed = 0;
    int test_total = 4;
    
    // Create test process
    quantix_process_t *process = quantix_create_process(
        "SymbolicTest", NULL, QP_PRIORITY_NORMAL, QP_FLAG_SYMBOLIC);
    
    if (!process) {
        printf("  ✗ Failed to create test process for symbolic computation\n");
        print_test_results("Symbolic Computation", 0, test_total);
        return;
    }
    
    quantix_start_process(process);
    
    // Test 1: Push values to symbolic stack
    quantum_number_t value1, value2;
    qn_set_ordinal(&value1, 0, 10);
    qn_set_ordinal(&value2, 0, 5);
    
    if (quantix_push_symbolic_value(process, &value1) == QUANTIX_SUCCESS &&
        quantix_push_symbolic_value(process, &value2) == QUANTIX_SUCCESS) {
        test_passed++;
        printf("  ✓ Symbolic stack push operations successful\n");
    } else {
        printf("  ✗ Symbolic stack push operations failed\n");
    }
    
    // Test 2: Perform symbolic addition
    if (quantix_perform_symbolic_operation(process, "add") == QUANTIX_SUCCESS) {
        test_passed++;
        printf("  ✓ Symbolic addition operation successful\n");
    } else {
        printf("  ✗ Symbolic addition operation failed\n");
    }
    
    // Test 3: Verify result
    quantum_number_t *result = quantix_pop_symbolic_value(process);
    if (result && qn_get_ordinal(result, 0) == 15) {
        test_passed++;
        printf("  ✓ Symbolic computation result is correct (10 + 5 = 15)\n");
    } else {
        printf("  ✗ Symbolic computation result is incorrect\n");
    }
    
    // Test 4: Verify operation statistics
    if (process->mathematical_operations_total > 0) {
        test_passed++;
        printf("  ✓ Operation statistics updated\n");
    } else {
        printf("  ✗ Operation statistics not updated\n");
    }
    
    print_test_results("Symbolic Computation", test_passed, test_total);
}

/**
 * Test process priority calculation
 */
static void test_process_priority_calculation(void) {
    printf("\nTesting Process Priority Calculation...\n");
    int test_passed = 0;
    int test_total = 3;
    
    // Create test processes with different priorities
    ast_node_t *test_ast = create_test_ast_complex_expression();
    
    quantix_process_t *high_priority = quantix_create_process(
        "HighPriority", test_ast, QP_PRIORITY_HIGH, QP_FLAG_MATHEMATICAL);
    quantix_process_t *normal_priority = quantix_create_process(
        "NormalPriority", test_ast, QP_PRIORITY_NORMAL, QP_FLAG_MATHEMATICAL);
    
    if (!high_priority || !normal_priority) {
        printf("  ✗ Failed to create test processes\n");
        print_test_results("Process Priority Calculation", 0, test_total);
        return;
    }
    
    // Test 1: Calculate mathematical priority for high priority process
    if (quantix_calculate_mathematical_priority(high_priority) == QUANTIX_SUCCESS) {
        test_passed++;
        printf("  ✓ High priority process calculation successful\n");
    } else {
        printf("  ✗ High priority process calculation failed\n");
    }
    
    // Test 2: Calculate mathematical priority for normal priority process
    if (quantix_calculate_mathematical_priority(normal_priority) == QUANTIX_SUCCESS) {
        test_passed++;
        printf("  ✓ Normal priority process calculation successful\n");
    } else {
        printf("  ✗ Normal priority process calculation failed\n");
    }
    
    // Test 3: Verify priority ordering
    uint32_t high_pri = qn_get_ordinal(&high_priority->math_metrics.mathematical_priority, 0);
    uint32_t normal_pri = qn_get_ordinal(&normal_priority->math_metrics.mathematical_priority, 0);
    
    if (high_pri > normal_pri) {
        test_passed++;
        printf("  ✓ Priority ordering is correct (high: %u > normal: %u)\n", 
               high_pri, normal_pri);
    } else {
        printf("  ✗ Priority ordering is incorrect (high: %u <= normal: %u)\n", 
               high_pri, normal_pri);
    }
    
    print_test_results("Process Priority Calculation", test_passed, test_total);
}

/**
 * Test AST complexity calculation
 */
static void test_ast_complexity_calculation(void) {
    printf("\nTesting AST Complexity Calculation...\n");
    int test_passed = 0;
    int test_total = 3;
    
    // Test 1: Simple AST complexity
    ast_node_t *simple_ast = create_test_ast_simple_addition();
    uint32_t simple_complexity = quantix_calculate_ast_complexity(simple_ast);
    
    if (simple_complexity > 0) {
        test_passed++;
        printf("  ✓ Simple AST complexity calculation successful (%u)\n", 
               simple_complexity);
    } else {
        printf("  ✗ Simple AST complexity calculation failed\n");
    }
    
    // Test 2: Complex AST complexity
    ast_node_t *complex_ast = create_test_ast_complex_expression();
    uint32_t complex_complexity = quantix_calculate_ast_complexity(complex_ast);
    
    if (complex_complexity > simple_complexity) {
        test_passed++;
        printf("  ✓ Complex AST has higher complexity (%u > %u)\n", 
               complex_complexity, simple_complexity);
    } else {
        printf("  ✗ Complex AST complexity is not higher than simple AST\n");
    }
    
    // Test 3: Symbolic AST complexity
    ast_node_t *symbolic_ast = create_test_ast_symbolic_computation();
    uint32_t symbolic_complexity = quantix_calculate_ast_complexity(symbolic_ast);
    
    if (symbolic_complexity > 0) {
        test_passed++;
        printf("  ✓ Symbolic AST complexity calculation successful (%u)\n", 
               symbolic_complexity);
    } else {
        printf("  ✗ Symbolic AST complexity calculation failed\n");
    }
    
    print_test_results("AST Complexity Calculation", test_passed, test_total);
}

/**
 * Test symbolic stack operations
 */
static void test_symbolic_stack_operations(void) {
    printf("\nTesting Symbolic Stack Operations...\n");
    int test_passed = 0;
    int test_total = 4;
    
    // Create test process
    quantix_process_t *process = quantix_create_process(
        "StackTest", NULL, QP_PRIORITY_NORMAL, QP_FLAG_SYMBOLIC);
    
    if (!process) {
        printf("  ✗ Failed to create test process\n");
        print_test_results("Symbolic Stack Operations", 0, test_total);
        return;
    }
    
    // Test 1: Push multiple values
    quantum_number_t values[3];
    qn_set_ordinal(&values[0], 0, 100);
    qn_set_ordinal(&values[1], 0, 200);
    qn_set_ordinal(&values[2], 0, 300);
    
    int push_success = 1;
    for (int i = 0; i < 3; i++) {
        if (quantix_push_symbolic_value(process, &values[i]) != QUANTIX_SUCCESS) {
            push_success = 0;
            break;
        }
    }
    
    if (push_success && process->symbolic_stack_pointer == 3) {
        test_passed++;
        printf("  ✓ Multiple push operations successful\n");
    } else {
        printf("  ✗ Multiple push operations failed\n");
    }
    
    // Test 2: Pop values in LIFO order
    quantum_number_t *popped1 = quantix_pop_symbolic_value(process);
    quantum_number_t *popped2 = quantix_pop_symbolic_value(process);
    
    if (popped1 && popped2 && 
        qn_get_ordinal(popped1, 0) == 300 && 
        qn_get_ordinal(popped2, 0) == 200) {
        test_passed++;
        printf("  ✓ LIFO pop operations successful\n");
    } else {
        printf("  ✗ LIFO pop operations failed\n");
    }
    
    // Test 3: Stack pointer management
    if (process->symbolic_stack_pointer == 1) {
        test_passed++;
        printf("  ✓ Stack pointer management correct\n");
    } else {
        printf("  ✗ Stack pointer management incorrect (expected 1, got %zu)\n", 
               process->symbolic_stack_pointer);
    }
    
    // Test 4: Pop remaining value
    quantum_number_t *popped3 = quantix_pop_symbolic_value(process);
    if (popped3 && qn_get_ordinal(popped3, 0) == 100 && 
        process->symbolic_stack_pointer == 0) {
        test_passed++;
        printf("  ✓ Final pop operation successful\n");
    } else {
        printf("  ✗ Final pop operation failed\n");
    }
    
    print_test_results("Symbolic Stack Operations", test_passed, test_total);
}

/**
 * Helper function to create simple addition AST: 5 + 3
 */
static ast_node_t* create_test_ast_simple_addition(void) {
    // Create root operation node (addition)
    ast_node_t *root = quantix_create_ast_node(AST_NODE_OPERATION);
    strcpy(root->operation, "add");
    
    // Create left operand (5)
    ast_node_t *left = quantix_create_ast_node(AST_NODE_QUANTUM_NUMBER);
    qn_set_ordinal(&left->value, 0, 5);
    
    // Create right operand (3)
    ast_node_t *right = quantix_create_ast_node(AST_NODE_QUANTUM_NUMBER);
    qn_set_ordinal(&right->value, 0, 3);
    
    // Connect nodes
    quantix_add_ast_child(root, left);
    quantix_add_ast_child(root, right);
    
    return root;
}

/**
 * Helper function to create complex expression AST: (10 * 2) + (8 / 4)
 */
static ast_node_t* create_test_ast_complex_expression(void) {
    // Create root addition node
    ast_node_t *root = quantix_create_ast_node(AST_NODE_OPERATION);
    strcpy(root->operation, "add");
    
    // Create left multiplication: 10 * 2
    ast_node_t *left_mult = quantix_create_ast_node(AST_NODE_OPERATION);
    strcpy(left_mult->operation, "multiply");
    
    ast_node_t *ten = quantix_create_ast_node(AST_NODE_QUANTUM_NUMBER);
    qn_set_ordinal(&ten->value, 0, 10);
    
    ast_node_t *two = quantix_create_ast_node(AST_NODE_QUANTUM_NUMBER);
    qn_set_ordinal(&two->value, 0, 2);
    
    quantix_add_ast_child(left_mult, ten);
    quantix_add_ast_child(left_mult, two);
    
    // Create right division: 8 / 4
    ast_node_t *right_div = quantix_create_ast_node(AST_NODE_OPERATION);
    strcpy(right_div->operation, "divide");
    
    ast_node_t *eight = quantix_create_ast_node(AST_NODE_QUANTUM_NUMBER);
    qn_set_ordinal(&eight->value, 0, 8);
    
    ast_node_t *four = quantix_create_ast_node(AST_NODE_QUANTUM_NUMBER);
    qn_set_ordinal(&four->value, 0, 4);
    
    quantix_add_ast_child(right_div, eight);
    quantix_add_ast_child(right_div, four);
    
    // Connect to root
    quantix_add_ast_child(root, left_mult);
    quantix_add_ast_child(root, right_div);
    
    return root;
}

/**
 * Helper function to create symbolic computation AST
 */
static ast_node_t* create_test_ast_symbolic_computation(void) {
    // Create symbolic expression node
    ast_node_t *root = quantix_create_ast_node(AST_NODE_SYMBOLIC_EXPRESSION);
    
    // Create deferred evaluation node
    ast_node_t *deferred = quantix_create_ast_node(AST_NODE_DEFERRED_EVALUATION);
    
    // Create simple operation inside deferred evaluation
    ast_node_t *operation = quantix_create_ast_node(AST_NODE_OPERATION);
    strcpy(operation->operation, "multiply");
    
    ast_node_t *value1 = quantix_create_ast_node(AST_NODE_QUANTUM_NUMBER);
    qn_set_ordinal(&value1->value, 0, 7);
    
    ast_node_t *value2 = quantix_create_ast_node(AST_NODE_QUANTUM_NUMBER);
    qn_set_ordinal(&value2->value, 0, 6);
    
    quantix_add_ast_child(operation, value1);
    quantix_add_ast_child(operation, value2);
    quantix_add_ast_child(deferred, operation);
    quantix_add_ast_child(root, deferred);
    
    return root;
}

/**
 * Helper function to print test results
 */
static void print_test_results(const char *test_name, int passed, int total) {
    total_tests += total;
    passed_tests += passed;
    
    printf("  Results: %d/%d passed", passed, total);
    if (passed == total) {
        printf(" ✅\n");
    } else {
        printf(" ❌\n");
    }
}
