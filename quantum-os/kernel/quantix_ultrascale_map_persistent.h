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
 * Quantix Ultra-Scale Map Persistent (QUMP) - Revolutionary Persistent secp256k1-based mapping system
 * 
 * This is the world's first persistent ultra-scale map data structure that uses the secp256k1
 * elliptic curve to provide 2^256 buckets for key-value storage with full QFS integration.
 * Unlike traditional persistent hash tables, this system maintains constant-time performance
 * regardless of scale and provides both synchronous and asynchronous persistence modes.
 * 
 * Key Features:
 * - 2^256 theoretical buckets (more than atoms in the observable universe)
 * - UTF-8 keys converted to secp256k1 curve points
 * - Constant-time O(1) operations at any scale
 * - Full QFS integration for mathematical persistence
 * - Synchronous and asynchronous persistence modes
 * - Cryptographically secure key distribution
 * - Mathematical collision resistance
 * - Quantum-resistant design principles
 * - Atomic transactions with rollback support
 * - Write-ahead logging for crash recovery
 * - Compression and encryption support
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTIX_ULTRASCALE_MAP_PERSISTENT_H
#define QUANTIX_ULTRASCALE_MAP_PERSISTENT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>
#include "quantum_number.h"
#include "quantix_ecc.h"
#include "quantix_qfs.h"
#include "quantix_ultrascale_map.h"

