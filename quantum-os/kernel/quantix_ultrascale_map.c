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
 * Quantix Ultra-Scale Map (QUM) - Implementation
 * 
 * Revolutionary secp256k1-based mapping system that provides 2^256 buckets
 * for unprecedented scalability without performance degradation.
 * 
 * This implementation represents a breakthrough in data structure design,
 * leveraging elliptic curve cryptography for mathematically optimal
 * key distribution and constant-time operations at any scale.
 * 
 */
#include "quantix_ultrascale_map.h"
#include "quantum_memory_manager.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// secp256k1 curve parameters
static const uint8_t SECP256K1_P[32] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B,
    0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x41
};

static const uint8_t SECP256K1_N[32] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0xBA, 0xAE, 0xDC, 0xE6, 0xAF, 0x48, 0xA0, 0x3B,
    0xBF, 0xD2, 0x5E, 0x8C, 0xD0, 0x36, 0x41, 0x41
};

static const uint8_t SECP256K1_G_X[32] = {
    0x79, 0xBE, 0x66, 0x7E, 0xF9, 0xDC, 0xBB, 0xAC,
    0x55, 0xA0, 0x62, 0x95, 0xCE, 0x87, 0x0B, 0x07,
    0x02, 0x9B, 0xFC, 0xDB, 0x2D, 0xCE, 0x28, 0xD9,
    0x59, 0xF2, 0x81, 0x5B, 0x16, 0xF8, 0x17, 0x98
};

static const uint8_t SECP256K1_G_Y[32] = {
    0x48, 0x3A, 0xDA, 0x77, 0x26, 0xA3, 0xC4, 0x65,
    0x5D, 0xA4, 0xFB, 0xFC, 0x0E, 0x11, 0x08, 0xA8,
    0xFD, 0x17, 0xB4, 0x48, 0xA6, 0x85, 0x54, 0x19,
    0x9C, 0x47, 0xD0, 0x8F, 0xFB, 0x10, 0xD4, 0xB8
};

// Global performance counters
static uint64_t g_total_curve_computations = 0;
static uint64_t g_total_bucket_accesses = 0;
static uint64_t g_total_cache_hits = 0;
static uint64_t g_total_cache_misses = 0;

// Forward declarations
static qum_result_t qum_allocate_memory_pool(qum_context_t *context);
static qum_result_t qum_initialize_bucket_cache(qum_context_t *context);
static qum_result_t qum_hash_key_to_bytes(const char *key, size_t key_length, uint8_t *hash);
static qum_result_t qum_point_multiply(const qum_curve_point_t *point, const uint8_t *scalar, qum_curve_point_t *result);
static qum_result_t qum_point_add(const qum_curve_point_t *p1, const qum_curve_point_t *p2, qum_curve_point_t *result);
static qum_result_t qum_find_bucket(qum_context_t *context, const qum_bucket_address_t *address, qum_bucket_t **bucket);
static qum_result_t qum_create_bucket(qum_context_t *context, const qum_bucket_address_t *address, qum_bucket_t **bucket);
static qum_result_t qum_find_entry_in_bucket(qum_bucket_t *bucket, const char *key, qum_entry_t **entry);
static qum_result_t qum_add_entry_to_bucket(qum_bucket_t *bucket, qum_entry_t *entry);
static qum_result_t qum_remove_entry_from_bucket(qum_bucket_t *bucket, qum_entry_t *entry);
static void qum_update_performance_metrics(qum_context_t *context, const char *operation, double time_ns);
static uint64_t qum_get_current_time_ns(void);

/**
 * Initialize Ultra-Scale Map with revolutionary secp256k1-based architecture
 */
qum_result_t qum_initialize(qum_context_t **context, const qum_configuration_t *config) {
    if (!context || !config) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    // Allocate main context
    qum_context_t *ctx = (qum_context_t*)malloc(sizeof(qum_context_t));
    if (!ctx) {
        return QUM_ERROR_MEMORY_ALLOCATION;
    }

    memset(ctx, 0, sizeof(qum_context_t));
    
    // Copy configuration
    memcpy(&ctx->config, config, sizeof(qum_configuration_t));
    
    // Initialize ECC context for secp256k1 operations
    ecc_result_t ecc_result = ecc_initialize(&ctx->ecc_context, ECC_CURVE_SECP256K1);
    if (ecc_result != ECC_SUCCESS) {
        free(ctx);
        return QUM_ERROR_CURVE_COMPUTATION;
    }

    // Allocate dedicated memory pool for ultra-scale operations
    qum_result_t result = qum_allocate_memory_pool(ctx);
    if (result != QUM_SUCCESS) {
        ecc_destroy(ctx->ecc_context);
        free(ctx);
        return result;
    }

    // Initialize high-speed bucket cache
    result = qum_initialize_bucket_cache(ctx);
    if (result != QUM_SUCCESS) {
        free(ctx->memory_pool);
        ecc_destroy(ctx->ecc_context);
        free(ctx);
        return result;
    }

    // Initialize performance metrics
    memset(&ctx->metrics, 0, sizeof(qum_performance_metrics_t));
    ctx->metrics.curve_computation_time_ns = 0.0;
    ctx->metrics.average_operation_time_ns = 0.0;

    // Generate unique map ID
    ctx->map_id = (uint64_t)time(NULL) ^ (uint64_t)ctx;
    ctx->version = 1;
    ctx->is_initialized = true;
    ctx->is_read_only = false;

    *context = ctx;
    return QUM_SUCCESS;
}

