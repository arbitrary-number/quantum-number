/**
 * Quantix File System (QFS) - AST Operations Implementation
 * 
 * AST-native file operations and symbolic expression handling for QFS.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include "quantix_qfs.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Internal AST operation functions
static qfs_result_t qfs_serialize_ast_node(ast_node_t *node, void **buffer, size_t *size);
static qfs_result_t qfs_deserialize_ast_node(const void *buffer, size_t size, ast_node_t **node);
static qfs_result_t qfs_compute_ast_complexity(ast_node_t *node, complexity_measure_t *complexity);
static qfs_result_t qfs_optimize_ast_node(ast_node_t **node);
static qfs_result_t qfs_evaluate_ast_recursive(ast_node_t *node, quantum_number_t *result);

/**
 * Load AST from a mathematical file
 */
qfs_result_t qfs_load_ast(const char *path, ast_node_t **root) {
    if (!path || !root) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    // Open the file
    qfs_file_handle_t handle;
    qfs_result_t result = qfs_open_quantum_file(path, QFS_MODE_READ | QFS_MODE_AST_NATIVE, &handle);
    if (result != QFS_SUCCESS) {
        return result;
    }
    
    // Check if file has AST representation
    if (!handle.file->root_node) {
        qfs_close_file(&handle);
        return QFS_ERROR_AST_MALFORMED;
    }
    
    // Copy the AST structure
    result = qfs_copy_ast_node(handle.file->root_node, root);
    
    qfs_close_file(&handle);
    return result;
}

/**
 * Save AST to a mathematical file
 */
qfs_result_t qfs_save_ast(const char *path, ast_node_t *root) {
    if (!path || !root) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    // Open or create the file
    qfs_file_handle_t handle;
    qfs_result_t result = qfs_open_quantum_file(path, 
                                               QFS_MODE_WRITE | QFS_MODE_CREATE | QFS_MODE_AST_NATIVE, 
                                               &handle);
    if (result != QFS_SUCCESS) {
        return result;
    }
    
    // Free existing AST if present
    if (handle.file->root_node) {
        qfs_free_ast_node(handle.file->root_node);
    }
    
    // Copy the new AST
    result = qfs_copy_ast_node(root, &handle.file->root_node);
    if (result != QFS_SUCCESS) {
        qfs_close_file(&handle);
        return result;
    }
    
    // Update file metadata
    qfs_compute_ast_complexity(handle.file->root_node, &handle.file->complexity_measure);
    handle.file->node_count = qfs_count_ast_nodes(handle.file->root_node);
    handle.file->tree_depth = qfs_compute_ast_depth(handle.file->root_node);
    
    // Compute mathematical signature
    qfs_compute_ast_signature(handle.file->root_node, &handle.file->mathematical_signature);
    
    // Serialize AST to mathematical blocks
    void *serialized_data;
    size_t serialized_size;
    result = qfs_serialize_ast_node(handle.file->root_node, &serialized_data, &serialized_size);
    if (result == QFS_SUCCESS) {
        // Store serialized AST in mathematical blocks
        if (handle.file->block_count == 0) {
            qfs_mathematical_block_t *block = qfs_allocate_mathematical_block(serialized_size);
            if (block) {
                memcpy(block->mathematical_data, serialized_data, serialized_size);
                block->ast_representation = handle.file->root_node;
                
                handle.file->blocks = (qfs_mathematical_block_t**)malloc(sizeof(qfs_mathematical_block_t*));
                if (handle.file->blocks) {
                    handle.file->blocks[0] = block;
                    handle.file->block_count = 1;
                }
            }
        }
        free(serialized_data);
    }
    
    qfs_close_file(&handle);
    return result;
}

/**
 * Evaluate AST file and return result
 */
qfs_result_t qfs_evaluate_ast_file(const char *path, quantum_number_t *result) {
    if (!path || !result) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    // Load AST from file
    ast_node_t *root;
    qfs_result_t load_result = qfs_load_ast(path, &root);
    if (load_result != QFS_SUCCESS) {
        return load_result;
    }
    
    // Evaluate the AST
    qfs_result_t eval_result = qfs_evaluate_ast_node(root, result);
    
    // Clean up
    qfs_free_ast_node(root);
    
    return eval_result;
}

/**
 * Optimize AST file structure
 */
qfs_result_t qfs_optimize_ast_file(const char *path) {
    if (!path) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    // Load AST from file
    ast_node_t *root;
    qfs_result_t result = qfs_load_ast(path, &root);
    if (result != QFS_SUCCESS) {
        return result;
    }
    
    // Optimize the AST structure
    result = qfs_optimize_ast_structure(&root);
    if (result != QFS_SUCCESS) {
        qfs_free_ast_node(root);
        return result;
    }
    
    // Save optimized AST back to file
    result = qfs_save_ast(path, root);
    
    // Clean up
    qfs_free_ast_node(root);
    
    return result;
}

