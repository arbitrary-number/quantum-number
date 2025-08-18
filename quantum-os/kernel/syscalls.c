/**
 * Quantum OS - Mathematical System Calls Implementation
 * 
 * Revolutionary system call interface designed for symbolic mathematical
 * computation and Quantum Number operations.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 * 
 * Date: August 18, 2025
 */

#include "syscalls.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// System call table
static qs_syscall_entry_t syscall_table[256];
static int syscalls_initialized = 0;

// System call statistics
static struct {
    uint64_t total_calls;
    uint64_t successful_calls;
    uint64_t failed_calls;
    uint64_t mathematical_calls;
    uint64_t symbolic_calls;
} syscall_stats = {0};

// Internal helper functions
static qs_result_t create_error_result(qs_error_t error);
static qs_result_t create_success_result(int64_t value);
static int validate_syscall_params(uint32_t syscall_num, qs_params_t *params);

/**
 * Initialize the system call interface
 */
int qs_init_syscalls(void) {
    if (syscalls_initialized) {
        return QS_SUCCESS;
    }
    
    // Clear system call table
    memset(syscall_table, 0, sizeof(syscall_table));
    
    // Register core system calls
    
    // Process management
    qs_register_syscall(QS_EXIT, "exit", qs_sys_exit, 1, QS_FLAG_USER_ALLOWED);
    qs_register_syscall(QS_FORK, "fork", qs_sys_fork, 0, QS_FLAG_USER_ALLOWED);
    qs_register_syscall(QS_GETPID, "getpid", qs_sys_getpid, 0, QS_FLAG_USER_ALLOWED);
    qs_register_syscall(QS_GETPPID, "getppid", qs_sys_getppid, 0, QS_FLAG_USER_ALLOWED);
    
    // Memory management
    qs_register_syscall(QS_MALLOC, "malloc", qs_sys_malloc, 1, QS_FLAG_USER_ALLOWED);
    qs_register_syscall(QS_FREE, "free", qs_sys_free, 1, QS_FLAG_USER_ALLOWED);
    qs_register_syscall(QS_REALLOC, "realloc", qs_sys_realloc, 2, QS_FLAG_USER_ALLOWED);
    
    // Quantum Number operations
    qs_register_syscall(QS_QN_CREATE, "qn_create", qs_sys_qn_create, 0, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_DESTROY, "qn_destroy", qs_sys_qn_destroy, 1, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_ADD, "qn_add", qs_sys_qn_add, 3, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_SUBTRACT, "qn_subtract", qs_sys_qn_subtract, 3, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_MULTIPLY, "qn_multiply", qs_sys_qn_multiply, 3, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_DIVIDE, "qn_divide", qs_sys_qn_divide, 3, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_COMPARE, "qn_compare", qs_sys_qn_compare, 2, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_SET_ORDINAL, "qn_set_ordinal", qs_sys_qn_set_ordinal, 3, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_GET_ORDINAL, "qn_get_ordinal", qs_sys_qn_get_ordinal, 2, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    
    // Complex number operations
    qs_register_syscall(QS_QN_COMPLEX_CREATE, "qn_complex_create", qs_sys_qn_complex_create, 2, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_COMPLEX_ADD, "qn_complex_add", qs_sys_qn_complex_add, 3, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_QN_COMPLEX_MULTIPLY, "qn_complex_multiply", qs_sys_qn_complex_multiply, 3, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    
    // Mathematical functions
    qs_register_syscall(QS_MATH_SIN, "math_sin", qs_sys_math_sin, 2, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_MATH_COS, "math_cos", qs_sys_math_cos, 2, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_MATH_LOG, "math_log", qs_sys_math_log, 2, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_MATH_EXP, "math_exp", qs_sys_math_exp, 2, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    qs_register_syscall(QS_MATH_SQRT, "math_sqrt", qs_sys_math_sqrt, 2, 
                       QS_FLAG_USER_ALLOWED | QS_FLAG_MATHEMATICAL);
    
    syscalls_initialized = 1;
    return QS_SUCCESS;
}

/**
 * Register a system call
 */
int qs_register_syscall(uint32_t syscall_num, const char *name, 
                       qs_handler_t handler, uint32_t param_count, uint32_t flags) {
    if (syscall_num >= 256 || !name || !handler) {
        return QS_ERROR_INVALID_PARAMS;
    }
    
    if (syscall_table[syscall_num].handler != NULL) {
        return QS_ERROR_RESOURCE_BUSY; // System call already registered
    }
    
    syscall_table[syscall_num].syscall_number = syscall_num;
    syscall_table[syscall_num].name = name;
    syscall_table[syscall_num].handler = handler;
    syscall_table[syscall_num].param_count = param_count;
    syscall_table[syscall_num].flags = flags;
    
    return QS_SUCCESS;
}

/**
 * Dispatch system call
 */
qs_result_t qs_dispatch_syscall(uint32_t syscall_num, qp_process_t *process, 
                               qs_params_t *params) {
    syscall_stats.total_calls++;
    
    // Validate system call number
    if (syscall_num >= 256) {
        syscall_stats.failed_calls++;
        return create_error_result(QS_ERROR_INVALID_SYSCALL);
    }
    
    // Check if system call is registered
    qs_syscall_entry_t *entry = &syscall_table[syscall_num];
    if (!entry->handler) {
        syscall_stats.failed_calls++;
        return create_error_result(QS_ERROR_INVALID_SYSCALL);
    }
    
    // Check permissions
    if (!process) {
        syscall_stats.failed_calls++;
        return create_error_result(QS_ERROR_PROCESS_NOT_FOUND);
    }
    
    // Check if user process can call this system call
    if (process->cpu_context.privilege_level > 0 && 
        !(entry->flags & QS_FLAG_USER_ALLOWED)) {
        syscall_stats.failed_calls++;
        return create_error_result(QS_ERROR_PERMISSION_DENIED);
    }
    
    // Validate parameters
    if (!params) {
        syscall_stats.failed_calls++;
        return create_error_result(QS_ERROR_INVALID_PARAMS);
    }
    
    // Update statistics
    if (entry->flags & QS_FLAG_MATHEMATICAL) {
        syscall_stats.mathematical_calls++;
        process->quantum_operations_count++;
    }
    if (entry->flags & QS_FLAG_SYMBOLIC) {
        syscall_stats.symbolic_calls++;
    }
    
    // Call the handler
    qs_result_t result = entry->handler(process, params);
    
    if (result.error_code == QS_SUCCESS) {
        syscall_stats.successful_calls++;
    } else {
        syscall_stats.failed_calls++;
    }
    
    return result;
}

/**
 * Process management system calls
 */
qs_result_t qs_sys_exit(qp_process_t *process, qs_params_t *params) {
    int exit_code = (int)params->param1;
    qp_terminate_process(process, exit_code);
    return create_success_result(0);
}

qs_result_t qs_sys_fork(qp_process_t *process, qs_params_t *params) {
    // Create child process (simplified implementation)
    qp_process_t *child = qp_create_process("child", 
                                           (void*)qn_get_ordinal(&process->cpu_context.qpc, 0),
                                           process->priority, 
                                           process->flags);
    if (!child) {
        return create_error_result(QS_ERROR_OUT_OF_MEMORY);
    }
    
    // Copy parent's memory layout
    child->memory = process->memory;
    
    // Start child process
    if (qp_start_process(child) != QP_SUCCESS) {
        return create_error_result(QS_ERROR_OUT_OF_MEMORY);
    }
    
    return create_success_result(child->pid);
}

qs_result_t qs_sys_getpid(qp_process_t *process, qs_params_t *params) {
    return create_success_result(process->pid);
}

qs_result_t qs_sys_getppid(qp_process_t *process, qs_params_t *params) {
    return create_success_result(process->ppid);
}

/**
 * Memory management system calls
 */
qs_result_t qs_sys_malloc(qp_process_t *process, qs_params_t *params) {
    size_t size = (size_t)params->param1;
    
    if (size == 0) {
        return create_error_result(QS_ERROR_INVALID_PARAMS);
    }
    
    void *ptr = qp_allocate_process_memory(process, size);
    if (!ptr) {
        return create_error_result(QS_ERROR_OUT_OF_MEMORY);
    }
    
    qs_result_t result = create_success_result(0);
    result.ptr_result = ptr;
    return result;
}

qs_result_t qs_sys_free(qp_process_t *process, qs_params_t *params) {
    void *ptr = params->ptr_param1;
    
    if (!ptr) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    int result = qp_free_process_memory(process, ptr);
    if (result != QP_SUCCESS) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    return create_success_result(0);
}

qs_result_t qs_sys_realloc(qp_process_t *process, qs_params_t *params) {
    void *old_ptr = params->ptr_param1;
    size_t new_size = (size_t)params->param1;
    
    // Simplified realloc implementation
    if (!old_ptr) {
        return qs_sys_malloc(process, params);
    }
    
    if (new_size == 0) {
        return qs_sys_free(process, params);
    }
    
    // Allocate new memory
    void *new_ptr = qp_allocate_process_memory(process, new_size);
    if (!new_ptr) {
        return create_error_result(QS_ERROR_OUT_OF_MEMORY);
    }
    
    // Copy old data (simplified - would need to track original size)
    memcpy(new_ptr, old_ptr, new_size);
    
    // Free old memory
    qp_free_process_memory(process, old_ptr);
    
    qs_result_t result = create_success_result(0);
    result.ptr_result = new_ptr;
    return result;
}

/**
 * Quantum Number system calls
 */
qs_result_t qs_sys_qn_create(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *qn = qm_alloc_quantum_number();
    if (!qn) {
        return create_error_result(QS_ERROR_OUT_OF_MEMORY);
    }
    
    qn_zero(qn);
    
    qs_result_t result = create_success_result(0);
    result.ptr_result = qn;
    return result;
}

qs_result_t qs_sys_qn_destroy(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *qn = (quantum_number_t*)params->ptr_param1;
    
    if (!qn || !qs_validate_user_pointer(process, qn, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    qm_free_quantum_number(qn);
    return create_success_result(0);
}

qs_result_t qs_sys_qn_add(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *a = (quantum_number_t*)params->ptr_param2;
    quantum_number_t *b = (quantum_number_t*)params->ptr_param3;
    
    if (!result_qn || !a || !b ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, a, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, b, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    int qn_result = qn_add(result_qn, a, b);
    if (qn_result != 0) {
        return create_error_result(QS_ERROR_MATHEMATICAL_ERROR);
    }
    
    return create_success_result(0);
}

qs_result_t qs_sys_qn_subtract(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *a = (quantum_number_t*)params->ptr_param2;
    quantum_number_t *b = (quantum_number_t*)params->ptr_param3;
    
    if (!result_qn || !a || !b ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, a, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, b, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    int qn_result = qn_subtract(result_qn, a, b);
    if (qn_result != 0) {
        return create_error_result(QS_ERROR_MATHEMATICAL_ERROR);
    }
    
    return create_success_result(0);
}

qs_result_t qs_sys_qn_multiply(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *a = (quantum_number_t*)params->ptr_param2;
    quantum_number_t *b = (quantum_number_t*)params->ptr_param3;
    
    if (!result_qn || !a || !b ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, a, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, b, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    int qn_result = qn_multiply(result_qn, a, b);
    if (qn_result != 0) {
        return create_error_result(QS_ERROR_MATHEMATICAL_ERROR);
    }
    
    return create_success_result(0);
}

qs_result_t qs_sys_qn_divide(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *a = (quantum_number_t*)params->ptr_param2;
    quantum_number_t *b = (quantum_number_t*)params->ptr_param3;
    
    if (!result_qn || !a || !b ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, a, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, b, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    // Check for division by zero
    if (qn_is_zero(b)) {
        return create_error_result(QS_ERROR_DIVISION_BY_ZERO);
    }
    
    int qn_result = qn_divide(result_qn, a, b);
    if (qn_result != 0) {
        return create_error_result(QS_ERROR_MATHEMATICAL_ERROR);
    }
    
    return create_success_result(0);
}

qs_result_t qs_sys_qn_compare(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *a = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *b = (quantum_number_t*)params->ptr_param2;
    
    if (!a || !b ||
        !qs_validate_user_pointer(process, a, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, b, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    int comparison = qn_compare(a, b);
    return create_success_result(comparison);
}

qs_result_t qs_sys_qn_set_ordinal(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *qn = (quantum_number_t*)params->ptr_param1;
    int index = (int)params->param1;
    uint32_t value = (uint32_t)params->param2;
    
    if (!qn || !qs_validate_user_pointer(process, qn, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    if (index < 0 || index >= 12) {
        return create_error_result(QS_ERROR_INVALID_PARAMS);
    }
    
    qn_set_ordinal(qn, index, value);
    return create_success_result(0);
}

qs_result_t qs_sys_qn_get_ordinal(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *qn = (quantum_number_t*)params->ptr_param1;
    int index = (int)params->param1;
    
    if (!qn || !qs_validate_user_pointer(process, qn, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    if (index < 0 || index >= 12) {
        return create_error_result(QS_ERROR_INVALID_PARAMS);
    }
    
    uint32_t value = qn_get_ordinal(qn, index);
    return create_success_result(value);
}

/**
 * Complex number system calls
 */
qs_result_t qs_sys_qn_complex_create(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *real = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *imag = (quantum_number_t*)params->ptr_param2;
    
    if (!real || !imag ||
        !qs_validate_user_pointer(process, real, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, imag, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    quantum_number_t *complex_qn = qm_alloc_quantum_number();
    if (!complex_qn) {
        return create_error_result(QS_ERROR_OUT_OF_MEMORY);
    }
    
    int result = qn_create_complex(complex_qn, real, imag);
    if (result != 0) {
        qm_free_quantum_number(complex_qn);
        return create_error_result(QS_ERROR_MATHEMATICAL_ERROR);
    }
    
    qs_result_t sys_result = create_success_result(0);
    sys_result.ptr_result = complex_qn;
    return sys_result;
}

qs_result_t qs_sys_qn_complex_add(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *a = (quantum_number_t*)params->ptr_param2;
    quantum_number_t *b = (quantum_number_t*)params->ptr_param3;
    
    if (!result_qn || !a || !b ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, a, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, b, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    int qn_result = qn_complex_add(result_qn, a, b);
    if (qn_result != 0) {
        return create_error_result(QS_ERROR_MATHEMATICAL_ERROR);
    }
    
    return create_success_result(0);
}

qs_result_t qs_sys_qn_complex_multiply(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *a = (quantum_number_t*)params->ptr_param2;
    quantum_number_t *b = (quantum_number_t*)params->ptr_param3;
    
    if (!result_qn || !a || !b ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, a, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, b, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    int qn_result = qn_complex_multiply(result_qn, a, b);
    if (qn_result != 0) {
        return create_error_result(QS_ERROR_MATHEMATICAL_ERROR);
    }
    
    return create_success_result(0);
}

/**
 * Mathematical function system calls (placeholder implementations)
 */
qs_result_t qs_sys_math_sin(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *input = (quantum_number_t*)params->ptr_param2;
    
    if (!result_qn || !input ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, input, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    // Placeholder: would implement actual sine calculation
    qn_copy(result_qn, input);
    return create_success_result(0);
}

qs_result_t qs_sys_math_cos(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *input = (quantum_number_t*)params->ptr_param2;
    
    if (!result_qn || !input ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, input, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    // Placeholder: would implement actual cosine calculation
    qn_one(result_qn);
    return create_success_result(0);
}

qs_result_t qs_sys_math_sqrt(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *input = (quantum_number_t*)params->ptr_param2;
    
    if (!result_qn || !input ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, input, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    // Placeholder: would implement actual square root calculation
    qn_copy(result_qn, input);
    return create_success_result(0);
}

/**
 * Utility functions
 */
int qs_validate_user_pointer(qp_process_t *process, void *ptr, size_t size) {
    if (!process || !ptr) {
        return 0;
    }
    
    // Check if pointer is within process memory space
    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t process_base = (uintptr_t)process->memory.data_base;
    uintptr_t process_end = process_base + process->memory.data_size + 
                           process->memory.stack_size + process->memory.heap_size;
    
    return (addr >= process_base && addr + size <= process_end);
}

/**
 * Helper functions
 */
static qs_result_t create_error_result(qs_error_t error) {
    qs_result_t result;
    result.return_value = -1;
    result.error_code = error;
    result.flags = 0;
    result.ptr_result = NULL;
    qn_zero(&result.qn_result);
    return result;
}

static qs_result_t create_success_result(int64_t value) {
    qs_result_t result;
    result.return_value = value;
    result.error_code = QS_SUCCESS;
    result.flags = 0;
    result.ptr_result = NULL;
    qn_zero(&result.qn_result);
    return result;
}

/**
 * Get error string
 */
const char* qs_error_string(qs_error_t error) {
    switch (error) {
        case QS_SUCCESS: return "Success";
        case QS_ERROR_INVALID_SYSCALL: return "Invalid system call";
        case QS_ERROR_INVALID_PARAMS: return "Invalid parameters";
        case QS_ERROR_PERMISSION_DENIED: return "Permission denied";
        case QS_ERROR_OUT_OF_MEMORY: return "Out of memory";
        case QS_ERROR_INVALID_POINTER: return "Invalid pointer";
        case QS_ERROR_PROCESS_NOT_FOUND: return "Process not found";
        case QS_ERROR_MATHEMATICAL_ERROR: return "Mathematical error";
        case QS_ERROR_DIVISION_BY_ZERO: return "Division by zero";
        case QS_ERROR_OVERFLOW: return "Overflow";
        case QS_ERROR_UNDERFLOW: return "Underflow";
        case QS_ERROR_INVALID_OPERATION: return "Invalid operation";
        case QS_ERROR_RESOURCE_BUSY: return "Resource busy";
        case QS_ERROR_TIMEOUT: return "Timeout";
        case QS_ERROR_INTERRUPTED: return "Interrupted";
        case QS_ERROR_NOT_IMPLEMENTED: return "Not implemented";
        default: return "Unknown error";
    }
}

// Placeholder implementations for missing system call handlers
qs_result_t qs_sys_math_log(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *input = (quantum_number_t*)params->ptr_param2;
    
    if (!result_qn || !input ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, input, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    // Placeholder: would implement actual logarithm calculation
    qn_copy(result_qn, input);
    return create_success_result(0);
}

qs_result_t qs_sys_math_exp(qp_process_t *process, qs_params_t *params) {
    quantum_number_t *result_qn = (quantum_number_t*)params->ptr_param1;
    quantum_number_t *input = (quantum_number_t*)params->ptr_param2;
    
    if (!result_qn || !input ||
        !qs_validate_user_pointer(process, result_qn, sizeof(quantum_number_t)) ||
        !qs_validate_user_pointer(process, input, sizeof(quantum_number_t))) {
        return create_error_result(QS_ERROR_INVALID_POINTER);
    }
    
    // Placeholder: would implement actual exponential calculation
    qn_copy(result_qn, input);
    return create_success_result(0);
}
