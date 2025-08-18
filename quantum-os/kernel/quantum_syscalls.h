/**
 * Quantum System Calls - Advanced Mathematical System Call Interface
 * 
 * Revolutionary system call interface that provides native support for
 * Quantum Numbers, symbolic computation, and mathematical operations.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_SYSCALLS_H
#define QUANTUM_SYSCALLS_H

#include "quantum_number.h"
#include "quantum_memory_manager.h"
#include "quantix_qfs_advanced.h"
#include "quantum_process_scheduler.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Quantum system call numbers
typedef enum {
    // Basic system calls
    QSC_EXIT = 0,
    QSC_FORK = 1,
    QSC_WAIT = 2,
    QSC_EXEC = 3,
    QSC_GETPID = 4,
    QSC_GETPPID = 5,
    
    // Mathematical system calls (100-199)
    QSC_QUANTUM_CREATE = 100,
    QSC_QUANTUM_DESTROY = 101,
    QSC_QUANTUM_ADD = 102,
    QSC_QUANTUM_SUBTRACT = 103,
    QSC_QUANTUM_MULTIPLY = 104,
    QSC_QUANTUM_DIVIDE = 105,
    QSC_QUANTUM_POWER = 106,
    QSC_QUANTUM_ROOT = 107,
    QSC_QUANTUM_COMPARE = 108,
    QSC_QUANTUM_COPY = 109,
    QSC_QUANTUM_CONVERT = 110,
    QSC_QUANTUM_VALIDATE = 111,
    
    // Symbolic computation system calls (200-299)
    QSC_AST_CREATE = 200,
    QSC_AST_DESTROY = 201,
    QSC_AST_EVALUATE = 202,
    QSC_AST_SIMPLIFY = 203,
    QSC_AST_DIFFERENTIATE = 204,
    QSC_AST_INTEGRATE = 205,
    QSC_AST_SUBSTITUTE = 206,
    QSC_AST_TRANSFORM = 207,
    QSC_AST_COPY = 208,
    QSC_AST_COMPARE = 209,
    QSC_AST_OPTIMIZE = 210,
    QSC_AST_SERIALIZE = 211,
    QSC_AST_DESERIALIZE = 212,
    
    // Mathematical file system calls (300-399)
    QSC_QFS_OPEN = 300,
    QSC_QFS_CLOSE = 301,
    QSC_QFS_READ_QUANTUM = 302,
    QSC_QFS_WRITE_QUANTUM = 303,
    QSC_QFS_READ_AST = 304,
    QSC_QFS_WRITE_AST = 305,
    QSC_QFS_CREATE_MATHEMATICAL = 306,
    QSC_QFS_DELETE_MATHEMATICAL = 307,
    QSC_QFS_QUERY_PROPERTIES = 308,
    QSC_QFS_SET_PROPERTIES = 309,
    QSC_QFS_SEARCH_EXPRESSIONS = 310,
    QSC_QFS_VERIFY_INTEGRITY = 311,
    QSC_QFS_OPTIMIZE_STORAGE = 312,
    QSC_QFS_CREATE_INDEX = 313,
    QSC_QFS_SEARCH_INDEX = 314,
    
    // Mathematical memory system calls (400-499)
    QSC_QMM_ALLOC = 400,
    QSC_QMM_FREE = 401,
    QSC_QMM_ALLOC_QUANTUM = 402,
    QSC_QMM_ALLOC_SYMBOLIC = 403,
    QSC_QMM_ALLOC_AST = 404,
    QSC_QMM_REALLOC = 405,
    QSC_QMM_GET_STATISTICS = 406,
    QSC_QMM_GARBAGE_COLLECT = 407,
    QSC_QMM_VERIFY_INTEGRITY = 408,
    QSC_QMM_OPTIMIZE_LAYOUT = 409,
    QSC_QMM_ADD_DEPENDENCY = 410,
    QSC_QMM_REMOVE_DEPENDENCY = 411,
    
    // Mathematical process system calls (500-599)
    QSC_QPS_CREATE_MATHEMATICAL = 500,
    QSC_QPS_SET_PRIORITY = 501,
    QSC_QPS_ADD_DEPENDENCY = 502,
    QSC_QPS_SATISFY_DEPENDENCY = 503,
    QSC_QPS_CREATE_CHANNEL = 504,
    QSC_QPS_SEND_MESSAGE = 505,
    QSC_QPS_RECEIVE_MESSAGE = 506,
    QSC_QPS_GET_STATISTICS = 507,
    QSC_QPS_YIELD_MATHEMATICAL = 508,
    QSC_QPS_BOOST_PRIORITY = 509,
    QSC_QPS_TRACE_COMPUTATION = 510,
    QSC_QPS_PROFILE_OPERATIONS = 511,
    
    // Mathematical proof system calls (600-699)
    QSC_PROOF_CREATE = 600,
    QSC_PROOF_VERIFY = 601,
    QSC_PROOF_ADD_STEP = 602,
    QSC_PROOF_GET_DEPENDENCIES = 603,
    QSC_PROOF_CHECK_CONSISTENCY = 604,
    QSC_PROOF_EXPORT = 605,
    QSC_PROOF_IMPORT = 606,
    QSC_PROOF_SEARCH = 607,
    
    // Mathematical relationship system calls (700-799)
    QSC_REL_DISCOVER = 700,
    QSC_REL_ADD = 701,
    QSC_REL_QUERY = 702,
    QSC_REL_VERIFY = 703,
    QSC_REL_DELETE = 704,
    QSC_REL_ANALYZE = 705,
    
    // Pattern matching system calls (800-899)
    QSC_PATTERN_CREATE = 800,
    QSC_PATTERN_MATCH = 801,
    QSC_PATTERN_TRANSFORM = 802,
    QSC_PATTERN_OPTIMIZE = 803,
    QSC_PATTERN_STATISTICS = 804,
    
    // Mathematical optimization system calls (900-999)
    QSC_OPT_ANALYZE_COMPLEXITY = 900,
    QSC_OPT_OPTIMIZE_EXPRESSION = 901,
    QSC_OPT_PRECOMPUTE_SUBEXPRESSIONS = 902,
    QSC_OPT_CACHE_RESULTS = 903,
    QSC_OPT_PARALLEL_EVALUATE = 904,
    QSC_OPT_MEMORY_LAYOUT = 905,
    
    // System information and debugging (1000-1099)
    QSC_SYS_GET_QUANTUM_INFO = 1000,
    QSC_SYS_GET_MATHEMATICAL_STATS = 1001,
    QSC_SYS_DEBUG_DUMP_STATE = 1002,
    QSC_SYS_TRACE_OPERATIONS = 1003,
    QSC_SYS_PROFILE_PERFORMANCE = 1004,
    QSC_SYS_VALIDATE_SYSTEM = 1005,
    
    QSC_MAX_SYSCALL = 1099
} quantum_syscall_number_t;

// System call result structure
typedef struct quantum_syscall_result {
    int64_t return_value;           // Primary return value
    uint32_t error_code;            // Error code (0 = success)
    char error_message[256];        // Human-readable error message
    
    // Extended result data
    union {
        quantum_number_t quantum_result;
        void *pointer_result;
        uint64_t numeric_result;
        bool boolean_result;
        struct {
            void *data;
            size_t size;
        } buffer_result;
    } extended_result;
    
    // Performance metrics
    uint64_t execution_time_ns;     // Execution time in nanoseconds
    uint32_t memory_used;           // Memory used during execution
    uint32_t operations_performed;  // Operations performed
} quantum_syscall_result_t;

// System call parameter structure
typedef struct quantum_syscall_params {
    quantum_syscall_number_t syscall_number;
    uint32_t param_count;
    
    // Parameters (up to 8 parameters)
    union {
        int64_t int_param;
        uint64_t uint_param;
        void *ptr_param;
        quantum_number_t quantum_param;
        double double_param;
        bool bool_param;
        char string_param[256];
    } params[8];
    
    // Parameter types for validation
    enum {
        QSC_PARAM_INT64,
        QSC_PARAM_UINT64,
        QSC_PARAM_POINTER,
        QSC_PARAM_QUANTUM_NUMBER,
        QSC_PARAM_DOUBLE,
        QSC_PARAM_BOOLEAN,
        QSC_PARAM_STRING
    } param_types[8];
    
    // Execution context
    uint32_t calling_process_id;
    uint32_t execution_flags;
    uint32_t timeout_ms;
    uint32_t priority_level;
} quantum_syscall_params_t;

// System call execution flags
#define QSC_FLAG_ASYNC              0x01    // Asynchronous execution
#define QSC_FLAG_MATHEMATICAL       0x02    // Mathematical operation
#define QSC_FLAG_SYMBOLIC           0x04    // Symbolic computation
#define QSC_FLAG_REAL_TIME          0x08    // Real-time execution
#define QSC_FLAG_CACHEABLE          0x10    // Result can be cached
#define QSC_FLAG_ATOMIC             0x20    // Atomic operation
#define QSC_FLAG_TRACE              0x40    // Enable tracing
#define QSC_FLAG_PROFILE            0x80    // Enable profiling

// System call handler function type
typedef quantum_syscall_result_t (*quantum_syscall_handler_t)(quantum_syscall_params_t *params);

// System call registration structure
typedef struct quantum_syscall_registration {
    quantum_syscall_number_t syscall_number;
    quantum_syscall_handler_t handler;
    const char *name;
    const char *description;
    uint32_t min_params;
    uint32_t max_params;
    uint32_t required_permissions;
    bool is_mathematical;
    bool is_real_time_safe;
    uint32_t average_execution_time_ns;
} quantum_syscall_registration_t;

// System call statistics
typedef struct quantum_syscall_statistics {
    uint64_t total_syscalls;        // Total system calls made
    uint64_t mathematical_syscalls; // Mathematical system calls
    uint64_t symbolic_syscalls;     // Symbolic computation syscalls
    uint64_t quantum_syscalls;      // Quantum number syscalls
    uint64_t failed_syscalls;       // Failed system calls
    
    // Performance statistics
    uint64_t total_execution_time_ns; // Total execution time
    uint32_t average_execution_time_ns; // Average execution time
    uint32_t peak_execution_time_ns; // Peak execution time
    uint32_t fastest_execution_time_ns; // Fastest execution time
    
    // Per-syscall statistics
    struct {
        quantum_syscall_number_t syscall_number;
        uint64_t call_count;
        uint64_t total_time_ns;
        uint32_t average_time_ns;
        uint32_t failure_count;
    } per_syscall_stats[QSC_MAX_SYSCALL + 1];
    
    // Resource usage
    uint64_t total_memory_allocated;
    uint64_t peak_memory_usage;
    uint32_t active_mathematical_operations;
    uint32_t cached_results;
} quantum_syscall_statistics_t;

// Core Quantum System Call API

// System call initialization and management
int qsc_initialize_syscalls(void);
int qsc_shutdown_syscalls(void);
int qsc_register_syscall(quantum_syscall_registration_t *registration);
int qsc_unregister_syscall(quantum_syscall_number_t syscall_number);

// System call execution
quantum_syscall_result_t qsc_execute_syscall(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_execute_syscall_async(quantum_syscall_params_t *params,
                                                   void **async_handle);
int qsc_wait_async_syscall(void *async_handle, quantum_syscall_result_t *result);
int qsc_cancel_async_syscall(void *async_handle);

// Mathematical system call helpers
quantum_syscall_result_t qsc_quantum_number_operation(quantum_syscall_number_t operation,
                                                      quantum_number_t *operand1,
                                                      quantum_number_t *operand2,
                                                      quantum_number_t *result);
quantum_syscall_result_t qsc_ast_operation(quantum_syscall_number_t operation,
                                           ast_node_t *ast,
                                           qfs_computation_context_t *context,
                                           ast_node_t **result);
quantum_syscall_result_t qsc_mathematical_file_operation(quantum_syscall_number_t operation,
                                                         const char *file_path,
                                                         void *data,
                                                         size_t data_size);

// System call validation and security
int qsc_validate_syscall_params(quantum_syscall_params_t *params);
int qsc_check_syscall_permissions(quantum_syscall_number_t syscall_number,
                                 uint32_t process_id);
int qsc_sanitize_syscall_input(quantum_syscall_params_t *params);

// System call caching and optimization
int qsc_enable_syscall_caching(bool enable);
int qsc_cache_syscall_result(quantum_syscall_params_t *params,
                            quantum_syscall_result_t *result);
int qsc_lookup_cached_result(quantum_syscall_params_t *params,
                            quantum_syscall_result_t *result,
                            bool *found);
int qsc_clear_syscall_cache(void);

// System call monitoring and statistics
int qsc_get_syscall_statistics(quantum_syscall_statistics_t *stats);
int qsc_reset_syscall_statistics(void);
int qsc_enable_syscall_tracing(bool enable);
int qsc_enable_syscall_profiling(bool enable);

// System call debugging and diagnostics
int qsc_dump_syscall_state(void);
int qsc_trace_syscall_execution(quantum_syscall_number_t syscall_number,
                               bool enable);
int qsc_profile_syscall_performance(quantum_syscall_number_t syscall_number,
                                   bool enable);
int qsc_validate_syscall_integrity(void);

// Specific Mathematical System Call Implementations

// Quantum Number system calls
quantum_syscall_result_t qsc_handle_quantum_create(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_quantum_destroy(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_quantum_add(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_quantum_subtract(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_quantum_multiply(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_quantum_divide(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_quantum_power(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_quantum_compare(quantum_syscall_params_t *params);

// AST system calls
quantum_syscall_result_t qsc_handle_ast_create(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_ast_destroy(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_ast_evaluate(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_ast_simplify(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_ast_differentiate(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_ast_integrate(quantum_syscall_params_t *params);

// QFS system calls
quantum_syscall_result_t qsc_handle_qfs_open(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qfs_close(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qfs_read_quantum(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qfs_write_quantum(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qfs_read_ast(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qfs_write_ast(quantum_syscall_params_t *params);

// QMM system calls
quantum_syscall_result_t qsc_handle_qmm_alloc(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qmm_free(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qmm_alloc_quantum(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qmm_alloc_symbolic(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qmm_garbage_collect(quantum_syscall_params_t *params);

// QPS system calls
quantum_syscall_result_t qsc_handle_qps_create_mathematical(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qps_set_priority(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qps_add_dependency(quantum_syscall_params_t *params);
quantum_syscall_result_t qsc_handle_qps_create_channel(quantum_syscall_params_t *params);

// Utility functions
const char* qsc_syscall_name(quantum_syscall_number_t syscall_number);
const char* qsc_error_string(uint32_t error_code);
bool qsc_is_mathematical_syscall(quantum_syscall_number_t syscall_number);
bool qsc_is_real_time_safe(quantum_syscall_number_t syscall_number);
uint32_t qsc_estimate_execution_time(quantum_syscall_number_t syscall_number);

// System call parameter helpers
int qsc_set_int_param(quantum_syscall_params_t *params, uint32_t index, int64_t value);
int qsc_set_uint_param(quantum_syscall_params_t *params, uint32_t index, uint64_t value);
int qsc_set_ptr_param(quantum_syscall_params_t *params, uint32_t index, void *value);
int qsc_set_quantum_param(quantum_syscall_params_t *params, uint32_t index, quantum_number_t *value);
int qsc_set_string_param(quantum_syscall_params_t *params, uint32_t index, const char *value);

int qsc_get_int_param(quantum_syscall_params_t *params, uint32_t index, int64_t *value);
int qsc_get_uint_param(quantum_syscall_params_t *params, uint32_t index, uint64_t *value);
int qsc_get_ptr_param(quantum_syscall_params_t *params, uint32_t index, void **value);
int qsc_get_quantum_param(quantum_syscall_params_t *params, uint32_t index, quantum_number_t **value);
int qsc_get_string_param(quantum_syscall_params_t *params, uint32_t index, char *buffer, size_t buffer_size);

// Error codes
#define QSC_SUCCESS                 0
#define QSC_ERROR_INVALID_SYSCALL   1
#define QSC_ERROR_INVALID_PARAMS    2
#define QSC_ERROR_PERMISSION_DENIED 3
#define QSC_ERROR_OUT_OF_MEMORY     4
#define QSC_ERROR_TIMEOUT           5
#define QSC_ERROR_MATHEMATICAL      6
#define QSC_ERROR_SYMBOLIC          7
#define QSC_ERROR_QUANTUM_NUMBER    8
#define QSC_ERROR_FILE_SYSTEM       9
#define QSC_ERROR_PROCESS           10
#define QSC_ERROR_DEPENDENCY        11
#define QSC_ERROR_INTEGRITY         12
#define QSC_ERROR_NOT_IMPLEMENTED   13
#define QSC_ERROR_SYSTEM_FAILURE    14

#endif // QUANTUM_SYSCALLS_H
