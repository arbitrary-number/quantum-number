/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright (c) Arbitrary Number Project Team. All rights reserved.
 */
/**
 * Quantix Ultra-Scale Map (QUM) - Comprehensive Test Suite
 * 
 * Tests for the revolutionary secp256k1-based mapping system that provides
 * 2^256 buckets for unprecedented scalability without performance degradation.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include "../kernel/quantix_ultrascale_map.h"
#include "../kernel/quantum_number.h"

// Test configuration
#define TEST_ITERATIONS 10000
#define PERFORMANCE_TEST_COUNT 100000
#define STRESS_TEST_COUNT 1000000

// Test result tracking
static int tests_passed = 0;
static int tests_failed = 0;
static int total_tests = 0;

// Test macros
#define TEST_ASSERT(condition, message) do { \
    total_tests++; \
    if (condition) { \
        tests_passed++; \
        printf("✓ %s\n", message); \
    } else { \
        tests_failed++; \
        printf("✗ %s\n", message); \
    } \
} while(0)

#define TEST_ASSERT_EQUAL(expected, actual, message) do { \
    total_tests++; \
    if ((expected) == (actual)) { \
        tests_passed++; \
        printf("✓ %s\n", message); \
    } else { \
        tests_failed++; \
        printf("✗ %s (expected: %d, actual: %d)\n", message, (int)(expected), (int)(actual)); \
    } \
} while(0)

// Forward declarations
static void test_initialization_and_destruction(void);
static void test_basic_operations(void);
static void test_quantum_number_operations(void);
static void test_string_operations(void);
static void test_binary_operations(void);
static void test_curve_point_operations(void);
static void test_performance_metrics(void);
static void test_statistics(void);
static void test_utf8_key_handling(void);
static void test_collision_handling(void);
static void test_memory_management(void);
static void test_concurrent_operations(void);
static void test_performance_benchmarks(void);
static void test_stress_operations(void);
static void test_edge_cases(void);
static void print_test_summary(void);

/**
 * Main test runner
 */
int main(void) {
    printf("=== Quantix Ultra-Scale Map Test Suite ===\n");
    printf("Testing revolutionary secp256k1-based mapping system\n\n");

    // Run all test suites
    test_initialization_and_destruction();
    test_basic_operations();
    test_quantum_number_operations();
    test_string_operations();
    test_binary_operations();
    test_curve_point_operations();
    test_performance_metrics();
    test_statistics();
    test_utf8_key_handling();
    test_collision_handling();
    test_memory_management();
    test_performance_benchmarks();
    test_stress_operations();
    test_edge_cases();

    // Print final results
    print_test_summary();

    return (tests_failed == 0) ? 0 : 1;
}

/**
 * Test initialization and destruction
 */
static void test_initialization_and_destruction(void) {
    printf("\n--- Testing Initialization and Destruction ---\n");

    qum_configuration_t config;
    qum_result_t result = qum_create_default_configuration(&config);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Create default configuration");

    qum_context_t *context = NULL;
    result = qum_initialize(&context, &config);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Initialize ultra-scale map");
    TEST_ASSERT(context != NULL, "Context is not NULL after initialization");
    TEST_ASSERT(context->is_initialized, "Context is marked as initialized");

    result = qum_destroy(context);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Destroy ultra-scale map");

    // Test invalid parameters
    result = qum_initialize(NULL, &config);
    TEST_ASSERT_EQUAL(QUM_ERROR_INVALID_PARAMETERS, result, "Initialize with NULL context pointer");

    result = qum_initialize(&context, NULL);
    TEST_ASSERT_EQUAL(QUM_ERROR_INVALID_PARAMETERS, result, "Initialize with NULL config");
}

/**
 * Test basic operations (put, get, remove, contains)
 */