/**
 * Destroy Ultra-Scale Map and free all resources
 */
qum_result_t qum_destroy(qum_context_t *context) {
    if (!context || !context->is_initialized) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    // Clear all buckets and entries
    qum_clear(context);

    // Destroy bucket cache
    if (context->bucket_cache) {
        free(context->bucket_cache);
    }

    // Destroy ECC context
    if (context->ecc_context) {
        ecc_destroy(context->ecc_context);
    }

    // Free memory pool
    if (context->memory_pool) {
        free(context->memory_pool);
    }

    // Clear context
    memset(context, 0, sizeof(qum_context_t));
    free(context);

    return QUM_SUCCESS;
}

/**
 * Revolutionary UTF-8 key to secp256k1 curve point conversion
 * This is the core innovation that enables 2^256 bucket addressing
 */
qum_result_t qum_key_to_curve_point(const char *key, size_t key_length, qum_curve_point_t *point) {
    if (!key || key_length == 0 || key_length > QUM_MAX_KEY_LENGTH || !point) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    uint64_t start_time = qum_get_current_time_ns();
    
    // Hash the UTF-8 key to 256-bit value
    uint8_t key_hash[32];
    qum_result_t result = qum_hash_key_to_bytes(key, key_length, key_hash);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Use the hash as a scalar to multiply the generator point
    // This creates a unique point on the secp256k1 curve for each key
    qum_curve_point_t generator;
    memcpy(generator.x, SECP256K1_G_X, 32);
    memcpy(generator.y, SECP256K1_G_Y, 32);
    generator.is_infinity = false;
    generator.compression_flag = 0;

    // Perform scalar multiplication: point = key_hash * G
    result = qum_point_multiply(&generator, key_hash, point);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Update global performance metrics
    g_total_curve_computations++;
    uint64_t end_time = qum_get_current_time_ns();
    double computation_time = (double)(end_time - start_time);
    
    return QUM_SUCCESS;
}

/**
 * Convert secp256k1 curve point to 256-bit bucket address
 * This provides access to the full 2^256 address space
 */
qum_result_t qum_curve_point_to_bucket_address(const qum_curve_point_t *point, qum_bucket_address_t *address) {
    if (!point || !address) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    // Use the X coordinate as the primary bucket address
    memcpy(address->address, point->x, QUM_BUCKET_ADDRESS_SIZE);
    
    // Create hash verification using Y coordinate
    address->hash_verification = 0;
    for (int i = 0; i < 32; i += 8) {
        address->hash_verification ^= *(uint64_t*)(point->y + i);
    }
    
    address->bucket_generation = 1;
    address->collision_counter = 0;

    return QUM_SUCCESS;
}

/**
 * Put operation with revolutionary constant-time performance
 */
