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
 * Quantum OS - Process Management System
 * 
 * Revolutionary process management with mathematical scheduling algorithms
 * designed for symbolic computation and Quantum Number operations.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 * 
 * Date: August 18, 2025
 */
#ifndef QUANTUM_PROCESS_H
#define QUANTUM_PROCESS_H

#include "quantum_number.h"
#include "memory.h"
#include <stdint.h>
#include <stddef.h>

// Maximum number of processes
#define QP_MAX_PROCESSES 1024

// Process states
typedef enum {
    QP_STATE_CREATED,       // Process created but not started
    QP_STATE_READY,         // Ready to run
    QP_STATE_RUNNING,       // Currently executing
    QP_STATE_BLOCKED,       // Blocked waiting for resource
    QP_STATE_SUSPENDED,     // Suspended by system
    QP_STATE_TERMINATED,    // Process terminated
    QP_STATE_ZOMBIE         // Terminated but not cleaned up
} qp_state_t;

// Process priority levels
typedef enum {
    QP_PRIORITY_IDLE = 0,       // Idle priority
    QP_PRIORITY_LOW = 1,        // Low priority
    QP_PRIORITY_NORMAL = 2,     // Normal priority
    QP_PRIORITY_HIGH = 3,       // High priority
    QP_PRIORITY_REALTIME = 4,   // Real-time priority
    QP_PRIORITY_KERNEL = 5      // Kernel priority
} qp_priority_t;

// Mathematical scheduling algorithms
typedef enum {
    QP_SCHED_QUANTUM_FAIR,      // Quantum Number based fair scheduling
    QP_SCHED_MATHEMATICAL,      // Mathematical relationship scheduling
    QP_SCHED_SYMBOLIC,          // Symbolic computation priority
    QP_SCHED_ROUND_ROBIN,       // Traditional round-robin
    QP_SCHED_PRIORITY,          // Priority-based scheduling
    QP_SCHED_REAL_TIME          // Real-time scheduling
} qp_scheduler_t;

// Process flags
#define QP_FLAG_KERNEL          0x01    // Kernel process
#define QP_FLAG_USER            0x02    // User process
#define QP_FLAG_MATHEMATICAL    0x04    // Mathematical computation process
#define QP_FLAG_SYMBOLIC        0x08    // Symbolic computation process
#define QP_FLAG_REAL_TIME       0x10    // Real-time process
#define QP_FLAG_SYSTEM          0x20    // System process

// CPU register context for x256 architecture
typedef struct qp_cpu_context {
    // General purpose registers (256-bit each)
    quantum_number_t qr[16];        // Quantum registers QR0-QR15
    
    // Special registers
    quantum_number_t qpc;           // Quantum Program Counter
    quantum_number_t qsp;           // Quantum Stack Pointer
    quantum_number_t qbp;           // Quantum Base Pointer
    quantum_number_t qflags;        // Quantum Flags Register
    
    // Mathematical computation registers
    quantum_number_t qmath[8];      // Mathematical operation registers
    quantum_number_t qast;          // AST root pointer
    quantum_number_t qsym;          // Symbol table pointer
    
    // Control registers
    uint32_t control_flags;         // Control flags
    uint32_t interrupt_mask;        // Interrupt mask
    uint32_t privilege_level;       // Privilege level (0=kernel, 3=user)
} qp_cpu_context_t;

// Process memory layout
typedef struct qp_memory_layout {
    void *code_base;                // Code segment base
    size_t code_size;               // Code segment size
    void *data_base;                // Data segment base
    size_t data_size;               // Data segment size
    void *stack_base;               // Stack base
    size_t stack_size;              // Stack size
    void *heap_base;                // Heap base
    size_t heap_size;               // Heap size
    void *quantum_base;             // Quantum Number space base
    size_t quantum_size;            // Quantum Number space size
    qm_pool_t *memory_pool;         // Process memory pool
} qp_memory_layout_t;

// Mathematical scheduling metrics
typedef struct qp_math_metrics {
    quantum_number_t computation_weight;   // Mathematical computation weight
    quantum_number_t symbolic_complexity;  // Symbolic expression complexity
    quantum_number_t ast_depth;           // AST tree depth
    quantum_number_t quantum_operations;  // Number of quantum operations
    uint64_t cpu_time_used;               // CPU time consumed
    uint64_t quantum_time_slice;          // Quantum-based time slice
    uint32_t mathematical_priority;       // Mathematical priority score
} qp_math_metrics_t;

// Process Control Block (PCB)
typedef struct qp_process {
    uint32_t pid;                   // Process ID
    uint32_t ppid;                  // Parent process ID
    char name[64];                  // Process name
    
    qp_state_t state;               // Current process state
    qp_priority_t priority;         // Process priority
    uint32_t flags;                 // Process flags
    
    qp_cpu_context_t cpu_context;   // CPU context
    qp_memory_layout_t memory;      // Memory layout
    qp_math_metrics_t math_metrics; // Mathematical scheduling metrics
    
    // Scheduling information
    qp_scheduler_t scheduler_type;  // Scheduler algorithm
    uint64_t time_slice;            // Time slice
    uint64_t cpu_time_total;        // Total CPU time
    uint64_t creation_time;         // Process creation time
    uint64_t last_scheduled;        // Last scheduled time
    
    // Mathematical computation state
    void *ast_root;                 // Root of AST tree
    void *symbol_table;             // Symbol table
    quantum_number_t *quantum_stack; // Quantum Number stack
    size_t quantum_stack_size;      // Stack size
    
    // Process relationships
    struct qp_process *parent;      // Parent process
    struct qp_process *children;    // Child processes
    struct qp_process *next_sibling; // Next sibling
    
    // Scheduling queues
    struct qp_process *next_ready;  // Next in ready queue
    struct qp_process *prev_ready;  // Previous in ready queue
    struct qp_process *next_blocked; // Next in blocked queue
    
    // Synchronization
    uint32_t wait_reason;           // Reason for blocking
    void *wait_object;              // Object being waited on
    
    // Statistics
    uint64_t context_switches;      // Number of context switches
    uint64_t page_faults;           // Page fault count
    uint64_t quantum_operations_count; // Quantum operations performed
    
    uint32_t checksum;              // Process integrity checksum
} qp_process_t;

