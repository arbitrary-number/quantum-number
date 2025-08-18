/**
 * Quantum Error Handler - Advanced Mathematical Error Handling and Recovery
 * 
 * Comprehensive error handling system designed specifically for mathematical
 * computation errors, quantum number inconsistencies, and symbolic computation failures.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_ERROR_HANDLER_H
#define QUANTUM_ERROR_HANDLER_H

#include "quantum_number.h"
#include "quantum_memory_manager.h"
#include "quantix_qfs_advanced.h"
#include "quantum_process_scheduler.h"
#include "quantum_io_manager.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Quantum error categories
typedef enum {
    QEH_CATEGORY_MATHEMATICAL = 0x0100,     // Mathematical computation errors
    QEH_CATEGORY_QUANTUM_NUMBER = 0x0200,   // Quantum Number specific errors
    QEH_CATEGORY_SYMBOLIC = 0x0300,         // Symbolic computation errors
    QEH_CATEGORY_MEMORY = 0x0400,           // Memory management errors
    QEH_CATEGORY_FILE_SYSTEM = 0x0500,      // File system errors
    QEH_CATEGORY_PROCESS = 0x0600,          // Process management errors
    QEH_CATEGORY_IO = 0x0700,               // I/O operation errors
    QEH_CATEGORY_SYSTEM = 0x0800,           // System-level errors
    QEH_CATEGORY_HARDWARE = 0x0900,         // Hardware-related errors
    QEH_CATEGORY_SECURITY = 0x0A00          // Security and integrity errors
} qeh_error_category_t;

// Quantum error severity levels
typedef enum {
    QEH_SEVERITY_INFO = 0,                  // Informational
    QEH_SEVERITY_WARNING = 1,               // Warning - system can continue
    QEH_SEVERITY_ERROR = 2,                 // Error - operation failed
    QEH_SEVERITY_CRITICAL = 3,              // Critical - system stability at risk
    QEH_SEVERITY_FATAL = 4                  // Fatal - system must halt
} qeh_error_severity_t;

// Mathematical error types
typedef enum {
    // Mathematical computation errors (0x0100-0x01FF)
    QEH_MATH_DIVISION_BY_ZERO = 0x0101,
    QEH_MATH_OVERFLOW = 0x0102,
    QEH_MATH_UNDERFLOW = 0x0103,
    QEH_MATH_PRECISION_LOSS = 0x0104,
    QEH_MATH_DOMAIN_ERROR = 0x0105,
    QEH_MATH_CONVERGENCE_FAILURE = 0x0106,
    QEH_MATH_INFINITE_RESULT = 0x0107,
    QEH_MATH_COMPLEX_RESULT = 0x0108,
    QEH_MATH_UNDEFINED_OPERATION = 0x0109,
    QEH_MATH_NUMERICAL_INSTABILITY = 0x010A,
    
    // Quantum Number errors (0x0200-0x02FF)
    QEH_QN_INVALID_ORDINAL = 0x0201,
    QEH_QN_INVALID_SIGN = 0x0202,
    QEH_QN_CHECKSUM_MISMATCH = 0x0203,
    QEH_QN_CORRUPTION_DETECTED = 0x0204,
    QEH_QN_INCOMPATIBLE_OPERATION = 0x0205,
    QEH_QN_PRECISION_EXCEEDED = 0x0206,
    QEH_QN_INVALID_FORMAT = 0x0207,
    QEH_QN_CONVERSION_ERROR = 0x0208,
    
    // Symbolic computation errors (0x0300-0x03FF)
    QEH_SYM_MALFORMED_EXPRESSION = 0x0301,
    QEH_SYM_UNDEFINED_VARIABLE = 0x0302,
    QEH_SYM_UNDEFINED_FUNCTION = 0x0303,
    QEH_SYM_CIRCULAR_REFERENCE = 0x0304,
    QEH_SYM_EVALUATION_TIMEOUT = 0x0305,
    QEH_SYM_STACK_OVERFLOW = 0x0306,
    QEH_SYM_INVALID_AST = 0x0307,
    QEH_SYM_SIMPLIFICATION_FAILURE = 0x0308,
    QEH_SYM_PROOF_VERIFICATION_FAILED = 0x0309,
    
    // Memory management errors (0x0400-0x04FF)
    QEH_MEM_OUT_OF_MEMORY = 0x0401,
    QEH_MEM_INVALID_POINTER = 0x0402,
    QEH_MEM_DOUBLE_FREE = 0x0403,
    QEH_MEM_CORRUPTION = 0x0404,
    QEH_MEM_ALIGNMENT_ERROR = 0x0405,
    QEH_MEM_POOL_EXHAUSTED = 0x0406,
    QEH_MEM_FRAGMENTATION_CRITICAL = 0x0407,
    QEH_MEM_GC_FAILURE = 0x0408,
    QEH_MEM_DEPENDENCY_CYCLE = 0x0409,
    
    // File system errors (0x0500-0x05FF)
    QEH_FS_FILE_NOT_FOUND = 0x0501,
    QEH_FS_PERMISSION_DENIED = 0x0502,
    QEH_FS_DISK_FULL = 0x0503,
    QEH_FS_CORRUPTION = 0x0504,
    QEH_FS_MATHEMATICAL_INCONSISTENCY = 0x0505,
    QEH_FS_INDEX_CORRUPTION = 0x0506,
    QEH_FS_CHECKSUM_MISMATCH = 0x0507,
    QEH_FS_COMPRESSION_ERROR = 0x0508,
    
    // Process management errors (0x0600-0x06FF)
    QEH_PROC_INVALID_PROCESS = 0x0601,
    QEH_PROC_SCHEDULING_CONFLICT = 0x0602,
    QEH_PROC_DEPENDENCY_DEADLOCK = 0x0603,
    QEH_PROC_RESOURCE_EXHAUSTED = 0x0604,
    QEH_PROC_MATHEMATICAL_CONTEXT_LOST = 0x0605,
    QEH_PROC_PRIORITY_INVERSION = 0x0606,
    
    // I/O errors (0x0700-0x07FF)
    QEH_IO_DEVICE_NOT_FOUND = 0x0701,
    QEH_IO_DEVICE_BUSY = 0x0702,
    QEH_IO_TIMEOUT = 0x0703,
    QEH_IO_HARDWARE_FAILURE = 0x0704,
    QEH_IO_MATHEMATICAL_FORMAT_ERROR = 0x0705,
    QEH_IO_STREAM_CORRUPTION = 0x0706,
    
    // System errors (0x0800-0x08FF)
    QEH_SYS_KERNEL_PANIC = 0x0801,
    QEH_SYS_ASSERTION_FAILED = 0x0802,
    QEH_SYS_INVALID_STATE = 0x0803,
    QEH_SYS_RESOURCE_LEAK = 0x0804,
    QEH_SYS_CONFIGURATION_ERROR = 0x0805,
    
    // Hardware errors (0x0900-0x09FF)
    QEH_HW_X256_FAULT = 0x0901,
    QEH_HW_MATHEMATICAL_UNIT_FAILURE = 0x0902,
    QEH_HW_MEMORY_ERROR = 0x0903,
    QEH_HW_QUANTUM_SENSOR_FAILURE = 0x0904,
    
    // Security errors (0x0A00-0x0AFF)
    QEH_SEC_INTEGRITY_VIOLATION = 0x0A01,
    QEH_SEC_MATHEMATICAL_TAMPERING = 0x0A02,
    QEH_SEC_UNAUTHORIZED_ACCESS = 0x0A03,
    QEH_SEC_PROOF_FORGERY = 0x0A04
} qeh_error_type_t;

// Error recovery strategies
typedef enum {
    QEH_RECOVERY_NONE = 0,                  // No recovery possible
    QEH_RECOVERY_RETRY = 1,                 // Retry the operation
    QEH_RECOVERY_FALLBACK = 2,              // Use fallback method
    QEH_RECOVERY_APPROXIMATE = 3,           // Use approximation
    QEH_RECOVERY_RESTART_COMPONENT = 4,     // Restart failed component
    QEH_RECOVERY_MATHEMATICAL_CORRECTION = 5, // Apply mathematical correction
    QEH_RECOVERY_SYMBOLIC_SIMPLIFICATION = 6, // Simplify symbolic expression
    QEH_RECOVERY_PRECISION_REDUCTION = 7,   // Reduce precision requirements
    QEH_RECOVERY_ALTERNATIVE_ALGORITHM = 8, // Use alternative algorithm
    QEH_RECOVERY_SYSTEM_RESTART = 9         // Restart entire system
} qeh_recovery_strategy_t;

// Error context structure
typedef struct qeh_error_context {
    // Error identification
    uint32_t error_id;                      // Unique error identifier
    qeh_error_type_t error_type;            // Error type
    qeh_error_category_t category;          // Error category
    qeh_error_severity_t severity;          // Error severity
    
    // Error location
    const char *source_file;                // Source file where error occurred
    uint32_t source_line;                   // Source line number
    const char *function_name;              // Function name
    uint32_t process_id;                    // Process ID (if applicable)
    uint32_t thread_id;                     // Thread ID (if applicable)
    
    // Error details
    char error_message[256];                // Human-readable error message
    char technical_details[512];            // Technical error details
    void *error_data;                       // Additional error-specific data
    size_t error_data_size;                 // Size of error data
    
    // Mathematical context
    quantum_number_t *quantum_context;      // Quantum Number context
    ast_node_t *symbolic_context;           // Symbolic expression context
    mathematical_domain_t domain;           // Mathematical domain
    complexity_measure_t complexity;        // Operation complexity
    
    // Timing information
    uint64_t error_time;                    // When error occurred
    uint64_t operation_start_time;          // When operation started
    uint32_t operation_duration;            // Operation duration before error
    
    // Recovery information
    qeh_recovery_strategy_t suggested_recovery; // Suggested recovery strategy
    uint32_t recovery_attempts;             // Number of recovery attempts
    bool is_recoverable;                    // Whether error is recoverable
    uint32_t recovery_timeout;              // Recovery timeout
    
    // Error propagation
    struct qeh_error_context *parent_error; // Parent error (if cascaded)
    struct qeh_error_context **child_errors; // Child errors
    uint32_t child_error_count;             // Number of child errors
} qeh_error_context_t;

// Error handler function type
typedef qeh_recovery_strategy_t (*qeh_error_handler_func_t)(qeh_error_context_t *context);

// Error handler registration structure
typedef struct qeh_error_handler {
    qeh_error_type_t error_type;            // Error type to handle
    qeh_error_handler_func_t handler_func;  // Handler function
    uint32_t priority;                      // Handler priority
    const char *handler_name;               // Handler name
    bool is_active;                         // Handler active status
} qeh_error_handler_t;

// Error statistics structure
typedef struct qeh_error_statistics {
    // Overall statistics
    uint64_t total_errors;                  // Total errors encountered
    uint64_t errors_by_category[16];        // Errors by category
    uint64_t errors_by_severity[5];         // Errors by severity
    
    // Recovery statistics
    uint64_t successful_recoveries;         // Successful recoveries
    uint64_t failed_recoveries;             // Failed recoveries
    uint64_t recovery_attempts;             // Total recovery attempts
    uint32_t average_recovery_time;         // Average recovery time
    
    // Mathematical error statistics
    uint64_t mathematical_errors;           // Mathematical computation errors
    uint64_t quantum_number_errors;         // Quantum Number errors
    uint64_t symbolic_errors;               // Symbolic computation errors
    uint64_t precision_errors;              // Precision-related errors
    
    // System impact statistics
    uint64_t system_restarts;               // System restarts due to errors
    uint64_t component_restarts;            // Component restarts
    uint64_t data_corruption_incidents;     // Data corruption incidents
    uint32_t mean_time_between_failures;    // MTBF
} qeh_error_statistics_t;

// Error logging structure
typedef struct qeh_error_log_entry {
    qeh_error_context_t error_context;      // Error context
    uint64_t log_timestamp;                 // Log entry timestamp
    uint32_t log_sequence;                  // Log sequence number
    char additional_info[256];              // Additional information
} qeh_error_log_entry_t;

// QEH result codes
typedef enum {
    QEH_SUCCESS = 0,
    QEH_ERROR_INVALID_PARAMETER = -1,
    QEH_ERROR_OUT_OF_MEMORY = -2,
    QEH_ERROR_HANDLER_NOT_FOUND = -3,
    QEH_ERROR_RECOVERY_FAILED = -4,
    QEH_ERROR_LOG_FULL = -5,
    QEH_ERROR_CIRCULAR_DEPENDENCY = -6,
    QEH_ERROR_TIMEOUT = -7,
    QEH_ERROR_SYSTEM_UNSTABLE = -8
} qeh_result_t;

// Core Quantum Error Handler API

// System initialization and management
qeh_result_t qeh_initialize_error_handler(void);
qeh_result_t qeh_shutdown_error_handler(void);
qeh_result_t qeh_configure_error_handler(uint32_t max_handlers, uint32_t log_size,
                                        bool enable_recovery);

// Error handler registration
qeh_result_t qeh_register_error_handler(qeh_error_type_t error_type,
                                       qeh_error_handler_func_t handler,
                                       uint32_t priority,
                                       const char *handler_name);
qeh_result_t qeh_unregister_error_handler(qeh_error_type_t error_type,
                                         const char *handler_name);
qeh_result_t qeh_set_handler_priority(qeh_error_type_t error_type,
                                     const char *handler_name,
                                     uint32_t priority);

// Error reporting and handling
qeh_result_t qeh_report_error(qeh_error_type_t error_type,
                             qeh_error_severity_t severity,
                             const char *source_file,
                             uint32_t source_line,
                             const char *function_name,
                             const char *error_message,
                             void *error_data,
                             size_t error_data_size);

qeh_result_t qeh_report_mathematical_error(qeh_error_type_t error_type,
                                          quantum_number_t *quantum_context,
                                          ast_node_t *symbolic_context,
                                          mathematical_domain_t domain,
                                          const char *error_message);

qeh_result_t qeh_handle_error(qeh_error_context_t *error_context);

// Error recovery operations
qeh_result_t qeh_attempt_recovery(qeh_error_context_t *error_context,
                                 qeh_recovery_strategy_t strategy);
qeh_result_t qeh_mathematical_recovery(qeh_error_context_t *error_context);
qeh_result_t qeh_symbolic_recovery(qeh_error_context_t *error_context);
qeh_result_t qeh_quantum_number_recovery(qeh_error_context_t *error_context);

// Error analysis and diagnostics
qeh_result_t qeh_analyze_error_pattern(qeh_error_type_t error_type,
                                      uint32_t time_window,
                                      uint32_t *pattern_count,
                                      char **analysis_report);
qeh_result_t qeh_diagnose_system_health(void);
qeh_result_t qeh_predict_failure_probability(qeh_error_category_t category,
                                            double *probability);

// Error logging and monitoring
qeh_result_t qeh_log_error(qeh_error_context_t *error_context);
qeh_result_t qeh_get_error_log(qeh_error_log_entry_t **log_entries,
                              uint32_t *entry_count,
                              uint32_t max_entries);
qeh_result_t qeh_clear_error_log(void);
qeh_result_t qeh_export_error_log(const char *file_path);

// Error statistics and reporting
qeh_result_t qeh_get_error_statistics(qeh_error_statistics_t *statistics);
qeh_result_t qeh_reset_error_statistics(void);
qeh_result_t qeh_generate_error_report(const char *report_path);

// Mathematical error correction
qeh_result_t qeh_correct_quantum_number_error(quantum_number_t *corrupted_number,
                                             quantum_number_t *corrected_number);
qeh_result_t qeh_correct_symbolic_expression_error(ast_node_t *corrupted_expression,
                                                  ast_node_t **corrected_expression);
qeh_result_t qeh_validate_mathematical_integrity(void *mathematical_data,
                                                size_t data_size,
                                                mathematical_domain_t domain,
                                                bool *is_valid);

// System stability monitoring
qeh_result_t qeh_monitor_system_stability(void);
qeh_result_t qeh_check_mathematical_consistency(void);
qeh_result_t qeh_verify_quantum_number_integrity(void);
qeh_result_t qeh_validate_symbolic_computation_state(void);

// Emergency procedures
qeh_result_t qeh_emergency_shutdown(qeh_error_severity_t trigger_severity);
qeh_result_t qeh_emergency_mathematical_state_save(void);
qeh_result_t qeh_emergency_mathematical_state_restore(void);
qeh_result_t qeh_initiate_safe_mode(void);

// Utility functions and macros
const char* qeh_error_type_string(qeh_error_type_t error_type);
const char* qeh_error_category_string(qeh_error_category_t category);
const char* qeh_error_severity_string(qeh_error_severity_t severity);
const char* qeh_recovery_strategy_string(qeh_recovery_strategy_t strategy);
bool qeh_is_mathematical_error(qeh_error_type_t error_type);
bool qeh_is_recoverable_error(qeh_error_type_t error_type);
uint32_t qeh_estimate_recovery_time(qeh_error_type_t error_type,
                                   qeh_recovery_strategy_t strategy);

// Error reporting macros
#define QEH_REPORT_ERROR(type, severity, message) \
    qeh_report_error(type, severity, __FILE__, __LINE__, __FUNCTION__, message, NULL, 0)

#define QEH_REPORT_MATHEMATICAL_ERROR(type, qn_ctx, sym_ctx, domain, message) \
    qeh_report_mathematical_error(type, qn_ctx, sym_ctx, domain, message)

#define QEH_ASSERT(condition, error_type, message) \
    do { \
        if (!(condition)) { \
            QEH_REPORT_ERROR(error_type, QEH_SEVERITY_CRITICAL, message); \
        } \
    } while(0)

#define QEH_CHECK_QUANTUM_NUMBER(qn, error_type) \
    do { \
        if (!quantum_number_is_valid(qn)) { \
            QEH_REPORT_ERROR(error_type, QEH_SEVERITY_ERROR, "Invalid Quantum Number"); \
        } \
    } while(0)

// Advanced error handling features
typedef struct qeh_error_prediction {
    qeh_error_type_t predicted_error;       // Predicted error type
    double probability;                     // Probability of occurrence
    uint64_t predicted_time;                // Predicted time of occurrence
    char prediction_basis[256];             // Basis for prediction
} qeh_error_prediction_t;

qeh_result_t qeh_enable_predictive_error_detection(bool enable);
qeh_result_t qeh_get_error_predictions(qeh_error_prediction_t **predictions,
                                      uint32_t *prediction_count);
qeh_result_t qeh_configure_error_prevention(qeh_error_type_t error_type,
                                           bool enable_prevention);

// Error context management
qeh_result_t qeh_create_error_context(qeh_error_context_t **context);
qeh_result_t qeh_destroy_error_context(qeh_error_context_t *context);
qeh_result_t qeh_copy_error_context(qeh_error_context_t *source,
                                   qeh_error_context_t **destination);
qeh_result_t qeh_merge_error_contexts(qeh_error_context_t **contexts,
                                     uint32_t context_count,
                                     qeh_error_context_t **merged_context);

#endif // QUANTUM_ERROR_HANDLER_H
