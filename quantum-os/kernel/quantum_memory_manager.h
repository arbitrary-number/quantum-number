/**
 * Quantum Memory Manager - Advanced Mathematical Memory Management
 * 
 * Enhanced memory management system with native support for Quantum Numbers,
 * symbolic computation, and mathematical object relationships.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_MEMORY_MANAGER_H
#define QUANTUM_MEMORY_MANAGER_H

#include "quantum_number.h"
#include "memory.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Mathematical memory types
typedef enum {
    QMM_TYPE_QUANTUM_NUMBER = 0x01,
    QMM_TYPE_SYMBOLIC_EXPRESSION = 0x02,
    QMM_TYPE_AST_NODE = 0x04,
    QMM_TYPE_MATHEMATICAL_MATRIX = 0x08,
    QMM_TYPE_COMPUTATIONAL_CONTEXT = 0x10,
    QMM_TYPE_DEFERRED_EVALUATION = 0x20,
    QMM_TYPE_MATHEMATICAL_PROOF = 0x40,
    QMM_TYPE_RELATIONSHIP_GRAPH = 0x80
} qmm_mathematical_type_t;

// Memory allocation flags
typedef enum {
    QMM_FLAG_ZERO_INIT = 0x01,          // Zero-initialize memory
    QMM_FLAG_MATHEMATICAL = 0x02,        // Mathematical object alignment
    QMM_FLAG_SYMBOLIC = 0x04,            // Symbolic computation optimized
    QMM_FLAG_PERSISTENT = 0x08,          // Persistent across computations
    QMM_FLAG_CACHEABLE = 0x10,           // Can be cached for performance
    QMM_FLAG_ATOMIC = 0x20,              // Atomic mathematical operations
    QMM_FLAG_IMMUTABLE = 0x40,           // Immutable mathematical object
    QMM_FLAG_SHARED = 0x80               // Shared between processes
} qmm_allocation_flags_t;

// Mathematical memory alignment requirements
#define QMM_ALIGN_QUANTUM_NUMBER    32   // 256-bit alignment
#define QMM_ALIGN_SYMBOLIC          16   // 128-bit alignment
#define QMM_ALIGN_AST_NODE          8    // 64-bit alignment
#define QMM_ALIGN_DEFAULT           4    // 32-bit alignment

// Mathematical memory pool structure
typedef struct qmm_memory_pool {
    void *base_address;                  // Pool base address
    size_t total_size;                   // Total pool size
    size_t used_size;                    // Currently used size
    size_t free_size;                    // Available free size
    
    // Mathematical properties
    qmm_mathematical_type_t primary_type; // Primary mathematical type
    uint32_t alignment_requirement;       // Memory alignment requirement
    uint32_t allocation_flags;            // Pool-specific flags
    
    // Pool management
    struct qmm_memory_block *first_block; // First memory block
    struct qmm_memory_block *last_block;  // Last memory block
    uint32_t block_count;                 // Number of blocks
    
    // Performance tracking
    uint64_t allocation_count;            // Total allocations
    uint64_t deallocation_count;          // Total deallocations
    uint64_t fragmentation_level;         // Fragmentation metric
    uint64_t last_compaction;             // Last compaction time
    
    // Mathematical optimization
    void *mathematical_cache;             // Mathematical computation cache
    uint32_t cache_hit_rate;              // Cache performance metric
    uint32_t optimization_level;          // Pool optimization level
    
    // Linked list for pool management
    struct qmm_memory_pool *next_pool;
    struct qmm_memory_pool *prev_pool;
} qmm_memory_pool_t;

// Mathematical memory block structure
typedef struct qmm_memory_block {
    void *address;                        // Block address
    size_t size;                          // Block size
    bool is_free;                         // Free/allocated status
    
    // Mathematical metadata
    qmm_mathematical_type_t type;         // Mathematical object type
    quantum_number_t mathematical_id;     // Unique mathematical identifier
    uint32_t reference_count;             // Reference counting for GC
    uint32_t mathematical_properties;     // Mathematical property flags
    
    // Relationship tracking
    struct qmm_memory_block **dependencies; // Mathematical dependencies
    uint32_t dependency_count;            // Number of dependencies
    struct qmm_memory_block **dependents; // Objects depending on this
    uint32_t dependent_count;             // Number of dependents
    
    // Computational metadata
    uint64_t creation_time;               // Block creation time
    uint64_t last_access;                 // Last access time
    uint32_t access_count;                // Access frequency
    uint32_t computation_complexity;      // Computational complexity
    
    // Memory management
    struct qmm_memory_block *next_block;  // Next block in pool
    struct qmm_memory_block *prev_block;  // Previous block in pool
    struct qmm_memory_block *next_free;   // Next free block
    struct qmm_memory_block *prev_free;   // Previous free block
    
    // Integrity and verification
    quantum_number_t integrity_checksum;  // Mathematical integrity check
    uint32_t verification_status;         // Verification status flags
} qmm_memory_block_t;

// Mathematical garbage collection structure
typedef struct qmm_gc_context {
    // GC configuration
    bool gc_enabled;                      // Garbage collection enabled
    uint32_t gc_threshold;                // GC trigger threshold
    uint32_t gc_frequency;                // GC frequency in ticks
    uint64_t last_gc_run;                 // Last GC execution time
    
    // Mathematical GC parameters
    uint32_t mathematical_depth_limit;    // Max mathematical dependency depth
    uint32_t symbolic_complexity_limit;   // Max symbolic complexity for GC
    bool preserve_mathematical_proofs;    // Preserve mathematical proofs
    bool preserve_computational_contexts; // Preserve computation contexts
    
    // GC statistics
    uint64_t total_gc_runs;               // Total GC executions
    uint64_t objects_collected;           // Total objects collected
    uint64_t memory_reclaimed;            // Total memory reclaimed
    uint64_t average_gc_time;             // Average GC execution time
    
    // Mathematical relationship tracking
    struct qmm_memory_block **root_objects; // GC root objects
    uint32_t root_object_count;           // Number of root objects
    uint32_t *reference_graph;            // Mathematical reference graph
    uint32_t graph_size;                  // Reference graph size
} qmm_gc_context_t;

// Mathematical memory statistics
typedef struct qmm_statistics {
    // Memory usage statistics
    size_t total_memory;                  // Total managed memory
    size_t used_memory;                   // Currently used memory
    size_t free_memory;                   // Available free memory
    size_t fragmented_memory;             // Fragmented memory
    
    // Pool statistics
    uint32_t total_pools;                 // Total memory pools
    uint32_t active_pools;                // Active memory pools
    uint32_t quantum_number_pools;        // Quantum Number pools
    uint32_t symbolic_pools;              // Symbolic expression pools
    uint32_t ast_pools;                   // AST node pools
    
    // Allocation statistics
    uint64_t total_allocations;           // Total allocations
    uint64_t total_deallocations;         // Total deallocations
    uint64_t current_allocations;         // Current active allocations
    uint64_t peak_allocations;            // Peak allocation count
    
    // Mathematical object statistics
    uint32_t quantum_number_objects;      // Active Quantum Number objects
    uint32_t symbolic_expression_objects; // Active symbolic expressions
    uint32_t ast_node_objects;            // Active AST nodes
    uint32_t mathematical_proof_objects;  // Active mathematical proofs
    
    // Performance statistics
    uint64_t allocation_time_total;       // Total allocation time
    uint64_t deallocation_time_total;     // Total deallocation time
    uint64_t gc_time_total;               // Total GC time
    uint32_t cache_hit_rate;              // Overall cache hit rate
    
    // Mathematical computation statistics
    uint64_t mathematical_operations;     // Mathematical operations performed
    uint64_t symbolic_computations;       // Symbolic computations performed
    uint64_t ast_evaluations;             // AST evaluations performed
    uint64_t proof_verifications;         // Proof verifications performed
} qmm_statistics_t;

// QMM result codes
typedef enum {
    QMM_SUCCESS = 0,
    QMM_ERROR_OUT_OF_MEMORY = -1,
    QMM_ERROR_INVALID_PARAMETER = -2,
    QMM_ERROR_INVALID_TYPE = -3,
    QMM_ERROR_ALIGNMENT_ERROR = -4,
    QMM_ERROR_MATHEMATICAL_INCONSISTENCY = -5,
    QMM_ERROR_DEPENDENCY_CYCLE = -6,
    QMM_ERROR_INTEGRITY_VIOLATION = -7,
    QMM_ERROR_GC_FAILURE = -8,
    QMM_ERROR_POOL_EXHAUSTED = -9,
    QMM_ERROR_FRAGMENTATION_CRITICAL = -10
} qmm_result_t;

// Core QMM API Functions

// System initialization and management
qmm_result_t qmm_initialize(size_t total_memory_size);
qmm_result_t qmm_shutdown(void);
qmm_result_t qmm_configure_gc(qmm_gc_context_t *gc_config);

// Memory pool management
qmm_result_t qmm_create_pool(qmm_mathematical_type_t type, size_t pool_size,
                            uint32_t alignment, qmm_memory_pool_t **pool);
qmm_result_t qmm_destroy_pool(qmm_memory_pool_t *pool);
qmm_result_t qmm_optimize_pool(qmm_memory_pool_t *pool);
qmm_result_t qmm_compact_pool(qmm_memory_pool_t *pool);

// Mathematical memory allocation
void* qmm_alloc_mathematical(size_t size, qmm_mathematical_type_t type,
                             qmm_allocation_flags_t flags);
void* qmm_alloc_quantum_number(size_t count);
void* qmm_alloc_symbolic_expression(size_t complexity_estimate);
void* qmm_alloc_ast_node(size_t node_count);
void* qmm_alloc_aligned(size_t size, uint32_t alignment, 
                       qmm_mathematical_type_t type);

// Memory deallocation
qmm_result_t qmm_free(void *ptr);
qmm_result_t qmm_free_mathematical(void *ptr, qmm_mathematical_type_t type);

// Mathematical memory operations
qmm_result_t qmm_copy_mathematical(void *dest, const void *src, size_t size,
                                  qmm_mathematical_type_t type);
qmm_result_t qmm_zero_mathematical(void *ptr, size_t size,
                                  qmm_mathematical_type_t type);
qmm_result_t qmm_compare_mathematical(const void *ptr1, const void *ptr2,
                                     size_t size, qmm_mathematical_type_t type,
                                     int *result);

// Mathematical relationship management
qmm_result_t qmm_add_dependency(void *dependent, void *dependency);
qmm_result_t qmm_remove_dependency(void *dependent, void *dependency);
qmm_result_t qmm_get_dependencies(void *ptr, void ***dependencies,
                                 uint32_t *count);
qmm_result_t qmm_get_dependents(void *ptr, void ***dependents,
                               uint32_t *count);

// Mathematical garbage collection
qmm_result_t qmm_gc_run(void);
qmm_result_t qmm_gc_mark_root(void *ptr);
qmm_result_t qmm_gc_unmark_root(void *ptr);
qmm_result_t qmm_gc_force_collection(void);
bool qmm_gc_is_reachable(void *ptr);

// Memory integrity and verification
qmm_result_t qmm_verify_integrity(void *ptr);
qmm_result_t qmm_compute_checksum(void *ptr, quantum_number_t *checksum);
qmm_result_t qmm_validate_mathematical_properties(void *ptr,
                                                 qmm_mathematical_type_t type);

// Performance and optimization
qmm_result_t qmm_optimize_layout(void);
qmm_result_t qmm_defragment_memory(void);
qmm_result_t qmm_update_access_patterns(void *ptr);
qmm_result_t qmm_prefetch_mathematical(void *ptr, size_t size);

// Statistics and monitoring
qmm_result_t qmm_get_statistics(qmm_statistics_t *stats);
qmm_result_t qmm_get_pool_statistics(qmm_memory_pool_t *pool,
                                    qmm_statistics_t *stats);
qmm_result_t qmm_print_memory_map(void);
qmm_result_t qmm_print_mathematical_objects(void);

// Advanced mathematical memory features
qmm_result_t qmm_create_mathematical_cache(size_t cache_size, void **cache);
qmm_result_t qmm_cache_mathematical_result(void *cache, const void *key,
                                          size_t key_size, const void *result,
                                          size_t result_size);
qmm_result_t qmm_lookup_cached_result(void *cache, const void *key,
                                     size_t key_size, void **result,
                                     size_t *result_size);

// Mathematical memory mapping
qmm_result_t qmm_map_mathematical_region(void *address, size_t size,
                                        qmm_mathematical_type_t type,
                                        uint32_t flags);
qmm_result_t qmm_unmap_mathematical_region(void *address, size_t size);
qmm_result_t qmm_protect_mathematical_region(void *address, size_t size,
                                            uint32_t protection_flags);

// Utility functions
const char* qmm_error_string(qmm_result_t error);
const char* qmm_type_string(qmm_mathematical_type_t type);
size_t qmm_get_type_alignment(qmm_mathematical_type_t type);
size_t qmm_get_type_size(qmm_mathematical_type_t type);

// Debug and development functions
#ifdef QMM_DEBUG
void qmm_debug_print_block(qmm_memory_block_t *block);
void qmm_debug_print_pool(qmm_memory_pool_t *pool);
void qmm_debug_validate_heap(void);
void qmm_debug_dump_mathematical_objects(void);
#endif

#endif // QUANTUM_MEMORY_MANAGER_H
