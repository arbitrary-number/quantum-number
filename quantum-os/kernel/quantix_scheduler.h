/**
 * Quantix OS - Advanced Mathematical Scheduler
 * 
 * Enhanced mathematical scheduling system with AST-based process management
 * and symbolic computation optimization for Quantum Number operations.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 * 
 * Date: August 19, 2025
 */

#ifndef QUANTIX_SCHEDULER_H
#define QUANTIX_SCHEDULER_H

#include "quantum_number.h"
#include "process.h"
#include <stdint.h>
#include <stddef.h>

// AST node types for process computation trees
typedef enum {
    AST_NODE_QUANTUM_NUMBER,        // Quantum Number leaf node
    AST_NODE_OPERATION,             // Mathematical operation node
    AST_NODE_FUNCTION_CALL,         // Function call node
    AST_NODE_CONDITIONAL,           // Conditional branch node
    AST_NODE_LOOP,                  // Loop construct node
    AST_NODE_SYMBOLIC_EXPRESSION,   // Symbolic expression node
    AST_NODE_DEFERRED_EVALUATION,   // Deferred evaluation node
    AST_NODE_PARALLEL_COMPUTATION   // Parallel computation node
} ast_node_type_t;

// AST node structure for process computation trees
typedef struct ast_node {
    ast_node_type_t type;           // Node type
    quantum_number_t value;         // Node value (for leaf nodes)
    char operation[32];             // Operation name (for operation nodes)
    struct ast_node *left;          // Left child
    struct ast_node *right;         // Right child
    struct ast_node *parent;        // Parent node
    uint32_t complexity_weight;     // Computational complexity weight
    uint32_t evaluation_cost;       // Evaluation cost estimate
    uint32_t symbolic_depth;        // Symbolic expression depth
    uint64_t last_evaluated;        // Last evaluation timestamp
    void *cached_result;            // Cached evaluation result
} ast_node_t;

// Mathematical process states (enhanced from basic process states)
typedef enum {
    QUANTIX_STATE_SYMBOLIC_READY,       // Ready for symbolic computation
    QUANTIX_STATE_MATHEMATICAL_RUNNING, // Performing mathematical operations
    QUANTIX_STATE_AST_EVALUATING,       // Evaluating AST nodes
    QUANTIX_STATE_QUANTUM_BLOCKED,      // Blocked on Quantum Number operation
    QUANTIX_STATE_SYMBOLIC_SUSPENDED,   // Suspended with symbolic state preserved
    QUANTIX_STATE_DEFERRED_WAITING,     // Waiting for deferred evaluation
    QUANTIX_STATE_MATHEMATICAL_TERMINATED // Terminated with mathematical cleanup
} quantix_process_state_t;

// Enhanced mathematical scheduling metrics
typedef struct quantix_math_metrics {
    quantum_number_t computation_complexity;   // Total computational complexity
    quantum_number_t symbolic_weight;          // Symbolic computation weight
    quantum_number_t ast_evaluation_cost;      // AST evaluation cost
    quantum_number_t quantum_operation_count;  // Quantum operations performed
    quantum_number_t mathematical_priority;    // Calculated mathematical priority
    quantum_number_t resource_efficiency;      // Resource utilization efficiency
    quantum_number_t symbolic_optimization;    // Symbolic optimization factor
    uint64_t ast_nodes_evaluated;             // Total AST nodes evaluated
    uint64_t deferred_evaluations;            // Deferred evaluations pending
    uint64_t mathematical_cache_hits;         // Mathematical cache hits
    uint64_t symbolic_transformations;        // Symbolic transformations performed
} quantix_math_metrics_t;

