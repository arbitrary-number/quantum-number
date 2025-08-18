/**
 * Quantum OS - Mathematical System Calls Interface
 * 
 * Revolutionary system call interface designed for symbolic mathematical
 * computation and Quantum Number operations.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 * 
 * Date: August 18, 2025
 */

#ifndef QUANTUM_SYSCALLS_H
#define QUANTUM_SYSCALLS_H

#include "quantum_number.h"
#include "process.h"
#include "memory.h"
#include <stdint.h>
#include <stddef.h>

// System call numbers
#define QS_EXIT                 0x00    // Process termination
#define QS_FORK                 0x01    // Create child process
#define QS_EXEC                 0x02    // Execute program
#define QS_WAIT                 0x03    // Wait for child process
#define QS_GETPID               0x04    // Get process ID
#define QS_GETPPID              0x05    // Get parent process ID

// Memory management system calls
#define QS_MMAP                 0x10    // Map memory
#define QS_MUNMAP               0x11    // Unmap memory
#define QS_MPROTECT             0x12    // Change memory protection
#define QS_MALLOC               0x13    // Allocate memory
#define QS_FREE                 0x14    // Free memory
#define QS_REALLOC              0x15    // Reallocate memory

// Quantum Number system calls
#define QS_QN_CREATE            0x20    // Create Quantum Number
#define QS_QN_DESTROY           0x21    // Destroy Quantum Number
#define QS_QN_ADD               0x22    // Add Quantum Numbers
#define QS_QN_SUBTRACT          0x23    // Subtract Quantum Numbers
#define QS_QN_MULTIPLY          0x24    // Multiply Quantum Numbers
#define QS_QN_DIVIDE            0x25    // Divide Quantum Numbers
#define QS_QN_POWER             0x26    // Power operation
#define QS_QN_ROOT              0x27    // Root operation
#define QS_QN_COMPARE           0x28    // Compare Quantum Numbers
#define QS_QN_COPY              0x29    // Copy Quantum Number
#define QS_QN_SET_ORDINAL       0x2A    // Set ordinal value
#define QS_QN_GET_ORDINAL       0x2B    // Get ordinal value
#define QS_QN_TO_STRING         0x2C    // Convert to string
#define QS_QN_FROM_STRING       0x2D    // Parse from string

// Complex number system calls
#define QS_QN_COMPLEX_CREATE    0x30    // Create complex number
#define QS_QN_COMPLEX_ADD       0x31    // Add complex numbers
#define QS_QN_COMPLEX_MULTIPLY  0x32    // Multiply complex numbers
#define QS_QN_COMPLEX_CONJUGATE 0x33    // Complex conjugate
#define QS_QN_COMPLEX_MAGNITUDE 0x34    // Complex magnitude
#define QS_QN_COMPLEX_PHASE     0x35    // Complex phase

// AST (Abstract Syntax Tree) system calls
#define QS_AST_CREATE           0x40    // Create AST node
#define QS_AST_DESTROY          0x41    // Destroy AST tree
#define QS_AST_ADD_CHILD        0x42    // Add child node
#define QS_AST_EVALUATE         0x43    // Evaluate AST
#define QS_AST_SIMPLIFY         0x44    // Simplify expression
#define QS_AST_DIFFERENTIATE    0x45    // Differentiate expression
#define QS_AST_INTEGRATE        0x46    // Integrate expression
#define QS_AST_SUBSTITUTE       0x47    // Variable substitution
#define QS_AST_SERIALIZE        0x48    // Serialize AST to string
#define QS_AST_DESERIALIZE      0x49    // Deserialize AST from string

// Symbolic computation system calls
#define QS_SYM_CREATE_VARIABLE  0x50    // Create symbolic variable
#define QS_SYM_CREATE_CONSTANT  0x51    // Create symbolic constant
#define QS_SYM_CREATE_FUNCTION  0x52    // Create symbolic function
#define QS_SYM_SOLVE_EQUATION   0x53    // Solve symbolic equation
#define QS_SYM_EXPAND           0x54    // Expand expression
#define QS_SYM_FACTOR           0x55    // Factor expression
#define QS_SYM_LIMIT            0x56    // Calculate limit
#define QS_SYM_SERIES           0x57    // Taylor/power series expansion

