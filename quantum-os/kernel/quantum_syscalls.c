/**
 * Quantum System Calls - Advanced Mathematical System Call Interface Implementation
 * 
 * Revolutionary system call interface that provides native support for
 * Quantum Numbers, symbolic computation, and mathematical operations.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include "quantum_syscalls.h"
#include "kernel.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global system call state
static bool qsc_initialized = false;
static quantum_syscall_registration_t syscall_table[QSC_MAX_SYSCALL + 1] = {0};
static quantum_syscall_statistics_t global_stats = {0};
static bool caching_enabled = false;
static bool tracing_enabled = false;
static bool profiling_enabled = false;

// System call cache
#define QSC_CACHE_SIZE 1024
typedef struct qsc_cache_entry {
    quantum_syscall_params_t params;
    quantum_syscall_result_t result;
    uint64_t creation_time;
    uint32_t access_count;
    struct qsc_cache_entry *next;
} qsc_cache_entry_t;

static qsc_cache_entry_t *syscall_cache[QSC_CACHE_SIZE] = {0};
static uint32_t cache_entry_count = 0;

// Internal helper functions
static uint32_t qsc_hash_params(quantum_syscall_params_t *params);
static qsc_cache_entry_t* qsc_find_cached_result(quantum_syscall_params_t *params);
static quantum_syscall_result_t qsc_create_error_result(uint32_t error_code, const char *message);
static quantum_syscall_result_t qsc_create_success_result(void);
static bool qsc_params_equal(quantum_syscall_params_t *params1, quantum_syscall_params_t *params2);
static void qsc_update_statistics(quantum_syscall_number_t syscall_number, 
                                 uint64_t execution_time, bool success);

/**
 * Initialize the Quantum System Call interface
 */
int qsc_initialize_syscalls(void) {
    if (qsc_initialized) {
        return QSC_SUCCESS;
    }
    
    // Initialize syscall table
    memset(syscall_table, 0, sizeof(syscall_table));
    
    // Initialize statistics
    memset(&global_stats, 0, sizeof(quantum_syscall_statistics_t));
    
    // Initialize cache
    memset(syscall_cache, 0, sizeof(syscall_cache));
    cache_entry_count = 0;
    
    // Register core system calls
    quantum_syscall_registration_t quantum_create_reg = {
        .syscall_number = QSC_QUANTUM_CREATE,
        .handler = qsc_handle_quantum_create,
        .name = "quantum_create",
        .description = "Create a new Quantum Number",
        .min_params = 0,
        .max_params = 1,
        .required_permissions = 0,
        .is_mathematical = true,
        .is_real_time_safe = true,
        .average_execution_time_ns = 1000
    };
    qsc_register_syscall(&quantum_create_reg);
    
    quantum_syscall_registration_t quantum_add_reg = {
        .syscall_number = QSC_QUANTUM_ADD,
        .handler = qsc_handle_quantum_add,
        .name = "quantum_add",
        .description = "Add two Quantum Numbers",
        .min_params = 3,
        .max_params = 3,
        .required_permissions = 0,
        .is_mathematical = true,
        .is_real_time_safe = true,
        .average_execution_time_ns = 2000
    };
    qsc_register_syscall(&quantum_add_reg);
    
    quantum_syscall_registration_t ast_evaluate_reg = {
        .syscall_number = QSC_AST_EVALUATE,
        .handler = qsc_handle_ast_evaluate,
        .name = "ast_evaluate",
        .description = "Evaluate AST expression",
        .min_params = 2,
        .max_params = 3,
        .required_permissions = 0,
        .is_mathematical = true,
        .is_real_time_safe = false,
        .average_execution_time_ns = 10000
    };
    qsc_register_syscall(&ast_evaluate_reg);
    
    quantum_syscall_registration_t qmm_alloc_reg = {
        .syscall_number = QSC_QMM_ALLOC,
        .handler = qsc_handle_qmm_alloc,
        .name = "qmm_alloc",
        .description = "Allocate mathematical memory",
        .min_params = 2,
        .max_params = 3,
        .required_permissions = 0,
        .is_mathematical = true,
        .is_real_time_safe = true,
        .average_execution_time_ns = 5000
    };
    qsc_register_syscall(&qmm_alloc_reg);
    
    qsc_initialized = true;
    
    kernel_log(KERNEL_LOG_INFO, "Quantum System Call interface initialized");
    
    return QSC_SUCCESS;
}

/**
 * Shutdown the Quantum System Call interface
 */