// Quantix process structure (enhanced process control block)
typedef struct quantix_process {
    // Basic process information
    uint32_t pid;                           // Process ID
    uint32_t ppid;                          // Parent process ID
    char name[64];                          // Process name
    quantix_process_state_t quantix_state;  // Enhanced process state
    qp_priority_t priority;                 // Base priority
    uint32_t flags;                         // Process flags
    
    // Mathematical computation tree
    ast_node_t *computation_tree;           // Root of computation AST
    ast_node_t *current_evaluation_node;    // Currently evaluating node
    quantum_number_t *symbolic_stack;       // Symbolic computation stack
    size_t symbolic_stack_size;             // Stack size
    size_t symbolic_stack_pointer;          // Current stack pointer
    
    // Enhanced mathematical metrics
    quantix_math_metrics_t math_metrics;    // Mathematical scheduling metrics
    
    // Symbolic memory management
    void *symbolic_memory_space;            // Symbolic memory space
    size_t symbolic_memory_size;            // Size of symbolic memory
    quantum_number_t *quantum_heap;         // Quantum Number heap
    size_t quantum_heap_size;               // Heap size
    
    // Mathematical dependencies
    struct quantix_process **dependencies;  // Mathematical dependencies
    uint32_t dependency_count;              // Number of dependencies
    quantum_number_t dependency_weight;     // Dependency weight
    
    // Scheduling information
    quantum_number_t mathematical_time_slice; // Mathematical time slice
    quantum_number_t execution_weight;      // Execution weight
    uint64_t last_mathematical_operation;   // Last mathematical operation time
    uint64_t symbolic_computation_time;     // Time spent in symbolic computation
    
    // AST evaluation state
    ast_node_t **evaluation_stack;          // AST evaluation stack
    size_t evaluation_stack_size;           // Evaluation stack size
    size_t evaluation_stack_pointer;        // Current evaluation position
    
    // Mathematical optimization state
    void *optimization_context;             // Mathematical optimization context
    uint32_t optimization_level;            // Current optimization level
    quantum_number_t optimization_benefit;  // Optimization benefit measure
    
    // Process relationships (enhanced)
    struct quantix_process *parent;         // Parent process
    struct quantix_process *children;       // Child processes
    struct quantix_process *next_sibling;   // Next sibling
    struct quantix_process *mathematical_group; // Mathematical computation group
    
    // Scheduling queues (enhanced)
    struct quantix_process *next_ready;     // Next in ready queue
    struct quantix_process *prev_ready;     // Previous in ready queue
    struct quantix_process *next_mathematical; // Next in mathematical queue
    struct quantix_process *next_symbolic;  // Next in symbolic queue
    
    // Mathematical synchronization
    quantum_number_t synchronization_key;   // Mathematical synchronization key
    void *mathematical_lock;                // Mathematical lock object
    uint32_t mathematical_wait_reason;      // Mathematical wait reason
    
    // Performance tracking
    uint64_t mathematical_operations_total; // Total mathematical operations
    uint64_t symbolic_evaluations_total;    // Total symbolic evaluations
    uint64_t ast_optimizations_performed;   // AST optimizations performed
    uint64_t quantum_cache_efficiency;      // Quantum Number cache efficiency
    
    // Integrity and verification
    quantum_number_t process_checksum;      // Process integrity checksum
    uint64_t last_verification_time;        // Last integrity verification
} quantix_process_t;

// Enhanced scheduler state
typedef struct quantix_scheduler_state {
    // Current execution state
    quantix_process_t *current_process;     // Currently executing process
    quantix_process_t *mathematical_queue;  // Mathematical computation queue
    quantix_process_t *symbolic_queue;      // Symbolic computation queue
    quantix_process_t *ast_evaluation_queue; // AST evaluation queue
    quantix_process_t *deferred_queue;      // Deferred evaluation queue
    
    // Mathematical scheduling parameters
    quantum_number_t global_mathematical_weight; // Global mathematical weight
    quantum_number_t symbolic_boost_factor;     // Symbolic computation boost
    quantum_number_t ast_evaluation_priority;   // AST evaluation priority
    quantum_number_t quantum_operation_weight;  // Quantum operation weight
    
    // Scheduling statistics
    uint64_t mathematical_operations_scheduled; // Mathematical operations scheduled
    uint64_t symbolic_computations_scheduled;   // Symbolic computations scheduled
    uint64_t ast_evaluations_scheduled;         // AST evaluations scheduled
    uint64_t quantum_operations_scheduled;      // Quantum operations scheduled
    
    // Performance optimization
    uint32_t mathematical_optimization_level;   // Mathematical optimization level
    uint32_t symbolic_cache_size;              // Symbolic computation cache size
    uint32_t ast_cache_size;                   // AST evaluation cache size
    quantum_number_t optimization_threshold;   // Optimization threshold
    
    // Load balancing
    uint32_t mathematical_load;                // Mathematical computation load
    uint32_t symbolic_load;                    // Symbolic computation load
    uint32_t ast_evaluation_load;              // AST evaluation load
    quantum_number_t load_balance_factor;      // Load balancing factor
} quantix_scheduler_state_t;

// Function prototypes for enhanced mathematical scheduling

// Quantix scheduler initialization and management
int quantix_scheduler_init(void);
void quantix_scheduler_shutdown(void);
quantix_scheduler_state_t* quantix_get_scheduler_state(void);

// Enhanced process creation and management
quantix_process_t* quantix_create_process(const char *name, ast_node_t *computation_tree,
                                         qp_priority_t priority, uint32_t flags);
