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
 * Quantum OS - Process Management System Implementation
 * 
 * Revolutionary process management with mathematical scheduling algorithms
 * designed for symbolic computation and Quantum Number operations.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 * 
 * Date: August 18, 2025
 */
#include "process.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Global process management state
static qp_process_t *process_table[QP_MAX_PROCESSES];
static qp_scheduler_state_t scheduler_state;
static uint32_t next_pid = 1;
static int process_system_initialized = 0;

// Internal helper functions
static uint32_t allocate_pid(void);
static int add_to_ready_queue(qp_process_t *process);
static int remove_from_ready_queue(qp_process_t *process);
static int add_to_blocked_queue(qp_process_t *process);
static int remove_from_blocked_queue(qp_process_t *process);
static uint32_t calculate_process_checksum(qp_process_t *process);
static uint64_t get_system_time(void);

/**
 * Initialize the process management system
 */
int qp_scheduler_init(qp_scheduler_t default_scheduler) {
    if (process_system_initialized) {
        return QP_SUCCESS;
    }
    
    // Initialize process table
    for (int i = 0; i < QP_MAX_PROCESSES; i++) {
        process_table[i] = NULL;
    }
    
    // Initialize scheduler state
    memset(&scheduler_state, 0, sizeof(qp_scheduler_state_t));
    scheduler_state.default_scheduler = default_scheduler;
    scheduler_state.quantum_time_unit = 10000; // 10ms base quantum
    scheduler_state.symbolic_boost_factor = 2;
    scheduler_state.quantum_priority_multiplier = 3;
    
    // Initialize global mathematical weight
    qn_one(&scheduler_state.global_math_weight);
    
    // Initialize ready queues
    for (int i = 0; i < 6; i++) {
        scheduler_state.ready_queues[i] = NULL;
    }
    
    scheduler_state.blocked_queue = NULL;
    scheduler_state.zombie_queue = NULL;
    scheduler_state.current_process = NULL;
    
    process_system_initialized = 1;
    return QP_SUCCESS;
}

/**
 * Create a new process
 */
qp_process_t* qp_create_process(const char *name, void *entry_point, 
                               qp_priority_t priority, uint32_t flags) {
    if (!process_system_initialized || !name || !entry_point) {
        return NULL;
    }
    
    // Allocate process structure
    qp_process_t *process = (qp_process_t*)qm_alloc(sizeof(qp_process_t), 
                                                    QM_ALLOC_KERNEL | QM_ALLOC_ZERO);
    if (!process) {
        return NULL;
    }
    
    // Allocate PID
    uint32_t pid = allocate_pid();
    if (pid == 0) {
        qm_free(process);
        return NULL;
    }
    
    // Initialize basic process information
    process->pid = pid;
    process->ppid = scheduler_state.current_process ? 
                   scheduler_state.current_process->pid : 0;
    strncpy(process->name, name, sizeof(process->name) - 1);
    process->name[sizeof(process->name) - 1] = '\0';
    
    process->state = QP_STATE_CREATED;
    process->priority = priority;
    process->flags = flags;
    process->scheduler_type = scheduler_state.default_scheduler;
    
    // Initialize CPU context
    memset(&process->cpu_context, 0, sizeof(qp_cpu_context_t));
    
    // Set entry point in program counter
    qn_zero(&process->cpu_context.qpc);
    qn_set_ordinal(&process->cpu_context.qpc, 0, (uint32_t)((uintptr_t)entry_point & 0xFFFFFFFF));
    qn_set_ordinal(&process->cpu_context.qpc, 1, (uint32_t)((uintptr_t)entry_point >> 32));
    
    // Set privilege level
    process->cpu_context.privilege_level = (flags & QP_FLAG_KERNEL) ? 0 : 3;
    
    // Initialize mathematical metrics
    qn_zero(&process->math_metrics.computation_weight);
    qn_zero(&process->math_metrics.symbolic_complexity);
    qn_zero(&process->math_metrics.ast_depth);
    qn_zero(&process->math_metrics.quantum_operations);
    process->math_metrics.cpu_time_used = 0;
    process->math_metrics.quantum_time_slice = scheduler_state.quantum_time_unit;
    process->math_metrics.mathematical_priority = 0;
    
    // Initialize timing information
    process->creation_time = get_system_time();
    process->last_scheduled = 0;
    process->cpu_time_total = 0;
    process->time_slice = scheduler_state.quantum_time_unit;
    
    // Initialize process relationships
    process->parent = scheduler_state.current_process;
    process->children = NULL;
    process->next_sibling = NULL;
    
    // Add to parent's children list
    if (process->parent) {
        process->next_sibling = process->parent->children;
        process->parent->children = process;
    }
    
    // Initialize synchronization
    process->wait_reason = 0;
    process->wait_object = NULL;
    
    // Initialize statistics
    process->context_switches = 0;
    process->page_faults = 0;
    process->quantum_operations_count = 0;
    
    // Calculate integrity checksum
    process->checksum = calculate_process_checksum(process);
    
    // Add to process table
    process_table[pid] = process;
    scheduler_state.total_processes++;
    
    return process;
}