int qsc_shutdown_syscalls(void) {
    if (!qsc_initialized) {
        return QSC_SUCCESS;
    }
    
    // Clear cache
    qsc_clear_syscall_cache();
    
    // Reset syscall table
    memset(syscall_table, 0, sizeof(syscall_table));
    
    // Reset statistics
    memset(&global_stats, 0, sizeof(quantum_syscall_statistics_t));
    
    qsc_initialized = false;
    
    kernel_log(KERNEL_LOG_INFO, "Quantum System Call interface shutdown");
    
    return QSC_SUCCESS;
}

/**
 * Register a system call
 */
int qsc_register_syscall(quantum_syscall_registration_t *registration) {
    if (!qsc_initialized || !registration) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    if (registration->syscall_number > QSC_MAX_SYSCALL) {
        return QSC_ERROR_INVALID_SYSCALL;
    }
    
    // Check if syscall is already registered
    if (syscall_table[registration->syscall_number].handler != NULL) {
        return QSC_ERROR_INVALID_SYSCALL;
    }
    
    syscall_table[registration->syscall_number] = *registration;
    
    kernel_log(KERNEL_LOG_INFO, "Registered system call %u: %s", 
               registration->syscall_number, registration->name);
    
    return QSC_SUCCESS;
}

/**
 * Execute a system call
 */
quantum_syscall_result_t qsc_execute_syscall(quantum_syscall_params_t *params) {
    if (!qsc_initialized || !params) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid parameters");
    }
    
    if (params->syscall_number > QSC_MAX_SYSCALL) {
        return qsc_create_error_result(QSC_ERROR_INVALID_SYSCALL, "Invalid system call number");
    }
    
    quantum_syscall_registration_t *registration = &syscall_table[params->syscall_number];
    if (!registration->handler) {
        return qsc_create_error_result(QSC_ERROR_NOT_IMPLEMENTED, "System call not implemented");
    }
    
    // Validate parameters
    if (qsc_validate_syscall_params(params) != QSC_SUCCESS) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Parameter validation failed");
    }
    
    // Check cache first if enabled
    if (caching_enabled) {
        qsc_cache_entry_t *cached = qsc_find_cached_result(params);
        if (cached) {
            cached->access_count++;
            global_stats.cached_results++;
            return cached->result;
        }
    }
    
    // Execute system call
    uint64_t start_time = kernel_get_ticks();
    
    if (tracing_enabled) {
        kernel_log(KERNEL_LOG_DEBUG, "Executing syscall %u: %s", 
                   params->syscall_number, registration->name);
    }
    
    quantum_syscall_result_t result = registration->handler(params);
    
    uint64_t end_time = kernel_get_ticks();
    uint64_t execution_time = end_time - start_time;
    
    // Update result with execution metrics
    result.execution_time_ns = execution_time;
    
    // Update statistics
    qsc_update_statistics(params->syscall_number, execution_time, 
                         result.error_code == QSC_SUCCESS);
    
    // Cache result if successful and caching is enabled
    if (caching_enabled && result.error_code == QSC_SUCCESS && 
        (params->execution_flags & QSC_FLAG_CACHEABLE)) {
        qsc_cache_syscall_result(params, &result);
    }
    
    if (tracing_enabled) {
        kernel_log(KERNEL_LOG_DEBUG, "Syscall %u completed in %llu ns, result: %d", 
                   params->syscall_number, execution_time, result.error_code);
    }
    
    return result;
}

/**
 * Validate system call parameters
 */
int qsc_validate_syscall_params(quantum_syscall_params_t *params) {
    if (!params) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    if (params->syscall_number > QSC_MAX_SYSCALL) {
        return QSC_ERROR_INVALID_SYSCALL;
    }
    
    quantum_syscall_registration_t *registration = &syscall_table[params->syscall_number];
    if (!registration->handler) {
        return QSC_ERROR_NOT_IMPLEMENTED;
    }
    
    // Check parameter count
    if (params->param_count < registration->min_params || 
        params->param_count > registration->max_params) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    // Additional validation could be added here
    
    return QSC_SUCCESS;
}

/**
 * Cache system call result
 */
int qsc_cache_syscall_result(quantum_syscall_params_t *params,
                            quantum_syscall_result_t *result) {
    if (!params || !result || !caching_enabled) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    // Check cache size limit
    if (cache_entry_count >= QSC_CACHE_SIZE) {
        return QSC_ERROR_OUT_OF_MEMORY;
    }
    
    // Create cache entry
    qsc_cache_entry_t *entry = (qsc_cache_entry_t*)malloc(sizeof(qsc_cache_entry_t));
    if (!entry) {
        return QSC_ERROR_OUT_OF_MEMORY;
    }
    
    entry->params = *params;
    entry->result = *result;
    entry->creation_time = kernel_get_ticks();
    entry->access_count = 1;
    entry->next = NULL;
    
    // Add to cache hash table
    uint32_t hash = qsc_hash_params(params) % QSC_CACHE_SIZE;
    entry->next = syscall_cache[hash];
    syscall_cache[hash] = entry;
    cache_entry_count++;
    
    return QSC_SUCCESS;
}