/**
 * Write symbolic expression to file
 */
qfs_result_t qfs_write_symbolic_expression(qfs_file_handle_t *handle, 
                                          ast_node_t *expression) {
    if (!handle || !handle->file || !expression) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    if (!(handle->access_mode & (QFS_MODE_WRITE | QFS_MODE_APPEND))) {
        return QFS_ERROR_PERMISSION_DENIED;
    }
    
    // Set the expression as the file's root AST node
    if (handle->file->root_node) {
        qfs_free_ast_node(handle->file->root_node);
    }
    
    qfs_result_t result = qfs_copy_ast_node(expression, &handle->file->root_node);
    if (result != QFS_SUCCESS) {
        return result;
    }
    
    // Update file metadata
    handle->file->type = QFS_FILE_SYMBOLIC_EXPRESSION;
    qfs_compute_ast_complexity(handle->file->root_node, &handle->file->complexity_measure);
    handle->file->node_count = qfs_count_ast_nodes(handle->file->root_node);
    handle->file->tree_depth = qfs_compute_ast_depth(handle->file->root_node);
    
    // Update mathematical signature
    qfs_compute_ast_signature(handle->file->root_node, &handle->file->mathematical_signature);
    
    return QFS_SUCCESS;
}

/**
 * Copy AST node structure
 */
qfs_result_t qfs_copy_ast_node(ast_node_t *source, ast_node_t **destination) {
    if (!source || !destination) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    // Create new node
    *destination = qfs_create_ast_node(source->type);
    if (!*destination) {
        return QFS_ERROR_MEMORY_ALLOCATION;
    }
    
    // Copy basic properties
    (*destination)->properties = source->properties;
    (*destination)->complexity = source->complexity;
    (*destination)->mathematical_signature = source->mathematical_signature;
    
    // Copy type-specific data
    switch (source->type) {
        case AST_QUANTUM_NUMBER:
            (*destination)->data.quantum_value = source->data.quantum_value;
            break;
            
        case AST_VARIABLE:
            strcpy((*destination)->data.variable_name, source->data.variable_name);
            break;
            
        case AST_OPERATOR:
            strcpy((*destination)->data.operator.operator_symbol, 
                   source->data.operator.operator_symbol);
            (*destination)->data.operator.operand_count = source->data.operator.operand_count;
            
            if (source->data.operator.operand_count > 0) {
                (*destination)->data.operator.operands = 
                    (ast_node_t**)malloc(source->data.operator.operand_count * sizeof(ast_node_t*));
                if (!(*destination)->data.operator.operands) {
                    qfs_free_ast_node(*destination);
                    return QFS_ERROR_MEMORY_ALLOCATION;
                }
                
                for (size_t i = 0; i < source->data.operator.operand_count; i++) {
                    qfs_result_t result = qfs_copy_ast_node(source->data.operator.operands[i], 
                                                           &(*destination)->data.operator.operands[i]);
                    if (result != QFS_SUCCESS) {
                        qfs_free_ast_node(*destination);
                        return result;
                    }
                    (*destination)->data.operator.operands[i]->parent = *destination;
                }
            }
            break;
            
        case AST_FUNCTION:
            strcpy((*destination)->data.function.function_name, 
                   source->data.function.function_name);
            (*destination)->data.function.argument_count = source->data.function.argument_count;
            
            if (source->data.function.argument_count > 0) {
                (*destination)->data.function.arguments = 
                    (ast_node_t**)malloc(source->data.function.argument_count * sizeof(ast_node_t*));
                if (!(*destination)->data.function.arguments) {
                    qfs_free_ast_node(*destination);
                    return QFS_ERROR_MEMORY_ALLOCATION;
                }
                
                for (size_t i = 0; i < source->data.function.argument_count; i++) {
                    qfs_result_t result = qfs_copy_ast_node(source->data.function.arguments[i], 
                                                           &(*destination)->data.function.arguments[i]);
                    if (result != QFS_SUCCESS) {
                        qfs_free_ast_node(*destination);
                        return result;
                    }
                    (*destination)->data.function.arguments[i]->parent = *destination;
                }
            }
            break;
            
        case AST_CONSTANT:
            strcpy((*destination)->data.constant.constant_name, 
                   source->data.constant.constant_name);
            (*destination)->data.constant.constant_value = source->data.constant.constant_value;
            break;
    }
    
    // Copy children
    if (source->child_count > 0) {
        (*destination)->children = (ast_node_t**)malloc(source->child_count * sizeof(ast_node_t*));
        if (!(*destination)->children) {
            qfs_free_ast_node(*destination);
            return QFS_ERROR_MEMORY_ALLOCATION;
        }
        
        (*destination)->child_count = source->child_count;
        for (size_t i = 0; i < source->child_count; i++) {
            qfs_result_t result = qfs_copy_ast_node(source->children[i], &(*destination)->children[i]);
            if (result != QFS_SUCCESS) {
                qfs_free_ast_node(*destination);
                return result;
            }
            (*destination)->children[i]->parent = *destination;
        }
    }
    
    return QFS_SUCCESS;
}

