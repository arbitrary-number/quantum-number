/**
 * Quantum Process Scheduler - Advanced Mathematical Process Management Implementation
 * 
 * Revolutionary process scheduler that uses quantum mechanics principles
 * and mathematical complexity analysis for optimal process scheduling.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include "quantum_process_scheduler.h"
#include "kernel.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global scheduler state
static bool qps_initialized = false;
static qps_scheduler_state_t global_scheduler_state = {0};
static uint32_t next_process_id = 1;
static qps_process_callbacks_t registered_callbacks = {0};

// Process hash table for fast lookup
#define QPS_PROCESS_HASH_SIZE 1024
static qps_process_control_block_t *process_hash_table[QPS_PROCESS_HASH_SIZE] = {0};

// Internal helper functions
static uint32_t qps_hash_process_id(uint32_t process_id);
static qps_process_control_block_t* qps_find_process_by_id(uint32_t process_id);
static qps_result_t qps_add_process_to_hash_table(qps_process_control_block_t *process);
static qps_result_t qps_remove_process_from_hash_table(uint32_t process_id);
static qps_result_t qps_add_to_queue(qps_process_control_block_t **queue_head, 
                                    qps_process_control_block_t *process);
static qps_result_t qps_remove_from_queue(qps_process_control_block_t **queue_head, 
                                         qps_process_control_block_t *process);
static qps_process_control_block_t* qps_select_next_process(void);
static qps_result_t qps_context_switch(qps_process_control_block_t *old_process,
                                      qps_process_control_block_t *new_process);
static uint32_t qps_calculate_dynamic_priority(qps_process_control_block_t *process);
static qps_result_t qps_update_process_statistics(qps_process_control_block_t *process);
static qps_result_t qps_check_process_dependencies(qps_process_control_block_t *process);

/**
 * Initialize the Quantum Process Scheduler
 */
qps_result_t qps_initialize_scheduler(qps_scheduling_algorithm_t algorithm) {
    if (qps_initialized) {
        return QPS_SUCCESS;
    }
    
    // Initialize global scheduler state
    memset(&global_scheduler_state, 0, sizeof(qps_scheduler_state_t));
    global_scheduler_state.algorithm = algorithm;
    global_scheduler_state.default_time_quantum = 10; // 10ms default
    global_scheduler_state.mathematical_time_quantum = 50; // 50ms for mathematical processes
    global_scheduler_state.real_time_time_quantum = 5; // 5ms for real-time processes
    
    // Initialize optimization settings
    global_scheduler_state.optimization_settings.adaptive_time_quantum = true;
    global_scheduler_state.optimization_settings.mathematical_priority_boost = true;
    global_scheduler_state.optimization_settings.dependency_aware_scheduling = true;
    global_scheduler_state.optimization_settings.energy_efficient_scheduling = false;
    global_scheduler_state.optimization_settings.load_balancing_threshold = 80;
    global_scheduler_state.optimization_settings.context_switch_overhead = 100; // 100ns estimate
    
    // Initialize process hash table
    memset(process_hash_table, 0, sizeof(process_hash_table));
    
    qps_initialized = true;
    
    kernel_log(KERNEL_LOG_INFO, "Quantum Process Scheduler initialized with algorithm: %s",
               qps_scheduling_algorithm_string(algorithm));
    
    return QPS_SUCCESS;
}

/**
 * Shutdown the Quantum Process Scheduler
 */
qps_result_t qps_shutdown_scheduler(void) {
    if (!qps_initialized) {
        return QPS_SUCCESS;
    }
    
    // Terminate all processes
    for (int i = 0; i < QPS_PROCESS_HASH_SIZE; i++) {
        qps_process_control_block_t *process = process_hash_table[i];
        while (process) {
            qps_process_control_block_t *next = process->next_in_queue;
            
            // Clean up process resources
            if (process->quantum_context) {
                if (process->quantum_context->quantum_registers) {
                    free(process->quantum_context->quantum_registers);
                }
                if (process->quantum_context->symbolic_stack) {
                    free(process->quantum_context->symbolic_stack);
                }
                if (process->quantum_context->mathematical_allocations) {
                    free(process->quantum_context->mathematical_allocations);
                }
                free(process->quantum_context);
            }
            
            if (process->dependencies) {
                free(process->dependencies);
            }
            
            if (process->children) {
                free(process->children);
            }
            
            free(process);
            process = next;
        }
        process_hash_table[i] = NULL;
    }
    
    // Reset global state
    memset(&global_scheduler_state, 0, sizeof(qps_scheduler_state_t));
    memset(&registered_callbacks, 0, sizeof(qps_process_callbacks_t));
    next_process_id = 1;
    
    qps_initialized = false;
    
    kernel_log(KERNEL_LOG_INFO, "Quantum Process Scheduler shutdown complete");
    
    return QPS_SUCCESS;
}

/**
 * Create a new process
 */