/**
 * Lookup cached result
 */
int qsc_lookup_cached_result(quantum_syscall_params_t *params,
                            quantum_syscall_result_t *result,
                            bool *found) {
    if (!params || !result || !found) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    *found = false;
    
    if (!caching_enabled) {
        return QSC_SUCCESS;
    }
    
    qsc_cache_entry_t *entry = qsc_find_cached_result(params);
    if (entry) {
        *result = entry->result;
        entry->access_count++;
        *found = true;
    }
    
    return QSC_SUCCESS;
}

/**
 * Clear system call cache
 */
int qsc_clear_syscall_cache(void) {
    for (int i = 0; i < QSC_CACHE_SIZE; i++) {
        qsc_cache_entry_t *entry = syscall_cache[i];
        while (entry) {
            qsc_cache_entry_t *next = entry->next;
            free(entry);
            entry = next;
        }
        syscall_cache[i] = NULL;
    }
    
    cache_entry_count = 0;
    
    return QSC_SUCCESS;
}

/**
 * Get system call statistics
 */
int qsc_get_syscall_statistics(quantum_syscall_statistics_t *stats) {
    if (!qsc_initialized || !stats) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    *stats = global_stats;
    
    return QSC_SUCCESS;
}

/**
 * Enable/disable system call caching
 */
int qsc_enable_syscall_caching(bool enable) {
    if (!qsc_initialized) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    caching_enabled = enable;
    
    if (!enable) {
        qsc_clear_syscall_cache();
    }
    
    kernel_log(KERNEL_LOG_INFO, "System call caching %s", enable ? "enabled" : "disabled");
    
    return QSC_SUCCESS;
}

/**
 * Enable/disable system call tracing
 */
int qsc_enable_syscall_tracing(bool enable) {
    if (!qsc_initialized) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    tracing_enabled = enable;
    
    kernel_log(KERNEL_LOG_INFO, "System call tracing %s", enable ? "enabled" : "disabled");
    
    return QSC_SUCCESS;
}

// System call handler implementations

/**
 * Handle quantum_create system call
 */
quantum_syscall_result_t qsc_handle_quantum_create(quantum_syscall_params_t *params) {
    quantum_syscall_result_t result = qsc_create_success_result();
    
    // Allocate new quantum number
    quantum_number_t *qn = (quantum_number_t*)qmm_alloc_quantum_number(1);
    if (!qn) {
        return qsc_create_error_result(QSC_ERROR_OUT_OF_MEMORY, "Failed to allocate quantum number");
    }
    
    // Initialize quantum number
    quantum_number_init(qn);
    
    // If initial value provided, set it
    if (params->param_count > 0 && params->param_types[0] == QSC_PARAM_UINT64) {
        quantum_number_set_from_uint64(qn, params->params[0].uint_param);
    }
    
    result.extended_result.pointer_result = qn;
    result.return_value = (int64_t)qn;
    
    kernel_track_mathematical_operation();
    
    return result;
}

/**
 * Handle quantum_add system call
 */
quantum_syscall_result_t qsc_handle_quantum_add(quantum_syscall_params_t *params) {
    if (params->param_count < 3) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    quantum_number_t *a = (quantum_number_t*)params->params[0].ptr_param;
    quantum_number_t *b = (quantum_number_t*)params->params[1].ptr_param;
    quantum_number_t *result_qn = (quantum_number_t*)params->params[2].ptr_param;
    
    if (!a || !b || !result_qn) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid quantum number pointers");
    }
    
    // Perform addition
    int add_result = quantum_number_add(a, b, result_qn);
    if (add_result != 0) {
        return qsc_create_error_result(QSC_ERROR_MATHEMATICAL, "Quantum number addition failed");
    }
    
    quantum_syscall_result_t result = qsc_create_success_result();
    result.extended_result.quantum_result = *result_qn;
    result.return_value = 0;
    
    kernel_track_mathematical_operation();
    
    return result;
}

/**
 * Handle ast_evaluate system call
 */
quantum_syscall_result_t qsc_handle_ast_evaluate(quantum_syscall_params_t *params) {
    if (params->param_count < 2) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    ast_node_t *ast = (ast_node_t*)params->params[0].ptr_param;
    qfs_computation_context_t *context = (qfs_computation_context_t*)params->params[1].ptr_param;
    
    if (!ast || !context) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid AST or context pointer");
    }
    
    // Evaluate AST
    quantum_number_t eval_result;
    quantum_number_init(&eval_result);
    
    qfs_result_t qfs_result = qfs_evaluate_mathematical_expression("", context, &eval_result);
    if (qfs_result != QFS_SUCCESS) {
        return qsc_create_error_result(QSC_ERROR_MATHEMATICAL, "AST evaluation failed");
    }
    
    quantum_syscall_result_t result = qsc_create_success_result();
    result.extended_result.quantum_result = eval_result;
    result.return_value = 0;
    
    kernel_track_mathematical_operation();
    
    return result;
}

