/**
 * Quantum Number System - Core Header
 * 
 * This file defines the fundamental Quantum Number data structure and operations
 * for the Quantum Operating System kernel.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTUM_NUMBER_H
#define QUANTUM_NUMBER_H

#include <stdint.h>
#include <stdbool.h>

// Quantum Number Structure (256 bits total)
typedef struct {
    // 12 ordinal components (20 bits each = 240 bits)
    uint32_t ordinals[12];  // Only lower 20 bits used per ordinal
    
    // 12 sign bits (1 bit each = 12 bits)
    uint16_t signs;         // Only lower 12 bits used
    
    // 4-bit checksum
    uint8_t checksum;       // Only lower 4 bits used
} __attribute__((packed)) quantum_number_t;

// Ordinal indices for special mathematical components
#define QN_ORDINAL_REAL      0   // Real component (ordinal 0)
#define QN_ORDINAL_I         8   // Imaginary component (ordinal i)
#define QN_ORDINAL_J         9   // Quaternion j component
#define QN_ORDINAL_K         10  // Quaternion k component

// Maximum value for 20-bit ordinal component
#define QN_ORDINAL_MAX       0xFFFFF

// Quantum Number operation results
typedef enum {
    QN_SUCCESS = 0,
    QN_ERROR_INVALID_ORDINAL,
    QN_ERROR_CHECKSUM_MISMATCH,
    QN_ERROR_DIVISION_BY_ZERO,
    QN_ERROR_OVERFLOW,
    QN_ERROR_INVALID_OPERATION
} qn_result_t;

// Core Quantum Number operations
qn_result_t qn_create(quantum_number_t* qn);
qn_result_t qn_zero(quantum_number_t* qn);
qn_result_t qn_one(quantum_number_t* qn);
qn_result_t qn_copy(quantum_number_t* dest, const quantum_number_t* src);

// Ordinal operations
qn_result_t qn_set_ordinal(quantum_number_t* qn, int ordinal_index, uint32_t value);
uint32_t qn_get_ordinal(const quantum_number_t* qn, int ordinal_index);
qn_result_t qn_set_sign(quantum_number_t* qn, int ordinal_index, bool negative);
bool qn_get_sign(const quantum_number_t* qn, int ordinal_index);

// Basic arithmetic operations
qn_result_t qn_add(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b);
qn_result_t qn_subtract(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b);
qn_result_t qn_multiply(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b);
qn_result_t qn_divide(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b);

// Complex number operations
qn_result_t qn_create_complex(quantum_number_t* qn, uint32_t real, uint32_t imag);
qn_result_t qn_complex_add(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b);
qn_result_t qn_complex_multiply(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b);
qn_result_t qn_complex_conjugate(quantum_number_t* result, const quantum_number_t* qn);

// Utility functions
qn_result_t qn_calculate_checksum(quantum_number_t* qn);
bool qn_verify_checksum(const quantum_number_t* qn);
bool qn_equals(const quantum_number_t* a, const quantum_number_t* b);
bool qn_is_zero(const quantum_number_t* qn);

// String conversion
qn_result_t qn_to_string(const quantum_number_t* qn, char* buffer, size_t buffer_size);
qn_result_t qn_from_string(quantum_number_t* qn, const char* str);

#endif // QUANTUM_NUMBER_H