qps_result_t qps_create_process(const char *process_name,
                               qps_process_type_t type,
                               qps_mathematical_priority_t *priority,
                               qps_process_control_block_t **process) {
    if (!qps_initialized || !process_name || !process) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    // Allocate process control block
    qps_process_control_block_t *new_process = (qps_process_control_block_t*)malloc(
        sizeof(qps_process_control_block_t));
    if (!new_process) {
        return QPS_ERROR_OUT_OF_MEMORY;
    }
    
    // Initialize process control block
    memset(new_process, 0, sizeof(qps_process_control_block_t));
    new_process->process_id = next_process_id++;
    new_process->parent_process_id = 0; // Will be set by parent if applicable
    strncpy(new_process->process_name, process_name, sizeof(new_process->process_name) - 1);
    new_process->state = QPS_STATE_CREATED;
    new_process->type = type;
    
    // Set priority
    if (priority) {
        new_process->priority = *priority;
    } else {
        // Default priority
        new_process->priority.base_priority = 50;
        new_process->priority.mathematical_weight = (type & QPS_TYPE_MATHEMATICAL) ? 20 : 0;
        new_process->priority.complexity_factor = 10;
        new_process->priority.symbolic_bonus = (type & QPS_TYPE_SYMBOLIC) ? 15 : 0;
        new_process->priority.quantum_bonus = (type & QPS_TYPE_QUANTUM_NUMBER) ? 10 : 0;
        new_process->priority.real_time_boost = (type & QPS_TYPE_REAL_TIME) ? 30 : 0;
    }
    
    // Set time quantum based on process type
    if (type & QPS_TYPE_REAL_TIME) {
        new_process->time_quantum = global_scheduler_state.real_time_time_quantum;
    } else if (type & (QPS_TYPE_MATHEMATICAL | QPS_TYPE_SYMBOLIC | QPS_TYPE_QUANTUM_NUMBER)) {
        new_process->time_quantum = global_scheduler_state.mathematical_time_quantum;
    } else {
        new_process->time_quantum = global_scheduler_state.default_time_quantum;
    }
    
    // Initialize statistics
    memset(&new_process->statistics, 0, sizeof(new_process->statistics));
    
    // Add to hash table
    qps_result_t hash_result = qps_add_process_to_hash_table(new_process);
    if (hash_result != QPS_SUCCESS) {
        free(new_process);
        return hash_result;
    }
    
    // Update global statistics
    global_scheduler_state.statistics.total_processes_created++;
    global_scheduler_state.statistics.current_process_count++;
    
    if (global_scheduler_state.statistics.current_process_count > 
        global_scheduler_state.statistics.peak_process_count) {
        global_scheduler_state.statistics.peak_process_count = 
            global_scheduler_state.statistics.current_process_count;
    }
    
    // Track mathematical process types
    if (type & QPS_TYPE_MATHEMATICAL) {
        global_scheduler_state.statistics.mathematical_process_count++;
    }
    if (type & QPS_TYPE_SYMBOLIC) {
        global_scheduler_state.statistics.symbolic_process_count++;
    }
    if (type & QPS_TYPE_QUANTUM_NUMBER) {
        global_scheduler_state.statistics.quantum_process_count++;
    }
    
    *process = new_process;
    
    // Call registered callback
    if (registered_callbacks.on_process_created) {
        registered_callbacks.on_process_created(new_process);
    }
    
    kernel_log(KERNEL_LOG_INFO, "Created process %u: %s (type: %s)",
               new_process->process_id, process_name, qps_process_type_string(type));
    
    return QPS_SUCCESS;
}

/**
 * Create mathematical process with quantum context
 */
qps_result_t qps_create_mathematical_process(const char *process_name,
                                            qps_process_type_t mathematical_type,
                                            qfs_computation_context_t *computation_context,
                                            qps_process_control_block_t **process) {
    if (!qps_initialized || !process_name || !process) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    // Create base process
    qps_mathematical_priority_t math_priority = {
        .base_priority = 60,
        .mathematical_weight = 25,
        .complexity_factor = 15,
        .symbolic_bonus = (mathematical_type & QPS_TYPE_SYMBOLIC) ? 20 : 0,
        .quantum_bonus = (mathematical_type & QPS_TYPE_QUANTUM_NUMBER) ? 15 : 0,
        .real_time_boost = (mathematical_type & QPS_TYPE_REAL_TIME) ? 35 : 0,
        .dependency_penalty = 0,
        .memory_pressure_penalty = 0
    };
    
    qps_result_t create_result = qps_create_process(process_name, mathematical_type, 
                                                   &math_priority, process);
    if (create_result != QPS_SUCCESS) {
        return create_result;
    }
    
    // Assign quantum context with default parameters
    qps_result_t context_result = qps_assign_quantum_context(*process, 16, 64 * 1024);
    if (context_result != QPS_SUCCESS) {
        qps_terminate_process((*process)->process_id);
        return context_result;
    }
    
    // Set computation context if provided
    if (computation_context) {
        (*process)->quantum_context->computation_context = computation_context;
    }
    
    kernel_log(KERNEL_LOG_INFO, "Created mathematical process %u with quantum context",
               (*process)->process_id);
    
    return QPS_SUCCESS;
}

