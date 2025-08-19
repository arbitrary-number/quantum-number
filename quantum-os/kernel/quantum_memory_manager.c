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
 * Quantum Number Memory Manager - Advanced Mathematical Memory Management Implementation
 * 
 * Enhanced memory management system with native support for Quantum Numbers,
 * symbolic computation, and mathematical object relationships.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include "quantum_memory_manager.h"
#include "kernel.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global QMM state
static bool qmm_initialized = false;
static size_t qmm_total_memory = 0;
static qmm_memory_pool_t *qmm_pool_list = NULL;
static qmm_gc_context_t qmm_gc_context = {0};
static qmm_statistics_t qmm_stats = {0};

// Memory pools for different mathematical types
static qmm_memory_pool_t *quantum_number_pool = NULL;
static qmm_memory_pool_t *symbolic_expression_pool = NULL;
static qmm_memory_pool_t *ast_node_pool = NULL;
static qmm_memory_pool_t *general_pool = NULL;

// Internal helper functions
static uint64_t qmm_get_current_time(void);
static qmm_memory_block_t* qmm_find_free_block(qmm_memory_pool_t *pool, size_t size);
static qmm_result_t qmm_split_block(qmm_memory_block_t *block, size_t size);
static qmm_result_t qmm_merge_free_blocks(qmm_memory_pool_t *pool);
static qmm_result_t qmm_add_block_to_pool(qmm_memory_pool_t *pool, qmm_memory_block_t *block);
static qmm_result_t qmm_remove_block_from_pool(qmm_memory_pool_t *pool, qmm_memory_block_t *block);
static qmm_memory_block_t* qmm_get_block_from_address(void *ptr);
static qmm_result_t qmm_update_statistics(void);

/**
 * Initialize the Quantum Memory Manager
 */