/**
 * Handle qmm_alloc system call
 */
quantum_syscall_result_t qsc_handle_qmm_alloc(quantum_syscall_params_t *params) {
    if (params->param_count < 2) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    size_t size = (size_t)params->params[0].uint_param;
    qmm_mathematical_type_t type = (qmm_mathematical_type_t)params->params[1].uint_param;
    
    qmm_allocation_flags_t flags = QMM_FLAG_MATHEMATICAL;
    if (params->param_count > 2) {
        flags = (qmm_allocation_flags_t)params->params[2].uint_param;
    }
    
    // Allocate memory
    void *ptr = qmm_alloc_mathematical(size, type, flags);
    if (!ptr) {
        return qsc_create_error_result(QSC_ERROR_OUT_OF_MEMORY, "Memory allocation failed");
    }
    
    quantum_syscall_result_t result = qsc_create_success_result();
    result.extended_result.pointer_result = ptr;
    result.return_value = (int64_t)ptr;
    
    return result;
}

// Helper function implementations

static uint32_t qsc_hash_params(quantum_syscall_params_t *params) {
    if (!params) {
        return 0;
    }
    
    uint32_t hash = params->syscall_number * 31;
    hash = hash * 31 + params->param_count;
    
    for (uint32_t i = 0; i < params->param_count && i < 8; i++) {
        switch (params->param_types[i]) {
            case QSC_PARAM_INT64:
                hash = hash * 31 + (uint32_t)params->params[i].int_param;
                break;
            case QSC_PARAM_UINT64:
                hash = hash * 31 + (uint32_t)params->params[i].uint_param;
                break;
            case QSC_PARAM_POINTER:
                hash = hash * 31 + (uint32_t)((uintptr_t)params->params[i].ptr_param);
                break;
            case QSC_PARAM_DOUBLE:
                hash = hash * 31 + (uint32_t)params->params[i].double_param;
                break;
            case QSC_PARAM_BOOLEAN:
                hash = hash * 31 + (params->params[i].bool_param ? 1 : 0);
                break;
            default:
                break;
        }
    }
    
    return hash;
}

static qsc_cache_entry_t* qsc_find_cached_result(quantum_syscall_params_t *params) {
    if (!params) {
        return NULL;
    }
    
    uint32_t hash = qsc_hash_params(params) % QSC_CACHE_SIZE;
    qsc_cache_entry_t *entry = syscall_cache[hash];
    
    while (entry) {
        if (qsc_params_equal(&entry->params, params)) {
            return entry;
        }
        entry = entry->next;
    }
    
    return NULL;
}

static quantum_syscall_result_t qsc_create_error_result(uint32_t error_code, const char *message) {
    quantum_syscall_result_t result;
    memset(&result, 0, sizeof(quantum_syscall_result_t));
    
    result.return_value = -1;
    result.error_code = error_code;
    if (message) {
        strncpy(result.error_message, message, sizeof(result.error_message) - 1);
    }
    
    return result;
}

static quantum_syscall_result_t qsc_create_success_result(void) {
    quantum_syscall_result_t result;
    memset(&result, 0, sizeof(quantum_syscall_result_t));
    
    result.return_value = 0;
    result.error_code = QSC_SUCCESS;
    strcpy(result.error_message, "Success");
    
    return result;
}

static bool qsc_params_equal(quantum_syscall_params_t *params1, quantum_syscall_params_t *params2) {
    if (!params1 || !params2) {
        return false;
    }
    
    if (params1->syscall_number != params2->syscall_number ||
        params1->param_count != params2->param_count) {
        return false;
    }
    
    for (uint32_t i = 0; i < params1->param_count && i < 8; i++) {
        if (params1->param_types[i] != params2->param_types[i]) {
            return false;
        }
        
        switch (params1->param_types[i]) {
            case QSC_PARAM_INT64:
                if (params1->params[i].int_param != params2->params[i].int_param) {
                    return false;
                }
                break;
            case QSC_PARAM_UINT64:
                if (params1->params[i].uint_param != params2->params[i].uint_param) {
                    return false;
                }
                break;
            case QSC_PARAM_POINTER:
                if (params1->params[i].ptr_param != params2->params[i].ptr_param) {
                    return false;
                }
                break;
            case QSC_PARAM_DOUBLE:
                if (params1->params[i].double_param != params2->params[i].double_param) {
                    return false;
                }
                break;
            case QSC_PARAM_BOOLEAN:
                if (params1->params[i].bool_param != params2->params[i].bool_param) {
                    return false;
                }
                break;
            case QSC_PARAM_STRING:
                if (strcmp(params1->params[i].string_param, params2->params[i].string_param) != 0) {
                    return false;
                }
                break;
            default:
                break;
        }
    }
    
    return true;
}