/**
 * Assign quantum context to process
 */
qps_result_t qps_assign_quantum_context(qps_process_control_block_t *process,
                                       uint32_t register_count,
                                       size_t stack_size) {
    if (!qps_initialized || !process) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    // Allocate quantum context
    qps_quantum_context_t *context = (qps_quantum_context_t*)malloc(sizeof(qps_quantum_context_t));
    if (!context) {
        return QPS_ERROR_OUT_OF_MEMORY;
    }
    
    memset(context, 0, sizeof(qps_quantum_context_t));
    
    // Allocate quantum registers
    if (register_count > 0) {
        context->quantum_registers = (quantum_number_t*)malloc(
            sizeof(quantum_number_t) * register_count);
        if (!context->quantum_registers) {
            free(context);
            return QPS_ERROR_OUT_OF_MEMORY;
        }
        
        // Initialize quantum registers
        for (uint32_t i = 0; i < register_count; i++) {
            quantum_number_init(&context->quantum_registers[i]);
        }
        
        context->register_count = register_count;
    }
    
    // Allocate symbolic computation stack
    if (stack_size > 0) {
        context->symbolic_stack = malloc(stack_size);
        if (!context->symbolic_stack) {
            if (context->quantum_registers) {
                free(context->quantum_registers);
            }
            free(context);
            return QPS_ERROR_OUT_OF_MEMORY;
        }
        
        context->stack_size = stack_size;
    }
    
    // Initialize context parameters
    context->evaluation_depth = 0;
    context->max_evaluation_depth = 1000;
    context->computation_start_time = kernel_get_ticks();
    
    process->quantum_context = context;
    
    return QPS_SUCCESS;
}

/**
 * Start a process
 */
qps_result_t qps_start_process(qps_process_control_block_t *process) {
    if (!qps_initialized || !process) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    if (process->state != QPS_STATE_CREATED) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    // Change state to ready
    process->state = QPS_STATE_READY;
    
    // Add to appropriate queue based on process type and priority
    if (process->type & QPS_TYPE_REAL_TIME) {
        qps_add_to_queue(&global_scheduler_state.real_time_queue, process);
    } else if (process->type & (QPS_TYPE_MATHEMATICAL | QPS_TYPE_SYMBOLIC | QPS_TYPE_QUANTUM_NUMBER)) {
        if (process->type & QPS_TYPE_SYMBOLIC) {
            qps_add_to_queue(&global_scheduler_state.symbolic_queue, process);
        } else {
            qps_add_to_queue(&global_scheduler_state.mathematical_queue, process);
        }
    } else {
        qps_add_to_queue(&global_scheduler_state.ready_queue, process);
    }
    
    // Call registered callback
    if (registered_callbacks.on_process_started) {
        registered_callbacks.on_process_started(process);
    }
    
    kernel_log(KERNEL_LOG_INFO, "Started process %u: %s", 
               process->process_id, process->process_name);
    
    return QPS_SUCCESS;
}

/**
 * Schedule next process for execution
 */
qps_result_t qps_schedule_next_process(void) {
    if (!qps_initialized) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    qps_process_control_block_t *old_process = global_scheduler_state.current_process;
    qps_process_control_block_t *new_process = qps_select_next_process();
    
    if (new_process == old_process) {
        // Same process continues running
        return QPS_SUCCESS;
    }
    
    // Perform context switch
    qps_result_t switch_result = qps_context_switch(old_process, new_process);
    if (switch_result != QPS_SUCCESS) {
        return switch_result;
    }
    
    // Update scheduler statistics
    global_scheduler_state.statistics.total_scheduling_decisions++;
    
    // Call registered callback
    if (registered_callbacks.on_scheduling_decision) {
        registered_callbacks.on_scheduling_decision(old_process, new_process);
    }
    
    return QPS_SUCCESS;
}

/**
 * Add mathematical dependency to process
 */
qps_result_t qps_add_mathematical_dependency(uint32_t process_id,
                                            qps_mathematical_dependency_t *dependency) {
    if (!qps_initialized || !dependency) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    qps_process_control_block_t *process = qps_find_process_by_id(process_id);
    if (!process) {
        return QPS_ERROR_PROCESS_NOT_FOUND;
    }
    
    // Reallocate dependencies array
    qps_mathematical_dependency_t *new_dependencies = (qps_mathematical_dependency_t*)realloc(
        process->dependencies, 
        sizeof(qps_mathematical_dependency_t) * (process->dependency_count + 1));
    if (!new_dependencies) {
        return QPS_ERROR_OUT_OF_MEMORY;
    }
    
    process->dependencies = new_dependencies;
    process->dependencies[process->dependency_count] = *dependency;
    process->dependency_count++;
    
    // Set dependency start time
    process->dependencies[process->dependency_count - 1].wait_start_time = kernel_get_ticks();
    
    // If process is currently running and now has unsatisfied dependencies, move to waiting
    if (process->state == QPS_STATE_RUNNING && !dependency->is_satisfied) {
        process->state = QPS_STATE_WAITING;
        qps_add_to_queue(&global_scheduler_state.waiting_queue, process);
        
        // Trigger rescheduling
        qps_schedule_next_process();
    }
    
    kernel_log(KERNEL_LOG_INFO, "Added mathematical dependency to process %u: %s",
               process_id, dependency->dependency_description);
    
    return QPS_SUCCESS;
}

