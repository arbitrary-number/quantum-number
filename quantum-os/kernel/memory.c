/**
 * Quantum OS - Memory Management System Implementation
 * 
 * Revolutionary memory management designed for symbolic mathematical computation
 * with fibre-linked addressing based on mathematical relationships.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 * 
 * Date: August 18, 2025
 */

#include "memory.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Global memory management state
static qm_pool_t *memory_pools[QM_MAX_POOLS];
static size_t total_system_memory = 0;
static qm_stats_t memory_stats = {0};
static int memory_initialized = 0;

// Internal helper functions
static uint32_t calculate_block_checksum(qm_block_t *block);
static int validate_block_integrity(qm_block_t *block);
static qm_block_t* find_free_block(qm_pool_t *pool, size_t size);
static void coalesce_free_blocks(qm_pool_t *pool);
static void split_block(qm_block_t *block, size_t size);

/**
 * Initialize the memory management system
 */
int qm_init(size_t total_memory) {
    if (memory_initialized) {
        return QM_SUCCESS;
    }
    
    total_system_memory = total_memory;
    
    // Initialize memory pools array
    for (int i = 0; i < QM_MAX_POOLS; i++) {
        memory_pools[i] = NULL;
    }
    
    // Initialize memory statistics
    memory_stats.total_memory = total_memory;
    memory_stats.kernel_memory = 0;
    memory_stats.user_memory = 0;
    memory_stats.quantum_memory = 0;
    memory_stats.ast_memory = 0;
    memory_stats.total_allocations = 0;
    memory_stats.active_allocations = 0;
    memory_stats.failed_allocations = 0;
    
    // Create default memory pools
    size_t kernel_pool_size = total_memory / 4;  // 25% for kernel
    size_t user_pool_size = total_memory / 2;    // 50% for user space
    size_t quantum_pool_size = total_memory / 8; // 12.5% for quantum numbers
    size_t ast_pool_size = total_memory / 8;     // 12.5% for AST nodes
    
    if (!qm_create_pool(QM_POOL_KERNEL, kernel_pool_size) ||
        !qm_create_pool(QM_POOL_USER, user_pool_size) ||
        !qm_create_pool(QM_POOL_QUANTUM, quantum_pool_size) ||
        !qm_create_pool(QM_POOL_AST, ast_pool_size)) {
        return QM_ERROR_OUT_OF_MEMORY;
    }
    
    memory_initialized = 1;
    return QM_SUCCESS;
}

/**
 * Cleanup memory management system
 */
void qm_cleanup(void) {
    if (!memory_initialized) {
        return;
    }
    
    // Destroy all memory pools
    for (int i = 0; i < QM_MAX_POOLS; i++) {
        if (memory_pools[i]) {
            qm_destroy_pool(memory_pools[i]);
        }
    }
    
    memory_initialized = 0;
}

/**
 * Create a new memory pool
 */
qm_pool_t* qm_create_pool(qm_pool_type_t type, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Find empty slot in pools array
    int pool_index = -1;
    for (int i = 0; i < QM_MAX_POOLS; i++) {
        if (memory_pools[i] == NULL) {
            pool_index = i;
            break;
        }
    }
    
    if (pool_index == -1) {
        return NULL; // No free pool slots
    }
    
    // Allocate pool structure
    qm_pool_t *pool = malloc(sizeof(qm_pool_t));
    if (!pool) {
        return NULL;
    }
    
    // Allocate pool memory
    void *pool_memory = malloc(size);
    if (!pool_memory) {
        free(pool);
        return NULL;
    }
    
    // Initialize pool
    pool->type = type;
    pool->base_addr = pool_memory;
    pool->total_size = size;
    pool->used_size = 0;
    pool->free_size = size;
    pool->free_list = NULL;
    pool->used_list = NULL;
    pool->allocation_count = 0;
    
    // Create pool identifier
    qn_create(&pool->pool_id);
    qn_set_ordinal(&pool->pool_id, 0, (uint32_t)type);
    qn_set_ordinal(&pool->pool_id, 1, (uint32_t)pool_index);
    
    // Create initial free block covering entire pool
    qm_block_t *initial_block = (qm_block_t*)pool_memory;
    initial_block->size = size - sizeof(qm_block_t);
    initial_block->flags = 0;
    qn_zero(&initial_block->qn_id);
    initial_block->next = NULL;
    initial_block->prev = NULL;
    initial_block->checksum = calculate_block_checksum(initial_block);
    
    pool->free_list = initial_block;
    pool->checksum = qm_calculate_checksum(pool, sizeof(qm_pool_t));
    
    memory_pools[pool_index] = pool;
    return pool;
}