qmm_result_t qmm_initialize(size_t total_memory_size) {
    if (qmm_initialized) {
        return QMM_SUCCESS;
    }
    
    if (total_memory_size < 1024 * 1024) { // Minimum 1MB
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    qmm_total_memory = total_memory_size;
    
    // Initialize GC context with default values
    qmm_gc_context.gc_enabled = true;
    qmm_gc_context.gc_threshold = 80; // 80% memory usage triggers GC
    qmm_gc_context.gc_frequency = 1000; // Every 1000 ticks
    qmm_gc_context.mathematical_depth_limit = 100;
    qmm_gc_context.symbolic_complexity_limit = 1000;
    qmm_gc_context.preserve_mathematical_proofs = true;
    qmm_gc_context.preserve_computational_contexts = true;
    qmm_gc_context.last_gc_run = qmm_get_current_time();
    
    // Create specialized memory pools
    size_t pool_size = total_memory_size / 4; // Each pool gets 1/4 of total memory
    
    // Quantum Number pool - optimized for 256-bit aligned operations
    qmm_result_t result = qmm_create_pool(QMM_TYPE_QUANTUM_NUMBER, pool_size,
                                         QMM_ALIGN_QUANTUM_NUMBER, &quantum_number_pool);
    if (result != QMM_SUCCESS) {
        return result;
    }
    
    // Symbolic Expression pool - optimized for symbolic computation
    result = qmm_create_pool(QMM_TYPE_SYMBOLIC_EXPRESSION, pool_size,
                            QMM_ALIGN_SYMBOLIC, &symbolic_expression_pool);
    if (result != QMM_SUCCESS) {
        return result;
    }
    
    // AST Node pool - optimized for tree structures
    result = qmm_create_pool(QMM_TYPE_AST_NODE, pool_size,
                            QMM_ALIGN_AST_NODE, &ast_node_pool);
    if (result != QMM_SUCCESS) {
        return result;
    }
    
    // General purpose pool
    result = qmm_create_pool(QMM_TYPE_QUANTUM_NUMBER, pool_size,
                            QMM_ALIGN_DEFAULT, &general_pool);
    if (result != QMM_SUCCESS) {
        return result;
    }
    
    // Initialize statistics
    memset(&qmm_stats, 0, sizeof(qmm_statistics_t));
    qmm_stats.total_memory = total_memory_size;
    qmm_stats.free_memory = total_memory_size;
    qmm_stats.total_pools = 4;
    qmm_stats.active_pools = 4;
    qmm_stats.quantum_number_pools = 1;
    qmm_stats.symbolic_pools = 1;
    qmm_stats.ast_pools = 1;
    
    qmm_initialized = true;
    
    kernel_log(KERNEL_LOG_INFO, "Quantum Memory Manager initialized with %zu bytes", 
               total_memory_size);
    
    return QMM_SUCCESS;
}

/**
 * Shutdown the Quantum Memory Manager
 */
qmm_result_t qmm_shutdown(void) {
    if (!qmm_initialized) {
        return QMM_SUCCESS;
    }
    
    // Destroy all memory pools
    qmm_memory_pool_t *current_pool = qmm_pool_list;
    while (current_pool) {
        qmm_memory_pool_t *next_pool = current_pool->next_pool;
        qmm_destroy_pool(current_pool);
        current_pool = next_pool;
    }
    
    // Clean up GC context
    if (qmm_gc_context.root_objects) {
        free(qmm_gc_context.root_objects);
    }
    if (qmm_gc_context.reference_graph) {
        free(qmm_gc_context.reference_graph);
    }
    
    // Reset global state
    qmm_pool_list = NULL;
    quantum_number_pool = NULL;
    symbolic_expression_pool = NULL;
    ast_node_pool = NULL;
    general_pool = NULL;
    
    memset(&qmm_gc_context, 0, sizeof(qmm_gc_context_t));
    memset(&qmm_stats, 0, sizeof(qmm_statistics_t));
    
    qmm_initialized = false;
    
    kernel_log(KERNEL_LOG_INFO, "Quantum Memory Manager shutdown complete");
    
    return QMM_SUCCESS;
}

/**
 * Create a memory pool for a specific mathematical type
 */
qmm_result_t qmm_create_pool(qmm_mathematical_type_t type, size_t pool_size,
                            uint32_t alignment, qmm_memory_pool_t **pool) {
    if (!pool || pool_size == 0) {
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    // Allocate pool structure
    qmm_memory_pool_t *new_pool = (qmm_memory_pool_t*)malloc(sizeof(qmm_memory_pool_t));
    if (!new_pool) {
        return QMM_ERROR_OUT_OF_MEMORY;
    }
    
    // Allocate pool memory
    void *pool_memory = malloc(pool_size);
    if (!pool_memory) {
        free(new_pool);
        return QMM_ERROR_OUT_OF_MEMORY;
    }
    
    // Initialize pool structure
    memset(new_pool, 0, sizeof(qmm_memory_pool_t));
    new_pool->base_address = pool_memory;
    new_pool->total_size = pool_size;
    new_pool->used_size = 0;
    new_pool->free_size = pool_size;
    new_pool->primary_type = type;
    new_pool->alignment_requirement = alignment;
    new_pool->allocation_flags = QMM_FLAG_MATHEMATICAL;
    new_pool->block_count = 0;
    new_pool->optimization_level = 1;
    
    // Create initial free block covering entire pool
    qmm_memory_block_t *initial_block = (qmm_memory_block_t*)malloc(sizeof(qmm_memory_block_t));
    if (!initial_block) {
        free(pool_memory);
        free(new_pool);
        return QMM_ERROR_OUT_OF_MEMORY;
    }
    
    memset(initial_block, 0, sizeof(qmm_memory_block_t));
    initial_block->address = pool_memory;
    initial_block->size = pool_size;
    initial_block->is_free = true;
    initial_block->type = type;
    quantum_number_init(&initial_block->mathematical_id);
    quantum_number_set_from_uint64(&initial_block->mathematical_id, 1);
    initial_block->creation_time = qmm_get_current_time();
    initial_block->last_access = initial_block->creation_time;
    quantum_number_init(&initial_block->integrity_checksum);
    
    new_pool->first_block = initial_block;
    new_pool->last_block = initial_block;
    new_pool->block_count = 1;
    
    // Add pool to global list
    if (qmm_pool_list) {
        new_pool->next_pool = qmm_pool_list;
        qmm_pool_list->prev_pool = new_pool;
    }
    qmm_pool_list = new_pool;
    
    *pool = new_pool;
    
    return QMM_SUCCESS;
}

/**
 * Allocate mathematical memory with specific type and flags
 */
void* qmm_alloc_mathematical(size_t size, qmm_mathematical_type_t type,
                             qmm_allocation_flags_t flags) {
    if (!qmm_initialized || size == 0) {
        return NULL;
    }
    
    // Select appropriate pool based on type
    qmm_memory_pool_t *target_pool = general_pool;
    switch (type) {
        case QMM_TYPE_QUANTUM_NUMBER:
            target_pool = quantum_number_pool;
            break;
        case QMM_TYPE_SYMBOLIC_EXPRESSION:
            target_pool = symbolic_expression_pool;
            break;
        case QMM_TYPE_AST_NODE:
            target_pool = ast_node_pool;
            break;
        default:
            target_pool = general_pool;
            break;
    }
    
    if (!target_pool) {
        return NULL;
    }
    
    // Align size to pool requirements
    size_t aligned_size = size;
    if (target_pool->alignment_requirement > 1) {
        aligned_size = (size + target_pool->alignment_requirement - 1) & 
                      ~(target_pool->alignment_requirement - 1);
    }
    
    // Find suitable free block
    qmm_memory_block_t *block = qmm_find_free_block(target_pool, aligned_size);
    if (!block) {
        // Try garbage collection if enabled
        if (qmm_gc_context.gc_enabled) {
            qmm_gc_run();
            block = qmm_find_free_block(target_pool, aligned_size);
        }
        
        if (!block) {
            return NULL; // Out of memory
        }
    }
    
    // Split block if necessary
    if (block->size > aligned_size + sizeof(qmm_memory_block_t)) {
        qmm_split_block(block, aligned_size);
    }
    
    // Mark block as allocated
    block->is_free = false;
    block->type = type;
    block->mathematical_properties = flags;
    block->last_access = qmm_get_current_time();
    block->access_count = 1;
    
    // Update pool statistics
    target_pool->used_size += aligned_size;
    target_pool->free_size -= aligned_size;
    target_pool->allocation_count++;
    
    // Update global statistics
    qmm_stats.total_allocations++;
    qmm_stats.current_allocations++;
    qmm_stats.used_memory += aligned_size;
    qmm_stats.free_memory -= aligned_size;
    
    // Track mathematical object type
    switch (type) {
        case QMM_TYPE_QUANTUM_NUMBER:
            qmm_stats.quantum_number_objects++;
            break;
        case QMM_TYPE_SYMBOLIC_EXPRESSION:
            qmm_stats.symbolic_expression_objects++;
            break;
        case QMM_TYPE_AST_NODE:
            qmm_stats.ast_node_objects++;
            break;
        default:
            break;
    }
    
    // Zero-initialize if requested
    if (flags & QMM_FLAG_ZERO_INIT) {
        memset(block->address, 0, aligned_size);
    }
    
    kernel_track_mathematical_operation();
    
    return block->address;
}

/**
 * Allocate memory specifically for Quantum Numbers
 */
void* qmm_alloc_quantum_number(size_t count) {
    if (count == 0) {
        return NULL;
    }
    
    size_t total_size = count * sizeof(quantum_number_t);
    return qmm_alloc_mathematical(total_size, QMM_TYPE_QUANTUM_NUMBER,
                                 QMM_FLAG_MATHEMATICAL | QMM_FLAG_ZERO_INIT);
}

/**
 * Allocate memory for symbolic expressions
 */
void* qmm_alloc_symbolic_expression(size_t complexity_estimate) {
    // Estimate memory requirements based on complexity
    size_t estimated_size = complexity_estimate * 64; // 64 bytes per complexity unit
    if (estimated_size < 256) {
        estimated_size = 256; // Minimum size
    }
    
    return qmm_alloc_mathematical(estimated_size, QMM_TYPE_SYMBOLIC_EXPRESSION,
                                 QMM_FLAG_SYMBOLIC | QMM_FLAG_ZERO_INIT);
}

/**
 * Allocate memory for AST nodes
 */
void* qmm_alloc_ast_node(size_t node_count) {
    if (node_count == 0) {
        return NULL;
    }
    
    size_t total_size = node_count * 128; // Estimated 128 bytes per AST node
    return qmm_alloc_mathematical(total_size, QMM_TYPE_AST_NODE,
                                 QMM_FLAG_MATHEMATICAL | QMM_FLAG_ZERO_INIT);
}

/**
 * Free mathematical memory
 */
qmm_result_t qmm_free(void *ptr) {
    if (!qmm_initialized || !ptr) {
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    // Find the memory block for this address
    qmm_memory_block_t *block = qmm_get_block_from_address(ptr);
    if (!block) {
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    if (block->is_free) {
        return QMM_ERROR_INVALID_PARAMETER; // Double free
    }
    
    // Mark block as free
    block->is_free = true;
    block->last_access = qmm_get_current_time();
    
    // Find the pool this block belongs to
    qmm_memory_pool_t *pool = NULL;
    for (qmm_memory_pool_t *p = qmm_pool_list; p; p = p->next_pool) {
        if (ptr >= p->base_address && 
            ptr < (char*)p->base_address + p->total_size) {
            pool = p;
            break;
        }
    }
    
    if (!pool) {
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    // Update pool statistics
    pool->used_size -= block->size;
    pool->free_size += block->size;
    pool->deallocation_count++;
    
    // Update global statistics
    qmm_stats.total_deallocations++;
    qmm_stats.current_allocations--;
    qmm_stats.used_memory -= block->size;
    qmm_stats.free_memory += block->size;
    
    // Track mathematical object type
    switch (block->type) {
        case QMM_TYPE_QUANTUM_NUMBER:
            if (qmm_stats.quantum_number_objects > 0) {
                qmm_stats.quantum_number_objects--;
            }
            break;
        case QMM_TYPE_SYMBOLIC_EXPRESSION:
            if (qmm_stats.symbolic_expression_objects > 0) {
                qmm_stats.symbolic_expression_objects--;
            }
            break;
        case QMM_TYPE_AST_NODE:
            if (qmm_stats.ast_node_objects > 0) {
                qmm_stats.ast_node_objects--;
            }
            break;
        default:
            break;
    }
    
    // Merge adjacent free blocks
    qmm_merge_free_blocks(pool);
    
    return QMM_SUCCESS;
}

/**
 * Run mathematical garbage collection
 */
qmm_result_t qmm_gc_run(void) {
    if (!qmm_initialized || !qmm_gc_context.gc_enabled) {
        return QMM_SUCCESS;
    }
    
    uint64_t gc_start_time = qmm_get_current_time();
    uint64_t objects_collected = 0;
    uint64_t memory_reclaimed = 0;
    
    // Mark phase: mark all reachable objects
    // This is a simplified implementation - real GC would be more sophisticated
    
    // Sweep phase: collect unreachable objects
    for (qmm_memory_pool_t *pool = qmm_pool_list; pool; pool = pool->next_pool) {
        qmm_memory_block_t *block = pool->first_block;
        while (block) {
            qmm_memory_block_t *next_block = block->next_block;
            
            if (!block->is_free && block->reference_count == 0) {
                // This block has no references and can be collected
                memory_reclaimed += block->size;
                objects_collected++;
                
                // Free the block
                qmm_free(block->address);
            }
            
            block = next_block;
        }
    }
    
    // Update GC statistics
    qmm_gc_context.total_gc_runs++;
    qmm_gc_context.objects_collected += objects_collected;
    qmm_gc_context.memory_reclaimed += memory_reclaimed;
    qmm_gc_context.last_gc_run = qmm_get_current_time();
    
    uint64_t gc_time = qmm_get_current_time() - gc_start_time;
    qmm_gc_context.average_gc_time = 
        (qmm_gc_context.average_gc_time * (qmm_gc_context.total_gc_runs - 1) + gc_time) /
        qmm_gc_context.total_gc_runs;
    
    qmm_stats.gc_time_total += gc_time;
    
    kernel_log(KERNEL_LOG_INFO, "GC collected %llu objects, reclaimed %llu bytes in %llu ticks",
               objects_collected, memory_reclaimed, gc_time);
    
    return QMM_SUCCESS;
}

/**
 * Get QMM statistics
 */
qmm_result_t qmm_get_statistics(qmm_statistics_t *stats) {
    if (!qmm_initialized || !stats) {
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    qmm_update_statistics();
    *stats = qmm_stats;
    
    return QMM_SUCCESS;
}

/**
 * Print memory map for debugging
 */
qmm_result_t qmm_print_memory_map(void) {
    if (!qmm_initialized) {
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    printf("=== Quantum Memory Manager - Memory Map ===\n");
    printf("Total Memory: %zu bytes\n", qmm_stats.total_memory);
    printf("Used Memory: %zu bytes\n", qmm_stats.used_memory);
    printf("Free Memory: %zu bytes\n", qmm_stats.free_memory);
    printf("Active Pools: %u\n", qmm_stats.active_pools);
    printf("\n");
    
    for (qmm_memory_pool_t *pool = qmm_pool_list; pool; pool = pool->next_pool) {
        printf("Pool: %s (Type: %s)\n", 
               pool == quantum_number_pool ? "Quantum Numbers" :
               pool == symbolic_expression_pool ? "Symbolic Expressions" :
               pool == ast_node_pool ? "AST Nodes" : "General",
               qmm_type_string(pool->primary_type));
        printf("  Base: %p, Size: %zu bytes\n", pool->base_address, pool->total_size);
        printf("  Used: %zu bytes, Free: %zu bytes\n", pool->used_size, pool->free_size);
        printf("  Blocks: %u, Allocations: %llu\n", pool->block_count, pool->allocation_count);
        printf("\n");
    }
    
    printf("Mathematical Objects:\n");
    printf("  Quantum Numbers: %u\n", qmm_stats.quantum_number_objects);
    printf("  Symbolic Expressions: %u\n", qmm_stats.symbolic_expression_objects);
    printf("  AST Nodes: %u\n", qmm_stats.ast_node_objects);
    printf("  Mathematical Proofs: %u\n", qmm_stats.mathematical_proof_objects);
    printf("\n");
    
    printf("Garbage Collection:\n");
    printf("  GC Runs: %llu\n", qmm_gc_context.total_gc_runs);
    printf("  Objects Collected: %llu\n", qmm_gc_context.objects_collected);
    printf("  Memory Reclaimed: %llu bytes\n", qmm_gc_context.memory_reclaimed);
    printf("  Average GC Time: %llu ticks\n", qmm_gc_context.average_gc_time);
    printf("==========================================\n");
    
    return QMM_SUCCESS;
}

// Helper function implementations

static uint64_t qmm_get_current_time(void) {
    return kernel_get_ticks();
}

static qmm_memory_block_t* qmm_find_free_block(qmm_memory_pool_t *pool, size_t size) {
    if (!pool) {
        return NULL;
    }
    
    // First-fit algorithm
    qmm_memory_block_t *block = pool->first_block;
    while (block) {
        if (block->is_free && block->size >= size) {
            return block;
        }
        block = block->next_block;
    }
    
    return NULL;
}

static qmm_result_t qmm_split_block(qmm_memory_block_t *block, size_t size) {
    if (!block || block->size <= size) {
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    // Create new block for remaining space
    qmm_memory_block_t *new_block = (qmm_memory_block_t*)malloc(sizeof(qmm_memory_block_t));
    if (!new_block) {
        return QMM_ERROR_OUT_OF_MEMORY;
    }
    
    memset(new_block, 0, sizeof(qmm_memory_block_t));
    new_block->address = (char*)block->address + size;
    new_block->size = block->size - size;
    new_block->is_free = true;
    new_block->type = block->type;
    quantum_number_init(&new_block->mathematical_id);
    new_block->creation_time = qmm_get_current_time();
    new_block->last_access = new_block->creation_time;
    quantum_number_init(&new_block->integrity_checksum);
    
    // Update original block
    block->size = size;
    
    // Insert new block into linked list
    new_block->next_block = block->next_block;
    new_block->prev_block = block;
    if (block->next_block) {
        block->next_block->prev_block = new_block;
    }
    block->next_block = new_block;
    
    return QMM_SUCCESS;
}

static qmm_result_t qmm_merge_free_blocks(qmm_memory_pool_t *pool) {
    if (!pool) {
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    qmm_memory_block_t *block = pool->first_block;
    while (block && block->next_block) {
        if (block->is_free && block->next_block->is_free &&
            (char*)block->address + block->size == block->next_block->address) {
            
            // Merge blocks
            qmm_memory_block_t *next_block = block->next_block;
            block->size += next_block->size;
            block->next_block = next_block->next_block;
            if (next_block->next_block) {
                next_block->next_block->prev_block = block;
            }
            
            free(next_block);
            pool->block_count--;
        } else {
            block = block->next_block;
        }
    }
    
    return QMM_SUCCESS;
}

static qmm_memory_block_t* qmm_get_block_from_address(void *ptr) {
    if (!ptr) {
        return NULL;
    }
    
    // Search all pools for the block containing this address
    for (qmm_memory_pool_t *pool = qmm_pool_list; pool; pool = pool->next_pool) {
        if (ptr >= pool->base_address && 
            ptr < (char*)pool->base_address + pool->total_size) {
            
            // Search blocks in this pool
            qmm_memory_block_t *block = pool->first_block;
            while (block) {
                if (block->address == ptr) {
                    return block;
                }
                block = block->next_block;
            }
        }
    }
    
    return NULL;
}

static qmm_result_t qmm_update_statistics(void) {
    // Update peak allocations
    if (qmm_stats.current_allocations > qmm_stats.peak_allocations) {
        qmm_stats.peak_allocations = qmm_stats.current_allocations;
    }
    
    // Calculate fragmentation
    size_t total_free = 0;
    size_t largest_free = 0;
    
    for (qmm_memory_pool_t *pool = qmm_pool_list; pool; pool = pool->next_pool) {
        qmm_memory_block_t *block = pool->first_block;
        while (block) {
            if (block->is_free) {
                total_free += block->size;
                if (block->size > largest_free) {
                    largest_free = block->size;
                }
            }
            block = block->next_block;
        }
    }
    
    if (total_free > 0) {
        qmm_stats.fragmented_memory = total_free - largest_free;
    }
    
    return QMM_SUCCESS;
}

const char* qmm_error_string(qmm_result_t error) {
    switch (error) {
        case QMM_SUCCESS: return "Success";
        case QMM_ERROR_OUT_OF_MEMORY: return "Out of memory";
        case QMM_ERROR_INVALID_PARAMETER: return "Invalid parameter";
        case QMM_ERROR_INVALID_TYPE: return "Invalid mathematical type";
        case QMM_ERROR_ALIGNMENT_ERROR: return "Memory alignment error";
        case QMM_ERROR_MATHEMATICAL_INCONSISTENCY: return "Mathematical inconsistency";
        case QMM_ERROR_DEPENDENCY_CYCLE: return "Dependency cycle detected";
        case QMM_ERROR_INTEGRITY_VIOLATION: return "Memory integrity violation";
        case QMM_ERROR_GC_FAILURE: return "Garbage collection failure";
        case QMM_ERROR_POOL_EXHAUSTED: return "Memory pool exhausted";
        case QMM_ERROR_FRAGMENTATION_CRITICAL: return "Critical memory fragmentation";
        default: return "Unknown error";
    }
}

const char* qmm_type_string(qmm_mathematical_type_t type) {
    switch (type) {
        case QMM_TYPE_QUANTUM_NUMBER: return "Quantum Number";
        case QMM_TYPE_SYMBOLIC_EXPRESSION: return "Symbolic Expression";
        case QMM_TYPE_AST_NODE: return "AST Node";
        case QMM_TYPE_MATHEMATICAL_MATRIX: return "Mathematical Matrix";
        case QMM_TYPE_COMPUTATIONAL_CONTEXT: return "Computational Context";
        case QMM_TYPE_DEFERRED_EVALUATION: return "Deferred Evaluation";
        case QMM_TYPE_MATHEMATICAL_PROOF: return "Mathematical Proof";
        case QMM_TYPE_RELATIONSHIP_GRAPH: return "Relationship Graph";
        default: return "Unknown Type";
    }
}

size_t qmm_get_type_alignment(qmm_mathematical_type_t type) {
    switch (type) {
        case QMM_TYPE_QUANTUM_NUMBER: return QMM_ALIGN_QUANTUM_NUMBER;
        case QMM_TYPE_SYMBOLIC_EXPRESSION: return QMM_ALIGN_SYMBOLIC;
        case QMM_TYPE_AST_NODE: return QMM_ALIGN_AST_NODE;
        default: return QMM_ALIGN_DEFAULT;
    }
}

size_t qmm_get_type_size(qmm_mathematical_type_t type) {
    switch (type) {
        case QMM_TYPE_QUANTUM_NUMBER: return sizeof(quantum_number_t);
        case QMM_TYPE_SYMBOLIC_EXPRESSION: return 256; // Estimated
        case QMM_TYPE_AST_NODE: return 128; // Estimated
        case QMM_TYPE_MATHEMATICAL_MATRIX: return 512; // Estimated
        case QMM_TYPE_COMPUTATIONAL_CONTEXT: return 1024; // Estimated
        case QMM_TYPE_DEFERRED_EVALUATION: return 256; // Estimated
        case QMM_TYPE_MATHEMATICAL_PROOF: return 2048; // Estimated
        case QMM_TYPE_RELATIONSHIP_GRAPH: return 1024; // Estimated
        default: return 64; // Default size
    }
}

/**
 * Destroy a memory pool and free all associated resources
 */
qmm_result_t qmm_destroy_pool(qmm_memory_pool_t *pool) {
    if (!pool) {
        return QMM_ERROR_INVALID_PARAMETER;
    }
    
    // Free all blocks in the pool
    qmm_memory_block_t *block = pool->first_block;
    while (block) {
        qmm_memory_block_t *next_block = block->next_block;
        
        // Free dependencies and dependents arrays
        if (block->dependencies) {
            free(block->dependencies);
        }
        if (block->dependents) {
            free(block->dependents);
        }
        
        free(block);
        block = next_block;
    }
    
    // Free pool memory
    if (pool->base_address) {
        free(pool->base_address);
    }
    
    // Free mathematical cache if present
    if (pool->mathematical_cache) {
        free(pool->mathematical_cache);
    }
    
    // Remove pool from global list
    if (pool->prev_pool) {
        pool->prev_pool->next_pool = pool->next_pool;
    } else {
        qmm_pool_list = pool->next_pool;
    }
    
    if (pool->next_pool) {
        pool->next_pool->prev_pool = pool->prev_pool;
    }
    
    // Free the pool structure itself
    free(pool);
    
    return QMM_SUCCESS;
}
