/**
 * Quantix File System (QFS) - Comprehensive Test Suite
 * 
 * Tests for mathematical file system operations, AST handling,
 * and Quantum Number storage functionality.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include "../kernel/quantix_qfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

// Test result tracking
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Test macros
#define TEST_ASSERT(condition, message) \
    do { \
        tests_run++; \
        if (condition) { \
            tests_passed++; \
            printf("✓ %s\n", message); \
        } else { \
            tests_failed++; \
            printf("✗ %s\n", message); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual, message) \
    TEST_ASSERT((expected) == (actual), message)

#define TEST_ASSERT_NOT_NULL(ptr, message) \
    TEST_ASSERT((ptr) != NULL, message)

#define TEST_ASSERT_NULL(ptr, message) \
    TEST_ASSERT((ptr) == NULL, message)

// Test helper functions
static void print_test_header(const char *test_name);
static void print_test_summary(void);
static quantum_number_t create_test_quantum_number(uint64_t value);
static ast_node_t* create_test_ast_expression(void);

/**
 * Test QFS initialization and shutdown
 */
void test_qfs_initialization(void) {
    print_test_header("QFS Initialization and Shutdown");
    
    // Test initialization
    qfs_result_t result = qfs_initialize_filesystem();
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "QFS initialization should succeed");
    
    // Test double initialization (should succeed without error)
    result = qfs_initialize_filesystem();
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Double initialization should succeed");
    
    // Test shutdown
    result = qfs_shutdown_filesystem();
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "QFS shutdown should succeed");
    
    // Test double shutdown (should succeed without error)
    result = qfs_shutdown_filesystem();
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Double shutdown should succeed");
    
    // Re-initialize for subsequent tests
    result = qfs_initialize_filesystem();
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Re-initialization should succeed");
}

/**
 * Test mathematical block allocation and management
 */
void test_mathematical_blocks(void) {
    print_test_header("Mathematical Block Operations");
    
    // Test block allocation
    size_t test_size = 1024;
    qfs_mathematical_block_t *block = qfs_allocate_mathematical_block(test_size);
    TEST_ASSERT_NOT_NULL(block, "Mathematical block allocation should succeed");
    
    if (block) {
        // Test block properties
        TEST_ASSERT_EQUAL(test_size, block->data_size, "Block data size should match requested size");
        TEST_ASSERT_NOT_NULL(block->mathematical_data, "Block data pointer should not be null");
        TEST_ASSERT_EQUAL(1, block->verification_status, "Block should be initially verified");
        
        // Test block ID uniqueness
        qfs_mathematical_block_t *block2 = qfs_allocate_mathematical_block(512);
        TEST_ASSERT_NOT_NULL(block2, "Second block allocation should succeed");
        
        if (block2) {
            // Block IDs should be different
            uint64_t id1 = quantum_number_get_ordinal(&block->block_id, 0);
            uint64_t id2 = quantum_number_get_ordinal(&block2->block_id, 0);
            TEST_ASSERT(id1 != id2, "Block IDs should be unique");
            
            qfs_free_mathematical_block(block2);
        }
        
        qfs_free_mathematical_block(block);
    }
    
    // Test zero-size block allocation
    qfs_mathematical_block_t *zero_block = qfs_allocate_mathematical_block(0);
    TEST_ASSERT_NOT_NULL(zero_block, "Zero-size block allocation should succeed");
    
    if (zero_block) {
        TEST_ASSERT_EQUAL(0, zero_block->data_size, "Zero-size block should have zero data size");
        TEST_ASSERT_NULL(zero_block->mathematical_data, "Zero-size block data should be null");
        qfs_free_mathematical_block(zero_block);
    }
}

/**
 * Test AST node creation and manipulation
 */