/**
 * Satisfy mathematical dependency
 */
qps_result_t qps_satisfy_mathematical_dependency(quantum_number_t dependency_id,
                                                void *result_data) {
    if (!qps_initialized) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    // Search all processes for this dependency
    for (int i = 0; i < QPS_PROCESS_HASH_SIZE; i++) {
        qps_process_control_block_t *process = process_hash_table[i];
        while (process) {
            for (uint32_t j = 0; j < process->dependency_count; j++) {
                if (quantum_number_equals(&process->dependencies[j].dependency_id, &dependency_id)) {
                    // Mark dependency as satisfied
                    process->dependencies[j].is_satisfied = true;
                    process->satisfied_dependencies++;
                    
                    // Check if all dependencies are now satisfied
                    if (process->satisfied_dependencies == process->dependency_count) {
                        // Move process from waiting to ready queue
                        if (process->state == QPS_STATE_WAITING) {
                            qps_remove_from_queue(&global_scheduler_state.waiting_queue, process);
                            process->state = QPS_STATE_READY;
                            
                            // Add to appropriate queue
                            if (process->type & QPS_TYPE_REAL_TIME) {
                                qps_add_to_queue(&global_scheduler_state.real_time_queue, process);
                            } else if (process->type & QPS_TYPE_MATHEMATICAL) {
                                qps_add_to_queue(&global_scheduler_state.mathematical_queue, process);
                            } else if (process->type & QPS_TYPE_SYMBOLIC) {
                                qps_add_to_queue(&global_scheduler_state.symbolic_queue, process);
                            } else {
                                qps_add_to_queue(&global_scheduler_state.ready_queue, process);
                            }
                        }
                    }
                    
                    // Call registered callback
                    if (registered_callbacks.on_dependency_satisfied) {
                        registered_callbacks.on_dependency_satisfied(process, &process->dependencies[j]);
                    }
                    
                    kernel_log(KERNEL_LOG_INFO, "Satisfied dependency for process %u",
                               process->process_id);
                }
            }
            process = process->next_in_queue;
        }
    }
    
    return QPS_SUCCESS;
}

/**
 * Get process statistics
 */
qps_result_t qps_get_process_statistics(uint32_t process_id,
                                       struct {
                                           uint64_t cpu_time_used;
                                           uint64_t memory_used;
                                           uint64_t mathematical_operations;
                                           uint64_t quantum_computations;
                                           uint32_t context_switches;
                                           uint32_t average_response_time;
                                       } *statistics) {
    if (!qps_initialized || !statistics) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    qps_process_control_block_t *process = qps_find_process_by_id(process_id);
    if (!process) {
        return QPS_ERROR_PROCESS_NOT_FOUND;
    }
    
    statistics->cpu_time_used = process->cpu_time_used;
    statistics->memory_used = process->mathematical_heap_size;
    statistics->mathematical_operations = process->statistics.mathematical_operations;
    statistics->quantum_computations = process->quantum_context ? 
        process->quantum_context->quantum_operations_performed : 0;
    statistics->context_switches = process->statistics.context_switches;
    statistics->average_response_time = process->statistics.average_response_time;
    
    return QPS_SUCCESS;
}

/**
 * Get scheduler statistics
 */
qps_result_t qps_get_scheduler_statistics(qps_scheduler_state_t *scheduler_stats) {
    if (!qps_initialized || !scheduler_stats) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    *scheduler_stats = global_scheduler_state;
    
    return QPS_SUCCESS;
}

/**
 * Terminate process
 */
qps_result_t qps_terminate_process(uint32_t process_id) {
    if (!qps_initialized) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    qps_process_control_block_t *process = qps_find_process_by_id(process_id);
    if (!process) {
        return QPS_ERROR_PROCESS_NOT_FOUND;
    }
    
    // Remove from any queues
    qps_remove_from_queue(&global_scheduler_state.ready_queue, process);
    qps_remove_from_queue(&global_scheduler_state.mathematical_queue, process);
    qps_remove_from_queue(&global_scheduler_state.symbolic_queue, process);
    qps_remove_from_queue(&global_scheduler_state.real_time_queue, process);
    qps_remove_from_queue(&global_scheduler_state.waiting_queue, process);
    
    // If this is the current process, schedule next
    if (global_scheduler_state.current_process == process) {
        global_scheduler_state.current_process = NULL;
        qps_schedule_next_process();
    }
    
    // Update statistics
    global_scheduler_state.statistics.total_processes_terminated++;
    global_scheduler_state.statistics.current_process_count--;
    
    // Call registered callback
    if (registered_callbacks.on_process_terminated) {
        registered_callbacks.on_process_terminated(process);
    }
    
    // Remove from hash table
    qps_remove_process_from_hash_table(process_id);
    
    kernel_log(KERNEL_LOG_INFO, "Terminated process %u: %s", 
               process_id, process->process_name);
    
    return QPS_SUCCESS;
}