/**
 * Destroy a memory pool
 */
int qm_destroy_pool(qm_pool_t *pool) {
    if (!pool) {
        return QM_ERROR_INVALID_POINTER;
    }
    
    // Find pool in array and remove it
    for (int i = 0; i < QM_MAX_POOLS; i++) {
        if (memory_pools[i] == pool) {
            memory_pools[i] = NULL;
            break;
        }
    }
    
    // Free pool memory
    if (pool->base_addr) {
        free(pool->base_addr);
    }
    
    free(pool);
    return QM_SUCCESS;
}

/**
 * Get pool by type
 */
qm_pool_t* qm_get_pool(qm_pool_type_t type) {
    for (int i = 0; i < QM_MAX_POOLS; i++) {
        if (memory_pools[i] && memory_pools[i]->type == type) {
            return memory_pools[i];
        }
    }
    return NULL;
}

/**
 * Allocate memory from default pools
 */
void* qm_alloc(size_t size, uint32_t flags) {
    if (size == 0) {
        return NULL;
    }
    
    qm_pool_t *pool = NULL;
    
    // Select appropriate pool based on flags
    if (flags & QM_ALLOC_QUANTUM) {
        pool = qm_get_pool(QM_POOL_QUANTUM);
    } else if (flags & QM_ALLOC_AST) {
        pool = qm_get_pool(QM_POOL_AST);
    } else if (flags & QM_ALLOC_KERNEL) {
        pool = qm_get_pool(QM_POOL_KERNEL);
    } else if (flags & QM_ALLOC_USER) {
        pool = qm_get_pool(QM_POOL_USER);
    } else {
        // Default to kernel pool
        pool = qm_get_pool(QM_POOL_KERNEL);
    }
    
    if (!pool) {
        memory_stats.failed_allocations++;
        return NULL;
    }
    
    return qm_alloc_from_pool(pool, size, flags);
}

/**
 * Allocate memory from specific pool
 */
void* qm_alloc_from_pool(qm_pool_t *pool, size_t size, uint32_t flags) {
    if (!pool || size == 0) {
        return NULL;
    }
    
    // Align size to 8-byte boundary
    size = (size + 7) & ~7;
    
    // Find suitable free block
    qm_block_t *block = find_free_block(pool, size);
    if (!block) {
        memory_stats.failed_allocations++;
        return NULL;
    }
    
    // Split block if it's significantly larger than needed
    if (block->size > size + sizeof(qm_block_t) + 64) {
        split_block(block, size);
    }
    
    // Remove from free list
    if (block->prev) {
        block->prev->next = block->next;
    } else {
        pool->free_list = block->next;
    }
    if (block->next) {
        block->next->prev = block->prev;
    }
    
    // Add to used list
    block->next = pool->used_list;
    block->prev = NULL;
    if (pool->used_list) {
        pool->used_list->prev = block;
    }
    pool->used_list = block;
    
    // Update block metadata
    block->flags = flags;
    qn_create(&block->qn_id);
    block->checksum = calculate_block_checksum(block);
    
    // Update pool statistics
    pool->used_size += block->size;
    pool->free_size -= block->size;
    pool->allocation_count++;
    
    // Update global statistics
    memory_stats.total_allocations++;
    memory_stats.active_allocations++;
    
    void *user_ptr = (char*)block + sizeof(qm_block_t);
    
    // Zero-initialize if requested
    if (flags & QM_ALLOC_ZERO) {
        memset(user_ptr, 0, block->size - sizeof(qm_block_t));
    }
    
    return user_ptr;
}