/**
 * Start a process
 */
int qp_start_process(qp_process_t *process) {
    if (!process || process->state != QP_STATE_CREATED) {
        return QP_ERROR_INVALID_STATE;
    }
    
    // Set up default memory layout if not already configured
    if (!process->memory.code_base) {
        if (qp_setup_process_memory(process, 0x100000, 0x100000, 0x100000, 0x100000) != QP_SUCCESS) {
            return QP_ERROR_OUT_OF_MEMORY;
        }
    }
    
    // Set up quantum stack if this is a mathematical process
    if (process->flags & (QP_FLAG_MATHEMATICAL | QP_FLAG_SYMBOLIC)) {
        if (qp_setup_quantum_stack(process, 1024) != QP_SUCCESS) {
            return QP_ERROR_OUT_OF_MEMORY;
        }
    }
    
    // Change state to ready and add to ready queue
    process->state = QP_STATE_READY;
    add_to_ready_queue(process);
    scheduler_state.ready_processes++;
    
    return QP_SUCCESS;
}

/**
 * Scheduler tick - called by timer interrupt
 */
void qp_scheduler_tick(void) {
    scheduler_state.scheduler_tick++;
    
    // Update current process time slice
    if (scheduler_state.current_process) {
        scheduler_state.current_process->cpu_time_total++;
        scheduler_state.current_process->math_metrics.cpu_time_used++;
        
        // Check if time slice expired
        if (scheduler_state.current_process->cpu_time_total >= 
            scheduler_state.current_process->time_slice) {
            
            // Preempt current process
            qp_yield_cpu();
        }
        
        // Update mathematical metrics
        qp_update_math_metrics(scheduler_state.current_process);
    }
    
    // Schedule next process if needed
    if (!scheduler_state.current_process || 
        scheduler_state.current_process->state != QP_STATE_RUNNING) {
        qp_process_t *next = qp_schedule_next_process();
        if (next && next != scheduler_state.current_process) {
            qp_context_switch(scheduler_state.current_process, next);
        }
    }
}

/**
 * Schedule next process using mathematical algorithms
 */
qp_process_t* qp_schedule_next_process(void) {
    qp_process_t *best_process = NULL;
    uint32_t best_priority_score = 0;
    
    // Search ready queues from highest to lowest priority
    for (int priority = QP_PRIORITY_KERNEL; priority >= QP_PRIORITY_IDLE; priority--) {
        qp_process_t *current = scheduler_state.ready_queues[priority];
        
        while (current) {
            uint32_t priority_score = 0;
            
            // Calculate priority score based on scheduler type
            switch (current->scheduler_type) {
                case QP_SCHED_QUANTUM_FAIR:
                    priority_score = qp_calculate_math_priority(current);
                    break;
                    
                case QP_SCHED_MATHEMATICAL:
                    priority_score = current->math_metrics.mathematical_priority * 2;
                    if (current->flags & QP_FLAG_MATHEMATICAL) {
                        priority_score *= scheduler_state.quantum_priority_multiplier;
                    }
                    break;
                    
                case QP_SCHED_SYMBOLIC:
                    priority_score = current->math_metrics.mathematical_priority;
                    if (current->flags & QP_FLAG_SYMBOLIC) {
                        priority_score *= scheduler_state.symbolic_boost_factor;
                    }
                    break;
                    
                case QP_SCHED_PRIORITY:
                    priority_score = (uint32_t)current->priority * 1000;
                    break;
                    
                case QP_SCHED_ROUND_ROBIN:
                    priority_score = (scheduler_state.scheduler_tick - current->last_scheduled);
                    break;
                    
                case QP_SCHED_REAL_TIME:
                    if (current->flags & QP_FLAG_REAL_TIME) {
                        return current; // Immediate scheduling for real-time
                    }
                    priority_score = (uint32_t)current->priority * 500;
                    break;
            }
            
            // Select process with highest priority score
            if (priority_score > best_priority_score) {
                best_priority_score = priority_score;
                best_process = current;
            }
            
            current = current->next_ready;
        }
        
        // If we found a process at this priority level, use it
        if (best_process) {
            break;
        }
    }
    
    return best_process;
}