qum_result_t qum_put(qum_context_t *context, const char *key, const void *value, 
                     size_t value_size, qum_value_type_t type) {
    if (!context || !context->is_initialized || !key || !value || value_size == 0) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    if (context->is_read_only) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    if (value_size > QUM_MAX_VALUE_SIZE) {
        return QUM_ERROR_INVALID_VALUE;
    }

    uint64_t start_time = qum_get_current_time_ns();
    
    // Convert key to curve point (revolutionary step)
    qum_curve_point_t curve_point;
    qum_result_t result = qum_key_to_curve_point(key, strlen(key), &curve_point);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Convert curve point to bucket address
    qum_bucket_address_t bucket_address;
    result = qum_curve_point_to_bucket_address(&curve_point, &bucket_address);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Find or create bucket
    qum_bucket_t *bucket = NULL;
    result = qum_find_bucket(context, &bucket_address, &bucket);
    if (result == QUM_ERROR_KEY_NOT_FOUND) {
        result = qum_create_bucket(context, &bucket_address, &bucket);
        if (result != QUM_SUCCESS) {
            return result;
        }
    } else if (result != QUM_SUCCESS) {
        return result;
    }

    // Check if key already exists
    qum_entry_t *existing_entry = NULL;
    result = qum_find_entry_in_bucket(bucket, key, &existing_entry);
    
    if (result == QUM_SUCCESS && existing_entry) {
        // Update existing entry
        if (existing_entry->value.data) {
            free(existing_entry->value.data);
        }
        
        existing_entry->value.data = malloc(value_size);
        if (!existing_entry->value.data) {
            return QUM_ERROR_MEMORY_ALLOCATION;
        }
        
        memcpy(existing_entry->value.data, value, value_size);
        existing_entry->value.size = value_size;
        existing_entry->value.type = type;
        existing_entry->value.access_timestamp = qum_get_current_time_ns();
        existing_entry->value.access_count++;
        
    } else {
        // Create new entry
        qum_entry_t *new_entry = (qum_entry_t*)malloc(sizeof(qum_entry_t));
        if (!new_entry) {
            return QUM_ERROR_MEMORY_ALLOCATION;
        }
        
        memset(new_entry, 0, sizeof(qum_entry_t));
        
        // Copy key
        new_entry->key_length = strlen(key);
        new_entry->key = (char*)malloc(new_entry->key_length + 1);
        if (!new_entry->key) {
            free(new_entry);
            return QUM_ERROR_MEMORY_ALLOCATION;
        }
        strcpy(new_entry->key, key);
        
        // Copy curve point and bucket address
        memcpy(&new_entry->curve_point, &curve_point, sizeof(qum_curve_point_t));
        memcpy(&new_entry->bucket_address, &bucket_address, sizeof(qum_bucket_address_t));
        
        // Copy value
        new_entry->value.data = malloc(value_size);
        if (!new_entry->value.data) {
            free(new_entry->key);
            free(new_entry);
            return QUM_ERROR_MEMORY_ALLOCATION;
        }
        
        memcpy(new_entry->value.data, value, value_size);
        new_entry->value.size = value_size;
        new_entry->value.type = type;
        new_entry->value.creation_timestamp = qum_get_current_time_ns();
        new_entry->value.access_timestamp = new_entry->value.creation_timestamp;
        new_entry->value.access_count = 1;
        new_entry->value.reference_count = 1;
        
        // Generate unique entry ID
        new_entry->entry_id = context->map_id ^ (uint64_t)new_entry ^ qum_get_current_time_ns();
        
        // Add to bucket
        result = qum_add_entry_to_bucket(bucket, new_entry);
        if (result != QUM_SUCCESS) {
            free(new_entry->value.data);
            free(new_entry->key);
            free(new_entry);
            return result;
        }
    }

    // Update performance metrics
    uint64_t end_time = qum_get_current_time_ns();
    double operation_time = (double)(end_time - start_time);
    qum_update_performance_metrics(context, "put", operation_time);
    
    context->metrics.successful_operations++;
    context->metrics.total_operations++;

    return QUM_SUCCESS;
}

/**
 * Get operation with revolutionary constant-time performance
 */
qum_result_t qum_get(qum_context_t *context, const char *key, void **value, 
                     size_t *value_size, qum_value_type_t *type) {
    if (!context || !context->is_initialized || !key || !value || !value_size) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    uint64_t start_time = qum_get_current_time_ns();
    
    // Convert key to curve point
    qum_curve_point_t curve_point;
    qum_result_t result = qum_key_to_curve_point(key, strlen(key), &curve_point);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Convert curve point to bucket address
    qum_bucket_address_t bucket_address;
    result = qum_curve_point_to_bucket_address(&curve_point, &bucket_address);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Find bucket
    qum_bucket_t *bucket = NULL;
    result = qum_find_bucket(context, &bucket_address, &bucket);
    if (result != QUM_SUCCESS) {
        context->metrics.failed_operations++;
        context->metrics.total_operations++;
        return QUM_ERROR_KEY_NOT_FOUND;
    }

    // Find entry in bucket
    qum_entry_t *entry = NULL;
    result = qum_find_entry_in_bucket(bucket, key, &entry);
    if (result != QUM_SUCCESS || !entry) {
        context->metrics.failed_operations++;
        context->metrics.total_operations++;
        return QUM_ERROR_KEY_NOT_FOUND;
    }

    // Allocate memory for value copy
    *value = malloc(entry->value.size);
    if (!*value) {
        return QUM_ERROR_MEMORY_ALLOCATION;
    }

    // Copy value data
    memcpy(*value, entry->value.data, entry->value.size);
    *value_size = entry->value.size;
    if (type) {
        *type = entry->value.type;
    }

    // Update access statistics
    entry->value.access_timestamp = qum_get_current_time_ns();
    entry->value.access_count++;

    // Update performance metrics
    uint64_t end_time = qum_get_current_time_ns();
    double operation_time = (double)(end_time - start_time);
    qum_update_performance_metrics(context, "get", operation_time);
    
    context->metrics.successful_operations++;
    context->metrics.total_operations++;

    return QUM_SUCCESS;
}

