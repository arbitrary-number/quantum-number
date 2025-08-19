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
 * Quantum Number System - Core Implementation
 * 
 * This file implements the fundamental Quantum Number operations
 * for the Quantum Operating System kernel.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include "quantum_number.h"
#include <string.h>
#include <stdio.h>

/**
 * Calculate 4-bit checksum for Quantum Number integrity verification
 */
static uint8_t calculate_checksum_internal(const quantum_number_t* qn) {
    uint32_t sum = 0;
    
    // Sum all ordinal components
    for (int i = 0; i < 12; i++) {
        sum += qn->ordinals[i] & QN_ORDINAL_MAX;
    }
    
    // Add sign bits
    sum += qn->signs & 0xFFF;
    
    // Return 4-bit checksum
    return (uint8_t)((sum ^ (sum >> 4) ^ (sum >> 8) ^ (sum >> 12)) & 0xF);
}

/**
 * Create a new Quantum Number initialized to zero
 */
qn_result_t qn_create(quantum_number_t* qn) {
    if (!qn) return QN_ERROR_INVALID_OPERATION;
    
    memset(qn, 0, sizeof(quantum_number_t));
    return qn_calculate_checksum(qn);
}

/**
 * Initialize Quantum Number to zero
 */
qn_result_t qn_zero(quantum_number_t* qn) {
    if (!qn) return QN_ERROR_INVALID_OPERATION;
    
    memset(qn, 0, sizeof(quantum_number_t));
    return qn_calculate_checksum(qn);
}

/**
 * Initialize Quantum Number to one (real component = 1)
 */
qn_result_t qn_one(quantum_number_t* qn) {
    if (!qn) return QN_ERROR_INVALID_OPERATION;
    
    memset(qn, 0, sizeof(quantum_number_t));
    qn->ordinals[QN_ORDINAL_REAL] = 1;
    return qn_calculate_checksum(qn);
}

/**
 * Copy one Quantum Number to another
 */
qn_result_t qn_copy(quantum_number_t* dest, const quantum_number_t* src) {
    if (!dest || !src) return QN_ERROR_INVALID_OPERATION;
    
    memcpy(dest, src, sizeof(quantum_number_t));
    return QN_SUCCESS;
}

/**
 * Set ordinal component value
 */
qn_result_t qn_set_ordinal(quantum_number_t* qn, int ordinal_index, uint32_t value) {
    if (!qn || ordinal_index < 0 || ordinal_index >= 12) {
        return QN_ERROR_INVALID_ORDINAL;
    }
    
    if (value > QN_ORDINAL_MAX) {
        return QN_ERROR_OVERFLOW;
    }
    
    qn->ordinals[ordinal_index] = value & QN_ORDINAL_MAX;
    return qn_calculate_checksum(qn);
}

/**
 * Get ordinal component value
 */
uint32_t qn_get_ordinal(const quantum_number_t* qn, int ordinal_index) {
    if (!qn || ordinal_index < 0 || ordinal_index >= 12) {
        return 0;
    }
    
    return qn->ordinals[ordinal_index] & QN_ORDINAL_MAX;
}

/**
 * Set sign bit for ordinal component
 */
qn_result_t qn_set_sign(quantum_number_t* qn, int ordinal_index, bool negative) {
    if (!qn || ordinal_index < 0 || ordinal_index >= 12) {
        return QN_ERROR_INVALID_ORDINAL;
    }
    
    if (negative) {
        qn->signs |= (1 << ordinal_index);
    } else {
        qn->signs &= ~(1 << ordinal_index);
    }
    
    return qn_calculate_checksum(qn);
}

/**
 * Get sign bit for ordinal component
 */
bool qn_get_sign(const quantum_number_t* qn, int ordinal_index) {
    if (!qn || ordinal_index < 0 || ordinal_index >= 12) {
        return false;
    }
    
    return (qn->signs & (1 << ordinal_index)) != 0;
}

/**
 * Add two Quantum Numbers
 */
qn_result_t qn_add(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b) {
    if (!result || !a || !b) return QN_ERROR_INVALID_OPERATION;
    
    // Verify checksums
    if (!qn_verify_checksum(a) || !qn_verify_checksum(b)) {
        return QN_ERROR_CHECKSUM_MISMATCH;
    }
    
    // Initialize result
    qn_zero(result);
    
    // Add corresponding ordinal components
    for (int i = 0; i < 12; i++) {
        uint32_t val_a = qn_get_ordinal(a, i);
        uint32_t val_b = qn_get_ordinal(b, i);
        bool sign_a = qn_get_sign(a, i);
        bool sign_b = qn_get_sign(b, i);
        
        // Simple addition logic (can be enhanced for full symbolic arithmetic)
        uint64_t sum;
        bool result_sign = false;
        
        if (sign_a == sign_b) {
            // Same signs: add magnitudes, keep sign
            sum = (uint64_t)val_a + val_b;
            result_sign = sign_a;
        } else {
            // Different signs: subtract magnitudes, sign of larger
            if (val_a >= val_b) {
                sum = val_a - val_b;
                result_sign = sign_a;
            } else {
                sum = val_b - val_a;
                result_sign = sign_b;
            }
        }
        
        // Check for overflow
        if (sum > QN_ORDINAL_MAX) {
            return QN_ERROR_OVERFLOW;
        }
        
        qn_set_ordinal(result, i, (uint32_t)sum);
        qn_set_sign(result, i, result_sign);
    }
    
    return qn_calculate_checksum(result);
}