/**
 * Perform context switch between processes
 */
int qp_context_switch(qp_process_t *from, qp_process_t *to) {
    if (!to) {
        return QP_ERROR_INVALID_PROCESS;
    }
    
    // Save current process context if switching away
    if (from && from != to) {
        // Context is automatically saved by interrupt handler
        from->context_switches++;
        
        // Move from running to ready state if still runnable
        if (from->state == QP_STATE_RUNNING) {
            from->state = QP_STATE_READY;
            add_to_ready_queue(from);
            scheduler_state.ready_processes++;
        }
        scheduler_state.running_processes--;
    }
    
    // Switch to new process
    if (to->state == QP_STATE_READY) {
        remove_from_ready_queue(to);
        scheduler_state.ready_processes--;
        
        to->state = QP_STATE_RUNNING;
        to->last_scheduled = scheduler_state.scheduler_tick;
        to->context_switches++;
        
        scheduler_state.current_process = to;
        scheduler_state.running_processes++;
        
        // Calculate new time slice based on mathematical metrics
        quantum_number_t new_time_slice = qp_calculate_quantum_time_slice(to);
        to->time_slice = qn_get_ordinal(&new_time_slice, 0);
        
        // Load new process context (would be done by low-level assembly)
        // This is a placeholder for the actual context switch
        
        return QP_SUCCESS;
    }
    
    return QP_ERROR_INVALID_STATE;
}

/**
 * Calculate mathematical priority for a process
 */
int qp_calculate_math_priority(qp_process_t *process) {
    if (!process) {
        return 0;
    }
    
    uint32_t priority = (uint32_t)process->priority * 100;
    
    // Boost priority for mathematical operations
    if (process->flags & QP_FLAG_MATHEMATICAL) {
        priority += qn_get_ordinal(&process->math_metrics.computation_weight, 0) / 1000;
    }
    
    // Boost priority for symbolic computation
    if (process->flags & QP_FLAG_SYMBOLIC) {
        priority += qn_get_ordinal(&process->math_metrics.symbolic_complexity, 0) / 500;
        priority *= scheduler_state.symbolic_boost_factor;
    }
    
    // Boost priority for quantum operations
    uint32_t quantum_ops = qn_get_ordinal(&process->math_metrics.quantum_operations, 0);
    if (quantum_ops > 0) {
        priority += quantum_ops * scheduler_state.quantum_priority_multiplier;
    }
    
    // Reduce priority based on CPU time used (fairness)
    if (process->math_metrics.cpu_time_used > scheduler_state.quantum_time_unit * 10) {
        priority = priority * scheduler_state.quantum_time_unit * 10 / 
                  process->math_metrics.cpu_time_used;
    }
    
    process->math_metrics.mathematical_priority = priority;
    return priority;
}

/**
 * Calculate quantum-based time slice
 */
quantum_number_t qp_calculate_quantum_time_slice(qp_process_t *process) {
    quantum_number_t time_slice;
    qn_zero(&time_slice);
    
    if (!process) {
        return time_slice;
    }
    
    uint64_t base_slice = scheduler_state.quantum_time_unit;
    
    // Adjust based on priority
    base_slice *= ((uint32_t)process->priority + 1);
    
    // Adjust for mathematical processes
    if (process->flags & QP_FLAG_MATHEMATICAL) {
        base_slice *= 2;
    }
    
    // Adjust for symbolic processes
    if (process->flags & QP_FLAG_SYMBOLIC) {
        base_slice *= scheduler_state.symbolic_boost_factor;
    }
    
    // Adjust for real-time processes
    if (process->flags & QP_FLAG_REAL_TIME) {
        base_slice *= 4;
    }
    
    // Set the calculated time slice
    qn_set_ordinal(&time_slice, 0, (uint32_t)(base_slice & 0xFFFFFFFF));
    qn_set_ordinal(&time_slice, 1, (uint32_t)(base_slice >> 32));
    
    return time_slice;
}

/**
 * Set up process memory layout
 */