// Mathematical library system calls
#define QS_MATH_SIN             0x60    // Sine function
#define QS_MATH_COS             0x61    // Cosine function
#define QS_MATH_TAN             0x62    // Tangent function
#define QS_MATH_LOG             0x63    // Natural logarithm
#define QS_MATH_EXP             0x64    // Exponential function
#define QS_MATH_SQRT            0x65    // Square root
#define QS_MATH_FACTORIAL       0x66    // Factorial
#define QS_MATH_GAMMA           0x67    // Gamma function
#define QS_MATH_BESSEL          0x68    // Bessel functions
#define QS_MATH_ELLIPTIC        0x69    // Elliptic functions

// Matrix operations system calls
#define QS_MATRIX_CREATE        0x70    // Create matrix
#define QS_MATRIX_DESTROY       0x71    // Destroy matrix
#define QS_MATRIX_ADD           0x72    // Matrix addition
#define QS_MATRIX_MULTIPLY      0x73    // Matrix multiplication
#define QS_MATRIX_TRANSPOSE     0x74    // Matrix transpose
#define QS_MATRIX_DETERMINANT   0x75    // Matrix determinant
#define QS_MATRIX_INVERSE       0x76    // Matrix inverse
#define QS_MATRIX_EIGENVALUES   0x77    // Calculate eigenvalues
#define QS_MATRIX_EIGENVECTORS  0x78    // Calculate eigenvectors
#define QS_MATRIX_LU_DECOMP     0x79    // LU decomposition
#define QS_MATRIX_QR_DECOMP     0x7A    // QR decomposition
#define QS_MATRIX_SVD           0x7B    // Singular value decomposition

// File system system calls
#define QS_OPEN                 0x80    // Open file
#define QS_CLOSE                0x81    // Close file
#define QS_READ                 0x82    // Read from file
#define QS_WRITE                0x83    // Write to file
#define QS_SEEK                 0x84    // Seek in file
#define QS_STAT                 0x85    // Get file statistics
#define QS_MKDIR                0x86    // Create directory
#define QS_RMDIR                0x87    // Remove directory
#define QS_UNLINK               0x88    // Remove file
#define QS_RENAME               0x89    // Rename file

// Inter-process communication
#define QS_PIPE                 0x90    // Create pipe
#define QS_MSGGET               0x91    // Get message queue
#define QS_MSGSND               0x92    // Send message
#define QS_MSGRCV               0x93    // Receive message
#define QS_SEMGET               0x94    // Get semaphore
#define QS_SEMOP                0x95    // Semaphore operation
#define QS_SHMGET               0x96    // Get shared memory
#define QS_SHMAT                0x97    // Attach shared memory
#define QS_SHMDT                0x98    // Detach shared memory

// Time and scheduling
#define QS_TIME                 0xA0    // Get current time
#define QS_SLEEP                0xA1    // Sleep for specified time
#define QS_YIELD                0xA2    // Yield CPU
#define QS_GETPRIORITY          0xA3    // Get process priority
#define QS_SETPRIORITY          0xA4    // Set process priority
#define QS_SCHED_SETSCHEDULER   0xA5    // Set scheduler algorithm
#define QS_SCHED_GETSCHEDULER   0xA6    // Get scheduler algorithm

// System information
#define QS_UNAME                0xB0    // Get system information
#define QS_GETUID               0xB1    // Get user ID
#define QS_GETGID               0xB2    // Get group ID
#define QS_SETUID               0xB3    // Set user ID
#define QS_SETGID               0xB4    // Set group ID

// System call parameter structure
typedef struct qs_params {
    uint64_t param1;        // First parameter
    uint64_t param2;        // Second parameter
    uint64_t param3;        // Third parameter
    uint64_t param4;        // Fourth parameter
    uint64_t param5;        // Fifth parameter
    uint64_t param6;        // Sixth parameter
    void *ptr_param1;       // First pointer parameter
    void *ptr_param2;       // Second pointer parameter
    void *ptr_param3;       // Third pointer parameter
} qs_params_t;

// System call result structure
typedef struct qs_result {
    int64_t return_value;   // Return value
    int32_t error_code;     // Error code
    uint32_t flags;         // Result flags
    void *ptr_result;       // Pointer result
    quantum_number_t qn_result; // Quantum Number result
} qs_result_t;

// System call handler function type
typedef qs_result_t (*qs_handler_t)(qp_process_t *process, qs_params_t *params);

// System call table entry
typedef struct qs_syscall_entry {
    uint32_t syscall_number;    // System call number
    const char *name;           // System call name
    qs_handler_t handler;       // Handler function
    uint32_t param_count;       // Number of parameters
    uint32_t flags;             // System call flags
} qs_syscall_entry_t;

