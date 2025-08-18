/**
 * Quantum OS - Memory Management System
 * 
 * Revolutionary memory management designed for symbolic mathematical computation
 * with fibre-linked addressing based on mathematical relationships.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 * 
 * Date: August 18, 2025
 */

#ifndef QUANTUM_MEMORY_H
#define QUANTUM_MEMORY_H

#include "quantum_number.h"
#include <stdint.h>
#include <stddef.h>

// Memory page size optimized for Quantum Numbers (4KB)
#define QM_PAGE_SIZE 4096
#define QM_PAGE_SHIFT 12

// Maximum number of memory pools
#define QM_MAX_POOLS 256

// Memory allocation flags
#define QM_ALLOC_ZERO     0x01  // Zero-initialize memory
#define QM_ALLOC_QUANTUM  0x02  // Allocate for Quantum Numbers
#define QM_ALLOC_AST      0x04  // Allocate for AST nodes
#define QM_ALLOC_KERNEL   0x08  // Kernel memory allocation
#define QM_ALLOC_USER     0x10  // User space allocation

// Memory pool types
typedef enum {
    QM_POOL_KERNEL,     // Kernel memory pool
    QM_POOL_USER,       // User space memory pool
    QM_POOL_QUANTUM,    // Quantum Number pool
    QM_POOL_AST,        // AST node pool
    QM_POOL_DEVICE,     // Device driver pool
    QM_POOL_CACHE       // Cache memory pool
} qm_pool_type_t;

// Memory block header for tracking allocations
typedef struct qm_block {
    size_t size;                    // Size of the block
    uint32_t flags;                 // Allocation flags
    quantum_number_t qn_id;         // Quantum Number identifier
    struct qm_block *next;          // Next block in chain
    struct qm_block *prev;          // Previous block in chain
    uint32_t checksum;              // Integrity checksum
} qm_block_t;

// Memory pool structure
typedef struct qm_pool {
    qm_pool_type_t type;            // Pool type
    void *base_addr;                // Base address of pool
    size_t total_size;              // Total pool size
    size_t used_size;               // Currently used size
    size_t free_size;               // Available free size
    qm_block_t *free_list;          // Free block list
    qm_block_t *used_list;          // Used block list
    quantum_number_t pool_id;       // Pool identifier
    uint32_t allocation_count;      // Number of allocations
    uint32_t checksum;              // Pool integrity checksum
} qm_pool_t;

// Memory statistics
typedef struct qm_stats {
    size_t total_memory;            // Total system memory
    size_t kernel_memory;           // Kernel memory usage
    size_t user_memory;             // User memory usage
    size_t quantum_memory;          // Quantum Number memory
    size_t ast_memory;              // AST memory usage
    uint32_t total_allocations;     // Total allocation count
    uint32_t active_allocations;    // Active allocations
    uint32_t failed_allocations;    // Failed allocation count
} qm_stats_t;

// Fibre-linked memory address structure
typedef struct qm_fibre_addr {
    quantum_number_t mathematical_id;   // Mathematical relationship ID
    void *physical_addr;                // Physical memory address
    size_t offset;                      // Offset within mathematical space
    uint32_t relationship_type;         // Type of mathematical relationship
    struct qm_fibre_addr *linked_addr;  // Linked mathematical address
} qm_fibre_addr_t;

// Memory management function prototypes

// Initialization and cleanup
int qm_init(size_t total_memory);
void qm_cleanup(void);

// Pool management
qm_pool_t* qm_create_pool(qm_pool_type_t type, size_t size);
int qm_destroy_pool(qm_pool_t *pool);
qm_pool_t* qm_get_pool(qm_pool_type_t type);

// Memory allocation and deallocation
void* qm_alloc(size_t size, uint32_t flags);
void* qm_alloc_from_pool(qm_pool_t *pool, size_t size, uint32_t flags);
void qm_free(void *ptr);
void* qm_realloc(void *ptr, size_t new_size);

// Quantum Number specific allocation
quantum_number_t* qm_alloc_quantum_number(void);
quantum_number_t* qm_alloc_quantum_array(size_t count);
void qm_free_quantum_number(quantum_number_t *qn);
void qm_free_quantum_array(quantum_number_t *qn_array, size_t count);

// AST node allocation
void* qm_alloc_ast_node(size_t node_size);
void qm_free_ast_node(void *node);

// Fibre-linked addressing
qm_fibre_addr_t* qm_create_fibre_address(quantum_number_t *math_id, 
                                         void *physical_addr,
                                         uint32_t relationship_type);
void* qm_resolve_fibre_address(qm_fibre_addr_t *fibre_addr);
int qm_link_addresses(qm_fibre_addr_t *addr1, qm_fibre_addr_t *addr2);
void qm_free_fibre_address(qm_fibre_addr_t *fibre_addr);

// Memory protection and validation
int qm_protect_memory(void *addr, size_t size, uint32_t protection);
int qm_validate_pointer(void *ptr);
int qm_validate_quantum_pointer(quantum_number_t *qn);
uint32_t qm_calculate_checksum(void *data, size_t size);

// Memory statistics and monitoring
qm_stats_t qm_get_statistics(void);
void qm_print_statistics(void);
int qm_check_memory_integrity(void);
void qm_dump_memory_map(void);

// Garbage collection for symbolic computation
int qm_garbage_collect(void);
int qm_mark_quantum_references(quantum_number_t *qn);
int qm_sweep_unreferenced_memory(void);

// Memory defragmentation
int qm_defragment_pool(qm_pool_t *pool);
int qm_compact_quantum_memory(void);

// Error handling
typedef enum {
    QM_SUCCESS = 0,
    QM_ERROR_OUT_OF_MEMORY,
    QM_ERROR_INVALID_POINTER,
    QM_ERROR_INVALID_SIZE,
    QM_ERROR_POOL_FULL,
    QM_ERROR_CHECKSUM_MISMATCH,
    QM_ERROR_DOUBLE_FREE,
    QM_ERROR_CORRUPTION
} qm_error_t;

const char* qm_error_string(qm_error_t error);

// Memory debugging (enabled in debug builds)
#ifdef QM_DEBUG
void qm_debug_allocation(void *ptr, size_t size, const char *file, int line);
void qm_debug_deallocation(void *ptr, const char *file, int line);
#define QM_ALLOC(size, flags) qm_debug_alloc(size, flags, __FILE__, __LINE__)
#define QM_FREE(ptr) qm_debug_free(ptr, __FILE__, __LINE__)
#else
#define QM_ALLOC(size, flags) qm_alloc(size, flags)
#define QM_FREE(ptr) qm_free(ptr)
#endif

#endif // QUANTUM_MEMORY_H