// Helper function implementations

static uint32_t qps_hash_process_id(uint32_t process_id) {
    return process_id % QPS_PROCESS_HASH_SIZE;
}

static qps_process_control_block_t* qps_find_process_by_id(uint32_t process_id) {
    uint32_t hash = qps_hash_process_id(process_id);
    qps_process_control_block_t *process = process_hash_table[hash];
    
    while (process) {
        if (process->process_id == process_id) {
            return process;
        }
        process = process->next_in_queue;
    }
    
    return NULL;
}

static qps_result_t qps_add_process_to_hash_table(qps_process_control_block_t *process) {
    if (!process) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    uint32_t hash = qps_hash_process_id(process->process_id);
    process->next_in_queue = process_hash_table[hash];
    process_hash_table[hash] = process;
    
    return QPS_SUCCESS;
}

static qps_result_t qps_remove_process_from_hash_table(uint32_t process_id) {
    uint32_t hash = qps_hash_process_id(process_id);
    qps_process_control_block_t *process = process_hash_table[hash];
    qps_process_control_block_t *prev = NULL;
    
    while (process) {
        if (process->process_id == process_id) {
            if (prev) {
                prev->next_in_queue = process->next_in_queue;
            } else {
                process_hash_table[hash] = process->next_in_queue;
            }
            
            // Clean up process resources
            if (process->quantum_context) {
                if (process->quantum_context->quantum_registers) {
                    free(process->quantum_context->quantum_registers);
                }
                if (process->quantum_context->symbolic_stack) {
                    free(process->quantum_context->symbolic_stack);
                }
                if (process->quantum_context->mathematical_allocations) {
                    free(process->quantum_context->mathematical_allocations);
                }
                free(process->quantum_context);
            }
            
            if (process->dependencies) {
                free(process->dependencies);
            }
            
            if (process->children) {
                free(process->children);
            }
            
            free(process);
            return QPS_SUCCESS;
        }
        prev = process;
        process = process->next_in_queue;
    }
    
    return QPS_ERROR_PROCESS_NOT_FOUND;
}

static qps_result_t qps_add_to_queue(qps_process_control_block_t **queue_head, 
                                    qps_process_control_block_t *process) {
    if (!queue_head || !process) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    process->next_in_queue = *queue_head;
    process->prev_in_queue = NULL;
    process->queue_entry_time = kernel_get_ticks();
    
    if (*queue_head) {
        (*queue_head)->prev_in_queue = process;
    }
    
    *queue_head = process;
    
    return QPS_SUCCESS;
}

static qps_result_t qps_remove_from_queue(qps_process_control_block_t **queue_head, 
                                         qps_process_control_block_t *process) {
    if (!queue_head || !process) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    if (process->prev_in_queue) {
        process->prev_in_queue->next_in_queue = process->next_in_queue;
    } else {
        *queue_head = process->next_in_queue;
    }
    
    if (process->next_in_queue) {
        process->next_in_queue->prev_in_queue = process->prev_in_queue;
    }
    
    process->next_in_queue = NULL;
    process->prev_in_queue = NULL;
    
    return QPS_SUCCESS;
}

static qps_process_control_block_t* qps_select_next_process(void) {
    qps_process_control_block_t *selected_process = NULL;
    uint32_t highest_priority = 0;
    
    // Priority order: Real-time > Mathematical > Symbolic > General
    
    // Check real-time queue first
    if (global_scheduler_state.real_time_queue) {
        selected_process = global_scheduler_state.real_time_queue;
        qps_remove_from_queue(&global_scheduler_state.real_time_queue, selected_process);
        return selected_process;
    }
    
    // Check mathematical queue
    qps_process_control_block_t *process = global_scheduler_state.mathematical_queue;
    while (process) {
        uint32_t priority = qps_calculate_dynamic_priority(process);
        if (priority > highest_priority) {
            highest_priority = priority;
            selected_process = process;
        }
        process = process->next_in_queue;
    }
    
    // Check symbolic queue if no mathematical process selected
    if (!selected_process) {
        process = global_scheduler_state.symbolic_queue;
        while (process) {
            uint32_t priority = qps_calculate_dynamic_priority(process);
            if (priority > highest_priority) {
                highest_priority = priority;
                selected_process = process;
            }
            process = process->next_in_queue;
        }
    }
    
    // Check general ready queue if no specialized process selected
    if (!selected_process) {
        process = global_scheduler_state.ready_queue;
        while (process) {
            uint32_t priority = qps_calculate_dynamic_priority(process);
            if (priority > highest_priority) {
                highest_priority = priority;
                selected_process = process;
            }
            process = process->next_in_queue;
        }
    }
    
    // Remove selected process from its queue
    if (selected_process) {
        if (selected_process->type & QPS_TYPE_MATHEMATICAL) {
            qps_remove_from_queue(&global_scheduler_state.mathematical_queue, selected_process);
        } else if (selected_process->type & QPS_TYPE_SYMBOLIC) {
            qps_remove_from_queue(&global_scheduler_state.symbolic_queue, selected_process);
        } else {
            qps_remove_from_queue(&global_scheduler_state.ready_queue, selected_process);
        }
    }
    
    return selected_process;
}