/**
 * Evaluate AST node and return quantum number result
 */
qfs_result_t qfs_evaluate_ast_node(ast_node_t *node, quantum_number_t *result) {
    if (!node || !result) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    return qfs_evaluate_ast_recursive(node, result);
}

/**
 * Validate AST structure for mathematical correctness
 */
qfs_result_t qfs_validate_ast_structure(ast_node_t *root) {
    if (!root) {
        return QFS_ERROR_AST_MALFORMED;
    }
    
    // Check node type validity
    if (root->type < AST_QUANTUM_NUMBER || root->type > AST_CONSTANT) {
        return QFS_ERROR_AST_MALFORMED;
    }
    
    // Validate type-specific structure
    switch (root->type) {
        case AST_OPERATOR:
            if (root->data.operator.operand_count == 0 || !root->data.operator.operands) {
                return QFS_ERROR_AST_MALFORMED;
            }
            
            // Validate all operands
            for (size_t i = 0; i < root->data.operator.operand_count; i++) {
                qfs_result_t result = qfs_validate_ast_structure(root->data.operator.operands[i]);
                if (result != QFS_SUCCESS) {
                    return result;
                }
            }
            break;
            
        case AST_FUNCTION:
            if (root->data.function.argument_count > 0) {
                if (!root->data.function.arguments) {
                    return QFS_ERROR_AST_MALFORMED;
                }
                
                // Validate all arguments
                for (size_t i = 0; i < root->data.function.argument_count; i++) {
                    qfs_result_t result = qfs_validate_ast_structure(root->data.function.arguments[i]);
                    if (result != QFS_SUCCESS) {
                        return result;
                    }
                }
            }
            break;
            
        default:
            // Other node types are always valid if they exist
            break;
    }
    
    // Validate children
    if (root->child_count > 0) {
        if (!root->children) {
            return QFS_ERROR_AST_MALFORMED;
        }
        
        for (size_t i = 0; i < root->child_count; i++) {
            qfs_result_t result = qfs_validate_ast_structure(root->children[i]);
            if (result != QFS_SUCCESS) {
                return result;
            }
        }
    }
    
    return QFS_SUCCESS;
}

/**
 * Optimize AST structure for better performance
 */
qfs_result_t qfs_optimize_ast_structure(ast_node_t **root) {
    if (!root || !*root) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    return qfs_optimize_ast_node(root);
}

// Helper function implementations

/**
 * Count total number of nodes in AST
 */
size_t qfs_count_ast_nodes(ast_node_t *root) {
    if (!root) {
        return 0;
    }
    
    size_t count = 1; // Count this node
    
    // Count operands/arguments
    switch (root->type) {
        case AST_OPERATOR:
            for (size_t i = 0; i < root->data.operator.operand_count; i++) {
                count += qfs_count_ast_nodes(root->data.operator.operands[i]);
            }
            break;
            
        case AST_FUNCTION:
            for (size_t i = 0; i < root->data.function.argument_count; i++) {
                count += qfs_count_ast_nodes(root->data.function.arguments[i]);
            }
            break;
            
        default:
            break;
    }
    
    // Count children
    for (size_t i = 0; i < root->child_count; i++) {
        count += qfs_count_ast_nodes(root->children[i]);
    }
    
    return count;
}

/**
 * Compute maximum depth of AST
 */
uint32_t qfs_compute_ast_depth(ast_node_t *root) {
    if (!root) {
        return 0;
    }
    
    uint32_t max_depth = 0;
    
    // Check operands/arguments depth
    switch (root->type) {
        case AST_OPERATOR:
            for (size_t i = 0; i < root->data.operator.operand_count; i++) {
                uint32_t depth = qfs_compute_ast_depth(root->data.operator.operands[i]);
                if (depth > max_depth) {
                    max_depth = depth;
                }
            }
            break;
            
        case AST_FUNCTION:
            for (size_t i = 0; i < root->data.function.argument_count; i++) {
                uint32_t depth = qfs_compute_ast_depth(root->data.function.arguments[i]);
                if (depth > max_depth) {
                    max_depth = depth;
                }
            }
            break;
            
        default:
            break;
    }
    
    // Check children depth
    for (size_t i = 0; i < root->child_count; i++) {
        uint32_t depth = qfs_compute_ast_depth(root->children[i]);
        if (depth > max_depth) {
            max_depth = depth;
        }
    }
    
    return max_depth + 1;
}