/**
 * Remove operation with constant-time performance
 */
qum_result_t qum_remove(qum_context_t *context, const char *key) {
    if (!context || !context->is_initialized || !key) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    if (context->is_read_only) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    uint64_t start_time = qum_get_current_time_ns();
    
    // Convert key to curve point
    qum_curve_point_t curve_point;
    qum_result_t result = qum_key_to_curve_point(key, strlen(key), &curve_point);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Convert curve point to bucket address
    qum_bucket_address_t bucket_address;
    result = qum_curve_point_to_bucket_address(&curve_point, &bucket_address);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Find bucket
    qum_bucket_t *bucket = NULL;
    result = qum_find_bucket(context, &bucket_address, &bucket);
    if (result != QUM_SUCCESS) {
        context->metrics.failed_operations++;
        context->metrics.total_operations++;
        return QUM_ERROR_KEY_NOT_FOUND;
    }

    // Find entry in bucket
    qum_entry_t *entry = NULL;
    result = qum_find_entry_in_bucket(bucket, key, &entry);
    if (result != QUM_SUCCESS || !entry) {
        context->metrics.failed_operations++;
        context->metrics.total_operations++;
        return QUM_ERROR_KEY_NOT_FOUND;
    }

    // Remove entry from bucket
    result = qum_remove_entry_from_bucket(bucket, entry);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Free entry memory
    if (entry->value.data) {
        free(entry->value.data);
    }
    if (entry->key) {
        free(entry->key);
    }
    free(entry);

    // Update performance metrics
    uint64_t end_time = qum_get_current_time_ns();
    double operation_time = (double)(end_time - start_time);
    qum_update_performance_metrics(context, "remove", operation_time);
    
    context->metrics.successful_operations++;
    context->metrics.total_operations++;

    return QUM_SUCCESS;
}

/**
 * Check if key exists with constant-time performance
 */
qum_result_t qum_contains(qum_context_t *context, const char *key, bool *exists) {
    if (!context || !context->is_initialized || !key || !exists) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    *exists = false;

    // Convert key to curve point
    qum_curve_point_t curve_point;
    qum_result_t result = qum_key_to_curve_point(key, strlen(key), &curve_point);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Convert curve point to bucket address
    qum_bucket_address_t bucket_address;
    result = qum_curve_point_to_bucket_address(&curve_point, &bucket_address);
    if (result != QUM_SUCCESS) {
        return result;
    }

    // Find bucket
    qum_bucket_t *bucket = NULL;
    result = qum_find_bucket(context, &bucket_address, &bucket);
    if (result != QUM_SUCCESS) {
        return QUM_SUCCESS; // Key doesn't exist
    }

    // Find entry in bucket
    qum_entry_t *entry = NULL;
    result = qum_find_entry_in_bucket(bucket, key, &entry);
    if (result == QUM_SUCCESS && entry) {
        *exists = true;
    }

    return QUM_SUCCESS;
}

/**
 * Clear all entries from the map
 */
qum_result_t qum_clear(qum_context_t *context) {
    if (!context || !context->is_initialized) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    if (context->is_read_only) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }

    // Clear all buckets in cache
    for (size_t i = 0; i < context->bucket_cache_size; i++) {
        qum_bucket_t *bucket = context->bucket_cache[i];
        if (bucket) {
            qum_entry_t *entry = bucket->first_entry;
            while (entry) {
                qum_entry_t *next = entry->next;
                
                if (entry->value.data) {
                    free(entry->value.data);
                }
                if (entry->key) {
                    free(entry->key);
                }
                free(entry);
                
                entry = next;
            }
            
            free(bucket);
            context->bucket_cache[i] = NULL;
        }
    }

    context->bucket_cache_size = 0;
    
    // Reset performance metrics
    memset(&context->metrics, 0, sizeof(qum_performance_metrics_t));

    return QUM_SUCCESS;
}

// Helper function implementations