static void qsc_update_statistics(quantum_syscall_number_t syscall_number, 
                                 uint64_t execution_time, bool success) {
    // Update global statistics
    global_stats.total_syscalls++;
    global_stats.total_execution_time_ns += execution_time;
    
    if (global_stats.total_syscalls > 0) {
        global_stats.average_execution_time_ns = 
            (uint32_t)(global_stats.total_execution_time_ns / global_stats.total_syscalls);
    }
    
    if (execution_time > global_stats.peak_execution_time_ns) {
        global_stats.peak_execution_time_ns = (uint32_t)execution_time;
    }
    
    if (global_stats.fastest_execution_time_ns == 0 || 
        execution_time < global_stats.fastest_execution_time_ns) {
        global_stats.fastest_execution_time_ns = (uint32_t)execution_time;
    }
    
    if (!success) {
        global_stats.failed_syscalls++;
    }
    
    // Update per-syscall statistics
    if (syscall_number <= QSC_MAX_SYSCALL) {
        global_stats.per_syscall_stats[syscall_number].syscall_number = syscall_number;
        global_stats.per_syscall_stats[syscall_number].call_count++;
        global_stats.per_syscall_stats[syscall_number].total_time_ns += execution_time;
        
        if (global_stats.per_syscall_stats[syscall_number].call_count > 0) {
            global_stats.per_syscall_stats[syscall_number].average_time_ns = 
                (uint32_t)(global_stats.per_syscall_stats[syscall_number].total_time_ns / 
                          global_stats.per_syscall_stats[syscall_number].call_count);
        }
        
        if (!success) {
            global_stats.per_syscall_stats[syscall_number].failure_count++;
        }
    }
    
    // Update mathematical syscall statistics
    if (syscall_number >= QSC_QUANTUM_CREATE && syscall_number <= QSC_QUANTUM_VALIDATE) {
        global_stats.quantum_syscalls++;
    } else if (syscall_number >= QSC_AST_CREATE && syscall_number <= QSC_AST_DESERIALIZE) {
        global_stats.symbolic_syscalls++;
    } else if (syscall_number >= QSC_QUANTUM_CREATE && syscall_number <= QSC_OPT_MEMORY_LAYOUT) {
        global_stats.mathematical_syscalls++;
    }
}

/**
 * Utility functions
 */
const char* qsc_syscall_name(quantum_syscall_number_t syscall_number) {
    if (syscall_number > QSC_MAX_SYSCALL) {
        return "Unknown";
    }
    
    if (syscall_table[syscall_number].name) {
        return syscall_table[syscall_number].name;
    }
    
    return "Unregistered";
}

const char* qsc_error_string(uint32_t error_code) {
    switch (error_code) {
        case QSC_SUCCESS: return "Success";
        case QSC_ERROR_INVALID_SYSCALL: return "Invalid system call";
        case QSC_ERROR_INVALID_PARAMS: return "Invalid parameters";
        case QSC_ERROR_PERMISSION_DENIED: return "Permission denied";
        case QSC_ERROR_OUT_OF_MEMORY: return "Out of memory";
        case QSC_ERROR_TIMEOUT: return "Timeout";
        case QSC_ERROR_MATHEMATICAL: return "Mathematical error";
        case QSC_ERROR_SYMBOLIC: return "Symbolic computation error";
        case QSC_ERROR_QUANTUM_NUMBER: return "Quantum number error";
        case QSC_ERROR_FILE_SYSTEM: return "File system error";
        case QSC_ERROR_PROCESS: return "Process error";
        case QSC_ERROR_DEPENDENCY: return "Dependency error";
        case QSC_ERROR_INTEGRITY: return "Integrity error";
        case QSC_ERROR_NOT_IMPLEMENTED: return "Not implemented";
        case QSC_ERROR_SYSTEM_FAILURE: return "System failure";
        default: return "Unknown error";
    }
}

bool qsc_is_mathematical_syscall(quantum_syscall_number_t syscall_number) {
    if (syscall_number > QSC_MAX_SYSCALL) {
        return false;
    }
    
    return syscall_table[syscall_number].is_mathematical;
}

bool qsc_is_real_time_safe(quantum_syscall_number_t syscall_number) {
    if (syscall_number > QSC_MAX_SYSCALL) {
        return false;
    }
    
    return syscall_table[syscall_number].is_real_time_safe;
}