// Process scheduler state
typedef struct qp_scheduler_state {
    qp_process_t *current_process;      // Currently running process
    qp_process_t *ready_queues[6];      // Ready queues by priority
    qp_process_t *blocked_queue;        // Blocked processes queue
    qp_process_t *zombie_queue;         // Zombie processes queue
    
    uint32_t total_processes;           // Total number of processes
    uint32_t running_processes;         // Number of running processes
    uint32_t ready_processes;           // Number of ready processes
    uint32_t blocked_processes;         // Number of blocked processes
    
    qp_scheduler_t default_scheduler;   // Default scheduling algorithm
    uint64_t scheduler_tick;            // Scheduler tick counter
    uint64_t quantum_time_unit;         // Base quantum time unit
    
    // Mathematical scheduling parameters
    quantum_number_t global_math_weight; // Global mathematical weight
    uint32_t symbolic_boost_factor;     // Symbolic computation boost
    uint32_t quantum_priority_multiplier; // Quantum operation priority
} qp_scheduler_state_t;

// Process management function prototypes

// Process lifecycle
qp_process_t* qp_create_process(const char *name, void *entry_point, 
                               qp_priority_t priority, uint32_t flags);
int qp_start_process(qp_process_t *process);
int qp_terminate_process(qp_process_t *process, int exit_code);
int qp_kill_process(uint32_t pid, int signal);
void qp_cleanup_zombie(qp_process_t *process);

// Process state management
int qp_set_process_state(qp_process_t *process, qp_state_t new_state);
qp_state_t qp_get_process_state(qp_process_t *process);
int qp_suspend_process(qp_process_t *process);
int qp_resume_process(qp_process_t *process);

// Process lookup and enumeration
qp_process_t* qp_find_process_by_pid(uint32_t pid);
qp_process_t* qp_find_process_by_name(const char *name);
qp_process_t* qp_get_current_process(void);
uint32_t qp_get_process_list(qp_process_t **processes, uint32_t max_count);

// Scheduling
int qp_scheduler_init(qp_scheduler_t default_scheduler);
void qp_scheduler_tick(void);
qp_process_t* qp_schedule_next_process(void);
int qp_context_switch(qp_process_t *from, qp_process_t *to);
int qp_yield_cpu(void);

// Mathematical scheduling
int qp_calculate_math_priority(qp_process_t *process);
int qp_update_math_metrics(qp_process_t *process);
int qp_boost_symbolic_priority(qp_process_t *process);
quantum_number_t qp_calculate_quantum_time_slice(qp_process_t *process);

// Process memory management
int qp_setup_process_memory(qp_process_t *process, size_t code_size,
                           size_t data_size, size_t stack_size, size_t heap_size);
int qp_cleanup_process_memory(qp_process_t *process);
void* qp_allocate_process_memory(qp_process_t *process, size_t size);
int qp_free_process_memory(qp_process_t *process, void *ptr);

// Process synchronization
int qp_block_process(qp_process_t *process, uint32_t reason, void *wait_object);
int qp_unblock_process(qp_process_t *process);
int qp_wait_for_process(qp_process_t *process, uint32_t pid);
int qp_signal_process(qp_process_t *process, uint32_t signal);

// Mathematical computation support
int qp_setup_quantum_stack(qp_process_t *process, size_t stack_size);
int qp_push_quantum_number(qp_process_t *process, quantum_number_t *qn);
quantum_number_t* qp_pop_quantum_number(qp_process_t *process);
int qp_set_ast_root(qp_process_t *process, void *ast_root);
void* qp_get_ast_root(qp_process_t *process);

// Process statistics and monitoring
void qp_print_process_info(qp_process_t *process);
void qp_print_scheduler_stats(void);
uint64_t qp_get_process_cpu_time(qp_process_t *process);
uint32_t qp_get_process_quantum_ops(qp_process_t *process);

// Error handling
typedef enum {
    QP_SUCCESS = 0,
    QP_ERROR_INVALID_PROCESS,
    QP_ERROR_INVALID_STATE,
    QP_ERROR_OUT_OF_MEMORY,
    QP_ERROR_PERMISSION_DENIED,
    QP_ERROR_PROCESS_NOT_FOUND,
    QP_ERROR_INVALID_PRIORITY,
    QP_ERROR_SCHEDULER_ERROR,
    QP_ERROR_CONTEXT_SWITCH_FAILED
} qp_error_t;

const char* qp_error_string(qp_error_t error);

// Process debugging
#ifdef QP_DEBUG
void qp_debug_process_state(qp_process_t *process);
void qp_debug_scheduler_queues(void);
void qp_debug_context_switch(qp_process_t *from, qp_process_t *to);
#endif

#endif // QUANTUM_PROCESS_H