int quantix_start_process(quantix_process_t *process);
int quantix_terminate_process(quantix_process_t *process, quantum_number_t *exit_value);
int quantix_suspend_process(quantix_process_t *process);
int quantix_resume_process(quantix_process_t *process);

// Mathematical scheduling algorithms
quantix_process_t* quantix_schedule_mathematical_process(void);
quantix_process_t* quantix_schedule_symbolic_process(void);
quantix_process_t* quantix_schedule_ast_evaluation(void);
int quantix_calculate_mathematical_priority(quantix_process_t *process);
quantum_number_t quantix_calculate_execution_weight(quantix_process_t *process);

// AST-based process management
int quantix_set_computation_tree(quantix_process_t *process, ast_node_t *tree);
ast_node_t* quantix_get_computation_tree(quantix_process_t *process);
int quantix_evaluate_ast_node(quantix_process_t *process, ast_node_t *node);
int quantix_optimize_computation_tree(quantix_process_t *process);

// Symbolic computation support
int quantix_push_symbolic_value(quantix_process_t *process, quantum_number_t *value);
quantum_number_t* quantix_pop_symbolic_value(quantix_process_t *process);
int quantix_perform_symbolic_operation(quantix_process_t *process, const char *operation);
int quantix_defer_evaluation(quantix_process_t *process, ast_node_t *node);

// Mathematical dependency management
int quantix_add_mathematical_dependency(quantix_process_t *process, 
                                       quantix_process_t *dependency);
int quantix_remove_mathematical_dependency(quantix_process_t *process,
                                          quantix_process_t *dependency);
int quantix_resolve_mathematical_dependencies(quantix_process_t *process);
quantum_number_t quantix_calculate_dependency_weight(quantix_process_t *process);

// Mathematical synchronization
int quantix_mathematical_lock(quantix_process_t *process, quantum_number_t *key);
int quantix_mathematical_unlock(quantix_process_t *process, quantum_number_t *key);
int quantix_wait_for_mathematical_condition(quantix_process_t *process, 
                                           quantum_number_t *condition);
int quantix_signal_mathematical_condition(quantum_number_t *condition);

// Performance optimization
int quantix_optimize_mathematical_computation(quantix_process_t *process);
int quantix_cache_symbolic_result(quantix_process_t *process, ast_node_t *node, 
                                 quantum_number_t *result);
quantum_number_t* quantix_lookup_cached_result(quantix_process_t *process, ast_node_t *node);
int quantix_balance_mathematical_load(void);

// Process monitoring and statistics
void quantix_print_process_info(quantix_process_t *process);
void quantix_print_mathematical_statistics(void);
void quantix_print_ast_evaluation_stats(void);
void quantix_print_symbolic_computation_stats(void);

// Mathematical verification and integrity
int quantix_verify_process_integrity(quantix_process_t *process);
int quantix_verify_computation_tree(ast_node_t *tree);
quantum_number_t quantix_calculate_process_checksum(quantix_process_t *process);
int quantix_validate_mathematical_state(quantix_process_t *process);

// AST node management
ast_node_t* quantix_create_ast_node(ast_node_type_t type);
int quantix_destroy_ast_node(ast_node_t *node);
int quantix_set_ast_node_value(ast_node_t *node, quantum_number_t *value);
int quantix_set_ast_node_operation(ast_node_t *node, const char *operation);
int quantix_add_ast_child(ast_node_t *parent, ast_node_t *child);
uint32_t quantix_calculate_ast_complexity(ast_node_t *node);

// Mathematical error handling
typedef enum {
    QUANTIX_SUCCESS = 0,
    QUANTIX_ERROR_INVALID_PROCESS,
    QUANTIX_ERROR_INVALID_AST,
    QUANTIX_ERROR_MATHEMATICAL_ERROR,
    QUANTIX_ERROR_SYMBOLIC_ERROR,
    QUANTIX_ERROR_DEPENDENCY_CYCLE,
    QUANTIX_ERROR_OPTIMIZATION_FAILED,
    QUANTIX_ERROR_CACHE_MISS,
    QUANTIX_ERROR_SYNCHRONIZATION_FAILED,
    QUANTIX_ERROR_VERIFICATION_FAILED
} quantix_error_t;

const char* quantix_error_string(quantix_error_t error);

// Mathematical debugging support
#ifdef QUANTIX_DEBUG
void quantix_debug_process_state(quantix_process_t *process);
void quantix_debug_ast_tree(ast_node_t *node, int depth);
void quantix_debug_symbolic_stack(quantix_process_t *process);
void quantix_debug_mathematical_dependencies(quantix_process_t *process);
#endif

#endif // QUANTIX_SCHEDULER_H