uint32_t qsc_estimate_execution_time(quantum_syscall_number_t syscall_number) {
    if (syscall_number > QSC_MAX_SYSCALL) {
        return 0;
    }
    
    return syscall_table[syscall_number].average_execution_time_ns;
}

/**
 * Parameter helper functions
 */
int qsc_set_int_param(quantum_syscall_params_t *params, uint32_t index, int64_t value) {
    if (!params || index >= 8) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    params->params[index].int_param = value;
    params->param_types[index] = QSC_PARAM_INT64;
    
    if (index >= params->param_count) {
        params->param_count = index + 1;
    }
    
    return QSC_SUCCESS;
}

int qsc_set_uint_param(quantum_syscall_params_t *params, uint32_t index, uint64_t value) {
    if (!params || index >= 8) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    params->params[index].uint_param = value;
    params->param_types[index] = QSC_PARAM_UINT64;
    
    if (index >= params->param_count) {
        params->param_count = index + 1;
    }
    
    return QSC_SUCCESS;
}

int qsc_set_ptr_param(quantum_syscall_params_t *params, uint32_t index, void *value) {
    if (!params || index >= 8) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    params->params[index].ptr_param = value;
    params->param_types[index] = QSC_PARAM_POINTER;
    
    if (index >= params->param_count) {
        params->param_count = index + 1;
    }
    
    return QSC_SUCCESS;
}

int qsc_set_quantum_param(quantum_syscall_params_t *params, uint32_t index, quantum_number_t *value) {
    if (!params || index >= 8 || !value) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    params->params[index].quantum_param = *value;
    params->param_types[index] = QSC_PARAM_QUANTUM_NUMBER;
    
    if (index >= params->param_count) {
        params->param_count = index + 1;
    }
    
    return QSC_SUCCESS;
}

int qsc_set_string_param(quantum_syscall_params_t *params, uint32_t index, const char *value) {
    if (!params || index >= 8 || !value) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    strncpy(params->params[index].string_param, value, sizeof(params->params[index].string_param) - 1);
    params->param_types[index] = QSC_PARAM_STRING;
    
    if (index >= params->param_count) {
        params->param_count = index + 1;
    }
    
    return QSC_SUCCESS;
}

int qsc_get_int_param(quantum_syscall_params_t *params, uint32_t index, int64_t *value) {
    if (!params || index >= params->param_count || !value) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    if (params->param_types[index] != QSC_PARAM_INT64) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    *value = params->params[index].int_param;
    
    return QSC_SUCCESS;
}

int qsc_get_uint_param(quantum_syscall_params_t *params, uint32_t index, uint64_t *value) {
    if (!params || index >= params->param_count || !value) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    if (params->param_types[index] != QSC_PARAM_UINT64) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    *value = params->params[index].uint_param;
    
    return QSC_SUCCESS;
}

int qsc_get_ptr_param(quantum_syscall_params_t *params, uint32_t index, void **value) {
    if (!params || index >= params->param_count || !value) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    if (params->param_types[index] != QSC_PARAM_POINTER) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    *value = params->params[index].ptr_param;
    
    return QSC_SUCCESS;
}

int qsc_get_quantum_param(quantum_syscall_params_t *params, uint32_t index, quantum_number_t **value) {
    if (!params || index >= params->param_count || !value) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    if (params->param_types[index] != QSC_PARAM_QUANTUM_NUMBER) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    *value = &params->params[index].quantum_param;
    
    return QSC_SUCCESS;
}

int qsc_get_string_param(quantum_syscall_params_t *params, uint32_t index, char *buffer, size_t buffer_size) {
    if (!params || index >= params->param_count || !buffer || buffer_size == 0) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    if (params->param_types[index] != QSC_PARAM_STRING) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    strncpy(buffer, params->params[index].string_param, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
    
    return QSC_SUCCESS;
}

/**
 * Additional system call handlers
 */
quantum_syscall_result_t qsc_handle_quantum_destroy(quantum_syscall_params_t *params) {
    if (params->param_count < 1) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    quantum_number_t *qn = (quantum_number_t*)params->params[0].ptr_param;
    if (!qn) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid quantum number pointer");
    }
    
    // Free quantum number memory
    qmm_result_t free_result = qmm_free(qn);
    if (free_result != QMM_SUCCESS) {
        return qsc_create_error_result(QSC_ERROR_OUT_OF_MEMORY, "Failed to free quantum number");
    }
    
    return qsc_create_success_result();
}