// System call flags
#define QS_FLAG_KERNEL_ONLY     0x01    // Kernel mode only
#define QS_FLAG_USER_ALLOWED    0x02    // User mode allowed
#define QS_FLAG_MATHEMATICAL    0x04    // Mathematical operation
#define QS_FLAG_SYMBOLIC        0x08    // Symbolic computation
#define QS_FLAG_ASYNC           0x10    // Asynchronous operation
#define QS_FLAG_DEPRECATED      0x20    // Deprecated system call

// System call interface functions
int qs_init_syscalls(void);
qs_result_t qs_dispatch_syscall(uint32_t syscall_num, qp_process_t *process, 
                               qs_params_t *params);
int qs_register_syscall(uint32_t syscall_num, const char *name, 
                       qs_handler_t handler, uint32_t param_count, uint32_t flags);
int qs_unregister_syscall(uint32_t syscall_num);

// Process management system call handlers
qs_result_t qs_sys_exit(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_fork(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_exec(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_wait(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_getpid(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_getppid(qp_process_t *process, qs_params_t *params);

// Memory management system call handlers
qs_result_t qs_sys_mmap(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_munmap(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_malloc(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_free(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_realloc(qp_process_t *process, qs_params_t *params);

// Quantum Number system call handlers
qs_result_t qs_sys_qn_create(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_destroy(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_add(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_subtract(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_multiply(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_divide(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_compare(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_copy(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_set_ordinal(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_get_ordinal(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_to_string(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_from_string(qp_process_t *process, qs_params_t *params);

// Complex number system call handlers
qs_result_t qs_sys_qn_complex_create(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_complex_add(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_complex_multiply(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_qn_complex_conjugate(qp_process_t *process, qs_params_t *params);

// AST system call handlers
qs_result_t qs_sys_ast_create(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_ast_destroy(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_ast_add_child(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_ast_evaluate(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_ast_simplify(qp_process_t *process, qs_params_t *params);

// Mathematical function system call handlers
qs_result_t qs_sys_math_sin(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_math_cos(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_math_log(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_math_exp(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_math_sqrt(qp_process_t *process, qs_params_t *params);

// Matrix operation system call handlers
qs_result_t qs_sys_matrix_create(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_matrix_destroy(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_matrix_add(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_matrix_multiply(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_matrix_transpose(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_matrix_determinant(qp_process_t *process, qs_params_t *params);
qs_result_t qs_sys_matrix_inverse(qp_process_t *process, qs_params_t *params);

// Utility functions
int qs_validate_user_pointer(qp_process_t *process, void *ptr, size_t size);
int qs_copy_from_user(qp_process_t *process, void *dest, const void *src, size_t size);
int qs_copy_to_user(qp_process_t *process, void *dest, const void *src, size_t size);
quantum_number_t* qs_get_user_quantum_number(qp_process_t *process, void *user_ptr);
int qs_put_user_quantum_number(qp_process_t *process, void *user_ptr, quantum_number_t *qn);

// Error codes
typedef enum {
    QS_SUCCESS = 0,
    QS_ERROR_INVALID_SYSCALL = -1,
    QS_ERROR_INVALID_PARAMS = -2,
    QS_ERROR_PERMISSION_DENIED = -3,
    QS_ERROR_OUT_OF_MEMORY = -4,
    QS_ERROR_INVALID_POINTER = -5,
    QS_ERROR_PROCESS_NOT_FOUND = -6,
    QS_ERROR_MATHEMATICAL_ERROR = -7,
    QS_ERROR_DIVISION_BY_ZERO = -8,
    QS_ERROR_OVERFLOW = -9,
    QS_ERROR_UNDERFLOW = -10,
    QS_ERROR_INVALID_OPERATION = -11,
    QS_ERROR_RESOURCE_BUSY = -12,
    QS_ERROR_TIMEOUT = -13,
    QS_ERROR_INTERRUPTED = -14,
    QS_ERROR_NOT_IMPLEMENTED = -15
} qs_error_t;

const char* qs_error_string(qs_error_t error);

// System call debugging
#ifdef QS_DEBUG
void qs_debug_syscall(uint32_t syscall_num, qp_process_t *process, 
                     qs_params_t *params, qs_result_t *result);
void qs_print_syscall_stats(void);
#endif

#endif // QUANTUM_SYSCALLS_H