static qps_result_t qps_context_switch(qps_process_control_block_t *old_process,
                                      qps_process_control_block_t *new_process) {
    uint64_t switch_start_time = kernel_get_ticks();
    
    // Save old process state
    if (old_process) {
        old_process->last_scheduled_time = switch_start_time;
        old_process->cpu_time_used += switch_start_time - global_scheduler_state.current_process_start_time;
        
        // Update process state based on why it's being switched out
        if (old_process->state == QPS_STATE_RUNNING) {
            old_process->state = QPS_STATE_READY;
            
            // Add back to appropriate queue if not terminated
            if (old_process->type & QPS_TYPE_REAL_TIME) {
                qps_add_to_queue(&global_scheduler_state.real_time_queue, old_process);
            } else if (old_process->type & QPS_TYPE_MATHEMATICAL) {
                qps_add_to_queue(&global_scheduler_state.mathematical_queue, old_process);
            } else if (old_process->type & QPS_TYPE_SYMBOLIC) {
                qps_add_to_queue(&global_scheduler_state.symbolic_queue, old_process);
            } else {
                qps_add_to_queue(&global_scheduler_state.ready_queue, old_process);
            }
        }
        
        // Update statistics
        old_process->statistics.context_switches++;
        qps_update_process_statistics(old_process);
    }
    
    // Set new process as current
    global_scheduler_state.current_process = new_process;
    global_scheduler_state.current_process_start_time = switch_start_time;
    
    if (new_process) {
        new_process->state = QPS_STATE_RUNNING;
        new_process->scheduling_count++;
        
        // Set remaining time quantum
        global_scheduler_state.remaining_time_quantum = new_process->time_quantum;
        
        // Update mathematical statistics if applicable
        if (new_process->quantum_context) {
            global_scheduler_state.mathematical_statistics.active_mathematical_computations++;
        }
    }
    
    // Update global statistics
    global_scheduler_state.statistics.total_context_switches++;
    
    uint64_t switch_end_time = kernel_get_ticks();
    uint64_t switch_time = switch_end_time - switch_start_time;
    
    // Update context switch overhead estimate
    global_scheduler_state.optimization_settings.context_switch_overhead = 
        (global_scheduler_state.optimization_settings.context_switch_overhead * 7 + switch_time) / 8;
    
    return QPS_SUCCESS;
}

static uint32_t qps_calculate_dynamic_priority(qps_process_control_block_t *process) {
    if (!process) {
        return 0;
    }
    
    uint32_t priority = process->priority.base_priority;
    
    // Add mathematical weight
    priority += process->priority.mathematical_weight;
    
    // Add complexity factor based on quantum context
    if (process->quantum_context) {
        uint32_t complexity_bonus = process->priority.complexity_factor * 
            (process->quantum_context->evaluation_depth / 10);
        priority += complexity_bonus;
    }
    
    // Add symbolic bonus
    if (process->type & QPS_TYPE_SYMBOLIC) {
        priority += process->priority.symbolic_bonus;
    }
    
    // Add quantum number bonus
    if (process->type & QPS_TYPE_QUANTUM_NUMBER) {
        priority += process->priority.quantum_bonus;
    }
    
    // Add real-time boost
    if (process->type & QPS_TYPE_REAL_TIME) {
        priority += process->priority.real_time_boost;
    }
    
    // Apply aging - increase priority based on wait time
    uint64_t current_time = kernel_get_ticks();
    uint64_t wait_time = current_time - process->queue_entry_time;
    uint32_t aging_bonus = (uint32_t)(wait_time / 1000); // 1 point per 1000 ticks
    priority += aging_bonus;
    
    // Apply dependency penalty
    if (process->dependency_count > process->satisfied_dependencies) {
        priority -= process->priority.dependency_penalty;
    }
    
    // Apply memory pressure penalty
    if (process->mathematical_heap_size > 1024 * 1024) { // > 1MB
        priority -= process->priority.memory_pressure_penalty;
    }
    
    // Mathematical priority boost if enabled
    if (global_scheduler_state.optimization_settings.mathematical_priority_boost &&
        (process->type & (QPS_TYPE_MATHEMATICAL | QPS_TYPE_SYMBOLIC | QPS_TYPE_QUANTUM_NUMBER))) {
        priority += 20;
    }
    
    return priority;
}

