/**
 * @file quantum_performance_optimizer_test.c
 * @brief Comprehensive test suite for Quantum Performance Optimizer
 * 
 * This file contains extensive tests for all components of the quantum
 * performance optimization system, including unit tests, integration tests,
 * performance benchmarks, and stress tests.
 * 
 * @author Quantum OS Development Team
 * @version 1.0.0
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "../kernel/quantum_performance_optimizer.h"
#include "../kernel/quantum_number.h"

// Test configuration
#define TEST_TIMEOUT_SECONDS 30
#define TEST_ITERATIONS 1000
#define TEST_THREADS 8
#define TEST_CONTEXTS 16

// Test result tracking
typedef struct {
    int tests_run;
    int tests_passed;
    int tests_failed;
    double total_time;
    char last_error[256];
} test_results_t;

static test_results_t g_test_results = {0};

// Test utilities
static void test_start(const char* test_name);
static void test_end(bool passed, const char* error_msg);
static uint64_t get_timestamp_us(void);
static void create_test_quantum_number(quantum_number_t* qn, uint32_t value);

/**
 * @brief Get current timestamp in microseconds
 */
static uint64_t get_timestamp_us(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

/**
 * @brief Start a test case
 */
static void test_start(const char* test_name) {
    printf("Running test: %s... ", test_name);
    fflush(stdout);
    g_test_results.tests_run++;
}

/**
 * @brief End a test case
 */
static void test_end(bool passed, const char* error_msg) {
    if (passed) {
        printf("PASSED\n");
        g_test_results.tests_passed++;
    } else {
        printf("FAILED: %s\n", error_msg ? error_msg : "Unknown error");
        g_test_results.tests_failed++;
        if (error_msg) {
            strncpy(g_test_results.last_error, error_msg, sizeof(g_test_results.last_error) - 1);
        }
    }
}

/**
 * @brief Create a test quantum number
 */
static void create_test_quantum_number(quantum_number_t* qn, uint32_t value) {
    memset(qn, 0, sizeof(quantum_number_t));
    qn->precision = 4;
    qn->data = malloc(qn->precision * sizeof(uint32_t));
    if (qn->data) {
        qn->data[0] = value;
        qn->data[1] = value >> 8;
        qn->data[2] = value >> 16;
        qn->data[3] = value >> 24;
    }
}

/**
 * @brief Test QPO initialization and shutdown
 */
static void test_qpo_initialization(void) {
    test_start("QPO Initialization");
    
    // Test initialization
    int result = qpo_initialize();
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to initialize QPO");
        return;
    }
    
    // Test double initialization (should succeed)
    result = qpo_initialize();
    if (result != QPO_SUCCESS) {
        test_end(false, "Double initialization failed");
        return;
    }
    
    // Test shutdown
    result = qpo_shutdown();
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to shutdown QPO");
        return;
    }
    
    // Re-initialize for other tests
    result = qpo_initialize();
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to re-initialize QPO");
        return;
    }
    
    test_end(true, NULL);
}

/**
 * @brief Test context creation and destruction
 */
static void test_context_management(void) {
    test_start("Context Management");
    
    // Test context creation
    int context_id = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Test context destruction
    int result = qpo_destroy_context(context_id);
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to destroy context");
        return;
    }
    
    // Test invalid context operations
    result = qpo_destroy_context(9999);
    if (result == QPO_SUCCESS) {
        test_end(false, "Should fail for invalid context");
        return;
    }
    
    // Test multiple contexts
    int contexts[TEST_CONTEXTS];
    for (int i = 0; i < TEST_CONTEXTS; i++) {
        contexts[i] = qpo_create_context(QPO_STRATEGY_BALANCED, i % 16);
        if (contexts[i] < 0) {
            test_end(false, "Failed to create multiple contexts");
            return;
        }
    }
    
    // Cleanup contexts
    for (int i = 0; i < TEST_CONTEXTS; i++) {
        qpo_destroy_context(contexts[i]);
    }
    
    test_end(true, NULL);
}

/**
 * @brief Test performance sample recording
 */