/**
 * Subtract two Quantum Numbers (a - b)
 */
qn_result_t qn_subtract(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b) {
    if (!result || !a || !b) return QN_ERROR_INVALID_OPERATION;
    
    // Create negated version of b
    quantum_number_t neg_b;
    qn_copy(&neg_b, b);
    
    // Flip all sign bits
    neg_b.signs ^= 0xFFF;
    qn_calculate_checksum(&neg_b);
    
    // Add a + (-b)
    return qn_add(result, a, &neg_b);
}

/**
 * Multiply two Quantum Numbers (simplified implementation)
 */
qn_result_t qn_multiply(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b) {
    if (!result || !a || !b) return QN_ERROR_INVALID_OPERATION;
    
    // Verify checksums
    if (!qn_verify_checksum(a) || !qn_verify_checksum(b)) {
        return QN_ERROR_CHECKSUM_MISMATCH;
    }
    
    // Simplified multiplication: multiply real components only
    // Full implementation would handle all ordinal cross-products
    uint32_t real_a = qn_get_ordinal(a, QN_ORDINAL_REAL);
    uint32_t real_b = qn_get_ordinal(b, QN_ORDINAL_REAL);
    bool sign_a = qn_get_sign(a, QN_ORDINAL_REAL);
    bool sign_b = qn_get_sign(b, QN_ORDINAL_REAL);
    
    uint64_t product = (uint64_t)real_a * real_b;
    if (product > QN_ORDINAL_MAX) {
        return QN_ERROR_OVERFLOW;
    }
    
    qn_zero(result);
    qn_set_ordinal(result, QN_ORDINAL_REAL, (uint32_t)product);
    qn_set_sign(result, QN_ORDINAL_REAL, sign_a ^ sign_b);
    
    return qn_calculate_checksum(result);
}

/**
 * Divide two Quantum Numbers (simplified implementation)
 */
qn_result_t qn_divide(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b) {
    if (!result || !a || !b) return QN_ERROR_INVALID_OPERATION;
    
    // Check for division by zero
    if (qn_is_zero(b)) {
        // In Quantum Numbers, division by zero creates a symbolic representation
        // For now, return error (full implementation would create symbolic div/0)
        return QN_ERROR_DIVISION_BY_ZERO;
    }
    
    // Simplified division: divide real components only
    uint32_t real_a = qn_get_ordinal(a, QN_ORDINAL_REAL);
    uint32_t real_b = qn_get_ordinal(b, QN_ORDINAL_REAL);
    bool sign_a = qn_get_sign(a, QN_ORDINAL_REAL);
    bool sign_b = qn_get_sign(b, QN_ORDINAL_REAL);
    
    if (real_b == 0) {
        return QN_ERROR_DIVISION_BY_ZERO;
    }
    
    uint32_t quotient = real_a / real_b;
    
    qn_zero(result);
    qn_set_ordinal(result, QN_ORDINAL_REAL, quotient);
    qn_set_sign(result, QN_ORDINAL_REAL, sign_a ^ sign_b);
    
    return qn_calculate_checksum(result);
}

/**
 * Create complex number (real + imag*i)
 */
qn_result_t qn_create_complex(quantum_number_t* qn, uint32_t real, uint32_t imag) {
    if (!qn) return QN_ERROR_INVALID_OPERATION;
    
    qn_zero(qn);
    qn_set_ordinal(qn, QN_ORDINAL_REAL, real);
    qn_set_ordinal(qn, QN_ORDINAL_I, imag);
    
    return qn_calculate_checksum(qn);
}

/**
 * Add two complex Quantum Numbers
 */
qn_result_t qn_complex_add(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b) {
    // Complex addition is the same as regular Quantum Number addition
    return qn_add(result, a, b);
}

/**
 * Multiply two complex Quantum Numbers: (a+bi)(c+di) = (ac-bd) + (ad+bc)i
 */