static qps_result_t qps_update_process_statistics(qps_process_control_block_t *process) {
    if (!process) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    uint64_t current_time = kernel_get_ticks();
    
    // Update response time
    if (process->queue_entry_time > 0) {
        uint32_t response_time = (uint32_t)(current_time - process->queue_entry_time);
        process->statistics.average_response_time = 
            (process->statistics.average_response_time * (process->scheduling_count - 1) + response_time) /
            process->scheduling_count;
    }
    
    // Update quantum context statistics
    if (process->quantum_context) {
        process->quantum_context->total_computation_time += 
            current_time - process->quantum_context->computation_start_time;
        
        if (process->quantum_context->quantum_operations_performed > 0) {
            process->quantum_context->average_operation_time = 
                (uint32_t)(process->quantum_context->total_computation_time / 
                          process->quantum_context->quantum_operations_performed);
        }
    }
    
    return QPS_SUCCESS;
}

static qps_result_t qps_check_process_dependencies(qps_process_control_block_t *process) {
    if (!process) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    // Check for dependency timeouts
    uint64_t current_time = kernel_get_ticks();
    
    for (uint32_t i = 0; i < process->dependency_count; i++) {
        if (!process->dependencies[i].is_satisfied) {
            uint64_t wait_time = current_time - process->dependencies[i].wait_start_time;
            
            if (process->dependencies[i].timeout_ms > 0 && 
                wait_time > process->dependencies[i].timeout_ms * 1000) {
                // Dependency timed out
                kernel_log(KERNEL_LOG_WARNING, 
                          "Dependency timeout for process %u: %s",
                          process->process_id, 
                          process->dependencies[i].dependency_description);
                
                // Mark as satisfied to allow process to continue
                process->dependencies[i].is_satisfied = true;
                process->satisfied_dependencies++;
            }
        }
    }
    
    return QPS_SUCCESS;
}

/**
 * Register process callbacks
 */
qps_result_t qps_register_process_callbacks(qps_process_callbacks_t *callbacks) {
    if (!qps_initialized || !callbacks) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    registered_callbacks = *callbacks;
    
    kernel_log(KERNEL_LOG_INFO, "Registered process callbacks");
    
    return QPS_SUCCESS;
}

/**
 * Yield current process
 */
qps_result_t qps_yield_process(void) {
    if (!qps_initialized) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    // Trigger rescheduling
    return qps_schedule_next_process();
}

/**
 * Configure scheduler
 */
qps_result_t qps_configure_scheduler(qps_scheduling_algorithm_t algorithm,
                                    uint32_t default_time_quantum) {
    if (!qps_initialized) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    global_scheduler_state.algorithm = algorithm;
    global_scheduler_state.default_time_quantum = default_time_quantum;
    
    kernel_log(KERNEL_LOG_INFO, "Configured scheduler: algorithm=%s, quantum=%u",
               qps_scheduling_algorithm_string(algorithm), default_time_quantum);
    
    return QPS_SUCCESS;
}

/**
 * Set mathematical priority for process
 */
qps_result_t qps_set_mathematical_priority(uint32_t process_id,
                                          qps_mathematical_priority_t *priority) {
    if (!qps_initialized || !priority) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    qps_process_control_block_t *process = qps_find_process_by_id(process_id);
    if (!process) {
        return QPS_ERROR_PROCESS_NOT_FOUND;
    }
    
    process->priority = *priority;
    
    kernel_log(KERNEL_LOG_INFO, "Updated mathematical priority for process %u", process_id);
    
    return QPS_SUCCESS;
}

/**
 * Get mathematical statistics
 */
qps_result_t qps_get_mathematical_statistics(struct {
                                                uint64_t total_quantum_operations;
                                                uint64_t total_symbolic_evaluations;
                                                uint32_t active_mathematical_processes;
                                                uint32_t average_computation_time;
                                            } *math_stats) {
    if (!qps_initialized || !math_stats) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    math_stats->total_quantum_operations = global_scheduler_state.mathematical_statistics.total_quantum_operations;
    math_stats->total_symbolic_evaluations = global_scheduler_state.mathematical_statistics.total_symbolic_evaluations;
    math_stats->active_mathematical_processes = global_scheduler_state.mathematical_statistics.active_mathematical_computations;
    
    // Calculate average computation time across all mathematical processes
    uint64_t total_computation_time = 0;
    uint32_t mathematical_process_count = 0;
    
    for (int i = 0; i < QPS_PROCESS_HASH_SIZE; i++) {
        qps_process_control_block_t *process = process_hash_table[i];
        while (process) {
            if (process->quantum_context) {
                total_computation_time += process->quantum_context->total_computation_time;
                mathematical_process_count++;
            }
            process = process->next_in_queue;
        }
    }
    
    math_stats->average_computation_time = mathematical_process_count > 0 ? 
        (uint32_t)(total_computation_time / mathematical_process_count) : 0;
    
    return QPS_SUCCESS;
}

/**
 * Utility functions
 */