// Internal helper function implementations

static qfs_result_t qfs_serialize_ast_node(ast_node_t *node, void **buffer, size_t *size) {
    if (!node || !buffer || !size) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    // Simple serialization - in real implementation would use more sophisticated format
    *size = sizeof(ast_node_t);
    *buffer = malloc(*size);
    if (!*buffer) {
        return QFS_ERROR_MEMORY_ALLOCATION;
    }
    
    memcpy(*buffer, node, *size);
    return QFS_SUCCESS;
}

static qfs_result_t qfs_deserialize_ast_node(const void *buffer, size_t size, ast_node_t **node) {
    if (!buffer || size == 0 || !node) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    // Simple deserialization - in real implementation would reconstruct full structure
    *node = (ast_node_t*)malloc(sizeof(ast_node_t));
    if (!*node) {
        return QFS_ERROR_MEMORY_ALLOCATION;
    }
    
    memcpy(*node, buffer, sizeof(ast_node_t));
    return QFS_SUCCESS;
}

static qfs_result_t qfs_compute_ast_complexity(ast_node_t *node, complexity_measure_t *complexity) {
    if (!node || !complexity) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    // Initialize complexity
    complexity->computational_depth = 1;
    complexity->symbolic_complexity = 1;
    complexity->memory_requirements = sizeof(ast_node_t);
    complexity->time_complexity_class = 1;
    
    // Compute based on node type and structure
    switch (node->type) {
        case AST_OPERATOR:
            complexity->computational_depth += node->data.operator.operand_count;
            complexity->symbolic_complexity += node->data.operator.operand_count * 2;
            break;
            
        case AST_FUNCTION:
            complexity->computational_depth += node->data.function.argument_count * 2;
            complexity->symbolic_complexity += node->data.function.argument_count * 3;
            break;
            
        default:
            break;
    }
    
    return QFS_SUCCESS;
}

static qfs_result_t qfs_optimize_ast_node(ast_node_t **node) {
    if (!node || !*node) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    // Simple optimization - constant folding for basic operations
    if ((*node)->type == AST_OPERATOR && 
        strcmp((*node)->data.operator.operator_symbol, "+") == 0 &&
        (*node)->data.operator.operand_count == 2) {
        
        ast_node_t *left = (*node)->data.operator.operands[0];
        ast_node_t *right = (*node)->data.operator.operands[1];
        
        if (left->type == AST_QUANTUM_NUMBER && right->type == AST_QUANTUM_NUMBER) {
            // Fold constant addition
            quantum_number_t result;
            quantum_number_init(&result);
            quantum_number_add(&left->data.quantum_value, &right->data.quantum_value, &result);
            
            // Replace operator node with constant result
            ast_node_t *new_node = qfs_create_ast_node(AST_QUANTUM_NUMBER);
            if (new_node) {
                new_node->data.quantum_value = result;
                new_node->parent = (*node)->parent;
                
                qfs_free_ast_node(*node);
                *node = new_node;
            }
        }
    }
    
    return QFS_SUCCESS;
}

static qfs_result_t qfs_evaluate_ast_recursive(ast_node_t *node, quantum_number_t *result) {
    if (!node || !result) {
        return QFS_ERROR_INVALID_PATH;
    }
    
    switch (node->type) {
        case AST_QUANTUM_NUMBER:
            *result = node->data.quantum_value;
            return QFS_SUCCESS;
            
        case AST_CONSTANT:
            *result = node->data.constant.constant_value;
            return QFS_SUCCESS;
            
        case AST_OPERATOR:
            if (strcmp(node->data.operator.operator_symbol, "+") == 0 && 
                node->data.operator.operand_count == 2) {
                
                quantum_number_t left_result, right_result;
                qfs_result_t left_eval = qfs_evaluate_ast_recursive(node->data.operator.operands[0], &left_result);
                qfs_result_t right_eval = qfs_evaluate_ast_recursive(node->data.operator.operands[1], &right_result);
                
                if (left_eval == QFS_SUCCESS && right_eval == QFS_SUCCESS) {
                    quantum_number_add(&left_result, &right_result, result);
                    return QFS_SUCCESS;
                }
            }
            // Add more operators as needed
            break;
            
        default:
            // Variables and functions would require symbol table lookup
            quantum_number_init(result);
            quantum_number_set_from_uint64(result, 0);
            return QFS_SUCCESS;
    }
    
    return QFS_ERROR_SYMBOLIC_ERROR;
}