/**
 * Free allocated memory
 */
void qm_free(void *ptr) {
    if (!ptr) {
        return;
    }
    
    // Get block header
    qm_block_t *block = (qm_block_t*)((char*)ptr - sizeof(qm_block_t));
    
    // Validate block integrity
    if (!validate_block_integrity(block)) {
        return; // Corrupted block
    }
    
    // Find the pool containing this block
    qm_pool_t *pool = NULL;
    for (int i = 0; i < QM_MAX_POOLS; i++) {
        if (memory_pools[i] && 
            (char*)block >= (char*)memory_pools[i]->base_addr &&
            (char*)block < (char*)memory_pools[i]->base_addr + memory_pools[i]->total_size) {
            pool = memory_pools[i];
            break;
        }
    }
    
    if (!pool) {
        return; // Block not found in any pool
    }
    
    // Remove from used list
    if (block->prev) {
        block->prev->next = block->next;
    } else {
        pool->used_list = block->next;
    }
    if (block->next) {
        block->next->prev = block->prev;
    }
    
    // Add to free list
    block->next = pool->free_list;
    block->prev = NULL;
    if (pool->free_list) {
        pool->free_list->prev = block;
    }
    pool->free_list = block;
    
    // Clear block data
    block->flags = 0;
    qn_zero(&block->qn_id);
    
    // Update pool statistics
    pool->used_size -= block->size;
    pool->free_size += block->size;
    
    // Update global statistics
    memory_stats.active_allocations--;
    
    // Coalesce adjacent free blocks
    coalesce_free_blocks(pool);
}

/**
 * Allocate a single Quantum Number
 */
quantum_number_t* qm_alloc_quantum_number(void) {
    return (quantum_number_t*)qm_alloc(sizeof(quantum_number_t), 
                                       QM_ALLOC_QUANTUM | QM_ALLOC_ZERO);
}

/**
 * Allocate array of Quantum Numbers
 */
quantum_number_t* qm_alloc_quantum_array(size_t count) {
    return (quantum_number_t*)qm_alloc(sizeof(quantum_number_t) * count,
                                       QM_ALLOC_QUANTUM | QM_ALLOC_ZERO);
}

/**
 * Free a Quantum Number
 */
void qm_free_quantum_number(quantum_number_t *qn) {
    qm_free(qn);
}

/**
 * Free array of Quantum Numbers
 */
void qm_free_quantum_array(quantum_number_t *qn_array, size_t count) {
    qm_free(qn_array);
}

/**
 * Calculate checksum for memory block
 */
static uint32_t calculate_block_checksum(qm_block_t *block) {
    uint32_t checksum = 0;
    checksum ^= (uint32_t)block->size;
    checksum ^= block->flags;
    
    // Include quantum number ID in checksum
    for (int i = 0; i < 12; i++) {
        checksum ^= qn_get_ordinal(&block->qn_id, i);
    }
    
    return checksum;
}

/**
 * Validate block integrity
 */
static int validate_block_integrity(qm_block_t *block) {
    if (!block) {
        return 0;
    }
    
    uint32_t expected_checksum = calculate_block_checksum(block);
    return (block->checksum == expected_checksum);
}

/**
 * Find free block of sufficient size
 */