int qp_setup_process_memory(qp_process_t *process, size_t code_size,
                           size_t data_size, size_t stack_size, size_t heap_size) {
    if (!process) {
        return QP_ERROR_INVALID_PROCESS;
    }
    
    // Create process-specific memory pool
    size_t total_size = code_size + data_size + stack_size + heap_size;
    process->memory.memory_pool = qm_create_pool(QM_POOL_USER, total_size);
    if (!process->memory.memory_pool) {
        return QP_ERROR_OUT_OF_MEMORY;
    }
    
    // Allocate memory segments
    process->memory.code_base = qm_alloc_from_pool(process->memory.memory_pool, 
                                                   code_size, QM_ALLOC_USER);
    process->memory.data_base = qm_alloc_from_pool(process->memory.memory_pool, 
                                                   data_size, QM_ALLOC_USER | QM_ALLOC_ZERO);
    process->memory.stack_base = qm_alloc_from_pool(process->memory.memory_pool, 
                                                    stack_size, QM_ALLOC_USER | QM_ALLOC_ZERO);
    process->memory.heap_base = qm_alloc_from_pool(process->memory.memory_pool, 
                                                   heap_size, QM_ALLOC_USER | QM_ALLOC_ZERO);
    
    if (!process->memory.code_base || !process->memory.data_base || 
        !process->memory.stack_base || !process->memory.heap_base) {
        qp_cleanup_process_memory(process);
        return QP_ERROR_OUT_OF_MEMORY;
    }
    
    // Set memory sizes
    process->memory.code_size = code_size;
    process->memory.data_size = data_size;
    process->memory.stack_size = stack_size;
    process->memory.heap_size = heap_size;
    
    // Initialize stack pointer
    qn_zero(&process->cpu_context.qsp);
    uintptr_t stack_top = (uintptr_t)process->memory.stack_base + stack_size;
    qn_set_ordinal(&process->cpu_context.qsp, 0, (uint32_t)(stack_top & 0xFFFFFFFF));
    qn_set_ordinal(&process->cpu_context.qsp, 1, (uint32_t)(stack_top >> 32));
    
    return QP_SUCCESS;
}

/**
 * Set up quantum number stack for mathematical processes
 */
int qp_setup_quantum_stack(qp_process_t *process, size_t stack_size) {
    if (!process) {
        return QP_ERROR_INVALID_PROCESS;
    }
    
    // Allocate quantum stack
    process->quantum_stack = qm_alloc_quantum_array(stack_size);
    if (!process->quantum_stack) {
        return QP_ERROR_OUT_OF_MEMORY;
    }
    
    process->quantum_stack_size = stack_size;
    
    // Initialize quantum stack pointer
    qn_zero(&process->cpu_context.qsp);
    qn_set_ordinal(&process->cpu_context.qsp, 2, stack_size - 1); // Use ordinal 2 for quantum stack
    
    return QP_SUCCESS;
}

/**
 * Add process to ready queue
 */
static int add_to_ready_queue(qp_process_t *process) {
    if (!process || process->priority > QP_PRIORITY_KERNEL) {
        return QP_ERROR_INVALID_PRIORITY;
    }
    
    int priority_index = (int)process->priority;
    
    // Add to front of priority queue
    process->next_ready = scheduler_state.ready_queues[priority_index];
    process->prev_ready = NULL;
    
    if (scheduler_state.ready_queues[priority_index]) {
        scheduler_state.ready_queues[priority_index]->prev_ready = process;
    }
    
    scheduler_state.ready_queues[priority_index] = process;
    return QP_SUCCESS;
}

/**
 * Remove process from ready queue
 */
static int remove_from_ready_queue(qp_process_t *process) {
    if (!process) {
        return QP_ERROR_INVALID_PROCESS;
    }
    
    // Remove from linked list
    if (process->prev_ready) {
        process->prev_ready->next_ready = process->next_ready;
    } else {
        // This was the head of the queue
        int priority_index = (int)process->priority;
        scheduler_state.ready_queues[priority_index] = process->next_ready;
    }
    
    if (process->next_ready) {
        process->next_ready->prev_ready = process->prev_ready;
    }
    
    process->next_ready = NULL;
    process->prev_ready = NULL;
    
    return QP_SUCCESS;
}

/**
 * Allocate a new process ID
 */
static uint32_t allocate_pid(void) {
    for (uint32_t i = 0; i < QP_MAX_PROCESSES; i++) {
        uint32_t pid = next_pid;
        next_pid = (next_pid % QP_MAX_PROCESSES) + 1;
        
        if (process_table[pid] == NULL) {
            return pid;
        }
    }
    return 0; // No free PIDs
}

/**
 * Calculate process integrity checksum
 */
static uint32_t calculate_process_checksum(qp_process_t *process) {
    if (!process) {
        return 0;
    }
    
    uint32_t checksum = 0;
    checksum ^= process->pid;
    checksum ^= process->ppid;
    checksum ^= (uint32_t)process->state;
    checksum ^= (uint32_t)process->priority;
    checksum ^= process->flags;
    
    // Include some quantum number data
    for (int i = 0; i < 4; i++) {
        checksum ^= qn_get_ordinal(&process->math_metrics.computation_weight, i);
    }
    
    return checksum;
}