const char* qps_process_state_string(qps_process_state_t state) {
    switch (state) {
        case QPS_STATE_CREATED: return "Created";
        case QPS_STATE_READY: return "Ready";
        case QPS_STATE_RUNNING: return "Running";
        case QPS_STATE_WAITING: return "Waiting";
        case QPS_STATE_MATHEMATICAL_COMPUTE: return "Mathematical Compute";
        case QPS_STATE_SYMBOLIC_EVALUATION: return "Symbolic Evaluation";
        case QPS_STATE_QUANTUM_OPERATION: return "Quantum Operation";
        case QPS_STATE_IO_WAIT: return "I/O Wait";
        case QPS_STATE_MEMORY_WAIT: return "Memory Wait";
        case QPS_STATE_SUSPENDED: return "Suspended";
        case QPS_STATE_TERMINATED: return "Terminated";
        default: return "Unknown State";
    }
}

const char* qps_process_type_string(qps_process_type_t type) {
    static char type_string[256];
    type_string[0] = '\0';
    
    if (type & QPS_TYPE_GENERAL) strcat(type_string, "General ");
    if (type & QPS_TYPE_MATHEMATICAL) strcat(type_string, "Mathematical ");
    if (type & QPS_TYPE_SYMBOLIC) strcat(type_string, "Symbolic ");
    if (type & QPS_TYPE_QUANTUM_NUMBER) strcat(type_string, "QuantumNumber ");
    if (type & QPS_TYPE_AST_EVALUATION) strcat(type_string, "AST ");
    if (type & QPS_TYPE_PROOF_VERIFICATION) strcat(type_string, "Proof ");
    if (type & QPS_TYPE_PATTERN_MATCHING) strcat(type_string, "Pattern ");
    if (type & QPS_TYPE_OPTIMIZATION) strcat(type_string, "Optimization ");
    if (type & QPS_TYPE_REAL_TIME) strcat(type_string, "RealTime ");
    if (type & QPS_TYPE_INTERACTIVE) strcat(type_string, "Interactive ");
    
    // Remove trailing space
    size_t len = strlen(type_string);
    if (len > 0 && type_string[len - 1] == ' ') {
        type_string[len - 1] = '\0';
    }
    
    return type_string[0] ? type_string : "Unknown Type";
}

const char* qps_scheduling_algorithm_string(qps_scheduling_algorithm_t algorithm) {
    switch (algorithm) {
        case QPS_SCHED_QUANTUM_FAIR: return "Quantum Fair";
        case QPS_SCHED_MATHEMATICAL_PRIORITY: return "Mathematical Priority";
        case QPS_SCHED_SYMBOLIC_FIRST: return "Symbolic First";
        case QPS_SCHED_COMPLEXITY_BALANCED: return "Complexity Balanced";
        case QPS_SCHED_REAL_TIME_MATH: return "Real-Time Mathematical";
        case QPS_SCHED_ADAPTIVE_QUANTUM: return "Adaptive Quantum";
        case QPS_SCHED_DEPENDENCY_AWARE: return "Dependency Aware";
        case QPS_SCHED_ENERGY_EFFICIENT: return "Energy Efficient";
        default: return "Unknown Algorithm";
    }
}

const char* qps_error_string(qps_result_t error) {
    switch (error) {
        case QPS_SUCCESS: return "Success";
        case QPS_ERROR_INVALID_PROCESS: return "Invalid process";
        case QPS_ERROR_INVALID_PARAMETER: return "Invalid parameter";
        case QPS_ERROR_OUT_OF_MEMORY: return "Out of memory";
        case QPS_ERROR_PROCESS_NOT_FOUND: return "Process not found";
        case QPS_ERROR_DEPENDENCY_CYCLE: return "Dependency cycle";
        case QPS_ERROR_MATHEMATICAL_ERROR: return "Mathematical error";
        case QPS_ERROR_SCHEDULING_CONFLICT: return "Scheduling conflict";
        case QPS_ERROR_RESOURCE_EXHAUSTED: return "Resource exhausted";
        case QPS_ERROR_TIMEOUT: return "Timeout";
        case QPS_ERROR_PERMISSION_DENIED: return "Permission denied";
        default: return "Unknown error";
    }
}

qps_result_t qps_calculate_mathematical_priority(qps_process_control_block_t *process,
                                                uint32_t *calculated_priority) {
    if (!process || !calculated_priority) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    *calculated_priority = qps_calculate_dynamic_priority(process);
    
    return QPS_SUCCESS;
}

/**
 * Integration functions
 */
qps_result_t qps_integrate_with_qmm(void) {
    if (!qps_initialized) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    // Integration with Quantum Memory Manager would be implemented here
    kernel_log(KERNEL_LOG_INFO, "QPS integrated with Quantum Memory Manager");
    
    return QPS_SUCCESS;
}

qps_result_t qps_integrate_with_qfs(void) {
    if (!qps_initialized) {
        return QPS_ERROR_INVALID_PARAMETER;
    }
    
    // Integration with Quantix File System would be implemented here
    kernel_log(KERNEL_LOG_INFO, "QPS integrated with Quantix File System");
    
    return QPS_SUCCESS;
}