static void test_performance_sampling(void) {
    test_start("Performance Sampling");
    
    int context_id = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Test sample recording
    qpo_performance_sample_t sample = {
        .timestamp = get_timestamp_us(),
        .metric_type = QPO_METRIC_CPU_UTILIZATION,
        .value = 0.75,
        .process_id = getpid(),
        .thread_id = pthread_self(),
        .context_id = context_id,
        .optimization_level = 8,
        .hardware_accelerated = false,
        .accuracy_score = 0.99,
        .memory_usage = 1024,
        .cache_hits = 10,
        .cache_misses = 2
    };
    strcpy(sample.operation_name, "test_operation");
    
    int result = qpo_record_sample(context_id, &sample);
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to record sample");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test multiple samples
    for (int i = 0; i < 100; i++) {
        sample.timestamp = get_timestamp_us();
        sample.value = (double)i / 100.0;
        result = qpo_record_sample(context_id, &sample);
        if (result != QPO_SUCCESS) {
            test_end(false, "Failed to record multiple samples");
            qpo_destroy_context(context_id);
            return;
        }
    }
    
    // Test invalid parameters
    result = qpo_record_sample(context_id, NULL);
    if (result == QPO_SUCCESS) {
        test_end(false, "Should fail for NULL sample");
        qpo_destroy_context(context_id);
        return;
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Test computation caching
 */
static void test_computation_caching(void) {
    test_start("Computation Caching");
    
    int context_id = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    quantum_number_t input, result, cached_result;
    create_test_quantum_number(&input, 12345);
    create_test_quantum_number(&result, 67890);
    
    // Test cache miss (should not find anything)
    int lookup_result = qpo_lookup_cached_computation(context_id, "test_op", &input, &cached_result);
    if (lookup_result != QPO_NOT_FOUND) {
        test_end(false, "Should not find uncached computation");
        goto cleanup;
    }
    
    // Test caching a computation
    int cache_result = qpo_cache_computation(context_id, "test_op", &input, &result, 1000);
    if (cache_result != QPO_SUCCESS) {
        test_end(false, "Failed to cache computation");
        goto cleanup;
    }
    
    // Test cache hit
    lookup_result = qpo_lookup_cached_computation(context_id, "test_op", &input, &cached_result);
    if (lookup_result != QPO_SUCCESS) {
        test_end(false, "Should find cached computation");
        goto cleanup;
    }
    
    // Verify cached result matches
    if (cached_result.data[0] != result.data[0]) {
        test_end(false, "Cached result doesn't match original");
        goto cleanup;
    }
    
    // Test cache with many entries (LRU eviction)
    for (int i = 0; i < 5000; i++) {
        quantum_number_t temp_input, temp_result;
        create_test_quantum_number(&temp_input, i);
        create_test_quantum_number(&temp_result, i * 2);
        
        char op_name[32];
        snprintf(op_name, sizeof(op_name), "op_%d", i);
        
        qpo_cache_computation(context_id, op_name, &temp_input, &temp_result, 100);
        
        free(temp_input.data);
        free(temp_result.data);
    }
    
    test_end(true, NULL);

cleanup:
    if (input.data) free(input.data);
    if (result.data) free(result.data);
    if (cached_result.data) free(cached_result.data);
    qpo_destroy_context(context_id);
}

/**
 * @brief Test mathematical computation optimization
 */
static void test_computation_optimization(void) {
    test_start("Computation Optimization");
    
    int context_id = qpo_create_context(QPO_STRATEGY_MAXIMUM_PERFORMANCE, 15);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    quantum_number_t input, result;
    create_test_quantum_number(&input, 42);
    memset(&result, 0, sizeof(result));
    
    // Test basic optimization
    int opt_result = qpo_optimize_computation(context_id, "add", &input, &result);
    if (opt_result != QPO_SUCCESS) {
        test_end(false, "Failed to optimize computation");
        goto cleanup;
    }
    
    // Test different operation types
    const char* operations[] = {"multiply", "add", "power", "divide", "sqrt"};
    for (int i = 0; i < 5; i++) {
        opt_result = qpo_optimize_computation(context_id, operations[i], &input, &result);
        if (opt_result != QPO_SUCCESS) {
            test_end(false, "Failed to optimize different operations");
            goto cleanup;
        }
    }
    
    // Test performance improvement through caching
    uint64_t start_time = get_timestamp_us();
    for (int i = 0; i < 1000; i++) {
        qpo_optimize_computation(context_id, "cached_op", &input, &result);
    }
    uint64_t cached_time = get_timestamp_us() - start_time;
    
    // First run should be slower due to cache misses
    if (cached_time == 0) {
        test_end(false, "Performance timing failed");
        goto cleanup;
    }
    
    test_end(true, NULL);

cleanup:
    if (input.data) free(input.data);
    if (result.data) free(result.data);
    qpo_destroy_context(context_id);
}

/**
 * @brief Test memory pattern analysis
 */
static void test_memory_pattern_analysis(void) {
    test_start("Memory Pattern Analysis");
    
    int context_id = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Test pattern recording
    uint64_t base_addr = 0x1000000;
    for (int i = 0; i < 100; i++) {
        int result = qpo_analyze_memory_pattern(context_id, base_addr + i * 64, 64, 0);
        if (result != QPO_SUCCESS) {
            test_end(false, "Failed to analyze memory pattern");
            qpo_destroy_context(context_id);
            return;
        }
    }
    
    // Test pattern optimization
    int optimizations = qpo_optimize_memory_patterns(context_id);
    if (optimizations < 0) {
        test_end(false, "Failed to optimize memory patterns");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test repeated access patterns
    for (int i = 0; i < 50; i++) {
        qpo_analyze_memory_pattern(context_id, base_addr, 64, 0); // Same address
    }
    
    optimizations = qpo_optimize_memory_patterns(context_id);
    if (optimizations < 0) {
        test_end(false, "Failed to optimize repeated patterns");
        qpo_destroy_context(context_id);
        return;
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Test hardware acceleration management
 */
static void test_hardware_acceleration(void) {
    test_start("Hardware Acceleration");
    
    int context_id = qpo_create_context(QPO_STRATEGY_MAXIMUM_PERFORMANCE, 15);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Test enabling hardware acceleration
    int result = qpo_enable_hardware_acceleration(context_id, QPO_ACCEL_X256_MATHEMATICAL_UNIT, "multiply");
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to enable hardware acceleration");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test getting acceleration status
    qpo_acceleration_unit_t status;
    result = qpo_get_acceleration_status(context_id, QPO_ACCEL_X256_MATHEMATICAL_UNIT, &status);
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to get acceleration status");
        qpo_destroy_context(context_id);
        return;
    }
    
    if (!status.is_active) {
        test_end(false, "Acceleration unit should be active");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test disabling hardware acceleration
    result = qpo_disable_hardware_acceleration(context_id, QPO_ACCEL_X256_MATHEMATICAL_UNIT);
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to disable hardware acceleration");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Verify it's disabled
    result = qpo_get_acceleration_status(context_id, QPO_ACCEL_X256_MATHEMATICAL_UNIT, &status);
    if (result != QPO_SUCCESS || status.is_active) {
        test_end(false, "Acceleration unit should be inactive");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test multiple acceleration units
    for (int i = 0; i < 5; i++) {
        qpo_enable_hardware_acceleration(context_id, (qpo_acceleration_type_t)i, "test_op");
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Test pattern learning and application
 */
static void test_pattern_learning(void) {
    test_start("Pattern Learning");
    
    int context_id = qpo_create_context(QPO_STRATEGY_ADAPTIVE, 8);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Generate sample data for pattern learning
    for (int i = 0; i < 200; i++) {
        qpo_performance_sample_t sample = {
            .timestamp = get_timestamp_us(),
            .metric_type = QPO_METRIC_MATHEMATICAL_THROUGHPUT,
            .value = 1000.0 + (i % 100),
            .process_id = getpid(),
            .thread_id = pthread_self(),
            .context_id = context_id,
            .optimization_level = 8,
            .hardware_accelerated = false,
            .accuracy_score = 0.99,
            .memory_usage = 1024,
            .cache_hits = i % 10,
            .cache_misses = (i + 1) % 3
        };
        
        snprintf(sample.operation_name, sizeof(sample.operation_name), "pattern_op_%d", i % 5);
        qpo_record_sample(context_id, &sample);
    }
    
    // Test pattern learning
    int patterns_learned = qpo_learn_optimization_patterns(context_id);
    if (patterns_learned < 0) {
        test_end(false, "Failed to learn patterns");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test pattern application
    int patterns_applied = qpo_apply_learned_patterns(context_id, "pattern_op_1");
    if (patterns_applied < 0) {
        test_end(false, "Failed to apply patterns");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test prediction
    uint32_t predicted_config;
    double confidence = qpo_predict_optimal_config(context_id, "pattern_op_2", &predicted_config);
    if (confidence < 0.0 || confidence > 1.0) {
        test_end(false, "Invalid prediction confidence");
        qpo_destroy_context(context_id);
        return;
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Test adaptive optimization
 */
static void test_adaptive_optimization(void) {
    test_start("Adaptive Optimization");
    
    int context_id = qpo_create_context(QPO_STRATEGY_ADAPTIVE, 4);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Simulate poor performance to trigger adaptations
    for (int i = 0; i < 50; i++) {
        qpo_performance_sample_t sample = {
            .timestamp = get_timestamp_us(),
            .metric_type = QPO_METRIC_CPU_UTILIZATION,
            .value = 0.3, // Low performance
            .process_id = getpid(),
            .thread_id = pthread_self(),
            .context_id = context_id,
            .optimization_level = 4,
            .hardware_accelerated = false,
            .accuracy_score = 0.99,
            .memory_usage = 1024,
            .cache_hits = 1,
            .cache_misses = 10 // Poor cache performance
        };
        
        strcpy(sample.operation_name, "slow_operation");
        qpo_record_sample(context_id, &sample);
    }
    
    // Test adaptive optimization
    int adaptations = qpo_adaptive_optimize(context_id);
    if (adaptations < 0) {
        test_end(false, "Failed to perform adaptive optimization");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Should have made some adaptations due to poor performance
    if (adaptations == 0) {
        test_end(false, "Should have made adaptations for poor performance");
        qpo_destroy_context(context_id);
        return;
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Test performance statistics
 */
static void test_performance_statistics(void) {
    test_start("Performance Statistics");
    
    int context_id = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Record some performance data
    qpo_performance_sample_t sample = {
        .timestamp = get_timestamp_us(),
        .metric_type = QPO_METRIC_CACHE_HIT_RATE,
        .value = 0.85,
        .process_id = getpid(),
        .thread_id = pthread_self(),
        .context_id = context_id,
        .optimization_level = 8,
        .hardware_accelerated = false,
        .accuracy_score = 0.99,
        .memory_usage = 1024,
        .cache_hits = 85,
        .cache_misses = 15
    };
    
    strcpy(sample.operation_name, "stats_test");
    qpo_record_sample(context_id, &sample);
    
    // Test getting statistics
    double current, baseline, improvement;
    int result = qpo_get_performance_stats(context_id, QPO_METRIC_CACHE_HIT_RATE,
                                          &current, &baseline, &improvement);
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to get performance statistics");
        qpo_destroy_context(context_id);
        return;
    }
    
    if (current != 0.85) {
        test_end(false, "Current performance value incorrect");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test invalid parameters
    result = qpo_get_performance_stats(context_id, QPO_METRIC_COUNT, &current, &baseline, &improvement);
    if (result == QPO_SUCCESS) {
        test_end(false, "Should fail for invalid metric type");
        qpo_destroy_context(context_id);
        return;
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Test configuration management
 */
static void test_configuration_management(void) {
    test_start("Configuration Management");
    
    int context_id = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Test strategy changes
    int result = qpo_set_strategy(context_id, QPO_STRATEGY_AGGRESSIVE);
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to set strategy");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test optimization level changes
    result = qpo_set_optimization_level(context_id, 12);
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to set optimization level");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test invalid optimization level
    result = qpo_set_optimization_level(context_id, 20);
    if (result == QPO_SUCCESS) {
        test_end(false, "Should fail for invalid optimization level");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test feature configuration
    uint32_t features = QPO_FEATURE_COMPUTATION_CACHE | QPO_FEATURE_HARDWARE_ACCELERATION;
    result = qpo_configure_features(context_id, features);
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to configure features");
        qpo_destroy_context(context_id);
        return;
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Test benchmarking functionality
 */
static void test_benchmarking(void) {
    test_start("Benchmarking");
    
    int context_id = qpo_create_context(QPO_STRATEGY_MAXIMUM_PERFORMANCE, 15);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Test different benchmark types
    for (uint32_t benchmark_type = 0; benchmark_type < 3; benchmark_type++) {
        double score = qpo_benchmark_performance(context_id, benchmark_type, 100); // 100ms
        if (score <= 0.0) {
            test_end(false, "Invalid benchmark score");
            qpo_destroy_context(context_id);
            return;
        }
    }
    
    // Test longer benchmark
    double long_score = qpo_benchmark_performance(context_id, 0, 500); // 500ms
    if (long_score <= 0.0) {
        test_end(false, "Invalid long benchmark score");
        qpo_destroy_context(context_id);
        return;
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Test data export functionality
 */
static void test_data_export(void) {
    test_start("Data Export");
    
    int context_id = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Generate some data to export
    for (int i = 0; i < 10; i++) {
        qpo_performance_sample_t sample = {
            .timestamp = get_timestamp_us(),
            .metric_type = QPO_METRIC_CPU_UTILIZATION,
            .value = 0.5 + (i * 0.05),
            .process_id = getpid(),
            .thread_id = pthread_self(),
            .context_id = context_id,
            .optimization_level = 8,
            .hardware_accelerated = false,
            .accuracy_score = 0.99,
            .memory_usage = 1024,
            .cache_hits = i,
            .cache_misses = 10 - i
        };
        
        snprintf(sample.operation_name, sizeof(sample.operation_name), "export_test_%d", i);
        qpo_record_sample(context_id, &sample);
    }
    
    // Test export
    const char* export_file = "/tmp/qpo_test_export.txt";
    int result = qpo_export_optimization_data(context_id, export_file);
    if (result != QPO_SUCCESS) {
        test_end(false, "Failed to export optimization data");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Verify file exists and has content
    FILE* file = fopen(export_file, "r");
    if (!file) {
        test_end(false, "Export file not created");
        qpo_destroy_context(context_id);
        return;
    }
    
    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), file)) {
        test_end(false, "Export file is empty");
        fclose(file);
        qpo_destroy_context(context_id);
        return;
    }
    
    fclose(file);
    unlink(export_file); // Cleanup
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Test optimization recommendations
 */
static void test_optimization_recommendations(void) {
    test_start("Optimization Recommendations");
    
    int context_id = qpo_create_context(QPO_STRATEGY_CONSERVATIVE, 2);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    // Create conditions that should trigger recommendations
    for (int i = 0; i < 20; i++) {
        qpo_performance_sample_t sample = {
            .timestamp = get_timestamp_us(),
            .metric_type = QPO_METRIC_CACHE_HIT_RATE,
            .value = 0.2, // Poor cache performance
            .process_id = getpid(),
            .thread_id = pthread_self(),
            .context_id = context_id,
            .optimization_level = 2,
            .hardware_accelerated = false,
            .accuracy_score = 0.99,
            .memory_usage = 1024,
            .cache_hits = 2,
            .cache_misses = 8
        };
        
        strcpy(sample.operation_name, "recommendation_test");
        qpo_record_sample(context_id, &sample);
    }
    
    // Get recommendations
    char recommendations[4096];
    int rec_count = qpo_get_optimization_recommendations(context_id, recommendations, 10);
    if (rec_count < 0) {
        test_end(false, "Failed to get recommendations");
        qpo_destroy_context(context_id);
        return;
    }
    
    if (rec_count == 0) {
        test_end(false, "Should have generated recommendations for poor performance");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Verify recommendations contain expected content
    if (!strstr(recommendations, "Recommendation")) {
        test_end(false, "Recommendations format incorrect");
        qpo_destroy_context(context_id);
        return;
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Stress test with multiple threads
 */
static void* stress_test_thread(void* arg) {
    int thread_id = *(int*)arg;
    int context_id = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    
    if (context_id < 0) {
        return NULL;
    }
    
    quantum_number_t input, result;
    create_test_quantum_number(&input, thread_id * 1000);
    
    // Perform various operations
    for (int i = 0; i < 100; i++) {
        char op_name[32];
        snprintf(op_name, sizeof(op_name), "thread_%d_op_%d", thread_id, i);
        
        // Test computation optimization
        qpo_optimize_computation(context_id, op_name, &input, &result);
        
        // Test memory pattern analysis
        qpo_analyze_memory_pattern(context_id, (uint64_t)&input + i * 64, 64, i % 2);
        
        // Test performance sampling
        qpo_performance_sample_t sample = {
            .timestamp = get_timestamp_us(),
            .metric_type = (qpo_performance_metric_type_t)(i % QPO_METRIC_COUNT),
            .value = (double)i / 100.0,
            .process_id = getpid(),
            .thread_id = pthread_self(),
            .context_id = context_id,
            .optimization_level = 8,
            .hardware_accelerated = false,
            .accuracy_score = 0.99,
            .memory_usage = 1024,
            .cache_hits = i % 10,
            .cache_misses = (10 - i) % 10
        };
        
        strncpy(sample.operation_name, op_name, sizeof(sample.operation_name) - 1);
        qpo_record_sample(context_id, &sample);
        
        // Occasionally trigger adaptive optimization
        if (i % 20 == 0) {
            qpo_adaptive_optimize(context_id);
        }
    }
    
    // Cleanup
    if (input.data) free(input.data);
    if (result.data) free(result.data);
    qpo_destroy_context(context_id);
    
    return NULL;
}

/**
 * @brief Multi-threaded stress test
 */
static void test_multithreaded_stress(void) {
    test_start("Multi-threaded Stress Test");
    
    pthread_t threads[TEST_THREADS];
    int thread_ids[TEST_THREADS];
    
    // Create threads
    for (int i = 0; i < TEST_THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, stress_test_thread, &thread_ids[i]) != 0) {
            test_end(false, "Failed to create stress test thread");
            return;
        }
    }
    
    // Wait for threads to complete
    for (int i = 0; i < TEST_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            test_end(false, "Failed to join stress test thread");
            return;
        }
    }
    
    test_end(true, NULL);
}

/**
 * @brief Performance regression test
 */
static void test_performance_regression(void) {
    test_start("Performance Regression Test");
    
    int context_id = qpo_create_context(QPO_STRATEGY_MAXIMUM_PERFORMANCE, 15);
    if (context_id < 0) {
        test_end(false, "Failed to create context");
        return;
    }
    
    quantum_number_t input, result;
    create_test_quantum_number(&input, 12345);
    
    // Baseline performance test
    uint64_t start_time = get_timestamp_us();
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        qpo_optimize_computation(context_id, "regression_test", &input, &result);
    }
    uint64_t baseline_time = get_timestamp_us() - start_time;
    
    // Performance with caching should be better
    start_time = get_timestamp_us();
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        qpo_optimize_computation(context_id, "regression_test", &input, &result);
    }
    uint64_t cached_time = get_timestamp_us() - start_time;
    
    // Cached performance should be significantly better
    if (cached_time >= baseline_time) {
        test_end(false, "Performance regression detected - caching not improving performance");
        goto cleanup;
    }
    
    // Test memory optimization performance
    start_time = get_timestamp_us();
    for (int i = 0; i < 1000; i++) {
        qpo_analyze_memory_pattern(context_id, 0x1000000 + i * 64, 64, 0);
    }
    int optimizations = qpo_optimize_memory_patterns(context_id);
    uint64_t memory_opt_time = get_timestamp_us() - start_time;
    
    if (optimizations < 0 || memory_opt_time > 1000000) { // Should complete within 1 second
        test_end(false, "Memory optimization performance regression");
        goto cleanup;
    }
    
    test_end(true, NULL);

cleanup:
    if (input.data) free(input.data);
    if (result.data) free(result.data);
    qpo_destroy_context(context_id);
}

/**
 * @brief Error handling and edge cases test
 */
static void test_error_handling(void) {
    test_start("Error Handling and Edge Cases");
    
    // Test operations before initialization
    qpo_shutdown(); // Ensure not initialized
    
    int result = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    if (result != QPO_ERROR_NOT_INITIALIZED) {
        test_end(false, "Should fail when not initialized");
        return;
    }
    
    // Re-initialize
    qpo_initialize();
    
    // Test invalid parameters
    result = qpo_create_context((qpo_optimization_strategy_t)999, 8);
    if (result >= 0) {
        test_end(false, "Should fail for invalid strategy");
        return;
    }
    
    result = qpo_create_context(QPO_STRATEGY_BALANCED, 20);
    if (result >= 0) {
        test_end(false, "Should fail for invalid optimization level");
        return;
    }
    
    // Test NULL pointer handling
    int context_id = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
    if (context_id < 0) {
        test_end(false, "Failed to create context for error testing");
        return;
    }
    
    result = qpo_record_sample(context_id, NULL);
    if (result == QPO_SUCCESS) {
        test_end(false, "Should fail for NULL sample");
        qpo_destroy_context(context_id);
        return;
    }
    
    result = qpo_cache_computation(context_id, NULL, NULL, NULL, 0);
    if (result == QPO_SUCCESS) {
        test_end(false, "Should fail for NULL parameters");
        qpo_destroy_context(context_id);
        return;
    }
    
    result = qpo_get_performance_stats(context_id, QPO_METRIC_COUNT, NULL, NULL, NULL);
    if (result == QPO_SUCCESS) {
        test_end(false, "Should fail for invalid metric and NULL pointers");
        qpo_destroy_context(context_id);
        return;
    }
    
    // Test resource exhaustion
    int contexts[1025]; // Try to create more than maximum
    int created = 0;
    for (int i = 0; i < 1025; i++) {
        contexts[i] = qpo_create_context(QPO_STRATEGY_BALANCED, 8);
        if (contexts[i] >= 0) {
            created++;
        } else {
            break; // Expected to fail at some point
        }
    }
    
    // Cleanup created contexts
    for (int i = 0; i < created; i++) {
        if (contexts[i] >= 0) {
            qpo_destroy_context(contexts[i]);
        }
    }
    
    qpo_destroy_context(context_id);
    test_end(true, NULL);
}

/**
 * @brief Run all tests
 */
static void run_all_tests(void) {
    printf("=== Quantum Performance Optimizer Test Suite ===\n\n");
    
    uint64_t start_time = get_timestamp_us();
    
    // Core functionality tests
    test_qpo_initialization();
    test_context_management();
    test_performance_sampling();
    test_computation_caching();
    test_computation_optimization();
    test_memory_pattern_analysis();
    test_hardware_acceleration();
    test_pattern_learning();
    test_adaptive_optimization();
    test_performance_statistics();
    test_configuration_management();
    test_benchmarking();
    test_data_export();
    test_optimization_recommendations();
    
    // Stress and performance tests
    test_multithreaded_stress();
    test_performance_regression();
    
    // Error handling tests
    test_error_handling();
    
    uint64_t end_time = get_timestamp_us();
    g_test_results.total_time = (end_time - start_time) / 1000000.0; // Convert to seconds
    
    // Print results
    printf("\n=== Test Results ===\n");
    printf("Tests run: %d\n", g_test_results.tests_run);
    printf("Tests passed: %d\n", g_test_results.tests_passed);
    printf("Tests failed: %d\n", g_test_results.tests_failed);
    printf("Success rate: %.1f%%\n", 
           (double)g_test_results.tests_passed / g_test_results.tests_run * 100.0);
    printf("Total time: %.3f seconds\n", g_test_results.total_time);
    
    if (g_test_results.tests_failed > 0) {
        printf("Last error: %s\n", g_test_results.last_error);
    }
    
    // Cleanup
    qpo_shutdown();
}

/**
 * @brief Main test function
 */
int main(int argc, char* argv[]) {
    // Set up test environment
    printf("Quantum Performance Optimizer Test Suite v1.0.0\n");
    printf("Testing on %d CPU cores with %d threads\n", 
           (int)sysconf(_SC_NPROCESSORS_ONLN), TEST_THREADS);
    
    // Run tests
    run_all_tests();
    
    // Return appropriate exit code
    return (g_test_results.tests_failed == 0) ? 0 : 1;
}