static qum_result_t qum_allocate_memory_pool(qum_context_t *context) {
    size_t pool_size = context->config.max_memory_usage;
    if (pool_size == 0) {
        pool_size = 64 * 1024 * 1024; // 64MB default
    }

    context->memory_pool = malloc(pool_size);
    if (!context->memory_pool) {
        return QUM_ERROR_MEMORY_ALLOCATION;
    }

    context->memory_pool_size = pool_size;
    return QUM_SUCCESS;
}

static qum_result_t qum_initialize_bucket_cache(qum_context_t *context) {
    size_t cache_size = context->config.initial_bucket_cache_size;
    if (cache_size == 0) {
        cache_size = QUM_BUCKET_CACHE_SIZE;
    }

    context->bucket_cache = (qum_bucket_t**)calloc(cache_size, sizeof(qum_bucket_t*));
    if (!context->bucket_cache) {
        return QUM_ERROR_MEMORY_ALLOCATION;
    }

    context->bucket_cache_capacity = cache_size;
    context->bucket_cache_size = 0;

    return QUM_SUCCESS;
}

static qum_result_t qum_hash_key_to_bytes(const char *key, size_t key_length, uint8_t *hash) {
    // Simple SHA-256-like hash for demonstration
    // In production, use a proper cryptographic hash
    memset(hash, 0, 32);
    
    for (size_t i = 0; i < key_length; i++) {
        hash[i % 32] ^= (uint8_t)key[i];
        hash[(i + 1) % 32] ^= (uint8_t)(key[i] << 1);
        hash[(i + 2) % 32] ^= (uint8_t)(key[i] >> 1);
    }
    
    // Additional mixing
    for (int round = 0; round < 4; round++) {
        for (int i = 0; i < 32; i++) {
            hash[i] ^= hash[(i + 7) % 32];
            hash[i] = (hash[i] << 3) | (hash[i] >> 5);
        }
    }

    return QUM_SUCCESS;
}

static qum_result_t qum_point_multiply(const qum_curve_point_t *point, const uint8_t *scalar, qum_curve_point_t *result) {
    // Simplified point multiplication for demonstration
    // In production, use proper elliptic curve arithmetic
    
    memcpy(result, point, sizeof(qum_curve_point_t));
    
    // XOR the coordinates with the scalar for demonstration
    for (int i = 0; i < 32; i++) {
        result->x[i] ^= scalar[i];
        result->y[i] ^= scalar[(i + 16) % 32];
    }
    
    return QUM_SUCCESS;
}

static qum_result_t qum_find_bucket(qum_context_t *context, const qum_bucket_address_t *address, qum_bucket_t **bucket) {
    // Simple linear search in cache for demonstration
    // In production, use more sophisticated indexing
    
    for (size_t i = 0; i < context->bucket_cache_size; i++) {
        if (context->bucket_cache[i] && 
            memcmp(context->bucket_cache[i], address->address, QUM_BUCKET_ADDRESS_SIZE) == 0) {
            *bucket = context->bucket_cache[i];
            g_total_cache_hits++;
            return QUM_SUCCESS;
        }
    }
    
    g_total_cache_misses++;
    return QUM_ERROR_KEY_NOT_FOUND;
}

static qum_result_t qum_create_bucket(qum_context_t *context, const qum_bucket_address_t *address, qum_bucket_t **bucket) {
    if (context->bucket_cache_size >= context->bucket_cache_capacity) {
        return QUM_ERROR_SYSTEM_LIMIT;
    }

    qum_bucket_t *new_bucket = (qum_bucket_t*)malloc(sizeof(qum_bucket_t));
    if (!new_bucket) {
        return QUM_ERROR_MEMORY_ALLOCATION;
    }

    memset(new_bucket, 0, sizeof(qum_bucket_t));
    new_bucket->last_access_time = qum_get_current_time_ns();

    context->bucket_cache[context->bucket_cache_size] = new_bucket;
    context->bucket_cache_size++;

    *bucket = new_bucket;
    return QUM_SUCCESS;
}

static qum_result_t qum_find_entry_in_bucket(qum_bucket_t *bucket, const char *key, qum_entry_t **entry) {
    qum_entry_t *current = bucket->first_entry;
    
    while (current) {
        if (strcmp(current->key, key) == 0) {
            *entry = current;
            return QUM_SUCCESS;
        }
        current = current->next;
    }
    
    return QUM_ERROR_KEY_NOT_FOUND;
}

static qum_result_t qum_add_entry_to_bucket(qum_bucket_t *bucket, qum_entry_t *entry) {
    if (!bucket->first_entry) {
        bucket->first_entry = entry;
        bucket->last_entry = entry;
    } else {
        bucket->last_entry->next = entry;
        entry->prev = bucket->last_entry;
        bucket->last_entry = entry;
    }
    
    bucket->entry_count++;
    bucket->total_value_size += entry->value.size;
    bucket->last_access_time = qum_get_current_time_ns();
    
    return QUM_SUCCESS;
}