#ifdef __cplusplus
extern "C" {
#endif

// Persistent Ultra-Scale Map Configuration
#define QUMP_MAX_PERSISTENCE_THREADS 16
#define QUMP_DEFAULT_SYNC_INTERVAL_MS 1000
#define QUMP_DEFAULT_CHECKPOINT_INTERVAL_MS 30000
#define QUMP_WAL_BUFFER_SIZE (4 * 1024 * 1024)  // 4MB WAL buffer
#define QUMP_COMPRESSION_THRESHOLD 1024  // Compress values > 1KB
#define QUMP_MAX_TRANSACTION_SIZE (64 * 1024 * 1024)  // 64MB max transaction
#define QUMP_PERSISTENCE_MAGIC 0x51554D50  // "QUMP"
#define QUMP_VERSION_MAJOR 1
#define QUMP_VERSION_MINOR 0

// Persistence modes
typedef enum {
    QUMP_PERSISTENCE_DISABLED = 0,
    QUMP_PERSISTENCE_SYNC = 1,        // Synchronous persistence (immediate)
    QUMP_PERSISTENCE_ASYNC = 2,       // Asynchronous persistence (batched)
    QUMP_PERSISTENCE_HYBRID = 3       // Hybrid mode (critical ops sync, others async)
} qump_persistence_mode_t;

// Persistence result codes
typedef enum {
    QUMP_SUCCESS = 0,
    QUMP_ERROR_PERSISTENCE_DISABLED = -100,
    QUMP_ERROR_QFS_OPERATION = -101,
    QUMP_ERROR_TRANSACTION_FAILED = -102,
    QUMP_ERROR_WAL_CORRUPTION = -103,
    QUMP_ERROR_CHECKPOINT_FAILED = -104,
    QUMP_ERROR_RECOVERY_FAILED = -105,
    QUMP_ERROR_COMPRESSION_FAILED = -106,
    QUMP_ERROR_ENCRYPTION_FAILED = -107,
    QUMP_ERROR_THREAD_CREATION = -108,
    QUMP_ERROR_LOCK_TIMEOUT = -109,
    QUMP_ERROR_STORAGE_FULL = -110
} qump_result_t;

// Transaction types for WAL
typedef enum {
    QUMP_TRANSACTION_PUT = 1,
    QUMP_TRANSACTION_REMOVE = 2,
    QUMP_TRANSACTION_CLEAR = 3,
    QUMP_TRANSACTION_CHECKPOINT = 4,
    QUMP_TRANSACTION_ROLLBACK = 5
} qump_transaction_type_t;

// Persistence statistics
typedef struct {
    uint64_t total_persisted_operations;
    uint64_t sync_operations;
    uint64_t async_operations;
    uint64_t failed_operations;
    uint64_t bytes_written;
    uint64_t bytes_read;
    uint64_t compression_ratio_percent;
    uint64_t last_checkpoint_time;
    uint64_t last_sync_time;
    uint32_t active_transactions;
    uint32_t pending_async_operations;
    double average_sync_time_ms;
    double average_async_time_ms;
} qump_persistence_stats_t;

// Write-Ahead Log entry
typedef struct {
    uint32_t magic;                    // QUMP_PERSISTENCE_MAGIC
    uint64_t transaction_id;           // Unique transaction ID
    uint64_t timestamp;                // Transaction timestamp
    qump_transaction_type_t type;      // Transaction type
    uint32_t key_length;               // Key length
    uint32_t value_length;             // Value length (0 for remove)
    qum_value_type_t value_type;       // Value type
    uint32_t checksum;                 // CRC32 checksum
    // Followed by: key data, value data (if any)
} qump_wal_entry_t;

// Persistence configuration
typedef struct {
    qump_persistence_mode_t mode;
    char storage_path[512];            // QFS path for persistent storage
    char wal_path[512];                // QFS path for WAL
    uint32_t sync_interval_ms;         // Async sync interval
    uint32_t checkpoint_interval_ms;   // Checkpoint interval
    uint32_t max_wal_size_mb;          // Max WAL size before rotation
    uint32_t compression_level;        // 0-9, 0=disabled
    bool enable_encryption;            // Enable value encryption
    bool enable_checksums;             // Enable integrity checksums
    bool enable_crash_recovery;       // Enable crash recovery
    uint32_t max_concurrent_ops;       // Max concurrent persistence ops
    uint32_t lock_timeout_ms;          // Lock timeout for operations
} qump_persistence_config_t;

// Async operation queue entry
typedef struct qump_async_op {
    qump_transaction_type_t type;
    char *key;
    void *value;
    size_t value_size;
    qum_value_type_t value_type;
    uint64_t timestamp;
    struct qump_async_op *next;
} qump_async_op_t;

// Transaction context for atomic operations
typedef struct {
    uint64_t transaction_id;
    uint64_t start_time;
    uint32_t operation_count;
    size_t total_size;
    bool is_active;
    bool is_committed;
    qump_wal_entry_t *operations;
    uint32_t operations_capacity;
} qump_transaction_t;

// Persistent context extension
typedef struct {
    // Base ultra-scale map context
    qum_context_t *base_context;
    
    // Persistence configuration
    qump_persistence_config_t config;
    
    // QFS integration
    qfs_file_handle_t storage_handle;
    qfs_file_handle_t wal_handle;
    char qfs_storage_path[512];
    char qfs_wal_path[512];
    
    // Write-Ahead Logging
    void *wal_buffer;
    size_t wal_buffer_size;
    size_t wal_buffer_used;
    uint64_t wal_sequence_number;
    pthread_mutex_t wal_mutex;
    
    // Async persistence
    pthread_t persistence_thread;
    pthread_mutex_t async_queue_mutex;
    pthread_cond_t async_queue_cond;
    qump_async_op_t *async_queue_head;
    qump_async_op_t *async_queue_tail;
    uint32_t async_queue_size;
    bool async_thread_running;
    
    // Transaction management
    qump_transaction_t *current_transaction;
    pthread_mutex_t transaction_mutex;
    uint64_t next_transaction_id;
    
    // Synchronization
    pthread_rwlock_t persistence_lock;
    pthread_mutex_t checkpoint_mutex;
    
    // Statistics and monitoring
    qump_persistence_stats_t stats;
    uint64_t last_checkpoint_time;
    uint64_t last_sync_time;
    
    // State management
    bool is_initialized;
    bool is_recovering;
    bool is_shutting_down;
    uint32_t active_operations;
} qump_persistent_context_t;

// Core Persistent Ultra-Scale Map Operations
qump_result_t qump_initialize(qump_persistent_context_t **context, 
                              const qum_configuration_t *base_config,
                              const qump_persistence_config_t *persistence_config);
qump_result_t qump_destroy(qump_persistent_context_t *context);

// Enhanced operations with persistence
qump_result_t qump_put_persistent(qump_persistent_context_t *context, const char *key, 
                                  const void *value, size_t value_size, 
                                  qum_value_type_t type, bool force_sync);
qump_result_t qump_get_persistent(qump_persistent_context_t *context, const char *key, 
                                  void **value, size_t *value_size, qum_value_type_t *type);
qump_result_t qump_remove_persistent(qump_persistent_context_t *context, const char *key, 
                                     bool force_sync);
qump_result_t qump_contains_persistent(qump_persistent_context_t *context, const char *key, 
                                       bool *exists);
qump_result_t qump_clear_persistent(qump_persistent_context_t *context, bool force_sync);

// Transaction support
qump_result_t qump_begin_transaction(qump_persistent_context_t *context, 
                                     qump_transaction_t **transaction);
qump_result_t qump_commit_transaction(qump_persistent_context_t *context, 
                                      qump_transaction_t *transaction);
qump_result_t qump_rollback_transaction(qump_persistent_context_t *context, 
                                        qump_transaction_t *transaction);
qump_result_t qump_transaction_put(qump_persistent_context_t *context, 
                                   qump_transaction_t *transaction,
                                   const char *key, const void *value, 
                                   size_t value_size, qum_value_type_t type);
qump_result_t qump_transaction_remove(qump_persistent_context_t *context, 
                                      qump_transaction_t *transaction, const char *key);

// Persistence control operations
qump_result_t qump_sync_to_storage(qump_persistent_context_t *context);
qump_result_t qump_create_checkpoint(qump_persistent_context_t *context);
qump_result_t qump_load_from_storage(qump_persistent_context_t *context);
qump_result_t qump_recover_from_wal(qump_persistent_context_t *context);
qump_result_t qump_compact_storage(qump_persistent_context_t *context);
qump_result_t qump_verify_storage_integrity(qump_persistent_context_t *context, bool *is_valid);

// Persistence mode control
qump_result_t qump_set_persistence_mode(qump_persistent_context_t *context, 
                                        qump_persistence_mode_t mode);
qump_result_t qump_get_persistence_mode(qump_persistent_context_t *context, 
                                        qump_persistence_mode_t *mode);
qump_result_t qump_pause_persistence(qump_persistent_context_t *context);
qump_result_t qump_resume_persistence(qump_persistent_context_t *context);

// Advanced QFS integration
qump_result_t qump_store_as_qfs_ast(qump_persistent_context_t *context, const char *key, 
                                    ast_node_t *ast_root);
qump_result_t qump_load_qfs_ast(qump_persistent_context_t *context, const char *key, 
                                ast_node_t **ast_root);
qump_result_t qump_store_quantum_computation(qump_persistent_context_t *context, 
                                             const char *key, 
                                             const quantum_number_t *computation_result,
                                             const mathematical_metadata_t *metadata);
qump_result_t qump_query_mathematical_domain(qump_persistent_context_t *context,
                                             mathematical_domain_t domain,
                                             char ***matching_keys, size_t *count);

// Performance and monitoring
qump_result_t qump_get_persistence_stats(qump_persistent_context_t *context, 
                                         qump_persistence_stats_t *stats);
qump_result_t qump_get_combined_metrics(qump_persistent_context_t *context,
                                        qum_performance_metrics_t *base_metrics,
                                        qump_persistence_stats_t *persistence_stats);
qump_result_t qump_optimize_persistence(qump_persistent_context_t *context);
qump_result_t qump_defragment_storage(qump_persistent_context_t *context);

// Configuration and utilities
qump_result_t qump_create_default_persistence_config(qump_persistence_config_t *config);
qump_result_t qump_validate_persistence_config(const qump_persistence_config_t *config);
qump_result_t qump_estimate_storage_requirements(const qump_persistence_config_t *config,
                                                 uint64_t expected_entries,
                                                 size_t average_value_size,
                                                 uint64_t *estimated_bytes);

// Backup and restore operations
qump_result_t qump_create_backup(qump_persistent_context_t *context, const char *backup_path);
qump_result_t qump_restore_from_backup(qump_persistent_context_t *context, 
                                       const char *backup_path);
qump_result_t qump_export_to_qfs_format(qump_persistent_context_t *context, 
                                        const char *export_path);
qump_result_t qump_import_from_qfs_format(qump_persistent_context_t *context, 
                                          const char *import_path);

// Compression and encryption utilities
qump_result_t qump_compress_value(const void *input, size_t input_size,
                                  void **output, size_t *output_size, uint32_t level);
qump_result_t qump_decompress_value(const void *input, size_t input_size,
                                    void **output, size_t *output_size);
qump_result_t qump_encrypt_value(const void *input, size_t input_size,
                                 void **output, size_t *output_size, const uint8_t *key);
qump_result_t qump_decrypt_value(const void *input, size_t input_size,
                                 void **output, size_t *output_size, const uint8_t *key);

// Internal persistence functions (implementation use only)
qump_result_t qump_write_wal_entry(qump_persistent_context_t *context, 
                                   const qump_wal_entry_t *entry,
                                   const char *key, const void *value);
qump_result_t qump_read_wal_entry(qump_persistent_context_t *context, 
                                  qump_wal_entry_t *entry,
                                  char **key, void **value);
qump_result_t qump_flush_wal_buffer(qump_persistent_context_t *context);
qump_result_t qump_rotate_wal_file(qump_persistent_context_t *context);
qump_result_t qump_cleanup_old_wal_files(qump_persistent_context_t *context);

// Async persistence thread functions
void* qump_async_persistence_thread(void *arg);
qump_result_t qump_enqueue_async_operation(qump_persistent_context_t *context,
                                           qump_transaction_type_t type,
                                           const char *key, const void *value,
                                           size_t value_size, qum_value_type_t value_type);
qump_result_t qump_process_async_queue(qump_persistent_context_t *context);

// Error handling and diagnostics
const char* qump_result_to_string(qump_result_t result);
qump_result_t qump_diagnose_persistence_issues(qump_persistent_context_t *context,
                                               char **diagnostic_report);
qump_result_t qump_repair_storage_corruption(qump_persistent_context_t *context,
                                             bool *repair_successful);

// Debug and testing functions
qump_result_t qump_dump_persistence_state(qump_persistent_context_t *context,
                                          const char *dump_path);
qump_result_t qump_simulate_crash_recovery(qump_persistent_context_t *context);
qump_result_t qump_benchmark_persistence(qump_persistent_context_t *context,
                                         uint32_t operation_count,
                                         double *sync_ops_per_second,
                                         double *async_ops_per_second);

#ifdef __cplusplus
}
#endif

#endif // QUANTIX_ULTRASCALE_MAP_PERSISTENT_H