/**
 * Get system time (placeholder implementation)
 */
static uint64_t get_system_time(void) {
    // This would be implemented using actual system timer
    return scheduler_state.scheduler_tick * scheduler_state.quantum_time_unit;
}

/**
 * Find process by PID
 */
qp_process_t* qp_find_process_by_pid(uint32_t pid) {
    if (pid == 0 || pid >= QP_MAX_PROCESSES) {
        return NULL;
    }
    return process_table[pid];
}

/**
 * Get current running process
 */
qp_process_t* qp_get_current_process(void) {
    return scheduler_state.current_process;
}

/**
 * Yield CPU to next process
 */
int qp_yield_cpu(void) {
    if (!scheduler_state.current_process) {
        return QP_ERROR_INVALID_PROCESS;
    }
    
    qp_process_t *next = qp_schedule_next_process();
    if (next && next != scheduler_state.current_process) {
        return qp_context_switch(scheduler_state.current_process, next);
    }
    
    return QP_SUCCESS;
}

/**
 * Update mathematical metrics for a process
 */
int qp_update_math_metrics(qp_process_t *process) {
    if (!process) {
        return QP_ERROR_INVALID_PROCESS;
    }
    
    // Update computation weight based on recent activity
    if (process->flags & QP_FLAG_MATHEMATICAL) {
        quantum_number_t increment;
        qn_one(&increment);
        qn_add(&process->math_metrics.computation_weight, 
               &process->math_metrics.computation_weight, &increment);
    }
    
    // Update quantum operations count
    if (process->quantum_operations_count > 0) {
        qn_set_ordinal(&process->math_metrics.quantum_operations, 0, 
                      process->quantum_operations_count);
    }
    
    // Recalculate mathematical priority
    qp_calculate_math_priority(process);
    
    return QP_SUCCESS;
}

/**
 * Get error string
 */
const char* qp_error_string(qp_error_t error) {
    switch (error) {
        case QP_SUCCESS: return "Success";
        case QP_ERROR_INVALID_PROCESS: return "Invalid process";
        case QP_ERROR_INVALID_STATE: return "Invalid state";
        case QP_ERROR_OUT_OF_MEMORY: return "Out of memory";
        case QP_ERROR_PERMISSION_DENIED: return "Permission denied";
        case QP_ERROR_PROCESS_NOT_FOUND: return "Process not found";
        case QP_ERROR_INVALID_PRIORITY: return "Invalid priority";
        case QP_ERROR_SCHEDULER_ERROR: return "Scheduler error";
        case QP_ERROR_CONTEXT_SWITCH_FAILED: return "Context switch failed";
        default: return "Unknown error";
    }
}

/**
 * Print process information
 */
void qp_print_process_info(qp_process_t *process) {
    if (!process) {
        printf("Invalid process\n");
        return;
    }
    
    printf("=== Process Information ===\n");
    printf("PID: %u\n", process->pid);
    printf("PPID: %u\n", process->ppid);
    printf("Name: %s\n", process->name);
    printf("State: %d\n", (int)process->state);
    printf("Priority: %d\n", (int)process->priority);
    printf("Flags: 0x%08X\n", process->flags);
    printf("CPU Time: %llu\n", process->cpu_time_total);
    printf("Context Switches: %llu\n", process->context_switches);
    printf("Quantum Operations: %llu\n", process->quantum_operations_count);
    printf("Mathematical Priority: %u\n", process->math_metrics.mathematical_priority);
    printf("===========================\n");
}

/**
 * Print scheduler statistics
 */
void qp_print_scheduler_stats(void) {
    printf("=== Quantum OS Scheduler Statistics ===\n");
    printf("Total Processes: %u\n", scheduler_state.total_processes);
    printf("Running Processes: %u\n", scheduler_state.running_processes);
    printf("Ready Processes: %u\n", scheduler_state.ready_processes);
    printf("Blocked Processes: %u\n", scheduler_state.blocked_processes);
    printf("Scheduler Ticks: %llu\n", scheduler_state.scheduler_tick);
    printf("Quantum Time Unit: %llu\n", scheduler_state.quantum_time_unit);
    printf("Current Process: %s (PID %u)\n", 
           scheduler_state.current_process ? scheduler_state.current_process->name : "None",
           scheduler_state.current_process ? scheduler_state.current_process->pid : 0);
    printf("=======================================\n");
}