static void test_basic_operations(void) {
    printf("\n--- Testing Basic Operations ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_result_t result = qum_initialize(&context, &config);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Initialize for basic operations");

    // Test put operation
    const char *test_key = "test_key_123";
    uint64_t test_value = 0x123456789ABCDEF0ULL;
    
    result = qum_put(context, test_key, &test_value, sizeof(test_value), QUM_VALUE_TYPE_BINARY_DATA);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Put operation successful");

    // Test contains operation
    bool exists = false;
    result = qum_contains(context, test_key, &exists);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Contains operation successful");
    TEST_ASSERT(exists, "Key exists after put");

    // Test get operation
    void *retrieved_value = NULL;
    size_t retrieved_size = 0;
    qum_value_type_t retrieved_type;
    
    result = qum_get(context, test_key, &retrieved_value, &retrieved_size, &retrieved_type);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Get operation successful");
    TEST_ASSERT_EQUAL(sizeof(test_value), retrieved_size, "Retrieved size matches");
    TEST_ASSERT_EQUAL(QUM_VALUE_TYPE_BINARY_DATA, retrieved_type, "Retrieved type matches");
    TEST_ASSERT(memcmp(&test_value, retrieved_value, sizeof(test_value)) == 0, "Retrieved value matches");
    
    free(retrieved_value);

    // Test map size
    uint64_t entry_count = 0;
    result = qum_size(context, &entry_count);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Size operation successful");
    TEST_ASSERT_EQUAL(1, entry_count, "Map size is 1 after single put");

    // Test remove operation
    result = qum_remove(context, test_key);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Remove operation successful");

    // Verify key no longer exists
    exists = true;
    result = qum_contains(context, test_key, &exists);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Contains check after remove");
    TEST_ASSERT(!exists, "Key does not exist after remove");

    // Test get on non-existent key
    result = qum_get(context, test_key, &retrieved_value, &retrieved_size, &retrieved_type);
    TEST_ASSERT_EQUAL(QUM_ERROR_KEY_NOT_FOUND, result, "Get on removed key returns not found");

    qum_destroy(context);
}

/**
 * Test Quantum Number operations
 */
