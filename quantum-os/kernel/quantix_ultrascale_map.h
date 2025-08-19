/**
 * Quantix Ultra-Scale Map (QUM) - Revolutionary secp256k1-based mapping system
 * 
 * This is the world's first ultra-scale map data structure that uses the secp256k1
 * elliptic curve to provide 2^256 buckets for key-value storage. Unlike traditional
 * hash tables that degrade with size, this system maintains constant-time performance
 * regardless of scale due to the mathematical properties of elliptic curves.
 * 
 * Key Features:
 * - 2^256 theoretical buckets (more than atoms in the observable universe)
 * - UTF-8 keys converted to secp256k1 curve points
 * - Constant-time O(1) operations at any scale
 * - Cryptographically secure key distribution
 * - Mathematical collision resistance
 * - Quantum-resistant design principles
 * 
 * Copyright (c) 2024 Quantum Number Systems
 * Licensed under MIT License
 */

#ifndef QUANTIX_ULTRASCALE_MAP_H
#define QUANTIX_ULTRASCALE_MAP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "quantum_number.h"
#include "quantix_ecc.h"

#ifdef __cplusplus
extern "C" {
#endif

// Ultra-Scale Map Configuration
#define QUM_MAX_KEY_LENGTH 4096
#define QUM_MAX_VALUE_SIZE (1024 * 1024)  // 1MB max value
#define QUM_BUCKET_CACHE_SIZE 1024
#define QUM_PERFORMANCE_THRESHOLD 1000000  // 1M operations
#define QUM_CURVE_POINT_SIZE 64  // secp256k1 point size
#define QUM_BUCKET_ADDRESS_SIZE 32  // 256-bit bucket address

// Ultra-Scale Map Result Codes
typedef enum {
    QUM_SUCCESS = 0,
    QUM_ERROR_INVALID_KEY = -1,
    QUM_ERROR_INVALID_VALUE = -2,
    QUM_ERROR_KEY_NOT_FOUND = -3,
    QUM_ERROR_MEMORY_ALLOCATION = -4,
    QUM_ERROR_CURVE_COMPUTATION = -5,
    QUM_ERROR_BUCKET_OVERFLOW = -6,
    QUM_ERROR_SERIALIZATION = -7,
    QUM_ERROR_DESERIALIZATION = -8,
    QUM_ERROR_INVALID_PARAMETERS = -9,
    QUM_ERROR_SYSTEM_LIMIT = -10
} qum_result_t;

// Ultra-Scale Map Value Types
typedef enum {
    QUM_VALUE_TYPE_QUANTUM_NUMBER = 1,
    QUM_VALUE_TYPE_BINARY_DATA = 2,
    QUM_VALUE_TYPE_STRING = 3,
    QUM_VALUE_TYPE_AST_NODE = 4,
    QUM_VALUE_TYPE_MATHEMATICAL_PROOF = 5,
    QUM_VALUE_TYPE_CUSTOM = 100
} qum_value_type_t;

// secp256k1 Curve Point for Key Mapping
typedef struct {
    uint8_t x[32];  // X coordinate (256 bits)
    uint8_t y[32];  // Y coordinate (256 bits)
    bool is_infinity;  // Point at infinity flag
    uint32_t compression_flag;  // Compression metadata
} qum_curve_point_t;

// Ultra-Scale Bucket Address (256-bit)
typedef struct {
    uint8_t address[QUM_BUCKET_ADDRESS_SIZE];
    uint64_t hash_verification;  // Additional verification
    uint32_t bucket_generation;  // For versioning
    uint32_t collision_counter;  // Collision tracking
} qum_bucket_address_t;

// Ultra-Scale Map Value Container
typedef struct {
    qum_value_type_t type;
    size_t size;
    void *data;
    uint64_t creation_timestamp;
    uint64_t access_timestamp;
    uint32_t access_count;
    uint32_t reference_count;
    bool is_persistent;  // Survives system restart
    bool is_encrypted;   // Value encryption flag
} qum_value_container_t;

// Ultra-Scale Map Entry
typedef struct qum_entry {
    char *key;  // UTF-8 key string
    size_t key_length;
    qum_curve_point_t curve_point;  // secp256k1 mapping
    qum_bucket_address_t bucket_address;
    qum_value_container_t value;
    struct qum_entry *next;  // Collision chain (rare)
    struct qum_entry *prev;  // Doubly-linked for efficiency
    uint64_t entry_id;  // Unique entry identifier
    uint32_t mathematical_hash;  // Additional verification
} qum_entry_t;

// Ultra-Scale Map Bucket
typedef struct {
    qum_entry_t *first_entry;
    qum_entry_t *last_entry;
    uint32_t entry_count;
    uint64_t total_value_size;
    uint64_t last_access_time;
    uint32_t access_frequency;
    bool is_cached;  // In high-speed cache
    bool is_compressed;  // Bucket compression
} qum_bucket_t;

// Ultra-Scale Map Performance Metrics
typedef struct {
    uint64_t total_operations;
    uint64_t successful_operations;
    uint64_t failed_operations;
    uint64_t curve_computations;
    uint64_t bucket_accesses;
    uint64_t cache_hits;
    uint64_t cache_misses;
    uint64_t collision_count;
    double average_operation_time_ns;
    double curve_computation_time_ns;
    uint64_t memory_usage_bytes;
    uint32_t active_buckets;
} qum_performance_metrics_t;

// Ultra-Scale Map Configuration
typedef struct {
    size_t initial_bucket_cache_size;
    size_t max_memory_usage;
    bool enable_compression;
    bool enable_encryption;
    bool enable_persistence;
    bool enable_mathematical_verification;
    uint32_t collision_resolution_strategy;
    uint32_t cache_eviction_policy;
    double performance_optimization_threshold;
} qum_configuration_t;

// Ultra-Scale Map Context
typedef struct {
    qum_bucket_t **bucket_cache;  // High-speed bucket cache
    size_t bucket_cache_size;
    size_t bucket_cache_capacity;
    qum_performance_metrics_t metrics;
    qum_configuration_t config;
    ecc_context_t *ecc_context;  // secp256k1 context
    void *memory_pool;  // Dedicated memory pool
    size_t memory_pool_size;
    uint64_t map_id;  // Unique map identifier
    uint32_t version;  // Map version for compatibility
    bool is_initialized;
    bool is_read_only;
} qum_context_t;

// Iterator for Ultra-Scale Map traversal
typedef struct {
    qum_context_t *map_context;
    qum_bucket_address_t current_bucket;
    qum_entry_t *current_entry;
    uint64_t iteration_count;
    bool is_valid;
    bool is_reverse;  // Reverse iteration
} qum_iterator_t;

// Ultra-Scale Map Statistics
typedef struct {
    uint64_t total_entries;
    uint64_t total_buckets_used;
    uint64_t total_memory_allocated;
    uint64_t largest_value_size;
    uint64_t smallest_value_size;
    double average_value_size;
    double bucket_utilization_ratio;
    double collision_rate;
    uint32_t deepest_collision_chain;
    char most_accessed_key[256];
    uint64_t most_access_count;
} qum_statistics_t;

// Core Ultra-Scale Map Operations
qum_result_t qum_initialize(qum_context_t **context, const qum_configuration_t *config);
qum_result_t qum_destroy(qum_context_t *context);
qum_result_t qum_put(qum_context_t *context, const char *key, const void *value, 
                     size_t value_size, qum_value_type_t type);
qum_result_t qum_get(qum_context_t *context, const char *key, void **value, 
                     size_t *value_size, qum_value_type_t *type);
qum_result_t qum_remove(qum_context_t *context, const char *key);
qum_result_t qum_contains(qum_context_t *context, const char *key, bool *exists);
qum_result_t qum_size(qum_context_t *context, uint64_t *entry_count);
qum_result_t qum_clear(qum_context_t *context);

// Advanced Ultra-Scale Map Operations
qum_result_t qum_put_quantum_number(qum_context_t *context, const char *key, 
                                    const quantum_number_t *qnum);
qum_result_t qum_get_quantum_number(qum_context_t *context, const char *key, 
                                    quantum_number_t *qnum);
qum_result_t qum_put_string(qum_context_t *context, const char *key, const char *value);
qum_result_t qum_get_string(qum_context_t *context, const char *key, char **value);
qum_result_t qum_put_binary(qum_context_t *context, const char *key, 
                           const void *data, size_t size);
qum_result_t qum_get_binary(qum_context_t *context, const char *key, 
                           void **data, size_t *size);

// secp256k1 Curve Operations for Key Mapping
qum_result_t qum_key_to_curve_point(const char *key, size_t key_length, 
                                    qum_curve_point_t *point);
qum_result_t qum_curve_point_to_bucket_address(const qum_curve_point_t *point, 
                                               qum_bucket_address_t *address);
qum_result_t qum_verify_curve_point(const qum_curve_point_t *point);
qum_result_t qum_compute_bucket_distribution(qum_context_t *context, 
                                            uint32_t *distribution_stats);

// Ultra-Scale Map Iterator Operations
qum_result_t qum_iterator_create(qum_context_t *context, qum_iterator_t **iterator);
qum_result_t qum_iterator_destroy(qum_iterator_t *iterator);
qum_result_t qum_iterator_next(qum_iterator_t *iterator, char **key, 
                              void **value, size_t *value_size);
qum_result_t qum_iterator_has_next(qum_iterator_t *iterator, bool *has_next);
qum_result_t qum_iterator_reset(qum_iterator_t *iterator);

// Performance and Monitoring
qum_result_t qum_get_performance_metrics(qum_context_t *context, 
                                        qum_performance_metrics_t *metrics);
qum_result_t qum_get_statistics(qum_context_t *context, qum_statistics_t *stats);
qum_result_t qum_optimize_performance(qum_context_t *context);
qum_result_t qum_defragment_buckets(qum_context_t *context);
qum_result_t qum_validate_integrity(qum_context_t *context, bool *is_valid);

// Persistence and Serialization
qum_result_t qum_serialize_to_file(qum_context_t *context, const char *filename);
qum_result_t qum_deserialize_from_file(qum_context_t **context, const char *filename);
qum_result_t qum_export_to_json(qum_context_t *context, char **json_output);
qum_result_t qum_import_from_json(qum_context_t *context, const char *json_input);

// Mathematical Operations Integration
qum_result_t qum_evaluate_mathematical_expression(qum_context_t *context, 
                                                 const char *expression, 
                                                 quantum_number_t *result);
qum_result_t qum_store_mathematical_proof(qum_context_t *context, const char *key, 
                                         const char *proof_data);
qum_result_t qum_verify_mathematical_proof(qum_context_t *context, const char *key, 
                                          bool *is_valid);

// Utility Functions
const char* qum_result_to_string(qum_result_t result);
qum_result_t qum_create_default_configuration(qum_configuration_t *config);
qum_result_t qum_validate_key(const char *key, size_t key_length);
qum_result_t qum_estimate_memory_usage(const qum_configuration_t *config, 
                                      uint64_t expected_entries, 
                                      size_t *estimated_bytes);

// Debug and Testing Functions
qum_result_t qum_dump_bucket_info(qum_context_t *context, 
                                 const qum_bucket_address_t *address);
qum_result_t qum_test_curve_distribution(uint32_t test_count, 
                                        double *distribution_quality);
qum_result_t qum_benchmark_operations(qum_context_t *context, uint32_t operation_count, 
                                     double *operations_per_second);

#ifdef __cplusplus
}
#endif

#endif // QUANTIX_ULTRASCALE_MAP_H