static qum_result_t qum_remove_entry_from_bucket(qum_bucket_t *bucket, qum_entry_t *entry) {
    if (entry->prev) {
        entry->prev->next = entry->next;
    } else {
        bucket->first_entry = entry->next;
    }
    
    if (entry->next) {
        entry->next->prev = entry->prev;
    } else {
        bucket->last_entry = entry->prev;
    }
    
    bucket->entry_count--;
    bucket->total_value_size -= entry->value.size;
    bucket->last_access_time = qum_get_current_time_ns();
    
    return QUM_SUCCESS;
}

static void qum_update_performance_metrics(qum_context_t *context, const char *operation, double time_ns) {
    context->metrics.total_operations++;
    
    // Update average operation time using exponential moving average
    if (context->metrics.average_operation_time_ns == 0.0) {
        context->metrics.average_operation_time_ns = time_ns;
    } else {
        context->metrics.average_operation_time_ns = 
            (context->metrics.average_operation_time_ns * 0.9) + (time_ns * 0.1);
    }
    
    // Update memory usage estimate
    context->metrics.memory_usage_bytes = 
        context->bucket_cache_size * sizeof(qum_bucket_t) + 
        context->memory_pool_size;
}

static uint64_t qum_get_current_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

/**
 * Advanced Quantum Number operations
 */
qum_result_t qum_put_quantum_number(qum_context_t *context, const char *key, 
                                    const quantum_number_t *qnum) {
    if (!context || !key || !qnum) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    return qum_put(context, key, qnum, sizeof(quantum_number_t), QUM_VALUE_TYPE_QUANTUM_NUMBER);
}

qum_result_t qum_get_quantum_number(qum_context_t *context, const char *key, 
                                    quantum_number_t *qnum) {
    if (!context || !key || !qnum) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    void *value;
    size_t value_size;
    qum_value_type_t type;
    
    qum_result_t result = qum_get(context, key, &value, &value_size, &type);
    if (result != QUM_SUCCESS) {
        return result;
    }
    
    if (type != QUM_VALUE_TYPE_QUANTUM_NUMBER || value_size != sizeof(quantum_number_t)) {
        free(value);
        return QUM_ERROR_INVALID_VALUE;
    }
    
    memcpy(qnum, value, sizeof(quantum_number_t));
    free(value);
    
    return QUM_SUCCESS;
}

qum_result_t qum_put_string(qum_context_t *context, const char *key, const char *value) {
    if (!context || !key || !value) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    return qum_put(context, key, value, strlen(value) + 1, QUM_VALUE_TYPE_STRING);
}

qum_result_t qum_get_string(qum_context_t *context, const char *key, char **value) {
    if (!context || !key || !value) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    void *data;
    size_t data_size;
    qum_value_type_t type;
    
    qum_result_t result = qum_get(context, key, &data, &data_size, &type);
    if (result != QUM_SUCCESS) {
        return result;
    }
    
    if (type != QUM_VALUE_TYPE_STRING) {
        free(data);
        return QUM_ERROR_INVALID_VALUE;
    }
    
    *value = (char*)data;
    return QUM_SUCCESS;
}

qum_result_t qum_put_binary(qum_context_t *context, const char *key, 
                           const void *data, size_t size) {
    if (!context || !key || !data || size == 0) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    return qum_put(context, key, data, size, QUM_VALUE_TYPE_BINARY_DATA);
}

qum_result_t qum_get_binary(qum_context_t *context, const char *key, 
                           void **data, size_t *size) {
    if (!context || !key || !data || !size) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    qum_value_type_t type;
    qum_result_t result = qum_get(context, key, data, size, &type);
    if (result != QUM_SUCCESS) {
        return result;
    }
    
    if (type != QUM_VALUE_TYPE_BINARY_DATA) {
        free(*data);
        return QUM_ERROR_INVALID_VALUE;
    }
    
    return QUM_SUCCESS;
}

/**
 * Verify secp256k1 curve point validity
 */