static void test_quantum_number_operations(void) {
    printf("\n--- Testing Quantum Number Operations ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_initialize(&context, &config);

    // Create test quantum number
    quantum_number_t test_qnum;
    memset(&test_qnum, 0, sizeof(quantum_number_t));
    
    // Set some test values (simplified)
    test_qnum.real_part[0] = 0x12345678;
    test_qnum.real_part[1] = 0x9ABCDEF0;
    test_qnum.imaginary_part[0] = 0xFEDCBA98;
    test_qnum.imaginary_part[1] = 0x76543210;

    const char *qnum_key = "quantum_test_key";

    // Test put quantum number
    qum_result_t result = qum_put_quantum_number(context, qnum_key, &test_qnum);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Put quantum number successful");

    // Test get quantum number
    quantum_number_t retrieved_qnum;
    result = qum_get_quantum_number(context, qnum_key, &retrieved_qnum);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Get quantum number successful");
    
    TEST_ASSERT(memcmp(&test_qnum, &retrieved_qnum, sizeof(quantum_number_t)) == 0, 
                "Retrieved quantum number matches original");

    qum_destroy(context);
}

/**
 * Test string operations
 */
static void test_string_operations(void) {
    printf("\n--- Testing String Operations ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_initialize(&context, &config);

    const char *string_key = "string_test_key";
    const char *test_string = "Hello, Ultra-Scale Map! 🚀";

    // Test put string
    qum_result_t result = qum_put_string(context, string_key, test_string);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Put string successful");

    // Test get string
    char *retrieved_string = NULL;
    result = qum_get_string(context, string_key, &retrieved_string);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Get string successful");
    TEST_ASSERT(strcmp(test_string, retrieved_string) == 0, "Retrieved string matches original");
    
    free(retrieved_string);

    qum_destroy(context);
}

/**
 * Test binary operations
 */
static void test_binary_operations(void) {
    printf("\n--- Testing Binary Operations ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_initialize(&context, &config);

    const char *binary_key = "binary_test_key";
    uint8_t test_data[256];
    
    // Fill with test pattern
    for (int i = 0; i < 256; i++) {
        test_data[i] = (uint8_t)(i ^ 0xAA);
    }

    // Test put binary
    qum_result_t result = qum_put_binary(context, binary_key, test_data, sizeof(test_data));
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Put binary data successful");

    // Test get binary
    void *retrieved_data = NULL;
    size_t retrieved_size = 0;
    result = qum_get_binary(context, binary_key, &retrieved_data, &retrieved_size);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Get binary data successful");
    TEST_ASSERT_EQUAL(sizeof(test_data), retrieved_size, "Retrieved binary size matches");
    TEST_ASSERT(memcmp(test_data, retrieved_data, sizeof(test_data)) == 0, 
                "Retrieved binary data matches original");
    
    free(retrieved_data);

    qum_destroy(context);
}

/**
 * Test curve point operations
 */
static void test_curve_point_operations(void) {
    printf("\n--- Testing Curve Point Operations ---\n");

    // Test key to curve point conversion
    const char *test_key = "secp256k1_test_key";
    qum_curve_point_t point;
    
    qum_result_t result = qum_key_to_curve_point(test_key, strlen(test_key), &point);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Key to curve point conversion successful");

    // Test curve point verification
    result = qum_verify_curve_point(&point);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Curve point verification successful");

    // Test curve point to bucket address conversion
    qum_bucket_address_t address;
    result = qum_curve_point_to_bucket_address(&point, &address);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Curve point to bucket address conversion successful");

    // Test that different keys produce different points
    const char *test_key2 = "different_secp256k1_key";
    qum_curve_point_t point2;
    
    result = qum_key_to_curve_point(test_key2, strlen(test_key2), &point2);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Second key to curve point conversion successful");
    
    TEST_ASSERT(memcmp(&point, &point2, sizeof(qum_curve_point_t)) != 0, 
                "Different keys produce different curve points");
}

/**
 * Test performance metrics
 */
static void test_performance_metrics(void) {
    printf("\n--- Testing Performance Metrics ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_initialize(&context, &config);

    // Perform some operations to generate metrics
    for (int i = 0; i < 100; i++) {
        char key[64];
        snprintf(key, sizeof(key), "perf_test_key_%d", i);
        
        uint64_t value = i * 12345;
        qum_put(context, key, &value, sizeof(value), QUM_VALUE_TYPE_BINARY_DATA);
        
        void *retrieved_value;
        size_t retrieved_size;
        qum_get(context, key, &retrieved_value, &retrieved_size, NULL);
        free(retrieved_value);
    }

    // Get performance metrics
    qum_performance_metrics_t metrics;
    qum_result_t result = qum_get_performance_metrics(context, &metrics);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Get performance metrics successful");
    
    TEST_ASSERT(metrics.total_operations > 0, "Total operations recorded");
    TEST_ASSERT(metrics.successful_operations > 0, "Successful operations recorded");
    TEST_ASSERT(metrics.curve_computations > 0, "Curve computations recorded");
    TEST_ASSERT(metrics.average_operation_time_ns > 0, "Average operation time recorded");

    printf("  Performance Metrics:\n");
    printf("    Total operations: %llu\n", (unsigned long long)metrics.total_operations);
    printf("    Successful operations: %llu\n", (unsigned long long)metrics.successful_operations);
    printf("    Curve computations: %llu\n", (unsigned long long)metrics.curve_computations);
    printf("    Average operation time: %.2f ns\n", metrics.average_operation_time_ns);

    qum_destroy(context);
}

/**
 * Test statistics
 */
static void test_statistics(void) {
    printf("\n--- Testing Statistics ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_initialize(&context, &config);

    // Add various sized entries
    const char *keys[] = {"small", "medium", "large"};
    size_t sizes[] = {10, 1000, 10000};
    
    for (int i = 0; i < 3; i++) {
        uint8_t *data = malloc(sizes[i]);
        memset(data, i + 1, sizes[i]);
        
        qum_put(context, keys[i], data, sizes[i], QUM_VALUE_TYPE_BINARY_DATA);
        free(data);
    }

    // Get statistics
    qum_statistics_t stats;
    qum_result_t result = qum_get_statistics(context, &stats);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Get statistics successful");
    
    TEST_ASSERT_EQUAL(3, stats.total_entries, "Total entries matches");
    TEST_ASSERT(stats.total_buckets_used > 0, "Buckets are being used");
    TEST_ASSERT(stats.largest_value_size >= 10000, "Largest value size recorded");
    TEST_ASSERT(stats.smallest_value_size <= 10, "Smallest value size recorded");
    TEST_ASSERT(stats.average_value_size > 0, "Average value size calculated");

    printf("  Statistics:\n");
    printf("    Total entries: %llu\n", (unsigned long long)stats.total_entries);
    printf("    Total buckets used: %llu\n", (unsigned long long)stats.total_buckets_used);
    printf("    Largest value: %llu bytes\n", (unsigned long long)stats.largest_value_size);
    printf("    Smallest value: %llu bytes\n", (unsigned long long)stats.smallest_value_size);
    printf("    Average value size: %.2f bytes\n", stats.average_value_size);

    qum_destroy(context);
}

/**
 * Test UTF-8 key handling
 */
static void test_utf8_key_handling(void) {
    printf("\n--- Testing UTF-8 Key Handling ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_initialize(&context, &config);

    // Test various UTF-8 keys
    const char *utf8_keys[] = {
        "simple_ascii",
        "café_français",
        "日本語キー",
        "🚀🌟💫",
        "Ελληνικά",
        "العربية",
        "русский"
    };
    
    int num_keys = sizeof(utf8_keys) / sizeof(utf8_keys[0]);
    
    for (int i = 0; i < num_keys; i++) {
        // Validate key
        qum_result_t result = qum_validate_key(utf8_keys[i], strlen(utf8_keys[i]));
        TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "UTF-8 key validation successful");
        
        // Test put/get with UTF-8 key
        uint32_t value = i * 1000;
        result = qum_put(context, utf8_keys[i], &value, sizeof(value), QUM_VALUE_TYPE_BINARY_DATA);
        TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Put with UTF-8 key successful");
        
        void *retrieved_value;
        size_t retrieved_size;
        result = qum_get(context, utf8_keys[i], &retrieved_value, &retrieved_size, NULL);
        TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Get with UTF-8 key successful");
        TEST_ASSERT(*(uint32_t*)retrieved_value == value, "UTF-8 key value matches");
        
        free(retrieved_value);
    }

    qum_destroy(context);
}

/**
 * Test collision handling
 */
static void test_collision_handling(void) {
    printf("\n--- Testing Collision Handling ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);
    config.initial_bucket_cache_size = 16; // Small cache to force collisions

    qum_context_t *context;
    qum_initialize(&context, &config);

    // Add many entries to potentially cause collisions
    const int num_entries = 100;
    for (int i = 0; i < num_entries; i++) {
        char key[64];
        snprintf(key, sizeof(key), "collision_test_%d", i);
        
        uint64_t value = i * 7919; // Prime multiplier
        qum_result_t result = qum_put(context, key, &value, sizeof(value), QUM_VALUE_TYPE_BINARY_DATA);
        TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Put during collision test successful");
    }

    // Verify all entries can be retrieved
    int successful_retrievals = 0;
    for (int i = 0; i < num_entries; i++) {
        char key[64];
        snprintf(key, sizeof(key), "collision_test_%d", i);
        
        void *retrieved_value;
        size_t retrieved_size;
        qum_result_t result = qum_get(context, key, &retrieved_value, &retrieved_size, NULL);
        
        if (result == QUM_SUCCESS) {
            uint64_t expected_value = i * 7919;
            if (*(uint64_t*)retrieved_value == expected_value) {
                successful_retrievals++;
            }
            free(retrieved_value);
        }
    }

    TEST_ASSERT_EQUAL(num_entries, successful_retrievals, "All entries retrievable despite collisions");

    qum_destroy(context);
}

/**
 * Test memory management
 */
static void test_memory_management(void) {
    printf("\n--- Testing Memory Management ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);
    config.max_memory_usage = 1024 * 1024; // 1MB limit

    qum_context_t *context;
    qum_initialize(&context, &config);

    // Test memory usage tracking
    qum_performance_metrics_t initial_metrics;
    qum_get_performance_metrics(context, &initial_metrics);

    // Add some data
    const char *key = "memory_test";
    size_t large_size = 100000; // 100KB
    uint8_t *large_data = malloc(large_size);
    memset(large_data, 0xAA, large_size);

    qum_result_t result = qum_put(context, key, large_data, large_size, QUM_VALUE_TYPE_BINARY_DATA);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Put large data successful");

    qum_performance_metrics_t after_metrics;
    qum_get_performance_metrics(context, &after_metrics);

    TEST_ASSERT(after_metrics.memory_usage_bytes > initial_metrics.memory_usage_bytes, 
                "Memory usage increased after adding data");

    // Test clear operation
    result = qum_clear(context);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Clear operation successful");

    uint64_t entry_count;
    qum_size(context, &entry_count);
    TEST_ASSERT_EQUAL(0, entry_count, "Map is empty after clear");

    free(large_data);
    qum_destroy(context);
}

/**
 * Test performance benchmarks
 */
static void test_performance_benchmarks(void) {
    printf("\n--- Testing Performance Benchmarks ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_initialize(&context, &config);

    // Run benchmark
    double operations_per_second = 0.0;
    qum_result_t result = qum_benchmark_operations(context, 1000, &operations_per_second);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Benchmark operations successful");
    TEST_ASSERT(operations_per_second > 0, "Operations per second measured");

    printf("  Benchmark Results:\n");
    printf("    Operations per second: %.2f\n", operations_per_second);
    printf("    Average time per operation: %.2f ns\n", 1000000000.0 / operations_per_second);

    // Verify performance is reasonable (should be very fast)
    TEST_ASSERT(operations_per_second > 10000, "Performance meets minimum threshold");

    qum_destroy(context);
}

/**
 * Test stress operations
 */
static void test_stress_operations(void) {
    printf("\n--- Testing Stress Operations ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_initialize(&context, &config);

    const int stress_count = 10000;
    int successful_operations = 0;

    printf("  Running %d stress operations...\n", stress_count);

    // Stress test with random operations
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < stress_count; i++) {
        char key[64];
        snprintf(key, sizeof(key), "stress_%d_%d", i, rand() % 1000);
        
        uint64_t value = (uint64_t)rand() * rand();
        
        if (qum_put(context, key, &value, sizeof(value), QUM_VALUE_TYPE_BINARY_DATA) == QUM_SUCCESS) {
            void *retrieved_value;
            size_t retrieved_size;
            
            if (qum_get(context, key, &retrieved_value, &retrieved_size, NULL) == QUM_SUCCESS) {
                if (*(uint64_t*)retrieved_value == value) {
                    successful_operations++;
                }
                free(retrieved_value);
            }
        }
        
        // Progress indicator
        if (i % 1000 == 0) {
            printf("    Progress: %d/%d operations\n", i, stress_count);
        }
    }

    double success_rate = (double)successful_operations / stress_count;
    printf("  Stress test success rate: %.2f%% (%d/%d)\n", 
           success_rate * 100, successful_operations, stress_count);

    TEST_ASSERT(success_rate > 0.95, "Stress test success rate above 95%");

    qum_destroy(context);
}

/**
 * Test edge cases
 */
static void test_edge_cases(void) {
    printf("\n--- Testing Edge Cases ---\n");

    qum_configuration_t config;
    qum_create_default_configuration(&config);

    qum_context_t *context;
    qum_initialize(&context, &config);

    // Test empty key (should fail)
    qum_result_t result = qum_put(context, "", "value", 5, QUM_VALUE_TYPE_STRING);
    TEST_ASSERT_EQUAL(QUM_ERROR_INVALID_PARAMETERS, result, "Empty key rejected");

    // Test NULL key (should fail)
    result = qum_put(context, NULL, "value", 5, QUM_VALUE_TYPE_STRING);
    TEST_ASSERT_EQUAL(QUM_ERROR_INVALID_PARAMETERS, result, "NULL key rejected");

    // Test very long key
    char long_key[QUM_MAX_KEY_LENGTH + 100];
    memset(long_key, 'A', sizeof(long_key) - 1);
    long_key[sizeof(long_key) - 1] = '\0';
    
    result = qum_put(context, long_key, "value", 5, QUM_VALUE_TYPE_STRING);
    TEST_ASSERT_EQUAL(QUM_ERROR_INVALID_PARAMETERS, result, "Overly long key rejected");

    // Test maximum allowed key length
    char max_key[QUM_MAX_KEY_LENGTH];
    memset(max_key, 'B', sizeof(max_key) - 1);
    max_key[sizeof(max_key) - 1] = '\0';
    
    result = qum_put(context, max_key, "value", 5, QUM_VALUE_TYPE_STRING);
    TEST_ASSERT_EQUAL(QUM_SUCCESS, result, "Maximum length key accepted");

    // Test very large value
    size_t large_size = QUM_MAX_VALUE_SIZE + 1000;
    uint8_t *large_value = malloc(large_size);
    memset(large_value, 0xFF, large_size);
    
    result = qum_put(context, "large_test", large_value, large_size, QUM_VALUE_TYPE_BINARY_DATA);
    TEST_ASSERT_EQUAL(QUM_ERROR_INVALID_VALUE, result, "Overly large value rejected");
    
    free(large_value);

    // Test operations on destroyed context
    qum_destroy(context);
    
    result = qum_put(context, "test", "value", 5, QUM_VALUE_TYPE_STRING);
    TEST_ASSERT_EQUAL(QUM_ERROR_INVALID_PARAMETERS, result, "Operations on destroyed context rejected");
}

/**
 * Print test summary
 */
static void print_test_summary(void) {
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", total_tests);
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Success rate: %.2f%%\n", (double)tests_passed / total_tests * 100);
    
    if (tests_failed == 0) {
        printf("\n🎉 All tests passed! Ultra-Scale Map is working perfectly!\n");
    } else {
        printf("\n❌ Some tests failed. Please review the implementation.\n");
    }
}
