/**
 * Quantix File System (QFS) - Core Header
 * 
 * The first file system designed from the ground up for symbolic mathematical
 * computation and native Quantum Number operations.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTIX_QFS_H
#define QUANTIX_QFS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "quantum_number.h"

// Forward declarations
typedef struct qfs_mathematical_block qfs_mathematical_block_t;
typedef struct qfs_symbolic_directory qfs_symbolic_directory_t;
typedef struct qfs_ast_file qfs_ast_file_t;
typedef struct qfs_quantum_storage qfs_quantum_storage_t;
typedef struct qfs_mathematical_index qfs_mathematical_index_t;

// Mathematical domain classifications
typedef enum {
    MATH_DOMAIN_ALGEBRA = 0x01,
    MATH_DOMAIN_CALCULUS = 0x02,
    MATH_DOMAIN_GEOMETRY = 0x04,
    MATH_DOMAIN_TOPOLOGY = 0x08,
    MATH_DOMAIN_NUMBER_THEORY = 0x10,
    MATH_DOMAIN_ANALYSIS = 0x20,
    MATH_DOMAIN_STATISTICS = 0x40,
    MATH_DOMAIN_LOGIC = 0x80
} mathematical_domain_t;

// Mathematical file types
typedef enum {
    QFS_FILE_QUANTUM_NUMBERS,
    QFS_FILE_SYMBOLIC_EXPRESSION,
    QFS_FILE_AST_COMPUTATION,
    QFS_FILE_MATHEMATICAL_PROOF,
    QFS_FILE_COMPUTATIONAL_CONTEXT,
    QFS_FILE_RELATIONSHIP_MAP
} mathematical_file_type_t;

// Symbolic properties structure
typedef struct {
    uint32_t is_commutative : 1;
    uint32_t is_associative : 1;
    uint32_t is_distributive : 1;
    uint32_t is_invertible : 1;
    uint32_t is_continuous : 1;
    uint32_t is_differentiable : 1;
    uint32_t is_integrable : 1;
    uint32_t has_closed_form : 1;
    uint32_t is_polynomial : 1;
    uint32_t is_transcendental : 1;
    uint32_t is_algebraic : 1;
    uint32_t reserved : 21;
} symbolic_properties_t;

// Mathematical complexity measure
typedef struct {
    uint32_t computational_depth;
    uint32_t symbolic_complexity;
    uint32_t memory_requirements;
    uint32_t time_complexity_class;
} complexity_measure_t;

// AST node structure for mathematical expressions
typedef struct ast_node {
    enum {
        AST_QUANTUM_NUMBER,
        AST_VARIABLE,
        AST_OPERATOR,
        AST_FUNCTION,
        AST_CONSTANT
    } type;
    
    union {
        quantum_number_t quantum_value;
        char variable_name[64];
        struct {
            char operator_symbol[16];
            struct ast_node **operands;
            size_t operand_count;
        } operator;
        struct {
            char function_name[64];
            struct ast_node **arguments;
            size_t argument_count;
        } function;
        struct {
            char constant_name[64];
            quantum_number_t constant_value;
        } constant;
    } data;
    
    // Mathematical metadata
    symbolic_properties_t properties;
    complexity_measure_t complexity;
    quantum_number_t mathematical_signature;
    
    // Tree structure
    struct ast_node *parent;
    struct ast_node **children;
    size_t child_count;
} ast_node_t;

// Computational history tracking
typedef struct {
    uint64_t creation_time;
    uint64_t last_modified;
    uint32_t transformation_count;
    struct {
        uint64_t timestamp;
        char transformation_type[64];
        quantum_number_t before_signature;
        quantum_number_t after_signature;
    } *transformations;
} computational_history_t;

// Mathematical metadata structure
typedef struct {
    mathematical_domain_t primary_domain;
    mathematical_domain_t secondary_domains;
    symbolic_properties_t properties;
    complexity_measure_t complexity;
    quantum_number_t mathematical_signature;
    computational_history_t history;
    char description[256];
    char author[128];
    uint64_t creation_date;
} mathematical_metadata_t;

// Mathematical Block Structure
struct qfs_mathematical_block {
    // Block header with mathematical properties
    quantum_number_t block_id;
    quantum_number_t mathematical_type;
    complexity_measure_t complexity_measure;
    uint32_t symbolic_depth;
    
    // Mathematical metadata
    mathematical_domain_t domain;
    symbolic_properties_t properties;
    computational_history_t history;
    
    // Block structure
    size_t data_size;
    void *mathematical_data;
    ast_node_t *ast_representation;
    
    // Integrity and verification
    quantum_number_t quantum_checksum;
    uint64_t creation_time;
    uint64_t last_modified;
    uint32_t verification_status;
    
    // Storage management
    struct qfs_mathematical_block *next_block;
    struct qfs_mathematical_block *prev_block;
};

// Symbolic Directory Structure
struct qfs_symbolic_directory {
    quantum_number_t directory_id;
    char name[256];
    mathematical_file_type_t type;
    
    // Mathematical organization
    symbolic_properties_t properties;
    mathematical_domain_t primary_domain;
    complexity_measure_t complexity_range;
    
    // Directory structure
    struct qfs_symbolic_directory *parent;
    struct qfs_symbolic_directory **children;
    struct qfs_ast_file **files;
    uint32_t child_count;
    uint32_t file_count;
    
    // Mathematical indexing
    struct qfs_mathematical_index *expression_index;
    struct qfs_mathematical_index *relationship_index;
    struct qfs_mathematical_index *complexity_index;
    
    // Access control and metadata
    uint32_t permissions;
    quantum_number_t access_checksum;
    uint64_t last_accessed;
};

// AST-Native File Structure
struct qfs_ast_file {
    quantum_number_t file_id;
    char filename[256];
    mathematical_file_type_t type;
    
    // AST structure
    ast_node_t *root_node;
    size_t node_count;
    uint32_t tree_depth;
    complexity_measure_t complexity_measure;
    
    // Mathematical properties
    mathematical_domain_t domain;
    symbolic_properties_t properties;
    quantum_number_t mathematical_signature;
    
    // Storage optimization
    void *evaluation_cache;
    uint32_t optimization_flags;
    uint32_t compression_type;
    
    // Metadata and history
    computational_history_t history;
    mathematical_metadata_t metadata;
    uint32_t verification_status;
    
    // File system integration
    qfs_mathematical_block_t **blocks;
    uint32_t block_count;
    quantum_number_t integrity_checksum;
    
    // Directory reference
    qfs_symbolic_directory_t *parent_directory;
};

// Quantum Number Storage Structure
struct qfs_quantum_storage {
    // Storage configuration
    size_t quantum_array_size;
    quantum_number_t *quantum_data;
    uint32_t layout_optimization;
    
    // Mathematical indexing
    struct {
        uint32_t *indices;
        size_t count;
    } ordinal_indices[12];
    
    struct {
        uint32_t *indices;
        size_t count;
    } sign_indices[12];
    
    struct {
        uint32_t *indices;
        size_t count;
    } checksum_index;
    
    // Compression and optimization
    uint32_t compression_algorithm;
    uint32_t compression_ratio;
    void *pattern_cache;
    
    // Access patterns
    struct {
        uint64_t read_count;
        uint64_t write_count;
        uint64_t last_access;
        uint32_t access_pattern;
    } access_stats;
};

// Mathematical Index Structure
struct qfs_mathematical_index {
    // Index types
    enum {
        QFS_INDEX_QUANTUM_NUMBER,
        QFS_INDEX_SYMBOLIC_EXPRESSION,
        QFS_INDEX_AST_STRUCTURE,
        QFS_INDEX_MATHEMATICAL_PROPERTY
    } index_type;
    
    // Index data structures
    void *btree_root;
    void *hash_table;
    void *spatial_index;
    
    // Index statistics
    size_t entry_count;
    size_t index_size;
    uint64_t last_optimized;
    uint32_t optimization_level;
};

// QFS Result codes
typedef enum {
    QFS_SUCCESS = 0,
    QFS_ERROR_INVALID_PATH,
    QFS_ERROR_FILE_NOT_FOUND,
    QFS_ERROR_PERMISSION_DENIED,
    QFS_ERROR_MATHEMATICAL_INCONSISTENCY,
    QFS_ERROR_CHECKSUM_MISMATCH,
    QFS_ERROR_MEMORY_ALLOCATION,
    QFS_ERROR_INVALID_QUANTUM_NUMBER,
    QFS_ERROR_AST_MALFORMED,
    QFS_ERROR_SYMBOLIC_ERROR,
    QFS_ERROR_COMPRESSION_FAILED,
    QFS_ERROR_INDEX_CORRUPTED
} qfs_result_t;

// File handle structure
typedef struct {
    qfs_ast_file_t *file;
    uint32_t access_mode;
    uint64_t position;
    uint32_t flags;
} qfs_file_handle_t;

// QFS mode flags
typedef enum {
    QFS_MODE_READ = 0x01,
    QFS_MODE_WRITE = 0x02,
    QFS_MODE_APPEND = 0x04,
    QFS_MODE_CREATE = 0x08,
    QFS_MODE_MATHEMATICAL = 0x10,
    QFS_MODE_SYMBOLIC = 0x20,
    QFS_MODE_AST_NATIVE = 0x40
} qfs_mode_t;

// Core QFS API Functions

// File operations
qfs_result_t qfs_create_mathematical_file(const char *path, 
                                         mathematical_metadata_t *properties);
qfs_result_t qfs_open_quantum_file(const char *path, qfs_mode_t mode, 
                                  qfs_file_handle_t *handle);
qfs_result_t qfs_close_file(qfs_file_handle_t *handle);
qfs_result_t qfs_read_quantum_numbers(qfs_file_handle_t *handle, 
                                     quantum_number_t *buffer, size_t count);
qfs_result_t qfs_write_quantum_numbers(qfs_file_handle_t *handle, 
                                      const quantum_number_t *buffer, size_t count);
qfs_result_t qfs_write_symbolic_expression(qfs_file_handle_t *handle, 
                                          ast_node_t *expression);

// AST operations
qfs_result_t qfs_load_ast(const char *path, ast_node_t **root);
qfs_result_t qfs_save_ast(const char *path, ast_node_t *root);
qfs_result_t qfs_evaluate_ast_file(const char *path, quantum_number_t *result);
qfs_result_t qfs_optimize_ast_file(const char *path);

// Directory operations
qfs_result_t qfs_create_symbolic_directory(const char *path, 
                                          mathematical_domain_t domain);
qfs_result_t qfs_list_mathematical_files(const char *path, 
                                        qfs_ast_file_t ***files, 
                                        size_t *count);

// Mathematical queries
qfs_result_t qfs_find_expressions_by_pattern(const char *pattern, 
                                            mathematical_domain_t domain,
                                            qfs_ast_file_t ***results,
                                            size_t *count);
qfs_result_t qfs_search_by_complexity(complexity_measure_t min_complexity,
                                     complexity_measure_t max_complexity,
                                     qfs_ast_file_t ***results,
                                     size_t *count);

// Verification and integrity
qfs_result_t qfs_verify_mathematical_integrity(const char *path);
qfs_result_t qfs_compute_quantum_checksum(const char *path, 
                                         quantum_number_t *checksum);
qfs_result_t qfs_validate_mathematical_properties(const char *path,
                                                 symbolic_properties_t *properties);

// Mathematical metadata
qfs_result_t qfs_get_mathematical_metadata(const char *path,
                                          mathematical_metadata_t *metadata);
qfs_result_t qfs_set_mathematical_properties(const char *path,
                                            symbolic_properties_t *properties);
qfs_result_t qfs_get_computational_history(const char *path,
                                          computational_history_t *history);

// System initialization and management
qfs_result_t qfs_initialize_filesystem(void);
qfs_result_t qfs_shutdown_filesystem(void);
qfs_result_t qfs_mount_mathematical_volume(const char *device_path, 
                                          const char *mount_point);
qfs_result_t qfs_unmount_mathematical_volume(const char *mount_point);

// Performance and optimization
qfs_result_t qfs_optimize_mathematical_storage(const char *path);
qfs_result_t qfs_rebuild_mathematical_indices(const char *path);
qfs_result_t qfs_compress_mathematical_data(const char *path, 
                                           uint32_t compression_level);

// Internal utility functions (for implementation use)
qfs_mathematical_block_t* qfs_allocate_mathematical_block(size_t data_size);
void qfs_free_mathematical_block(qfs_mathematical_block_t *block);
qfs_result_t qfs_compute_mathematical_checksum(const void *data, size_t size,
                                              quantum_number_t *checksum);
qfs_result_t qfs_validate_ast_structure(ast_node_t *root);
qfs_result_t qfs_optimize_ast_structure(ast_node_t **root);

// AST utility functions
ast_node_t* qfs_create_ast_node(int type);
void qfs_free_ast_node(ast_node_t *node);
qfs_result_t qfs_copy_ast_node(ast_node_t *source, ast_node_t **destination);
qfs_result_t qfs_evaluate_ast_node(ast_node_t *node, quantum_number_t *result);
size_t qfs_count_ast_nodes(ast_node_t *root);
uint32_t qfs_compute_ast_depth(ast_node_t *root);
qfs_result_t qfs_compute_ast_signature(ast_node_t *node, quantum_number_t *signature);

// Mathematical indexing functions
qfs_result_t qfs_create_mathematical_index(qfs_mathematical_index_t **index, 
                                          int index_type);
qfs_result_t qfs_insert_into_index(qfs_mathematical_index_t *index, 
                                  const void *key, void *value);
qfs_result_t qfs_search_index(qfs_mathematical_index_t *index, 
                             const void *key, void **value);
qfs_result_t qfs_optimize_index(qfs_mathematical_index_t *index);

#endif // QUANTIX_QFS_H