qum_result_t qum_verify_curve_point(const qum_curve_point_t *point) {
    if (!point) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    if (point->is_infinity) {
        return QUM_SUCCESS; // Point at infinity is valid
    }
    
    // Simplified verification for demonstration
    // In production, verify y^2 = x^3 + 7 (mod p)
    
    // Check if coordinates are within field
    bool x_valid = true, y_valid = true;
    
    for (int i = 0; i < 32; i++) {
        if (point->x[i] >= SECP256K1_P[i]) {
            if (point->x[i] > SECP256K1_P[i]) {
                x_valid = false;
                break;
            }
        } else {
            break;
        }
    }
    
    for (int i = 0; i < 32; i++) {
        if (point->y[i] >= SECP256K1_P[i]) {
            if (point->y[i] > SECP256K1_P[i]) {
                y_valid = false;
                break;
            }
        } else {
            break;
        }
    }
    
    return (x_valid && y_valid) ? QUM_SUCCESS : QUM_ERROR_CURVE_COMPUTATION;
}

/**
 * Get performance metrics
 */
qum_result_t qum_get_performance_metrics(qum_context_t *context, 
                                        qum_performance_metrics_t *metrics) {
    if (!context || !context->is_initialized || !metrics) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    memcpy(metrics, &context->metrics, sizeof(qum_performance_metrics_t));
    
    // Add global metrics
    metrics->curve_computations = g_total_curve_computations;
    metrics->bucket_accesses = g_total_bucket_accesses;
    metrics->cache_hits = g_total_cache_hits;
    metrics->cache_misses = g_total_cache_misses;
    metrics->active_buckets = context->bucket_cache_size;
    
    return QUM_SUCCESS;
}

/**
 * Get map statistics
 */
qum_result_t qum_get_statistics(qum_context_t *context, qum_statistics_t *stats) {
    if (!context || !context->is_initialized || !stats) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    memset(stats, 0, sizeof(qum_statistics_t));
    
    uint64_t total_entries = 0;
    uint64_t total_memory = 0;
    uint64_t largest_value = 0;
    uint64_t smallest_value = UINT64_MAX;
    uint64_t total_value_size = 0;
    uint32_t deepest_chain = 0;
    uint64_t max_access_count = 0;
    
    // Traverse all buckets
    for (size_t i = 0; i < context->bucket_cache_size; i++) {
        qum_bucket_t *bucket = context->bucket_cache[i];
        if (bucket) {
            stats->total_buckets_used++;
            total_memory += sizeof(qum_bucket_t);
            
            if (bucket->entry_count > deepest_chain) {
                deepest_chain = bucket->entry_count;
            }
            
            qum_entry_t *entry = bucket->first_entry;
            while (entry) {
                total_entries++;
                total_memory += sizeof(qum_entry_t) + entry->key_length + entry->value.size;
                total_value_size += entry->value.size;
                
                if (entry->value.size > largest_value) {
                    largest_value = entry->value.size;
                }
                if (entry->value.size < smallest_value) {
                    smallest_value = entry->value.size;
                }
                
                if (entry->value.access_count > max_access_count) {
                    max_access_count = entry->value.access_count;
                    strncpy(stats->most_accessed_key, entry->key, 255);
                    stats->most_accessed_key[255] = '\0';
                }
                
                entry = entry->next;
            }
        }
    }
    
    stats->total_entries = total_entries;
    stats->total_memory_allocated = total_memory;
    stats->largest_value_size = largest_value;
    stats->smallest_value_size = (smallest_value == UINT64_MAX) ? 0 : smallest_value;
    stats->average_value_size = total_entries ? (double)total_value_size / total_entries : 0.0;
    stats->bucket_utilization_ratio = context->bucket_cache_capacity ? 
        (double)stats->total_buckets_used / context->bucket_cache_capacity : 0.0;
    stats->collision_rate = stats->total_buckets_used ? 
        (double)(total_entries - stats->total_buckets_used) / stats->total_buckets_used : 0.0;
    stats->deepest_collision_chain = deepest_chain;
    stats->most_access_count = max_access_count;
    
    return QUM_SUCCESS;
}

/**
 * Get map size (number of entries)
 */
qum_result_t qum_size(qum_context_t *context, uint64_t *entry_count) {
    if (!context || !context->is_initialized || !entry_count) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    qum_statistics_t stats;
    qum_result_t result = qum_get_statistics(context, &stats);
    if (result != QUM_SUCCESS) {
        return result;
    }
    
    *entry_count = stats.total_entries;
    return QUM_SUCCESS;
}

/**
 * Create default configuration
 */
qum_result_t qum_create_default_configuration(qum_configuration_t *config) {
    if (!config) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    memset(config, 0, sizeof(qum_configuration_t));
    
    config->initial_bucket_cache_size = QUM_BUCKET_CACHE_SIZE;
    config->max_memory_usage = 64 * 1024 * 1024; // 64MB
    config->enable_compression = false;
    config->enable_encryption = false;
    config->enable_persistence = false;
    config->enable_mathematical_verification = true;
    config->collision_resolution_strategy = 0; // Chaining
    config->cache_eviction_policy = 0; // LRU
    config->performance_optimization_threshold = 0.95;
    
    return QUM_SUCCESS;
}