void test_ast_operations(void) {
    print_test_header("AST Node Operations");
    
    // Test AST node creation for different types
    ast_node_t *quantum_node = qfs_create_ast_node(AST_QUANTUM_NUMBER);
    TEST_ASSERT_NOT_NULL(quantum_node, "Quantum number AST node creation should succeed");
    TEST_ASSERT_EQUAL(AST_QUANTUM_NUMBER, quantum_node->type, "Node type should be quantum number");
    
    ast_node_t *variable_node = qfs_create_ast_node(AST_VARIABLE);
    TEST_ASSERT_NOT_NULL(variable_node, "Variable AST node creation should succeed");
    TEST_ASSERT_EQUAL(AST_VARIABLE, variable_node->type, "Node type should be variable");
    
    ast_node_t *operator_node = qfs_create_ast_node(AST_OPERATOR);
    TEST_ASSERT_NOT_NULL(operator_node, "Operator AST node creation should succeed");
    TEST_ASSERT_EQUAL(AST_OPERATOR, operator_node->type, "Node type should be operator");
    
    // Test AST node copying
    if (quantum_node) {
        quantum_node->data.quantum_value = create_test_quantum_number(42);
        
        ast_node_t *copied_node;
        qfs_result_t result = qfs_copy_ast_node(quantum_node, &copied_node);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "AST node copying should succeed");
        
        if (copied_node) {
            TEST_ASSERT_EQUAL(quantum_node->type, copied_node->type, "Copied node type should match");
            uint64_t orig_val = quantum_number_get_ordinal(&quantum_node->data.quantum_value, 0);
            uint64_t copy_val = quantum_number_get_ordinal(&copied_node->data.quantum_value, 0);
            TEST_ASSERT_EQUAL(orig_val, copy_val, "Copied quantum number value should match");
            
            qfs_free_ast_node(copied_node);
        }
    }
    
    // Test AST validation
    if (quantum_node) {
        qfs_result_t result = qfs_validate_ast_structure(quantum_node);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Valid AST structure should pass validation");
    }
    
    // Clean up
    qfs_free_ast_node(quantum_node);
    qfs_free_ast_node(variable_node);
    qfs_free_ast_node(operator_node);
}

/**
 * Test mathematical file creation and operations
 */
void test_mathematical_files(void) {
    print_test_header("Mathematical File Operations");
    
    // Create test metadata
    mathematical_metadata_t metadata = {0};
    metadata.primary_domain = MATH_DOMAIN_ALGEBRA;
    metadata.creation_date = time(NULL);
    strcpy(metadata.description, "Test mathematical file");
    strcpy(metadata.author, "QFS Test Suite");
    
    // Test file creation
    const char *test_path = "/test_math_file.qfs";
    qfs_result_t result = qfs_create_mathematical_file(test_path, &metadata);
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Mathematical file creation should succeed");
    
    // Test file opening
    qfs_file_handle_t handle;
    result = qfs_open_quantum_file(test_path, QFS_MODE_READ | QFS_MODE_WRITE, &handle);
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "File opening should succeed");
    
    if (result == QFS_SUCCESS) {
        TEST_ASSERT_NOT_NULL(handle.file, "File handle should contain valid file pointer");
        TEST_ASSERT_EQUAL(QFS_MODE_READ | QFS_MODE_WRITE, handle.access_mode, "Access mode should match");
        
        // Test quantum number writing and reading
        quantum_number_t test_numbers[3];
        test_numbers[0] = create_test_quantum_number(100);
        test_numbers[1] = create_test_quantum_number(200);
        test_numbers[2] = create_test_quantum_number(300);
        
        result = qfs_write_quantum_numbers(&handle, test_numbers, 3);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Quantum number writing should succeed");
        
        // Read back the numbers
        quantum_number_t read_numbers[3];
        result = qfs_read_quantum_numbers(&handle, read_numbers, 3);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Quantum number reading should succeed");
        
        // Verify the data
        for (int i = 0; i < 3; i++) {
            uint64_t written = quantum_number_get_ordinal(&test_numbers[i], 0);
            uint64_t read = quantum_number_get_ordinal(&read_numbers[i], 0);
            TEST_ASSERT_EQUAL(written, read, "Read quantum number should match written value");
        }
        
        // Test file closing
        result = qfs_close_file(&handle);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "File closing should succeed");
    }
}

/**
 * Test AST file operations
 */
void test_ast_files(void) {
    print_test_header("AST File Operations");
    
    // Create test AST expression: 5 + 10
    ast_node_t *root = qfs_create_ast_node(AST_OPERATOR);
    TEST_ASSERT_NOT_NULL(root, "Root operator node creation should succeed");
    
    if (root) {
        strcpy(root->data.operator.operator_symbol, "+");
        root->data.operator.operand_count = 2;
        root->data.operator.operands = (ast_node_t**)malloc(2 * sizeof(ast_node_t*));
        
        // Create left operand (5)
        root->data.operator.operands[0] = qfs_create_ast_node(AST_QUANTUM_NUMBER);
        root->data.operator.operands[0]->data.quantum_value = create_test_quantum_number(5);
        root->data.operator.operands[0]->parent = root;
        
        // Create right operand (10)
        root->data.operator.operands[1] = qfs_create_ast_node(AST_QUANTUM_NUMBER);
        root->data.operator.operands[1]->data.quantum_value = create_test_quantum_number(10);
        root->data.operator.operands[1]->parent = root;
        
        // Test AST saving
        const char *ast_path = "/test_expression.ast";
        qfs_result_t result = qfs_save_ast(ast_path, root);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "AST saving should succeed");
        
        // Test AST loading
        ast_node_t *loaded_ast;
        result = qfs_load_ast(ast_path, &loaded_ast);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "AST loading should succeed");
        
        if (loaded_ast) {
            TEST_ASSERT_EQUAL(AST_OPERATOR, loaded_ast->type, "Loaded AST root should be operator");
            TEST_ASSERT_EQUAL(2, loaded_ast->data.operator.operand_count, "Operator should have 2 operands");
            
            qfs_free_ast_node(loaded_ast);
        }
        
        // Test AST evaluation
        quantum_number_t eval_result;
        result = qfs_evaluate_ast_file(ast_path, &eval_result);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "AST evaluation should succeed");
        
        uint64_t result_value = quantum_number_get_ordinal(&eval_result, 0);
        TEST_ASSERT_EQUAL(15, result_value, "5 + 10 should equal 15");
        
        // Test AST optimization
        result = qfs_optimize_ast_file(ast_path);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "AST optimization should succeed");
        
        qfs_free_ast_node(root);
    }
}