static qm_block_t* find_free_block(qm_pool_t *pool, size_t size) {
    qm_block_t *current = pool->free_list;
    qm_block_t *best_fit = NULL;
    
    // First-fit with best-fit optimization
    while (current) {
        if (current->size >= size) {
            if (!best_fit || current->size < best_fit->size) {
                best_fit = current;
            }
            // If exact fit, use it immediately
            if (current->size == size) {
                break;
            }
        }
        current = current->next;
    }
    
    return best_fit;
}

/**
 * Coalesce adjacent free blocks
 */
static void coalesce_free_blocks(qm_pool_t *pool) {
    qm_block_t *current = pool->free_list;
    
    while (current && current->next) {
        char *current_end = (char*)current + sizeof(qm_block_t) + current->size;
        char *next_start = (char*)current->next;
        
        // Check if blocks are adjacent
        if (current_end == next_start) {
            // Merge blocks
            qm_block_t *next_block = current->next;
            current->size += sizeof(qm_block_t) + next_block->size;
            current->next = next_block->next;
            if (next_block->next) {
                next_block->next->prev = current;
            }
            current->checksum = calculate_block_checksum(current);
        } else {
            current = current->next;
        }
    }
}

/**
 * Split a block into two parts
 */
static void split_block(qm_block_t *block, size_t size) {
    if (block->size <= size + sizeof(qm_block_t)) {
        return; // Block too small to split
    }
    
    // Create new block from remainder
    qm_block_t *new_block = (qm_block_t*)((char*)block + sizeof(qm_block_t) + size);
    new_block->size = block->size - size - sizeof(qm_block_t);
    new_block->flags = 0;
    qn_zero(&new_block->qn_id);
    new_block->next = block->next;
    new_block->prev = block;
    new_block->checksum = calculate_block_checksum(new_block);
    
    // Update original block
    block->size = size;
    block->next = new_block;
    block->checksum = calculate_block_checksum(block);
    
    // Update next block's prev pointer
    if (new_block->next) {
        new_block->next->prev = new_block;
    }
}

/**
 * Get memory statistics
 */
qm_stats_t qm_get_statistics(void) {
    return memory_stats;
}

/**
 * Calculate checksum for arbitrary data
 */
uint32_t qm_calculate_checksum(void *data, size_t size) {
    uint32_t checksum = 0;
    uint8_t *bytes = (uint8_t*)data;
    
    for (size_t i = 0; i < size; i++) {
        checksum ^= bytes[i];
        checksum = (checksum << 1) | (checksum >> 31); // Rotate left
    }
    
    return checksum;
}

/**
 * Get error string
 */
const char* qm_error_string(qm_error_t error) {
    switch (error) {
        case QM_SUCCESS: return "Success";
        case QM_ERROR_OUT_OF_MEMORY: return "Out of memory";
        case QM_ERROR_INVALID_POINTER: return "Invalid pointer";
        case QM_ERROR_INVALID_SIZE: return "Invalid size";
        case QM_ERROR_POOL_FULL: return "Pool full";
        case QM_ERROR_CHECKSUM_MISMATCH: return "Checksum mismatch";
        case QM_ERROR_DOUBLE_FREE: return "Double free";
        case QM_ERROR_CORRUPTION: return "Memory corruption";
        default: return "Unknown error";
    }
}

/**
 * Print memory statistics
 */
void qm_print_statistics(void) {
    printf("=== Quantum OS Memory Statistics ===\n");
    printf("Total Memory: %zu bytes\n", memory_stats.total_memory);
    printf("Kernel Memory: %zu bytes\n", memory_stats.kernel_memory);
    printf("User Memory: %zu bytes\n", memory_stats.user_memory);
    printf("Quantum Memory: %zu bytes\n", memory_stats.quantum_memory);
    printf("AST Memory: %zu bytes\n", memory_stats.ast_memory);
    printf("Total Allocations: %u\n", memory_stats.total_allocations);
    printf("Active Allocations: %u\n", memory_stats.active_allocations);
    printf("Failed Allocations: %u\n", memory_stats.failed_allocations);
    printf("=====================================\n");
}