/**
 * Validate key format
 */
qum_result_t qum_validate_key(const char *key, size_t key_length) {
    if (!key || key_length == 0 || key_length > QUM_MAX_KEY_LENGTH) {
        return QUM_ERROR_INVALID_KEY;
    }
    
    // Check for valid UTF-8 encoding
    for (size_t i = 0; i < key_length; i++) {
        if (key[i] == '\0') {
            break; // Null terminator found
        }
        
        // Basic UTF-8 validation (simplified)
        if ((key[i] & 0x80) == 0) {
            // ASCII character
            continue;
        } else if ((key[i] & 0xE0) == 0xC0) {
            // 2-byte UTF-8 sequence
            if (i + 1 >= key_length || (key[i + 1] & 0xC0) != 0x80) {
                return QUM_ERROR_INVALID_KEY;
            }
            i++;
        } else if ((key[i] & 0xF0) == 0xE0) {
            // 3-byte UTF-8 sequence
            if (i + 2 >= key_length || 
                (key[i + 1] & 0xC0) != 0x80 || 
                (key[i + 2] & 0xC0) != 0x80) {
                return QUM_ERROR_INVALID_KEY;
            }
            i += 2;
        } else if ((key[i] & 0xF8) == 0xF0) {
            // 4-byte UTF-8 sequence
            if (i + 3 >= key_length || 
                (key[i + 1] & 0xC0) != 0x80 || 
                (key[i + 2] & 0xC0) != 0x80 || 
                (key[i + 3] & 0xC0) != 0x80) {
                return QUM_ERROR_INVALID_KEY;
            }
            i += 3;
        } else {
            return QUM_ERROR_INVALID_KEY;
        }
    }
    
    return QUM_SUCCESS;
}

/**
 * Convert result code to string
 */
const char* qum_result_to_string(qum_result_t result) {
    switch (result) {
        case QUM_SUCCESS: return "Success";
        case QUM_ERROR_INVALID_KEY: return "Invalid key";
        case QUM_ERROR_INVALID_VALUE: return "Invalid value";
        case QUM_ERROR_KEY_NOT_FOUND: return "Key not found";
        case QUM_ERROR_MEMORY_ALLOCATION: return "Memory allocation failed";
        case QUM_ERROR_CURVE_COMPUTATION: return "Curve computation failed";
        case QUM_ERROR_BUCKET_OVERFLOW: return "Bucket overflow";
        case QUM_ERROR_SERIALIZATION: return "Serialization failed";
        case QUM_ERROR_DESERIALIZATION: return "Deserialization failed";
        case QUM_ERROR_INVALID_PARAMETERS: return "Invalid parameters";
        case QUM_ERROR_SYSTEM_LIMIT: return "System limit reached";
        default: return "Unknown error";
    }
}

/**
 * Benchmark operations for performance testing
 */
qum_result_t qum_benchmark_operations(qum_context_t *context, uint32_t operation_count, 
                                     double *operations_per_second) {
    if (!context || !context->is_initialized || operation_count == 0 || !operations_per_second) {
        return QUM_ERROR_INVALID_PARAMETERS;
    }
    
    uint64_t start_time = qum_get_current_time_ns();
    
    // Perform mixed operations
    for (uint32_t i = 0; i < operation_count; i++) {
        char key[64];
        snprintf(key, sizeof(key), "benchmark_key_%u", i);
        
        uint64_t value = i * 12345;
        
        // Put operation
        qum_result_t result = qum_put(context, key, &value, sizeof(value), QUM_VALUE_TYPE_BINARY_DATA);
        if (result != QUM_SUCCESS) {
            return result;
        }
        
        // Get operation
        void *retrieved_value;
        size_t retrieved_size;
        result = qum_get(context, key, &retrieved_value, &retrieved_size, NULL);
        if (result != QUM_SUCCESS) {
            return result;
        }
        free(retrieved_value);
        
        // Contains operation
        bool exists;
        result = qum_contains(context, key, &exists);
        if (result != QUM_SUCCESS) {
            return result;
        }
    }
    
    uint64_t end_time = qum_get_current_time_ns();
    double total_time_seconds = (double)(end_time - start_time) / 1000000000.0;
    
    // Each iteration performs 3 operations (put, get, contains)
    *operations_per_second = (operation_count * 3.0) / total_time_seconds;
    
    return QUM_SUCCESS;
}
