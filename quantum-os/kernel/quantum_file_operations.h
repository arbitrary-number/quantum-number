/*
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
 *
 * Copyright (c) Arbitrary Number Project Team. All rights reserved.
 */

/**
 * Quantum OS - File Operations System
 * 
 * Comprehensive file system operations for the Quantum OS with full QFS integration,
 * mathematical metadata support, and persistent ultra-scale mapping.
 * 
 * This system provides POSIX-compatible file operations while leveraging the
 * revolutionary Quantum File System for mathematical computation persistence.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_FILE_OPERATIONS_H
#define QUANTUM_FILE_OPERATIONS_H

#include "quantix_qfs.h"
#include "quantix_qfs_advanced.h"
#include "quantix_ultrascale_map_persistent.h"
#include "quantum_number.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// File operation constants
#define QFO_MAX_PATH_LENGTH 4096
#define QFO_MAX_FILENAME_LENGTH 255
#define QFO_MAX_OPEN_FILES 65536
#define QFO_BUFFER_SIZE (64 * 1024)  // 64KB default buffer
#define QFO_MAX_SYMLINK_DEPTH 32

// File access modes
#define QFO_O_RDONLY    0x0001  // Read only
#define QFO_O_WRONLY    0x0002  // Write only
#define QFO_O_RDWR      0x0003  // Read/Write
#define QFO_O_CREAT     0x0004  // Create if not exists
#define QFO_O_EXCL      0x0008  // Exclusive create
#define QFO_O_TRUNC     0x0010  // Truncate on open
#define QFO_O_APPEND    0x0020  // Append mode
#define QFO_O_SYNC      0x0040  // Synchronous I/O
#define QFO_O_ASYNC     0x0080  // Asynchronous I/O
#define QFO_O_QUANTUM   0x0100  // Quantum Number file
#define QFO_O_MATH      0x0200  // Mathematical metadata
#define QFO_O_SYMBOLIC  0x0400  // Symbolic computation

// File permissions
#define QFO_S_IRUSR     0x0100  // User read
#define QFO_S_IWUSR     0x0200  // User write
#define QFO_S_IXUSR     0x0400  // User execute
#define QFO_S_IRGRP     0x0020  // Group read
#define QFO_S_IWGRP     0x0040  // Group write
#define QFO_S_IXGRP     0x0080  // Group execute
#define QFO_S_IROTH     0x0004  // Other read
#define QFO_S_IWOTH     0x0008  // Other write
#define QFO_S_IXOTH     0x0010  // Other execute

// File types
typedef enum {
    QFO_TYPE_REGULAR = 1,       // Regular file
    QFO_TYPE_DIRECTORY = 2,     // Directory
    QFO_TYPE_SYMLINK = 3,       // Symbolic link
    QFO_TYPE_QUANTUM = 4,       // Quantum Number file
    QFO_TYPE_MATHEMATICAL = 5,  // Mathematical computation file
    QFO_TYPE_SYMBOLIC_EXPR = 6, // Symbolic expression file
    QFO_TYPE_AST = 7,          // Abstract Syntax Tree file
    QFO_TYPE_DEVICE = 8,       // Device file
    QFO_TYPE_PIPE = 9,         // Named pipe
    QFO_TYPE_SOCKET = 10       // Socket
} qfo_file_type_t;

// Seek origins
typedef enum {
    QFO_SEEK_SET = 0,  // From beginning
    QFO_SEEK_CUR = 1,  // From current position
    QFO_SEEK_END = 2   // From end
} qfo_seek_origin_t;

// File statistics structure
typedef struct {
    qfo_file_type_t type;           // File type
    uint64_t size;                  // File size in bytes
    uint64_t blocks;                // Number of blocks allocated
    uint32_t block_size;            // Block size
    uint32_t permissions;           // File permissions
    uint32_t uid;                   // User ID
    uint32_t gid;                   // Group ID
    uint64_t atime;                 // Access time
    uint64_t mtime;                 // Modification time
    uint64_t ctime;                 // Creation time
    uint64_t inode;                 // Inode number
    uint32_t links;                 // Number of hard links
    
    // Quantum-specific metadata
    bool has_quantum_data;          // Contains Quantum Numbers
    bool has_mathematical_metadata; // Contains mathematical metadata
    bool has_symbolic_expressions;  // Contains symbolic expressions
    quantum_number_t quantum_id;    // Quantum identifier
    mathematical_domain_t math_domain; // Mathematical domain
    uint32_t computation_complexity; // Computational complexity
} qfo_stat_t;

// File descriptor structure
typedef struct {
    int fd;                         // File descriptor number
    qfs_file_handle_t qfs_handle;   // QFS file handle
    char path[QFO_MAX_PATH_LENGTH]; // File path
    uint32_t flags;                 // Open flags
    uint64_t position;              // Current file position
    qfo_file_type_t type;           // File type
    bool is_open;                   // File is open
    bool is_quantum;                // Quantum Number file
    bool is_mathematical;           // Mathematical file
    uint32_t ref_count;             // Reference count
    
    // Buffering
    void *buffer;                   // I/O buffer
    size_t buffer_size;             // Buffer size
    size_t buffer_pos;              // Buffer position
    bool buffer_dirty;              // Buffer needs flush
    
    // Quantum-specific data
    qump_persistent_context_t *quantum_context; // Quantum persistence
    ast_node_t *ast_cache;          // Cached AST
    mathematical_metadata_t *math_metadata; // Mathematical metadata
} qfo_file_descriptor_t;

// Directory entry structure
typedef struct {
    char name[QFO_MAX_FILENAME_LENGTH + 1]; // Entry name
    qfo_file_type_t type;           // Entry type
    uint64_t inode;                 // Inode number
    uint64_t size;                  // Entry size
    quantum_number_t quantum_id;    // Quantum identifier (if applicable)
    mathematical_domain_t math_domain; // Mathematical domain (if applicable)
} qfo_dirent_t;

// Directory handle structure
typedef struct {
    char path[QFO_MAX_PATH_LENGTH]; // Directory path
    qfs_file_handle_t qfs_handle;   // QFS directory handle
    qfo_dirent_t *entries;          // Directory entries
    size_t entry_count;             // Number of entries
    size_t current_entry;           // Current entry index
    bool is_open;                   // Directory is open
} qfo_dir_handle_t;

// File operation result codes
typedef enum {
    QFO_SUCCESS = 0,
    QFO_ERROR_INVALID_PATH = -1,
    QFO_ERROR_FILE_NOT_FOUND = -2,
    QFO_ERROR_PERMISSION_DENIED = -3,
    QFO_ERROR_FILE_EXISTS = -4,
    QFO_ERROR_NOT_DIRECTORY = -5,
    QFO_ERROR_IS_DIRECTORY = -6,
    QFO_ERROR_TOO_MANY_OPEN_FILES = -7,
    QFO_ERROR_INVALID_DESCRIPTOR = -8,
    QFO_ERROR_IO_ERROR = -9,
    QFO_ERROR_NO_SPACE = -10,
    QFO_ERROR_NAME_TOO_LONG = -11,
    QFO_ERROR_SYMLINK_LOOP = -12,
    QFO_ERROR_QUANTUM_ERROR = -13,
    QFO_ERROR_MATHEMATICAL_ERROR = -14,
    QFO_ERROR_QFS_ERROR = -15,
    QFO_ERROR_OUT_OF_MEMORY = -16,
    QFO_ERROR_INVALID_ARGUMENT = -17,
    QFO_ERROR_NOT_IMPLEMENTED = -18
} qfo_result_t;

// Core file operations
qfo_result_t qfo_initialize(void);
qfo_result_t qfo_cleanup(void);

// File operations
int qfo_open(const char *path, uint32_t flags, uint32_t mode);
qfo_result_t qfo_close(int fd);
ssize_t qfo_read(int fd, void *buffer, size_t count);
ssize_t qfo_write(int fd, const void *buffer, size_t count);
off_t qfo_lseek(int fd, off_t offset, qfo_seek_origin_t whence);
qfo_result_t qfo_fsync(int fd);
qfo_result_t qfo_ftruncate(int fd, off_t length);

// File metadata operations
qfo_result_t qfo_stat(const char *path, qfo_stat_t *stat_buf);
qfo_result_t qfo_fstat(int fd, qfo_stat_t *stat_buf);
qfo_result_t qfo_lstat(const char *path, qfo_stat_t *stat_buf);
qfo_result_t qfo_chmod(const char *path, uint32_t mode);
qfo_result_t qfo_fchmod(int fd, uint32_t mode);
qfo_result_t qfo_chown(const char *path, uint32_t uid, uint32_t gid);
qfo_result_t qfo_fchown(int fd, uint32_t uid, uint32_t gid);

// Directory operations
qfo_result_t qfo_mkdir(const char *path, uint32_t mode);
qfo_result_t qfo_rmdir(const char *path);
qfo_dir_handle_t* qfo_opendir(const char *path);
qfo_dirent_t* qfo_readdir(qfo_dir_handle_t *dir);
qfo_result_t qfo_closedir(qfo_dir_handle_t *dir);
qfo_result_t qfo_rewinddir(qfo_dir_handle_t *dir);

// File system operations
qfo_result_t qfo_unlink(const char *path);
qfo_result_t qfo_rename(const char *old_path, const char *new_path);
qfo_result_t qfo_link(const char *old_path, const char *new_path);
qfo_result_t qfo_symlink(const char *target, const char *link_path);
ssize_t qfo_readlink(const char *path, char *buffer, size_t buffer_size);

// Quantum Number file operations
qfo_result_t qfo_create_quantum_file(const char *path, const quantum_number_t *qn,
                                     const mathematical_metadata_t *metadata);
qfo_result_t qfo_read_quantum_file(const char *path, quantum_number_t **qn,
                                   mathematical_metadata_t **metadata);
qfo_result_t qfo_write_quantum_data(int fd, const quantum_number_t *qn,
                                    const mathematical_metadata_t *metadata);
qfo_result_t qfo_read_quantum_data(int fd, quantum_number_t **qn,
                                   mathematical_metadata_t **metadata);

// AST file operations
qfo_result_t qfo_create_ast_file(const char *path, const ast_node_t *ast_root);
qfo_result_t qfo_read_ast_file(const char *path, ast_node_t **ast_root);
qfo_result_t qfo_write_ast_data(int fd, const ast_node_t *ast_root);
qfo_result_t qfo_read_ast_data(int fd, ast_node_t **ast_root);

// Mathematical computation file operations
qfo_result_t qfo_store_computation_result(const char *path,
                                          const quantum_number_t *result,
                                          const mathematical_metadata_t *metadata,
                                          const ast_node_t *computation_ast);
qfo_result_t qfo_load_computation_result(const char *path,
                                         quantum_number_t **result,
                                         mathematical_metadata_t **metadata,
                                         ast_node_t **computation_ast);

// Symbolic expression file operations
qfo_result_t qfo_store_symbolic_expression(const char *path,
                                           const char *expression,
                                           const ast_node_t *parsed_ast,
                                           const mathematical_metadata_t *metadata);
qfo_result_t qfo_load_symbolic_expression(const char *path,
                                          char **expression,
                                          ast_node_t **parsed_ast,
                                          mathematical_metadata_t **metadata);

// Advanced file operations
qfo_result_t qfo_copy_file(const char *src_path, const char *dest_path);
qfo_result_t qfo_move_file(const char *src_path, const char *dest_path);
qfo_result_t qfo_create_backup(const char *path, const char *backup_path);
qfo_result_t qfo_restore_backup(const char *backup_path, const char *restore_path);

// File locking
qfo_result_t qfo_lock_file(int fd, bool exclusive);
qfo_result_t qfo_unlock_file(int fd);
qfo_result_t qfo_try_lock_file(int fd, bool exclusive);

// Memory-mapped file operations
void* qfo_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
qfo_result_t qfo_munmap(void *addr, size_t length);
qfo_result_t qfo_msync(void *addr, size_t length, int flags);

// File system information
qfo_result_t qfo_statvfs(const char *path, struct statvfs *buf);
qfo_result_t qfo_get_filesystem_info(const char *path, char *fs_type, size_t type_size);

// Path manipulation utilities
qfo_result_t qfo_realpath(const char *path, char *resolved_path);
qfo_result_t qfo_dirname(const char *path, char *dir_name, size_t dir_size);
qfo_result_t qfo_basename(const char *path, char *base_name, size_t base_size);
qfo_result_t qfo_join_path(const char *dir, const char *file, char *result, size_t result_size);
bool qfo_is_absolute_path(const char *path);
bool qfo_path_exists(const char *path);

// File type detection
qfo_file_type_t qfo_detect_file_type(const char *path);
bool qfo_is_quantum_file(const char *path);
bool qfo_is_mathematical_file(const char *path);
bool qfo_is_symbolic_file(const char *path);

// Batch operations
qfo_result_t qfo_batch_create_files(const char **paths, size_t count, uint32_t mode);
qfo_result_t qfo_batch_delete_files(const char **paths, size_t count);
qfo_result_t qfo_batch_copy_files(const char **src_paths, const char **dest_paths, size_t count);

// File search and filtering
qfo_result_t qfo_find_files(const char *directory, const char *pattern,
                            char ***results, size_t *result_count);
qfo_result_t qfo_find_quantum_files(const char *directory, mathematical_domain_t domain,
                                    char ***results, size_t *result_count);
qfo_result_t qfo_find_mathematical_files(const char *directory, uint32_t complexity_min,
                                         char ***results, size_t *result_count);

// File compression and encryption
qfo_result_t qfo_compress_file(const char *src_path, const char *dest_path, uint32_t level);
qfo_result_t qfo_decompress_file(const char *src_path, const char *dest_path);
qfo_result_t qfo_encrypt_file(const char *src_path, const char *dest_path, const uint8_t *key);
qfo_result_t qfo_decrypt_file(const char *src_path, const char *dest_path, const uint8_t *key);

// File integrity and verification
qfo_result_t qfo_calculate_checksum(const char *path, uint32_t *checksum);
qfo_result_t qfo_verify_file_integrity(const char *path, bool *is_valid);
qfo_result_t qfo_repair_file_corruption(const char *path, bool *repair_successful);

// Performance monitoring
typedef struct {
    uint64_t files_opened;          // Total files opened
    uint64_t files_closed;          // Total files closed
    uint64_t bytes_read;            // Total bytes read
    uint64_t bytes_written;         // Total bytes written
    uint64_t quantum_operations;    // Quantum file operations
    uint64_t mathematical_operations; // Mathematical file operations
    uint64_t cache_hits;            // Buffer cache hits
    uint64_t cache_misses;          // Buffer cache misses
    double average_read_time_ms;    // Average read time
    double average_write_time_ms;   // Average write time
} qfo_performance_stats_t;

qfo_result_t qfo_get_performance_stats(qfo_performance_stats_t *stats);
qfo_result_t qfo_reset_performance_stats(void);

// Error handling
const char* qfo_result_to_string(qfo_result_t result);
qfo_result_t qfo_get_last_error(void);
void qfo_set_error_handler(void (*handler)(qfo_result_t error, const char *message));

// Configuration and tuning
qfo_result_t qfo_set_buffer_size(size_t size);
qfo_result_t qfo_set_max_open_files(uint32_t max_files);
qfo_result_t qfo_enable_async_io(bool enable);
qfo_result_t qfo_set_cache_size(size_t cache_size);

// Debug and diagnostics
qfo_result_t qfo_dump_open_files(const char *dump_path);
qfo_result_t qfo_validate_file_system(const char *path, bool *is_valid);
qfo_result_t qfo_benchmark_operations(const char *test_dir, uint32_t operation_count);

#ifdef __cplusplus
}
#endif

#endif // QUANTUM_FILE_OPERATIONS_H
