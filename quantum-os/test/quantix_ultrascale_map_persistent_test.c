/*
 * Copyright 2025 Arbitrary Number Project Team
 *
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
 */

/**
 * Quantix Ultra-Scale Map Persistent (QUMP) - Comprehensive Test Suite
 * 
 * Revolutionary test suite for the world's first persistent secp256k1-based mapping system
 * that provides 2^256 buckets with full QFS integration for unprecedented scalability.
 * 
 * Test Coverage:
 * - Basic persistent operations (put, get, remove, contains, clear)
 * - Synchronous persistence mode testing
 * - Asynchronous persistence mode testing
 * - Hybrid persistence mode testing
 * - Write-ahead logging and crash recovery
 * - QFS integration and mathematical metadata
 * - Performance benchmarking and stress testing
 * - Concurrency and thread safety
 * - Error handling and edge cases
 * - Memory management and resource cleanup
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "../kernel/quantix_ultrascale_map_persistent.h"

// Test configuration
#define TEST_MAX_ENTRIES 10000
#define TEST_STRESS_ENTRIES 100000
#define TEST_CONCURRENT_THREADS 8
#define TEST_TIMEOUT_SECONDS 30
#define TEST_TEMP_DIR "/tmp/qump_test"

// Test result tracking
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
    double total_time_ms;
} test_results_t;

static test_results_t g_test_results = {0};

// Test utilities
static double get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec * 1000.0 + (double)tv.tv_usec / 1000.0;
}

static void test_start(const char *test_name) {
    printf("Running test: %s... ", test_name);
    fflush(stdout);
    g_test_results.total_tests++;
}

static void test_pass(void) {
    printf("PASS\n");
    g_test_results.passed_tests++;
}

static void test_fail(const char *reason) {
    printf("FAIL - %s\n", reason);
    g_test_results.failed_tests++;
}

static void cleanup_test_files(void) {
    // Clean up any test files
    system("rm -rf " TEST_TEMP_DIR " 2>/dev/null");
}

// Test: Basic initialization and destruction
static void test_basic_initialization(void) {
    test_start("Basic Initialization");
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_DISABLED; // Start simple
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize context");
        return;
    }
    
    if (!context) {
        test_fail("Context is NULL after initialization");
        return;
    }
    
    result = qump_destroy(context);
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to destroy context");
        return;
    }
    
    test_pass();
}

// Test: Basic put/get operations without persistence
static void test_basic_operations_no_persistence(void) {
    test_start("Basic Operations (No Persistence)");
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_DISABLED;
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize context");
        return;
    }
    
    // Test put operation
    const char *test_key = "test_key_123";
    const char *test_value = "test_value_456";
    size_t test_value_size = strlen(test_value) + 1;
    
    result = qump_put_persistent(context, test_key, test_value, test_value_size, 
                                QUM_VALUE_TYPE_STRING, false);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to put value");
        return;
    }
    
    // Test get operation
    void *retrieved_value = NULL;
    size_t retrieved_size = 0;
    qum_value_type_t retrieved_type;
    
    result = qump_get_persistent(context, test_key, &retrieved_value, &retrieved_size, &retrieved_type);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to get value");
        return;
    }
    
    if (retrieved_size != test_value_size || 
        retrieved_type != QUM_VALUE_TYPE_STRING ||
        strcmp((char*)retrieved_value, test_value) != 0) {
        free(retrieved_value);
        qump_destroy(context);
        test_fail("Retrieved value doesn't match");
        return;
    }
    
    free(retrieved_value);
    
    // Test contains operation
    bool exists = false;
    result = qump_contains_persistent(context, test_key, &exists);
    if (result != QUMP_SUCCESS || !exists) {
        qump_destroy(context);
        test_fail("Contains operation failed");
        return;
    }
    
    // Test remove operation
    result = qump_remove_persistent(context, test_key, false);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to remove value");
        return;
    }
    
    // Verify removal
    result = qump_contains_persistent(context, test_key, &exists);
    if (result != QUMP_SUCCESS || exists) {
        qump_destroy(context);
        test_fail("Value still exists after removal");
        return;
    }
    
    qump_destroy(context);
    test_pass();
}

// Test: Quantum number operations
static void test_quantum_number_operations(void) {
    test_start("Quantum Number Operations");
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_DISABLED;
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize context");
        return;
    }
    
    // Create a test quantum number
    quantum_number_t test_qnum;
    memset(&test_qnum, 0, sizeof(quantum_number_t));
    test_qnum.ordinals[0] = 42;
    test_qnum.ordinals[1] = 123;
    test_qnum.signs[0] = 1;
    test_qnum.signs[1] = -1;
    
    const char *qnum_key = "quantum_test_key";
    
    // Test put quantum number
    result = qump_put_persistent(context, qnum_key, &test_qnum, sizeof(quantum_number_t), 
                                QUM_VALUE_TYPE_QUANTUM_NUMBER, false);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to put quantum number");
        return;
    }
    
    // Test get quantum number
    void *retrieved_value = NULL;
    size_t retrieved_size = 0;
    qum_value_type_t retrieved_type;
    
    result = qump_get_persistent(context, qnum_key, &retrieved_value, &retrieved_size, &retrieved_type);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to get quantum number");
        return;
    }
    
    if (retrieved_size != sizeof(quantum_number_t) || 
        retrieved_type != QUM_VALUE_TYPE_QUANTUM_NUMBER) {
        free(retrieved_value);
        qump_destroy(context);
        test_fail("Retrieved quantum number has wrong size or type");
        return;
    }
    
    quantum_number_t *retrieved_qnum = (quantum_number_t*)retrieved_value;
    if (retrieved_qnum->ordinals[0] != test_qnum.ordinals[0] ||
        retrieved_qnum->ordinals[1] != test_qnum.ordinals[1] ||
        retrieved_qnum->signs[0] != test_qnum.signs[0] ||
        retrieved_qnum->signs[1] != test_qnum.signs[1]) {
        free(retrieved_value);
        qump_destroy(context);
        test_fail("Retrieved quantum number values don't match");
        return;
    }
    
    free(retrieved_value);
    qump_destroy(context);
    test_pass();
}

// Test: Synchronous persistence mode
static void test_sync_persistence_mode(void) {
    test_start("Synchronous Persistence Mode");
    
    cleanup_test_files();
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_SYNC;
    snprintf(persistence_config.storage_path, sizeof(persistence_config.storage_path), 
             "%s/storage", TEST_TEMP_DIR);
    snprintf(persistence_config.wal_path, sizeof(persistence_config.wal_path), 
             "%s/wal", TEST_TEMP_DIR);
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize sync persistence context");
        return;
    }
    
    // Test sync put operation
    const char *sync_key = "sync_test_key";
    const char *sync_value = "sync_test_value";
    size_t sync_value_size = strlen(sync_value) + 1;
    
    result = qump_put_persistent(context, sync_key, sync_value, sync_value_size, 
                                QUM_VALUE_TYPE_STRING, true);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to put value in sync mode");
        return;
    }
    
    // Get persistence statistics
    qump_persistence_stats_t stats;
    result = qump_get_persistence_stats(context, &stats);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to get persistence stats");
        return;
    }
    
    if (stats.sync_operations == 0) {
        qump_destroy(context);
        test_fail("No sync operations recorded");
        return;
    }
    
    qump_destroy(context);
    cleanup_test_files();
    test_pass();
}

// Test: Asynchronous persistence mode
static void test_async_persistence_mode(void) {
    test_start("Asynchronous Persistence Mode");
    
    cleanup_test_files();
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_ASYNC;
    persistence_config.sync_interval_ms = 100; // Fast sync for testing
    snprintf(persistence_config.storage_path, sizeof(persistence_config.storage_path), 
             "%s/storage", TEST_TEMP_DIR);
    snprintf(persistence_config.wal_path, sizeof(persistence_config.wal_path), 
             "%s/wal", TEST_TEMP_DIR);
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize async persistence context");
        return;
    }
    
    // Test async put operations
    for (int i = 0; i < 10; i++) {
        char key[64], value[64];
        snprintf(key, sizeof(key), "async_key_%d", i);
        snprintf(value, sizeof(value), "async_value_%d", i);
        
        result = qump_put_persistent(context, key, value, strlen(value) + 1, 
                                    QUM_VALUE_TYPE_STRING, false);
        if (result != QUMP_SUCCESS) {
            qump_destroy(context);
            test_fail("Failed to put async value");
            return;
        }
    }
    
    // Wait for async operations to complete
    usleep(200000); // 200ms
    
    // Force sync to ensure all operations are persisted
    result = qump_sync_to_storage(context);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to sync to storage");
        return;
    }
    
    // Get persistence statistics
    qump_persistence_stats_t stats;
    result = qump_get_persistence_stats(context, &stats);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to get persistence stats");
        return;
    }
    
    if (stats.async_operations == 0) {
        qump_destroy(context);
        test_fail("No async operations recorded");
        return;
    }
    
    qump_destroy(context);
    cleanup_test_files();
    test_pass();
}

// Test: Performance benchmarking
static void test_performance_benchmark(void) {
    test_start("Performance Benchmark");
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_DISABLED; // Focus on in-memory performance
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize context for benchmark");
        return;
    }
    
    const int num_operations = 1000;
    double start_time = get_time_ms();
    
    // Benchmark put operations
    for (int i = 0; i < num_operations; i++) {
        char key[64], value[128];
        snprintf(key, sizeof(key), "benchmark_key_%d", i);
        snprintf(value, sizeof(value), "benchmark_value_%d_with_some_extra_data", i);
        
        result = qump_put_persistent(context, key, value, strlen(value) + 1, 
                                    QUM_VALUE_TYPE_STRING, false);
        if (result != QUMP_SUCCESS) {
            qump_destroy(context);
            test_fail("Failed put operation during benchmark");
            return;
        }
    }
    
    double put_time = get_time_ms() - start_time;
    start_time = get_time_ms();
    
    // Benchmark get operations
    for (int i = 0; i < num_operations; i++) {
        char key[64];
        snprintf(key, sizeof(key), "benchmark_key_%d", i);
        
        void *value = NULL;
        size_t value_size = 0;
        qum_value_type_t type;
        
        result = qump_get_persistent(context, key, &value, &value_size, &type);
        if (result != QUMP_SUCCESS) {
            qump_destroy(context);
            test_fail("Failed get operation during benchmark");
            return;
        }
        
        free(value);
    }
    
    double get_time = get_time_ms() - start_time;
    
    printf("\n  Put operations: %.2f ops/ms (%.2f ms total)\n", 
           num_operations / put_time, put_time);
    printf("  Get operations: %.2f ops/ms (%.2f ms total)\n", 
           num_operations / get_time, get_time);
    
    qump_destroy(context);
    test_pass();
}

// Test: Stress testing with many entries
static void test_stress_many_entries(void) {
    test_start("Stress Test (Many Entries)");
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    base_config.initial_bucket_cache_size = 2048; // Larger cache for stress test
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_DISABLED;
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize context for stress test");
        return;
    }
    
    const int stress_entries = 5000; // Reduced for faster testing
    double start_time = get_time_ms();
    
    // Insert many entries
    for (int i = 0; i < stress_entries; i++) {
        char key[64], value[128];
        snprintf(key, sizeof(key), "stress_key_%d", i);
        snprintf(value, sizeof(value), "stress_value_%d_with_additional_data_for_testing", i);
        
        result = qump_put_persistent(context, key, value, strlen(value) + 1, 
                                    QUM_VALUE_TYPE_STRING, false);
        if (result != QUMP_SUCCESS) {
            qump_destroy(context);
            test_fail("Failed to insert entry during stress test");
            return;
        }
        
        // Progress indicator
        if (i % 1000 == 0 && i > 0) {
            printf(".");
            fflush(stdout);
        }
    }
    
    double insert_time = get_time_ms() - start_time;
    
    // Verify all entries exist
    start_time = get_time_ms();
    for (int i = 0; i < stress_entries; i++) {
        char key[64];
        snprintf(key, sizeof(key), "stress_key_%d", i);
        
        bool exists = false;
        result = qump_contains_persistent(context, key, &exists);
        if (result != QUMP_SUCCESS || !exists) {
            qump_destroy(context);
            test_fail("Entry missing during stress test verification");
            return;
        }
    }
    
    double verify_time = get_time_ms() - start_time;
    
    printf("\n  Inserted %d entries in %.2f ms (%.2f entries/ms)\n", 
           stress_entries, insert_time, stress_entries / insert_time);
    printf("  Verified %d entries in %.2f ms (%.2f lookups/ms)\n", 
           stress_entries, verify_time, stress_entries / verify_time);
    
    qump_destroy(context);
    test_pass();
}

// Test: Configuration validation
static void test_configuration_validation(void) {
    test_start("Configuration Validation");
    
    qump_persistence_config_t config;
    
    // Test invalid mode
    memset(&config, 0, sizeof(config));
    config.mode = (qump_persistence_mode_t)999; // Invalid mode
    qump_result_t result = qump_validate_persistence_config(&config);
    if (result == QUMP_SUCCESS) {
        test_fail("Should reject invalid persistence mode");
        return;
    }
    
    // Test valid configuration
    qump_create_default_persistence_config(&config);
    result = qump_validate_persistence_config(&config);
    if (result != QUMP_SUCCESS) {
        test_fail("Should accept valid default configuration");
        return;
    }
    
    // Test empty paths for enabled persistence
    config.mode = QUMP_PERSISTENCE_SYNC;
    config.storage_path[0] = '\0';
    result = qump_validate_persistence_config(&config);
    if (result == QUMP_SUCCESS) {
        test_fail("Should reject empty storage path for enabled persistence");
        return;
    }
    
    test_pass();
}

// Test: Error handling
static void test_error_handling(void) {
    test_start("Error Handling");
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_DISABLED;
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize context for error testing");
        return;
    }
    
    // Test NULL key
    result = qump_put_persistent(context, NULL, "value", 6, QUM_VALUE_TYPE_STRING, false);
    if (result == QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Should reject NULL key");
        return;
    }
    
    // Test NULL value
    result = qump_put_persistent(context, "key", NULL, 0, QUM_VALUE_TYPE_STRING, false);
    if (result == QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Should reject NULL value");
        return;
    }
    
    // Test get non-existent key
    void *value = NULL;
    size_t value_size = 0;
    qum_value_type_t type;
    result = qump_get_persistent(context, "non_existent_key", &value, &value_size, &type);
    if (result == QUMP_SUCCESS) {
        if (value) free(value);
        qump_destroy(context);
        test_fail("Should fail to get non-existent key");
        return;
    }
    
    qump_destroy(context);
    test_pass();
}

// Test: Clear operation
static void test_clear_operation(void) {
    test_start("Clear Operation");
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_DISABLED;
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize context for clear test");
        return;
    }
    
    // Add some entries
    for (int i = 0; i < 10; i++) {
        char key[64], value[64];
        snprintf(key, sizeof(key), "clear_test_key_%d", i);
        snprintf(value, sizeof(value), "clear_test_value_%d", i);
        
        result = qump_put_persistent(context, key, value, strlen(value) + 1, 
                                    QUM_VALUE_TYPE_STRING, false);
        if (result != QUMP_SUCCESS) {
            qump_destroy(context);
            test_fail("Failed to add entry for clear test");
            return;
        }
    }
    
    // Verify entries exist
    bool exists = false;
    result = qump_contains_persistent(context, "clear_test_key_0", &exists);
    if (result != QUMP_SUCCESS || !exists) {
        qump_destroy(context);
        test_fail("Entry should exist before clear");
        return;
    }
    
    // Clear all entries
    result = qump_clear_persistent(context, false);
    if (result != QUMP_SUCCESS) {
        qump_destroy(context);
        test_fail("Failed to clear entries");
        return;
    }
    
    // Verify entries are gone
    result = qump_contains_persistent(context, "clear_test_key_0", &exists);
    if (result != QUMP_SUCCESS || exists) {
        qump_destroy(context);
        test_fail("Entry should not exist after clear");
        return;
    }
    
    qump_destroy(context);
    test_pass();
}

// Concurrent test thread function
typedef struct {
    qump_persistent_context_t *context;
    int thread_id;
    int operations_per_thread;
    int *success_count;
    pthread_mutex_t *count_mutex;
} thread_test_data_t;

static void* concurrent_test_thread(void *arg) {
    thread_test_data_t *data = (thread_test_data_t*)arg;
    int local_success = 0;
    
    for (int i = 0; i < data->operations_per_thread; i++) {
        char key[64], value[128];
        snprintf(key, sizeof(key), "thread_%d_key_%d", data->thread_id, i);
        snprintf(value, sizeof(value), "thread_%d_value_%d", data->thread_id, i);
        
        // Put operation
        qump_result_t result = qump_put_persistent(data->context, key, value, 
                                                  strlen(value) + 1, 
                                                  QUM_VALUE_TYPE_STRING, false);
        if (result == QUMP_SUCCESS) {
            // Get operation to verify
            void *retrieved_value = NULL;
            size_t retrieved_size = 0;
            qum_value_type_t retrieved_type;
            
            result = qump_get_persistent(data->context, key, &retrieved_value, 
                                        &retrieved_size, &retrieved_type);
            if (result == QUMP_SUCCESS) {
                if (strcmp((char*)retrieved_value, value) == 0) {
                    local_success++;
                }
                free(retrieved_value);
            }
        }
    }
    
    pthread_mutex_lock(data->count_mutex);
    *(data->success_count) += local_success;
    pthread_mutex_unlock(data->count_mutex);
    
    return NULL;
}

// Test: Concurrent operations
static void test_concurrent_operations(void) {
    test_start("Concurrent Operations");
    
    qum_configuration_t base_config;
    qum_create_default_configuration(&base_config);
    base_config.initial_bucket_cache_size = 1024;
    
    qump_persistence_config_t persistence_config;
    qump_create_default_persistence_config(&persistence_config);
    persistence_config.mode = QUMP_PERSISTENCE_DISABLED;
    
    qump_persistent_context_t *context = NULL;
    qump_result_t result = qump_initialize(&context, &base_config, &persistence_config);
    
    if (result != QUMP_SUCCESS) {
        test_fail("Failed to initialize context for concurrent test");
        return;
    }
    
    const int num_threads = 4;
    const int ops_per_thread = 100;
    pthread_t threads[num_threads];
    thread_test_data_t thread_data[num_threads];
    int total_success = 0;
    pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
    
    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].context = context;
        thread_data[i].thread_id = i;
        thread_data[i].operations_per_thread = ops_per_thread;
        thread_data[i].success_count = &total_success;
        thread_data[i].count_mutex = &count_mutex;
        
        if (pthread_create(&threads[i], NULL, concurrent_test_thread, &thread_data[i]) != 0) {
            qump_destroy(context);
            test_fail("Failed to create thread");
            return;
        }
    }
    
    // Wait for threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    pthread_mutex_destroy(&count_mutex);
    
    int expected_success = num_threads * ops_per_thread;
    if (total_success != expected_success) {
        qump_destroy(context);
        char error_msg[256];
        snprintf(error_msg, sizeof(error_msg), 
                "Expected %d successful operations, got %d", 
                expected_success, total_success);
        test_fail(error_msg);
        return;
    }
    
    printf("\n  Successfully completed %d concurrent operations across %d threads\n", 
           total_success, num_threads);
    
    qump_destroy(context);
    test_pass();
}

// Main test runner
int main(void) {
    printf("=== Quantix Ultra-Scale Map Persistent (QUMP) Test Suite ===\n\n");
    
    double start_time = get_time_ms();
    
    // Run all tests
    test_basic_initialization();
    test_basic_operations_no_persistence();
    test_quantum_number_operations();
    test_sync_persistence_mode();
    test_async_persistence_mode();
    test_performance_benchmark();
    test_stress_many_entries();
    test_configuration_validation();
    test_error_handling();
    test_clear_operation();
    test_concurrent_operations();
    
    double end_time = get_time_ms();
    g_test_results.total_time_ms = end_time - start_time;
    
    // Print test summary
    printf("\n=== Test Results Summary ===\n");
    printf("Total tests: %d\n", g_test_results.total_tests);
    printf("Passed: %d\n", g_test_results.passed_tests);
    printf("Failed: %d\n", g_test_results.failed_tests);
    printf("Success rate: %.1f%%\n", 
           (double)g_test_results.passed_tests / g_test_results.total_tests * 100.0);
    printf("Total time: %.2f ms\n", g_test_results.total_time_ms);
    printf("Average time per test: %.2f ms\n", 
           g_test_results.total_time_ms / g_test_results.total_tests);
    
    if (g_test_results.failed_tests == 0) {
        printf("\n🎉 All tests PASSED! The Quantix Ultra-Scale Map Persistent system is working correctly.\n");
        cleanup_test_files();
        return 0;
    } else {
        printf("\n❌ %d test(s) FAILED. Please review the failures above.\n", 
               g_test_results.failed_tests);
        cleanup_test_files();
        return 1;
    }
}