quantum_syscall_result_t qsc_handle_quantum_subtract(quantum_syscall_params_t *params) {
    if (params->param_count < 3) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    quantum_number_t *a = (quantum_number_t*)params->params[0].ptr_param;
    quantum_number_t *b = (quantum_number_t*)params->params[1].ptr_param;
    quantum_number_t *result_qn = (quantum_number_t*)params->params[2].ptr_param;
    
    if (!a || !b || !result_qn) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid quantum number pointers");
    }
    
    // Perform subtraction
    int sub_result = quantum_number_subtract(a, b, result_qn);
    if (sub_result != 0) {
        return qsc_create_error_result(QSC_ERROR_MATHEMATICAL, "Quantum number subtraction failed");
    }
    
    quantum_syscall_result_t result = qsc_create_success_result();
    result.extended_result.quantum_result = *result_qn;
    result.return_value = 0;
    
    kernel_track_mathematical_operation();
    
    return result;
}

quantum_syscall_result_t qsc_handle_quantum_multiply(quantum_syscall_params_t *params) {
    if (params->param_count < 3) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    quantum_number_t *a = (quantum_number_t*)params->params[0].ptr_param;
    quantum_number_t *b = (quantum_number_t*)params->params[1].ptr_param;
    quantum_number_t *result_qn = (quantum_number_t*)params->params[2].ptr_param;
    
    if (!a || !b || !result_qn) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid quantum number pointers");
    }
    
    // Perform multiplication
    int mul_result = quantum_number_multiply(a, b, result_qn);
    if (mul_result != 0) {
        return qsc_create_error_result(QSC_ERROR_MATHEMATICAL, "Quantum number multiplication failed");
    }
    
    quantum_syscall_result_t result = qsc_create_success_result();
    result.extended_result.quantum_result = *result_qn;
    result.return_value = 0;
    
    kernel_track_mathematical_operation();
    
    return result;
}

quantum_syscall_result_t qsc_handle_quantum_divide(quantum_syscall_params_t *params) {
    if (params->param_count < 3) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    quantum_number_t *a = (quantum_number_t*)params->params[0].ptr_param;
    quantum_number_t *b = (quantum_number_t*)params->params[1].ptr_param;
    quantum_number_t *result_qn = (quantum_number_t*)params->params[2].ptr_param;
    
    if (!a || !b || !result_qn) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid quantum number pointers");
    }
    
    // Check for division by zero
    if (quantum_number_is_zero(b)) {
        return qsc_create_error_result(QSC_ERROR_MATHEMATICAL, "Division by zero");
    }
    
    // Perform division
    int div_result = quantum_number_divide(a, b, result_qn);
    if (div_result != 0) {
        return qsc_create_error_result(QSC_ERROR_MATHEMATICAL, "Quantum number division failed");
    }
    
    quantum_syscall_result_t result = qsc_create_success_result();
    result.extended_result.quantum_result = *result_qn;
    result.return_value = 0;
    
    kernel_track_mathematical_operation();
    
    return result;
}

quantum_syscall_result_t qsc_handle_quantum_compare(quantum_syscall_params_t *params) {
    if (params->param_count < 2) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    quantum_number_t *a = (quantum_number_t*)params->params[0].ptr_param;
    quantum_number_t *b = (quantum_number_t*)params->params[1].ptr_param;
    
    if (!a || !b) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid quantum number pointers");
    }
    
    // Perform comparison
    int cmp_result = quantum_number_compare(a, b);
    
    quantum_syscall_result_t result = qsc_create_success_result();
    result.return_value = cmp_result;
    result.extended_result.numeric_result = cmp_result;
    
    return result;
}

quantum_syscall_result_t qsc_handle_ast_create(quantum_syscall_params_t *params) {
    if (params->param_count < 1) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    ast_node_type_t node_type = (ast_node_type_t)params->params[0].uint_param;
    
    // Create AST node
    ast_node_t *node = ast_create_node(node_type);
    if (!node) {
        return qsc_create_error_result(QSC_ERROR_OUT_OF_MEMORY, "Failed to create AST node");
    }
    
    quantum_syscall_result_t result = qsc_create_success_result();
    result.extended_result.pointer_result = node;
    result.return_value = (int64_t)node;
    
    return result;
}

quantum_syscall_result_t qsc_handle_ast_destroy(quantum_syscall_params_t *params) {
    if (params->param_count < 1) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    ast_node_t *node = (ast_node_t*)params->params[0].ptr_param;
    if (!node) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid AST node pointer");
    }
    
    // Destroy AST node
    ast_destroy_node(node);
    
    return qsc_create_success_result();
}

quantum_syscall_result_t qsc_handle_qmm_free(quantum_syscall_params_t *params) {
    if (params->param_count < 1) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Insufficient parameters");
    }
    
    void *ptr = params->params[0].ptr_param;
    if (!ptr) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid pointer");
    }
    
    // Free memory
    qmm_result_t free_result = qmm_free(ptr);
    if (free_result != QMM_SUCCESS) {
        return qsc_create_error_result(QSC_ERROR_OUT_OF_MEMORY, "Memory deallocation failed");
    }
    
    return qsc_create_success_result();
}