/**
 * Test mathematical checksum computation
 */
void test_mathematical_checksums(void) {
    print_test_header("Mathematical Checksum Operations");
    
    // Test checksum computation
    const char test_data[] = "Test mathematical data for checksum";
    quantum_number_t checksum1, checksum2;
    
    qfs_result_t result1 = qfs_compute_mathematical_checksum(test_data, strlen(test_data), &checksum1);
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result1, "First checksum computation should succeed");
    
    qfs_result_t result2 = qfs_compute_mathematical_checksum(test_data, strlen(test_data), &checksum2);
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result2, "Second checksum computation should succeed");
    
    // Checksums should be identical for same data
    uint64_t sum1 = quantum_number_get_ordinal(&checksum1, 0);
    uint64_t sum2 = quantum_number_get_ordinal(&checksum2, 0);
    TEST_ASSERT_EQUAL(sum1, sum2, "Checksums for identical data should match");
    
    // Test checksum for different data
    const char different_data[] = "Different test data";
    quantum_number_t checksum3;
    qfs_result_t result3 = qfs_compute_mathematical_checksum(different_data, strlen(different_data), &checksum3);
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result3, "Third checksum computation should succeed");
    
    uint64_t sum3 = quantum_number_get_ordinal(&checksum3, 0);
    TEST_ASSERT(sum1 != sum3, "Checksums for different data should differ");
}

/**
 * Test mathematical indexing operations
 */
void test_mathematical_indexing(void) {
    print_test_header("Mathematical Indexing Operations");
    
    // Test index creation
    qfs_mathematical_index_t *index;
    qfs_result_t result = qfs_create_mathematical_index(&index, QFS_INDEX_QUANTUM_NUMBER);
    TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Mathematical index creation should succeed");
    TEST_ASSERT_NOT_NULL(index, "Index pointer should not be null");
    
    if (index) {
        TEST_ASSERT_EQUAL(QFS_INDEX_QUANTUM_NUMBER, index->index_type, "Index type should match");
        TEST_ASSERT_EQUAL(0, index->entry_count, "New index should have zero entries");
        
        // Test different index types
        qfs_mathematical_index_t *expr_index;
        result = qfs_create_mathematical_index(&expr_index, QFS_INDEX_SYMBOLIC_EXPRESSION);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Expression index creation should succeed");
        
        if (expr_index) {
            TEST_ASSERT_EQUAL(QFS_INDEX_SYMBOLIC_EXPRESSION, expr_index->index_type, "Expression index type should match");
            free(expr_index);
        }
        
        free(index);
    }
}

/**
 * Test error handling and edge cases
 */
void test_error_handling(void) {
    print_test_header("Error Handling and Edge Cases");
    
    // Test null pointer handling
    qfs_result_t result = qfs_create_mathematical_file(NULL, NULL);
    TEST_ASSERT_EQUAL(QFS_ERROR_INVALID_PATH, result, "Null path should return invalid path error");
    
    // Test invalid file operations
    qfs_file_handle_t invalid_handle = {0};
    result = qfs_read_quantum_numbers(&invalid_handle, NULL, 0);
    TEST_ASSERT_EQUAL(QFS_ERROR_INVALID_PATH, result, "Invalid handle should return error");
    
    // Test AST validation with null
    result = qfs_validate_ast_structure(NULL);
    TEST_ASSERT_EQUAL(QFS_ERROR_AST_MALFORMED, result, "Null AST should return malformed error");
    
    // Test file not found
    qfs_file_handle_t handle;
    result = qfs_open_quantum_file("/nonexistent_file.qfs", QFS_MODE_READ, &handle);
    TEST_ASSERT_EQUAL(QFS_ERROR_FILE_NOT_FOUND, result, "Nonexistent file should return not found error");
}