qn_result_t qn_complex_multiply(quantum_number_t* result, const quantum_number_t* a, const quantum_number_t* b) {
    if (!result || !a || !b) return QN_ERROR_INVALID_OPERATION;
    
    uint32_t a_real = qn_get_ordinal(a, QN_ORDINAL_REAL);
    uint32_t a_imag = qn_get_ordinal(a, QN_ORDINAL_I);
    uint32_t b_real = qn_get_ordinal(b, QN_ORDINAL_REAL);
    uint32_t b_imag = qn_get_ordinal(b, QN_ORDINAL_I);
    
    // Calculate: ac - bd
    uint64_t ac = (uint64_t)a_real * b_real;
    uint64_t bd = (uint64_t)a_imag * b_imag;
    uint64_t real_part = (ac > bd) ? (ac - bd) : 0; // Simplified
    
    // Calculate: ad + bc
    uint64_t ad = (uint64_t)a_real * b_imag;
    uint64_t bc = (uint64_t)a_imag * b_real;
    uint64_t imag_part = ad + bc;
    
    // Check for overflow
    if (real_part > QN_ORDINAL_MAX || imag_part > QN_ORDINAL_MAX) {
        return QN_ERROR_OVERFLOW;
    }
    
    qn_zero(result);
    qn_set_ordinal(result, QN_ORDINAL_REAL, (uint32_t)real_part);
    qn_set_ordinal(result, QN_ORDINAL_I, (uint32_t)imag_part);
    
    return qn_calculate_checksum(result);
}

/**
 * Complex conjugate: a + bi -> a - bi
 */
qn_result_t qn_complex_conjugate(quantum_number_t* result, const quantum_number_t* qn) {
    if (!result || !qn) return QN_ERROR_INVALID_OPERATION;
    
    qn_copy(result, qn);
    
    // Flip sign of imaginary component
    bool imag_sign = qn_get_sign(result, QN_ORDINAL_I);
    qn_set_sign(result, QN_ORDINAL_I, !imag_sign);
    
    return qn_calculate_checksum(result);
}

/**
 * Calculate and update checksum
 */
qn_result_t qn_calculate_checksum(quantum_number_t* qn) {
    if (!qn) return QN_ERROR_INVALID_OPERATION;
    
    qn->checksum = calculate_checksum_internal(qn) & 0xF;
    return QN_SUCCESS;
}

/**
 * Verify checksum integrity
 */
bool qn_verify_checksum(const quantum_number_t* qn) {
    if (!qn) return false;
    
    uint8_t calculated = calculate_checksum_internal(qn) & 0xF;
    return calculated == (qn->checksum & 0xF);
}

/**
 * Check if two Quantum Numbers are equal
 */
bool qn_equals(const quantum_number_t* a, const quantum_number_t* b) {
    if (!a || !b) return false;
    
    return memcmp(a, b, sizeof(quantum_number_t)) == 0;
}

/**
 * Check if Quantum Number is zero
 */
bool qn_is_zero(const quantum_number_t* qn) {
    if (!qn) return false;
    
    for (int i = 0; i < 12; i++) {
        if (qn->ordinals[i] != 0) return false;
    }
    return true;
}

/**
 * Convert Quantum Number to string representation
 */
qn_result_t qn_to_string(const quantum_number_t* qn, char* buffer, size_t buffer_size) {
    if (!qn || !buffer || buffer_size < 64) return QN_ERROR_INVALID_OPERATION;
    
    uint32_t real = qn_get_ordinal(qn, QN_ORDINAL_REAL);
    uint32_t imag = qn_get_ordinal(qn, QN_ORDINAL_I);
    bool real_sign = qn_get_sign(qn, QN_ORDINAL_REAL);
    bool imag_sign = qn_get_sign(qn, QN_ORDINAL_I);
    
    if (imag == 0) {
        // Real number only
        snprintf(buffer, buffer_size, "%s%u", real_sign ? "-" : "", real);
    } else if (real == 0) {
        // Imaginary number only
        snprintf(buffer, buffer_size, "%s%ui", imag_sign ? "-" : "", imag);
    } else {
        // Complex number
        snprintf(buffer, buffer_size, "%s%u%s%ui", 
                real_sign ? "-" : "", real,
                imag_sign ? "-" : "+", imag);
    }
    
    return QN_SUCCESS;
}

/**
 * Parse Quantum Number from string (simplified implementation)
 */
qn_result_t qn_from_string(quantum_number_t* qn, const char* str) {
    if (!qn || !str) return QN_ERROR_INVALID_OPERATION;
    
    // Simplified parser - full implementation would handle complex expressions
    qn_zero(qn);
    
    int value = 0;
    if (sscanf(str, "%d", &value) == 1) {
        if (value < 0) {
            qn_set_ordinal(qn, QN_ORDINAL_REAL, (uint32_t)(-value));
            qn_set_sign(qn, QN_ORDINAL_REAL, true);
        } else {
            qn_set_ordinal(qn, QN_ORDINAL_REAL, (uint32_t)value);
        }
        return qn_calculate_checksum(qn);
    }
    
    return QN_ERROR_INVALID_OPERATION;
}
