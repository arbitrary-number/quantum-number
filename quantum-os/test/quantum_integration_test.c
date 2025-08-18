/**
 * Quantum OS Integration Tests
 * 
 * Comprehensive test suite for validating the integration of all
 * quantum-aware subsystems including QMM, QFS, QPS, and QSC.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include "../kernel/quantum_memory_manager.h"
#include "../kernel/quantix_qfs_advanced.h"
#include "../kernel/quantum_process_scheduler.h"
#include "../kernel/quantum_syscalls.h"
#include "../kernel/kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

// Test result tracking
typedef struct test_result {
    const char *test_name;
    bool passed;
    uint64_t execution_time_ns;
    char error_message[256];
} test_result_t;

static test_result_t test_results[100];
static int test_count = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Test utilities
#define TEST_START(name) \
    do { \
        printf("Running test: %s\n", name); \
        uint64_t start_time = get_current_time_ns(); \
        bool test_passed = true; \
        const char *test_name = name;

#define TEST_END() \
        uint64_t end_time = get_current_time_ns(); \
        test_results[test_count].test_name = test_name; \
        test_results[test_count].passed = test_passed; \
        test_results[test_count].execution_time_ns = end_time - start_time; \
        if (test_passed) { \
            printf("  PASSED (%llu ns)\n", end_time - start_time); \
            tests_passed++; \
        } else { \
            printf("  FAILED (%llu ns): %s\n", end_time - start_time, \
                   test_results[test_count].error_message); \
            tests_failed++; \
        } \
        test_count++; \
    } while(0)

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            test_passed = false; \
            strncpy(test_results[test_count].error_message, message, \
                    sizeof(test_results[test_count].error_message) - 1); \
            break; \
        } \
    } while(0)

// Helper functions
static uint64_t get_current_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

static void print_test_summary(void) {
    printf("\n=== Quantum OS Integration Test Summary ===\n");
    printf("Total tests: %d\n", test_count);
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Success rate: %.2f%%\n", 
           test_count > 0 ? (double)tests_passed / test_count * 100.0 : 0.0);
    
    if (tests_failed > 0) {
        printf("\nFailed tests:\n");
        for (int i = 0; i < test_count; i++) {
            if (!test_results[i].passed) {
                printf("  - %s: %s\n", test_results[i].test_name, 
                       test_results[i].error_message);
            }
        }
    }
    printf("==========================================\n");
}

// Test 1: Quantum Memory Manager Basic Operations
static void test_qmm_basic_operations(void) {
    TEST_START("QMM Basic Operations");
    
    // Initialize QMM
    qmm_result_t result = qmm_initialize(64 * 1024 * 1024); // 64MB
    TEST_ASSERT(result == QMM_SUCCESS, "QMM initialization failed");
    
    // Test quantum number allocation
    void *qn_memory = qmm_alloc_quantum_number(10);
    TEST_ASSERT(qn_memory != NULL, "Quantum number allocation failed");
    
    // Test symbolic expression allocation
    void *sym_memory = qmm_alloc_symbolic_expression(100);
    TEST_ASSERT(sym_memory != NULL, "Symbolic expression allocation failed");
    
    // Test AST node allocation
    void *ast_memory = qmm_alloc_ast_node(5);
    TEST_ASSERT(ast_memory != NULL, "AST node allocation failed");
    
    // Test memory deallocation
    result = qmm_free(qn_memory);
    TEST_ASSERT(result == QMM_SUCCESS, "Quantum number deallocation failed");
    
    result = qmm_free(sym_memory);
    TEST_ASSERT(result == QMM_SUCCESS, "Symbolic expression deallocation failed");
    
    result = qmm_free(ast_memory);
    TEST_ASSERT(result == QMM_SUCCESS, "AST node deallocation failed");
    
    // Test statistics
    qmm_statistics_t stats;
    result = qmm_get_statistics(&stats);
    TEST_ASSERT(result == QMM_SUCCESS, "Failed to get QMM statistics");
    TEST_ASSERT(stats.total_memory > 0, "Invalid total memory in statistics");
    
    // Cleanup
    qmm_shutdown();
    
    TEST_END();
}

// Test 2: Quantum File System Operations
static void test_qfs_operations(void) {
    TEST_START("QFS Operations");
    
    // Initialize QFS
    qfs_result_t result = qfs_initialize_filesystem();
    TEST_ASSERT(result == QFS_SUCCESS, "QFS initialization failed");
    
    // Create mathematical metadata
    mathematical_metadata_t metadata = {0};
    metadata.primary_domain = MATH_DOMAIN_ALGEBRA;
    metadata.creation_date = time(NULL);
    strcpy(metadata.description, "Test mathematical file");
    strcpy(metadata.author, "Integration Test");
    
    // Create a mathematical file
    result = qfs_create_mathematical_file("/test_math_file.qn", &metadata);
    TEST_ASSERT(result == QFS_SUCCESS, "Failed to create mathematical file");
    
    // Open the file
    qfs_file_handle_t handle;
    result = qfs_open_quantum_file("/test_math_file.qn", 
                                  QFS_MODE_READ | QFS_MODE_WRITE, &handle);
    TEST_ASSERT(result == QFS_SUCCESS, "Failed to open mathematical file");
    
    // Create test quantum numbers
    quantum_number_t test_numbers[5];
    for (int i = 0; i < 5; i++) {
        quantum_number_init(&test_numbers[i]);
        quantum_number_set_from_uint64(&test_numbers[i], i + 1);
    }
    
    // Write quantum numbers to file
    result = qfs_write_quantum_numbers(&handle, test_numbers, 5);
    TEST_ASSERT(result == QFS_SUCCESS, "Failed to write quantum numbers");
    
    // Read quantum numbers back
    quantum_number_t read_numbers[5];
    result = qfs_read_quantum_numbers(&handle, read_numbers, 5);
    TEST_ASSERT(result == QFS_SUCCESS, "Failed to read quantum numbers");
    
    // Verify data integrity
    for (int i = 0; i < 5; i++) {
        uint64_t original = quantum_number_get_ordinal(&test_numbers[i], 0);
        uint64_t read_back = quantum_number_get_ordinal(&read_numbers[i], 0);
        TEST_ASSERT(original == read_back, "Data integrity check failed");
    }
    
    // Close file
    result = qfs_close_file(&handle);
    TEST_ASSERT(result == QFS_SUCCESS, "Failed to close file");
    
    // Cleanup
    qfs_shutdown_filesystem();
    
    TEST_END();
}

// Test 3: Quantum Process Scheduler Operations
static void test_qps_operations(void) {
    TEST_START("QPS Operations");
    
    // Initialize QPS
    qps_result_t result = qps_initialize_scheduler(QPS_SCHED_QUANTUM_FAIR);
    TEST_ASSERT(result == QPS_SUCCESS, "QPS initialization failed");
    
    // Create mathematical priority
    qps_mathematical_priority_t priority = {0};
    priority.base_priority = 100;
    priority.mathematical_weight = 50;
    priority.complexity_factor = 25;
    priority.symbolic_bonus = 10;
    
    // Create a mathematical process
    qps_process_control_block_t *process;
    result = qps_create_process("test_math_process", 
                               QPS_TYPE_MATHEMATICAL, &priority, &process);
    TEST_ASSERT(result == QPS_SUCCESS, "Failed to create mathematical process");
    TEST_ASSERT(process != NULL, "Process control block is NULL");
    
    // Assign quantum context
    result = qps_assign_quantum_context(process, 16, 4096);
    TEST_ASSERT(result == QPS_SUCCESS, "Failed to assign quantum context");
    
    // Start the process
    result = qps_start_process(process);
    TEST_ASSERT(result == QPS_SUCCESS, "Failed to start process");
    
    // Create mathematical dependency
    qps_mathematical_dependency_t dependency = {0};
    dependency.process_id = process->process_id;
    quantum_number_init(&dependency.dependency_id);
    quantum_number_set_from_uint64(&dependency.dependency_id, 12345);
    strcpy(dependency.dependency_description, "Test dependency");
    dependency.dependency_type = QPS_DEP_COMPUTATION_RESULT;
    dependency.timeout_ms = 5000;
    
    result = qps_add_mathematical_dependency(process->process_id, &dependency);
    TEST_ASSERT(result == QPS_SUCCESS, "Failed to add mathematical dependency");
    
    // Get scheduler statistics
    qps_scheduler_state_t scheduler_stats;
    result = qps_get_scheduler_statistics(&scheduler_stats);
    TEST_ASSERT(result == QPS_SUCCESS, "Failed to get scheduler statistics");
    TEST_ASSERT(scheduler_stats.statistics.current_process_count > 0, 
                "Invalid process count in statistics");
    
    // Terminate the process
    result = qps_terminate_process(process->process_id);
    TEST_ASSERT(result == QPS_SUCCESS, "Failed to terminate process");
    
    // Cleanup
    qps_shutdown_scheduler();
    
    TEST_END();
}

// Test 4: Quantum System Calls
static void test_quantum_syscalls(void) {
    TEST_START("Quantum System Calls");
    
    // Initialize syscall system
    int result = qsc_initialize_syscalls();
    TEST_ASSERT(result == QSC_SUCCESS, "QSC initialization failed");
    
    // Test quantum number creation syscall
    quantum_syscall_params_t params = {0};
    params.syscall_number = QSC_QUANTUM_CREATE;
    params.param_count = 0;
    params.calling_process_id = 1;
    params.execution_flags = QSC_FLAG_MATHEMATICAL;
    
    quantum_syscall_result_t syscall_result = qsc_execute_syscall(&params);
    TEST_ASSERT(syscall_result.error_code == QSC_SUCCESS, 
                "Quantum create syscall failed");
    
    // Test quantum number addition syscall
    quantum_number_t num1, num2;
    quantum_number_init(&num1);
    quantum_number_init(&num2);
    quantum_number_set_from_uint64(&num1, 42);
    quantum_number_set_from_uint64(&num2, 58);
    
    params.syscall_number = QSC_QUANTUM_ADD;
    params.param_count = 2;
    params.params[0].ptr_param = &num1;
    params.param_types[0] = QSC_PARAM_POINTER;
    params.params[1].ptr_param = &num2;
    params.param_types[1] = QSC_PARAM_POINTER;
    
    syscall_result = qsc_execute_syscall(&params);
    TEST_ASSERT(syscall_result.error_code == QSC_SUCCESS, 
                "Quantum add syscall failed");
    
    // Test syscall statistics
    quantum_syscall_statistics_t syscall_stats;
    result = qsc_get_syscall_statistics(&syscall_stats);
    TEST_ASSERT(result == QSC_SUCCESS, "Failed to get syscall statistics");
    TEST_ASSERT(syscall_stats.total_syscalls >= 2, 
                "Invalid syscall count in statistics");
    
    // Cleanup
    qsc_shutdown_syscalls();
    
    TEST_END();
}

// Test 5: Integrated Mathematical Computation
static void test_integrated_mathematical_computation(void) {
    TEST_START("Integrated Mathematical Computation");
    
    // Initialize all subsystems
    qmm_result_t qmm_result = qmm_initialize(32 * 1024 * 1024);
    TEST_ASSERT(qmm_result == QMM_SUCCESS, "QMM initialization failed");
    
    qfs_result_t qfs_result = qfs_initialize_filesystem();
    TEST_ASSERT(qfs_result == QFS_SUCCESS, "QFS initialization failed");
    
    qps_result_t qps_result = qps_initialize_scheduler(QPS_SCHED_MATHEMATICAL_PRIORITY);
    TEST_ASSERT(qps_result == QPS_SUCCESS, "QPS initialization failed");
    
    int qsc_result = qsc_initialize_syscalls();
    TEST_ASSERT(qsc_result == QSC_SUCCESS, "QSC initialization failed");
    
    // Allocate quantum numbers using QMM
    quantum_number_t *numbers = (quantum_number_t*)qmm_alloc_quantum_number(3);
    TEST_ASSERT(numbers != NULL, "Failed to allocate quantum numbers");
    
    // Initialize quantum numbers
    quantum_number_init(&numbers[0]);
    quantum_number_init(&numbers[1]);
    quantum_number_init(&numbers[2]);
    quantum_number_set_from_uint64(&numbers[0], 10);
    quantum_number_set_from_uint64(&numbers[1], 20);
    
    // Perform mathematical operation using syscall
    quantum_syscall_params_t params = {0};
    params.syscall_number = QSC_QUANTUM_ADD;
    params.param_count = 3;
    params.params[0].ptr_param = &numbers[0];
    params.param_types[0] = QSC_PARAM_POINTER;
    params.params[1].ptr_param = &numbers[1];
    params.param_types[1] = QSC_PARAM_POINTER;
    params.params[2].ptr_param = &numbers[2];
    params.param_types[2] = QSC_PARAM_POINTER;
    params.calling_process_id = 1;
    params.execution_flags = QSC_FLAG_MATHEMATICAL;
    
    quantum_syscall_result_t syscall_result = qsc_execute_syscall(&params);
    TEST_ASSERT(syscall_result.error_code == QSC_SUCCESS, 
                "Mathematical computation syscall failed");
    
    // Verify result
    uint64_t result_value = quantum_number_get_ordinal(&numbers[2], 0);
    TEST_ASSERT(result_value == 30, "Mathematical computation result incorrect");
    
    // Store result in QFS
    mathematical_metadata_t metadata = {0};
    metadata.primary_domain = MATH_DOMAIN_ALGEBRA;
    metadata.creation_date = time(NULL);
    strcpy(metadata.description, "Integration test computation result");
    
    qfs_result = qfs_create_mathematical_file("/computation_result.qn", &metadata);
    TEST_ASSERT(qfs_result == QFS_SUCCESS, "Failed to create result file");
    
    qfs_file_handle_t handle;
    qfs_result = qfs_open_quantum_file("/computation_result.qn", 
                                      QFS_MODE_WRITE, &handle);
    TEST_ASSERT(qfs_result == QFS_SUCCESS, "Failed to open result file");
    
    qfs_result = qfs_write_quantum_numbers(&handle, &numbers[2], 1);
    TEST_ASSERT(qfs_result == QFS_SUCCESS, "Failed to write result to file");
    
    qfs_result = qfs_close_file(&handle);
    TEST_ASSERT(qfs_result == QFS_SUCCESS, "Failed to close result file");
    
    // Read back and verify
    qfs_result = qfs_open_quantum_file("/computation_result.qn", 
                                      QFS_MODE_READ, &handle);
    TEST_ASSERT(qfs_result == QFS_SUCCESS, "Failed to reopen result file");
    
    quantum_number_t read_result;
    quantum_number_init(&read_result);
    qfs_result = qfs_read_quantum_numbers(&handle, &read_result, 1);
    TEST_ASSERT(qfs_result == QFS_SUCCESS, "Failed to read result from file");
    
    uint64_t read_value = quantum_number_get_ordinal(&read_result, 0);
    TEST_ASSERT(read_value == 30, "File storage/retrieval integrity check failed");
    
    qfs_result = qfs_close_file(&handle);
    TEST_ASSERT(qfs_result == QFS_SUCCESS, "Failed to close file after reading");
    
    // Cleanup
    qmm_result = qmm_free(numbers);
    TEST_ASSERT(qmm_result == QMM_SUCCESS, "Failed to free quantum numbers");
    
    qsc_shutdown_syscalls();
    qps_shutdown_scheduler();
    qfs_shutdown_filesystem();
    qmm_shutdown();
    
    TEST_END();
}

// Test 6: Performance and Stress Testing
static void test_performance_stress(void) {
    TEST_START("Performance and Stress Testing");
    
    // Initialize subsystems
    qmm_result_t qmm_result = qmm_initialize(128 * 1024 * 1024);
    TEST_ASSERT(qmm_result == QMM_SUCCESS, "QMM initialization failed");
    
    int qsc_result = qsc_initialize_syscalls();
    TEST_ASSERT(qsc_result == QSC_SUCCESS, "QSC initialization failed");
    
    const int num_operations = 1000;
    uint64_t start_time = get_current_time_ns();
    
    // Perform many mathematical operations
    for (int i = 0; i < num_operations; i++) {
        // Allocate quantum numbers
        quantum_number_t *numbers = (quantum_number_t*)qmm_alloc_quantum_number(2);
        TEST_ASSERT(numbers != NULL, "Allocation failed during stress test");
        
        // Initialize and set values
        quantum_number_init(&numbers[0]);
        quantum_number_init(&numbers[1]);
        quantum_number_set_from_uint64(&numbers[0], i);
        quantum_number_set_from_uint64(&numbers[1], i * 2);
        
        // Perform operation via syscall
        quantum_syscall_params_t params = {0};
        params.syscall_number = QSC_QUANTUM_MULTIPLY;
        params.param_count = 2;
        params.params[0].ptr_param = &numbers[0];
        params.param_types[0] = QSC_PARAM_POINTER;
        params.params[1].ptr_param = &numbers[1];
        params.param_types[1] = QSC_PARAM_POINTER;
        params.calling_process_id = 1;
        params.execution_flags = QSC_FLAG_MATHEMATICAL;
        
        quantum_syscall_result_t syscall_result = qsc_execute_syscall(&params);
        TEST_ASSERT(syscall_result.error_code == QSC_SUCCESS, 
                    "Syscall failed during stress test");
        
        // Free memory
        qmm_result = qmm_free(numbers);
        TEST_ASSERT(qmm_result == QMM_SUCCESS, 
                    "Deallocation failed during stress test");
    }
    
    uint64_t end_time = get_current_time_ns();
    uint64_t total_time = end_time - start_time;
    uint64_t avg_time_per_op = total_time / num_operations;
    
    printf("  Stress test completed: %d operations in %llu ns\n", 
           num_operations, total_time);
    printf("  Average time per operation: %llu ns\n", avg_time_per_op);
    
    // Verify system statistics
    qmm_statistics_t qmm_stats;
    qmm_result = qmm_get_statistics(&qmm_stats);
    TEST_ASSERT(qmm_result == QMM_SUCCESS, "Failed to get QMM statistics");
    TEST_ASSERT(qmm_stats.total_allocations >= num_operations, 
                "Allocation count mismatch");
    TEST_ASSERT(qmm_stats.total_deallocations >= num_operations, 
                "Deallocation count mismatch");
    
    quantum_syscall_statistics_t syscall_stats;
    qsc_result = qsc_get_syscall_statistics(&syscall_stats);
    TEST_ASSERT(qsc_result == QSC_SUCCESS, "Failed to get syscall statistics");
    TEST_ASSERT(syscall_stats.total_syscalls >= num_operations, 
                "Syscall count mismatch");
    
    // Cleanup
    qsc_shutdown_syscalls();
    qmm_shutdown();
    
    TEST_END();
}

// Test 7: Error Handling and Recovery
static void test_error_handling(void) {
    TEST_START("Error Handling and Recovery");
    
    // Test invalid memory allocation
    qmm_result_t qmm_result = qmm_initialize(1024 * 1024);
    TEST_ASSERT(qmm_result == QMM_SUCCESS, "QMM initialization failed");
    
    // Try to allocate more memory than available
    void *large_alloc = qmm_alloc_mathematical(2 * 1024 * 1024, 
                                              QMM_TYPE_QUANTUM_NUMBER, 
                                              QMM_FLAG_ZERO_INIT);
    TEST_ASSERT(large_alloc == NULL, "Large allocation should have failed");
    
    // Test invalid syscall
    int qsc_result = qsc_initialize_syscalls();
    TEST_ASSERT(qsc_result == QSC_SUCCESS, "QSC initialization failed");
    
    quantum_syscall_params_t params = {0};
    params.syscall_number = 9999; // Invalid syscall number
    params.param_count = 0;
    params.calling_process_id = 1;
    
    quantum_syscall_result_t syscall_result = qsc_execute_syscall(&params);
    TEST_ASSERT(syscall_result.error_code != QSC_SUCCESS, 
                "Invalid syscall should have failed");
    
    // Test double free
    void *test_alloc = qmm_alloc_quantum_number(1);
    TEST_ASSERT(test_alloc != NULL, "Test allocation failed");
    
    qmm_result = qmm_free(test_alloc);
    TEST_ASSERT(qmm_result == QMM_SUCCESS, "First free failed");
    
    qmm_result = qmm_free(test_alloc);
    TEST_ASSERT(qmm_result != QMM_SUCCESS, "Double free should have failed");
    
    // Test system recovery
    qmm_statistics_t stats;
    qmm_result = qmm_get_statistics(&stats);
    TEST_ASSERT(qmm_result == QMM_SUCCESS, "Failed to get statistics after errors");
    
    // Cleanup
    qsc_shutdown_syscalls();
    qmm_shutdown();
    
    TEST_END();
}

// Main test runner
int main(void) {
    printf("=== Quantum OS Integration Test Suite ===\n");
    printf("Testing all quantum-aware subsystems...\n\n");
    
    // Run all tests
    test_qmm_basic_operations();
    test_qfs_operations();
    test_qps_operations();
    test_quantum_syscalls();
    test_integrated_mathematical_computation();
    test_performance_stress();
    test_error_handling();
    
    // Print summary
    print_test_summary();
    
    // Return appropriate exit code
    return tests_failed > 0 ? 1 : 0;
}