/**
 * Test performance and stress scenarios
 */
void test_performance_scenarios(void) {
    print_test_header("Performance and Stress Tests");
    
    // Test large quantum number array
    const size_t large_count = 1000;
    quantum_number_t *large_array = (quantum_number_t*)malloc(large_count * sizeof(quantum_number_t));
    TEST_ASSERT_NOT_NULL(large_array, "Large array allocation should succeed");
    
    if (large_array) {
        // Initialize array
        for (size_t i = 0; i < large_count; i++) {
            large_array[i] = create_test_quantum_number(i);
        }
        
        // Create file for large data
        mathematical_metadata_t metadata = {0};
        metadata.primary_domain = MATH_DOMAIN_ALGEBRA;
        strcpy(metadata.description, "Large data test file");
        
        qfs_result_t result = qfs_create_mathematical_file("/large_test.qfs", &metadata);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Large file creation should succeed");
        
        // Write large data
        qfs_file_handle_t handle;
        result = qfs_open_quantum_file("/large_test.qfs", QFS_MODE_WRITE, &handle);
        if (result == QFS_SUCCESS) {
            result = qfs_write_quantum_numbers(&handle, large_array, large_count);
            TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Large data write should succeed");
            qfs_close_file(&handle);
        }
        
        free(large_array);
    }
    
    // Test multiple file operations
    const int file_count = 10;
    for (int i = 0; i < file_count; i++) {
        char filename[64];
        snprintf(filename, sizeof(filename), "/stress_test_%d.qfs", i);
        
        mathematical_metadata_t metadata = {0};
        metadata.primary_domain = MATH_DOMAIN_ALGEBRA;
        snprintf(metadata.description, sizeof(metadata.description), "Stress test file %d", i);
        
        qfs_result_t result = qfs_create_mathematical_file(filename, &metadata);
        TEST_ASSERT_EQUAL(QFS_SUCCESS, result, "Stress test file creation should succeed");
    }
}

// Helper function implementations

static void print_test_header(const char *test_name) {
    printf("\n=== %s ===\n", test_name);
}

static void print_test_summary(void) {
    printf("\n==================================================\n");
    printf("TEST SUMMARY\n");
    printf("==================================================\n");
    printf("Total tests run: %d\n", tests_run);
    printf("Tests passed:    %d\n", tests_passed);
    printf("Tests failed:    %d\n", tests_failed);
    printf("Success rate:    %.1f%%\n", tests_run > 0 ? (100.0 * tests_passed / tests_run) : 0.0);
    printf("==================================================\n");
}

static quantum_number_t create_test_quantum_number(uint64_t value) {
    quantum_number_t qn;
    quantum_number_init(&qn);
    quantum_number_set_from_uint64(&qn, value);
    return qn;
}

static ast_node_t* create_test_ast_expression(void) {
    // Create simple expression: x + 42
    ast_node_t *root = qfs_create_ast_node(AST_OPERATOR);
    if (!root) return NULL;
    
    strcpy(root->data.operator.operator_symbol, "+");
    root->data.operator.operand_count = 2;
    root->data.operator.operands = (ast_node_t**)malloc(2 * sizeof(ast_node_t*));
    
    if (!root->data.operator.operands) {
        qfs_free_ast_node(root);
        return NULL;
    }
    
    // Left operand: variable x
    root->data.operator.operands[0] = qfs_create_ast_node(AST_VARIABLE);
    strcpy(root->data.operator.operands[0]->data.variable_name, "x");
    root->data.operator.operands[0]->parent = root;
    
    // Right operand: constant 42
    root->data.operator.operands[1] = qfs_create_ast_node(AST_QUANTUM_NUMBER);
    root->data.operator.operands[1]->data.quantum_value = create_test_quantum_number(42);
    root->data.operator.operands[1]->parent = root;
    
    return root;
}

/**
 * Main test runner
 */
int main(void) {
    printf("Quantix File System (QFS) - Comprehensive Test Suite\n");
    printf("Copyright (c) 2025 Arbitrary Number Project Team\n");
    printf("Starting QFS tests...\n");
    
    // Run all test suites
    test_qfs_initialization();
    test_mathematical_blocks();
    test_ast_operations();
    test_mathematical_files();
    test_ast_files();
    test_mathematical_checksums();
    test_mathematical_indexing();
    test_error_handling();
    test_performance_scenarios();
    
    // Print final summary
    print_test_summary();
    
    // Cleanup
    qfs_shutdown_filesystem();
    
    // Return appropriate exit code
    return (tests_failed == 0) ? 0 : 1;
}