quantum_syscall_result_t qsc_handle_qmm_garbage_collect(quantum_syscall_params_t *params) {
    // Run garbage collection
    qmm_result_t gc_result = qmm_gc_run();
    if (gc_result != QMM_SUCCESS) {
        return qsc_create_error_result(QSC_ERROR_SYSTEM_FAILURE, "Garbage collection failed");
    }
    
    return qsc_create_success_result();
}

/**
 * Mathematical operation helper
 */
quantum_syscall_result_t qsc_quantum_number_operation(quantum_syscall_number_t operation,
                                                      quantum_number_t *operand1,
                                                      quantum_number_t *operand2,
                                                      quantum_number_t *result) {
    if (!operand1 || !result) {
        return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Invalid operands");
    }
    
    int math_result = 0;
    
    switch (operation) {
        case QSC_QUANTUM_ADD:
            if (!operand2) {
                return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Missing second operand");
            }
            math_result = quantum_number_add(operand1, operand2, result);
            break;
            
        case QSC_QUANTUM_SUBTRACT:
            if (!operand2) {
                return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Missing second operand");
            }
            math_result = quantum_number_subtract(operand1, operand2, result);
            break;
            
        case QSC_QUANTUM_MULTIPLY:
            if (!operand2) {
                return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Missing second operand");
            }
            math_result = quantum_number_multiply(operand1, operand2, result);
            break;
            
        case QSC_QUANTUM_DIVIDE:
            if (!operand2) {
                return qsc_create_error_result(QSC_ERROR_INVALID_PARAMS, "Missing second operand");
            }
            if (quantum_number_is_zero(operand2)) {
                return qsc_create_error_result(QSC_ERROR_MATHEMATICAL, "Division by zero");
            }
            math_result = quantum_number_divide(operand1, operand2, result);
            break;
            
        case QSC_QUANTUM_COPY:
            *result = *operand1;
            math_result = 0;
            break;
            
        default:
            return qsc_create_error_result(QSC_ERROR_NOT_IMPLEMENTED, "Operation not implemented");
    }
    
    if (math_result != 0) {
        return qsc_create_error_result(QSC_ERROR_MATHEMATICAL, "Mathematical operation failed");
    }
    
    quantum_syscall_result_t syscall_result = qsc_create_success_result();
    syscall_result.extended_result.quantum_result = *result;
    
    kernel_track_mathematical_operation();
    
    return syscall_result;
}

/**
 * Reset system call statistics
 */
int qsc_reset_syscall_statistics(void) {
    if (!qsc_initialized) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    memset(&global_stats, 0, sizeof(quantum_syscall_statistics_t));
    
    kernel_log(KERNEL_LOG_INFO, "System call statistics reset");
    
    return QSC_SUCCESS;
}

/**
 * Dump system call state for debugging
 */
int qsc_dump_syscall_state(void) {
    if (!qsc_initialized) {
        return QSC_ERROR_INVALID_PARAMS;
    }
    
    printf("=== Quantum System Call State ===\n");
    printf("Initialized: %s\n", qsc_initialized ? "Yes" : "No");
    printf("Caching: %s\n", caching_enabled ? "Enabled" : "Disabled");
    printf("Tracing: %s\n", tracing_enabled ? "Enabled" : "Disabled");
    printf("Profiling: %s\n", profiling_enabled ? "Enabled" : "Disabled");
    printf("Cache entries: %u / %d\n", cache_entry_count, QSC_CACHE_SIZE);
    printf("\n");
    
    printf("Statistics:\n");
    printf("  Total syscalls: %llu\n", global_stats.total_syscalls);
    printf("  Mathematical syscalls: %llu\n", global_stats.mathematical_syscalls);
    printf("  Symbolic syscalls: %llu\n", global_stats.symbolic_syscalls);
    printf("  Quantum syscalls: %llu\n", global_stats.quantum_syscalls);
    printf("  Failed syscalls: %llu\n", global_stats.failed_syscalls);
    printf("  Cached results: %u\n", global_stats.cached_results);
    printf("  Average execution time: %u ns\n", global_stats.average_execution_time_ns);
    printf("  Peak execution time: %u ns\n", global_stats.peak_execution_time_ns);
    printf("  Fastest execution time: %u ns\n", global_stats.fastest_execution_time_ns);
    printf("\n");
    
    printf("Registered system calls:\n");
    for (int i = 0; i <= QSC_MAX_SYSCALL; i++) {
        if (syscall_table[i].handler) {
            printf("  %d: %s - %s\n", i, syscall_table[i].name, syscall_table[i].description);
        }
    }
    printf("================================\n");
    
    return QSC_SUCCESS;
}
