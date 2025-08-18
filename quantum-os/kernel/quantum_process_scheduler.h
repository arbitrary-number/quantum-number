/**
 * Quantum Process Scheduler - Advanced Mathematical Process Management
 * 
 * Revolutionary process scheduler that uses quantum mechanics principles
 * and mathematical complexity analysis for optimal process scheduling.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_PROCESS_SCHEDULER_H
#define QUANTUM_PROCESS_SCHEDULER_H

#include "quantum_number.h"
#include "quantum_memory_manager.h"
#include "quantix_qfs.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Quantum process states
typedef enum {
    QPS_STATE_CREATED,              // Process created but not started
    QPS_STATE_READY,                // Ready to run
    QPS_STATE_RUNNING,              // Currently executing
    QPS_STATE_WAITING,              // Waiting for resource
    QPS_STATE_MATHEMATICAL_COMPUTE, // Performing mathematical computation
    QPS_STATE_SYMBOLIC_EVALUATION,  // Evaluating symbolic expressions
    QPS_STATE_QUANTUM_OPERATION,    // Executing quantum number operations
    QPS_STATE_IO_WAIT,              // Waiting for I/O operation
    QPS_STATE_MEMORY_WAIT,          // Waiting for memory allocation
    QPS_STATE_SUSPENDED,            // Suspended by system
    QPS_STATE_TERMINATED            // Process terminated
} qps_process_state_t;

// Mathematical process types
typedef enum {
    QPS_TYPE_GENERAL = 0x01,        // General purpose process
    QPS_TYPE_MATHEMATICAL = 0x02,   // Mathematical computation process
    QPS_TYPE_SYMBOLIC = 0x04,       // Symbolic computation process
    QPS_TYPE_QUANTUM_NUMBER = 0x08, // Quantum Number operations
    QPS_TYPE_AST_EVALUATION = 0x10, // AST evaluation process
    QPS_TYPE_PROOF_VERIFICATION = 0x20, // Mathematical proof verification
    QPS_TYPE_PATTERN_MATCHING = 0x40,   // Pattern matching process
    QPS_TYPE_OPTIMIZATION = 0x80,   // Mathematical optimization
    QPS_TYPE_REAL_TIME = 0x100,     // Real-time mathematical process
    QPS_TYPE_INTERACTIVE = 0x200    // Interactive mathematical session
} qps_process_type_t;

// Quantum scheduling algorithms
typedef enum {
    QPS_SCHED_QUANTUM_FAIR,         // Quantum-aware fair scheduling
    QPS_SCHED_MATHEMATICAL_PRIORITY, // Priority based on mathematical complexity
    QPS_SCHED_SYMBOLIC_FIRST,       // Symbolic computations get priority
    QPS_SCHED_COMPLEXITY_BALANCED,  // Balance based on computational complexity
    QPS_SCHED_REAL_TIME_MATH,       // Real-time mathematical scheduling
    QPS_SCHED_ADAPTIVE_QUANTUM,     // Adaptive quantum scheduling
    QPS_SCHED_DEPENDENCY_AWARE,     // Mathematical dependency-aware scheduling
    QPS_SCHED_ENERGY_EFFICIENT      // Energy-efficient mathematical scheduling
} qps_scheduling_algorithm_t;

// Mathematical process priority
typedef struct qps_mathematical_priority {
    uint32_t base_priority;         // Base process priority
    uint32_t mathematical_weight;   // Weight based on mathematical importance
    uint32_t complexity_factor;     // Factor based on computational complexity
    uint32_t symbolic_bonus;        // Bonus for symbolic computations
    uint32_t quantum_bonus;         // Bonus for quantum number operations
    uint32_t real_time_boost;       // Real-time priority boost
    uint32_t dependency_penalty;    // Penalty for dependency waiting
    uint32_t memory_pressure_penalty; // Penalty for high memory usage
} qps_mathematical_priority_t;

// Quantum process context
typedef struct qps_quantum_context {
    // Mathematical computation state
    quantum_number_t *quantum_registers; // Quantum number registers
    uint32_t register_count;        // Number of quantum registers
    ast_node_t *current_expression; // Currently evaluating expression
    qfs_computation_context_t *computation_context; // Computation context
    
    // Symbolic computation state
    void *symbolic_stack;           // Symbolic computation stack
    size_t stack_size;              // Stack size
    uint32_t evaluation_depth;      // Current evaluation depth
    uint32_t max_evaluation_depth;  // Maximum allowed depth
    
    // Mathematical memory context
    void **mathematical_allocations; // Mathematical memory allocations
    uint32_t allocation_count;      // Number of allocations
    size_t total_mathematical_memory; // Total mathematical memory used
    
    // Computation statistics
    uint64_t quantum_operations_performed; // Quantum operations count
    uint64_t symbolic_evaluations;  // Symbolic evaluations count
    uint64_t ast_traversals;        // AST traversal count
    uint64_t mathematical_cache_hits; // Cache hit count
    uint64_t mathematical_cache_misses; // Cache miss count
    
    // Performance metrics
    uint64_t computation_start_time; // When computation started
    uint64_t total_computation_time; // Total computation time
    uint32_t average_operation_time; // Average operation time
    uint32_t peak_memory_usage;     // Peak memory usage
} qps_quantum_context_t;

// Mathematical dependency structure
typedef struct qps_mathematical_dependency {
    uint32_t process_id;            // Dependent process ID
    quantum_number_t dependency_id; // Mathematical dependency identifier
    char dependency_description[128]; // Human-readable description
    bool is_satisfied;              // Whether dependency is satisfied
    uint64_t wait_start_time;       // When waiting started
    uint32_t timeout_ms;            // Dependency timeout
    
    // Dependency type
    enum {
        QPS_DEP_COMPUTATION_RESULT, // Waiting for computation result
        QPS_DEP_MATHEMATICAL_PROOF, // Waiting for proof verification
        QPS_DEP_SYMBOLIC_EVALUATION, // Waiting for symbolic evaluation
        QPS_DEP_QUANTUM_OPERATION,  // Waiting for quantum operation
        QPS_DEP_MEMORY_ALLOCATION,  // Waiting for memory allocation
        QPS_DEP_FILE_OPERATION      // Waiting for file operation
    } dependency_type;
} qps_mathematical_dependency_t;

// Quantum process control block
typedef struct qps_process_control_block {
    // Basic process information
    uint32_t process_id;            // Unique process identifier
    uint32_t parent_process_id;     // Parent process ID
    char process_name[64];          // Process name
    qps_process_state_t state;      // Current process state
    qps_process_type_t type;        // Process type
    
    // Mathematical priority and scheduling
    qps_mathematical_priority_t priority; // Mathematical priority
    uint32_t time_quantum;          // Time quantum for scheduling
    uint64_t cpu_time_used;         // Total CPU time used
    uint64_t last_scheduled_time;   // Last time process was scheduled
    uint32_t scheduling_count;      // Number of times scheduled
    
    // Quantum context
    qps_quantum_context_t *quantum_context; // Quantum computation context
    
    // Mathematical dependencies
    qps_mathematical_dependency_t *dependencies; // Process dependencies
    uint32_t dependency_count;      // Number of dependencies
    uint32_t satisfied_dependencies; // Number of satisfied dependencies
    
    // Memory management
    void *memory_base;              // Process memory base
    size_t memory_size;             // Process memory size
    void *mathematical_heap;        // Mathematical heap pointer
    size_t mathematical_heap_size;  // Mathematical heap size
    
    // File system context
    qfs_file_handle_t *open_files;  // Open mathematical files
    uint32_t open_file_count;       // Number of open files
    char working_directory[256];    // Current working directory
    
    // Performance and statistics
    struct {
        uint64_t context_switches;  // Number of context switches
        uint64_t page_faults;       // Page fault count
        uint64_t mathematical_operations; // Mathematical operations performed
        uint64_t quantum_computations; // Quantum computations performed
        uint64_t symbolic_evaluations; // Symbolic evaluations performed
        uint64_t memory_allocations; // Memory allocations
        uint64_t file_operations;   // File operations performed
        uint32_t average_response_time; // Average response time
        uint32_t peak_memory_usage; // Peak memory usage
    } statistics;
    
    // Process relationships
    struct qps_process_control_block *parent; // Parent process
    struct qps_process_control_block **children; // Child processes
    uint32_t child_count;           // Number of child processes
    
    // Scheduling metadata
    struct qps_process_control_block *next_in_queue; // Next in scheduling queue
    struct qps_process_control_block *prev_in_queue; // Previous in scheduling queue
    uint64_t queue_entry_time;      // When entered current queue
    uint32_t queue_priority;        // Priority within queue
} qps_process_control_block_t;

// Quantum scheduler state
typedef struct qps_scheduler_state {
    // Scheduling algorithm and configuration
    qps_scheduling_algorithm_t algorithm; // Current scheduling algorithm
    uint32_t default_time_quantum;  // Default time quantum
    uint32_t mathematical_time_quantum; // Time quantum for mathematical processes
    uint32_t real_time_time_quantum; // Time quantum for real-time processes
    
    // Process queues
    qps_process_control_block_t *ready_queue; // Ready process queue
    qps_process_control_block_t *mathematical_queue; // Mathematical process queue
    qps_process_control_block_t *symbolic_queue; // Symbolic computation queue
    qps_process_control_block_t *real_time_queue; // Real-time process queue
    qps_process_control_block_t *waiting_queue; // Waiting process queue
    
    // Currently running process
    qps_process_control_block_t *current_process; // Currently executing process
    uint64_t current_process_start_time; // When current process started
    uint32_t remaining_time_quantum; // Remaining time quantum
    
    // Scheduler statistics
    struct {
        uint64_t total_processes_created; // Total processes created
        uint64_t total_processes_terminated; // Total processes terminated
        uint32_t current_process_count; // Current number of processes
        uint32_t peak_process_count;    // Peak number of processes
        uint64_t total_context_switches; // Total context switches
        uint64_t total_scheduling_decisions; // Total scheduling decisions
        uint32_t average_response_time; // Average response time
        uint32_t average_turnaround_time; // Average turnaround time
        uint32_t mathematical_process_count; // Mathematical processes
        uint32_t symbolic_process_count; // Symbolic computation processes
        uint32_t quantum_process_count; // Quantum number processes
    } statistics;
    
    // Mathematical computation tracking
    struct {
        uint64_t total_quantum_operations; // Total quantum operations
        uint64_t total_symbolic_evaluations; // Total symbolic evaluations
        uint64_t total_ast_traversals;  // Total AST traversals
        uint64_t total_mathematical_memory; // Total mathematical memory used
        uint32_t active_mathematical_computations; // Active computations
        uint32_t queued_mathematical_computations; // Queued computations
    } mathematical_statistics;
    
    // Performance optimization
    struct {
        bool adaptive_time_quantum;    // Adaptive time quantum enabled
        bool mathematical_priority_boost; // Mathematical priority boost
        bool dependency_aware_scheduling; // Dependency-aware scheduling
        bool energy_efficient_scheduling; // Energy-efficient scheduling
        uint32_t load_balancing_threshold; // Load balancing threshold
        uint32_t context_switch_overhead; // Context switch overhead estimate
    } optimization_settings;
} qps_scheduler_state_t;

// Quantum scheduler result codes
typedef enum {
    QPS_SUCCESS = 0,
    QPS_ERROR_INVALID_PROCESS = -1,
    QPS_ERROR_INVALID_PARAMETER = -2,
    QPS_ERROR_OUT_OF_MEMORY = -3,
    QPS_ERROR_PROCESS_NOT_FOUND = -4,
    QPS_ERROR_DEPENDENCY_CYCLE = -5,
    QPS_ERROR_MATHEMATICAL_ERROR = -6,
    QPS_ERROR_SCHEDULING_CONFLICT = -7,
    QPS_ERROR_RESOURCE_EXHAUSTED = -8,
    QPS_ERROR_TIMEOUT = -9,
    QPS_ERROR_PERMISSION_DENIED = -10
} qps_result_t;

// Core Quantum Process Scheduler API

// Scheduler initialization and management
qps_result_t qps_initialize_scheduler(qps_scheduling_algorithm_t algorithm);
qps_result_t qps_shutdown_scheduler(void);
qps_result_t qps_configure_scheduler(qps_scheduling_algorithm_t algorithm,
                                    uint32_t default_time_quantum);

// Process creation and management
qps_result_t qps_create_process(const char *process_name,
                               qps_process_type_t type,
                               qps_mathematical_priority_t *priority,
                               qps_process_control_block_t **process);
qps_result_t qps_start_process(qps_process_control_block_t *process);
qps_result_t qps_terminate_process(uint32_t process_id);
qps_result_t qps_suspend_process(uint32_t process_id);
qps_result_t qps_resume_process(uint32_t process_id);

// Mathematical process operations
qps_result_t qps_create_mathematical_process(const char *process_name,
                                            qps_process_type_t mathematical_type,
                                            qfs_computation_context_t *computation_context,
                                            qps_process_control_block_t **process);
qps_result_t qps_assign_quantum_context(qps_process_control_block_t *process,
                                       uint32_t register_count,
                                       size_t stack_size);
qps_result_t qps_set_mathematical_priority(uint32_t process_id,
                                          qps_mathematical_priority_t *priority);

// Process scheduling
qps_result_t qps_schedule_next_process(void);
qps_result_t qps_yield_process(void);
qps_result_t qps_preempt_current_process(void);
qps_result_t qps_boost_mathematical_processes(void);

// Mathematical dependency management
qps_result_t qps_add_mathematical_dependency(uint32_t process_id,
                                            qps_mathematical_dependency_t *dependency);
qps_result_t qps_satisfy_mathematical_dependency(quantum_number_t dependency_id,
                                                void *result_data);
qps_result_t qps_check_dependency_satisfaction(uint32_t process_id,
                                              bool *all_satisfied);
qps_result_t qps_resolve_dependency_cycles(void);

// Process communication and synchronization
qps_result_t qps_create_mathematical_channel(uint32_t source_process_id,
                                            uint32_t target_process_id,
                                            void **channel_handle);
qps_result_t qps_send_mathematical_message(void *channel_handle,
                                          quantum_number_t *message_data,
                                          size_t data_size);
qps_result_t qps_receive_mathematical_message(void *channel_handle,
                                             quantum_number_t *message_buffer,
                                             size_t buffer_size,
                                             size_t *received_size);

// Process monitoring and statistics
qps_result_t qps_get_process_statistics(uint32_t process_id,
                                       struct {
                                           uint64_t cpu_time_used;
                                           uint64_t memory_used;
                                           uint64_t mathematical_operations;
                                           uint64_t quantum_computations;
                                           uint32_t context_switches;
                                           uint32_t average_response_time;
                                       } *statistics);
qps_result_t qps_get_scheduler_statistics(qps_scheduler_state_t *scheduler_stats);
qps_result_t qps_get_mathematical_statistics(struct {
                                                uint64_t total_quantum_operations;
                                                uint64_t total_symbolic_evaluations;
                                                uint32_t active_mathematical_processes;
                                                uint32_t average_computation_time;
                                            } *math_stats);

// Process debugging and profiling
qps_result_t qps_dump_process_state(uint32_t process_id);
qps_result_t qps_trace_mathematical_computation(uint32_t process_id,
                                               bool enable_tracing);
qps_result_t qps_profile_quantum_operations(uint32_t process_id,
                                           bool enable_profiling);
qps_result_t qps_analyze_scheduling_performance(void);

// Advanced scheduling features
qps_result_t qps_enable_adaptive_scheduling(bool enable);
qps_result_t qps_set_mathematical_priority_boost(uint32_t boost_percentage);
qps_result_t qps_configure_load_balancing(uint32_t threshold,
                                         bool enable_migration);
qps_result_t qps_optimize_for_mathematical_workload(void);

// Integration with other subsystems
qps_result_t qps_integrate_with_qmm(void);
qps_result_t qps_integrate_with_qfs(void);
qps_result_t qps_register_mathematical_interrupt_handler(void (*handler)(uint32_t));

// Utility functions
const char* qps_process_state_string(qps_process_state_t state);
const char* qps_process_type_string(qps_process_type_t type);
const char* qps_scheduling_algorithm_string(qps_scheduling_algorithm_t algorithm);
const char* qps_error_string(qps_result_t error);
qps_result_t qps_calculate_mathematical_priority(qps_process_control_block_t *process,
                                                uint32_t *calculated_priority);

// Process lifecycle callbacks
typedef struct qps_process_callbacks {
    void (*on_process_created)(qps_process_control_block_t *process);
    void (*on_process_started)(qps_process_control_block_t *process);
    void (*on_process_terminated)(qps_process_control_block_t *process);
    void (*on_mathematical_operation_complete)(qps_process_control_block_t *process,
                                              quantum_number_t *result);
    void (*on_dependency_satisfied)(qps_process_control_block_t *process,
                                   qps_mathematical_dependency_t *dependency);
    void (*on_scheduling_decision)(qps_process_control_block_t *old_process,
                                  qps_process_control_block_t *new_process);
} qps_process_callbacks_t;

qps_result_t qps_register_process_callbacks(qps_process_callbacks_t *callbacks);

#endif // QUANTUM_PROCESS_SCHEDULER_H
