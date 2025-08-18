/**
 * Quantix ECC Mathematics Integration - Header File
 * 
 * This file defines the core data structures and function prototypes for
 * native secp256k1 elliptic curve cryptography integration in Quantix OS.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTIX_ECC_H
#define QUANTIX_ECC_H

#include <stdint.h>
#include <stddef.h>
#include "quantum_number.h"

// Forward declarations
typedef struct quantix_ecc_context quantix_ecc_context_t;
typedef struct quantix_ecc_point quantix_ecc_point_t;
typedef struct quantix_ecc_scalar quantix_ecc_scalar_t;

// ECC result codes
typedef enum {
    ECC_SUCCESS = 0,
    ECC_ERROR_INVALID_PARAMETER = 1,
    ECC_ERROR_POINT_NOT_ON_CURVE = 2,
    ECC_ERROR_POINT_AT_INFINITY = 3,
    ECC_ERROR_INVALID_SCALAR = 4,
    ECC_ERROR_MEMORY_ALLOCATION = 5,
    ECC_ERROR_COMPUTATION_FAILED = 6,
    ECC_ERROR_POINT_NOT_FOUND = 7,
    ECC_ERROR_TABLE_FULL = 8,
    ECC_ERROR_INVALID_SIGNATURE = 9,
    ECC_ERROR_VERIFICATION_FAILED = 10
} ecc_result_t;

// Coordinate system types
typedef enum {
    COORDINATE_AFFINE = 0,
    COORDINATE_JACOBIAN = 1,
    COORDINATE_MONTGOMERY = 2,
    COORDINATE_EXTENDED = 3
} coordinate_system_t;

// Point flags
#define ECC_POINT_INFINITY     0x01
#define ECC_POINT_COMPRESSED   0x02
#define ECC_POINT_VALIDATED    0x04
#define ECC_POINT_CACHED       0x08
#define ECC_POINT_PRECOMPUTED  0x10

// Elliptic curve point representation using Quantum Numbers
typedef struct quantix_ecc_point {
    quantum_number_t x;                     // X coordinate (256-bit)
    quantum_number_t y;                     // Y coordinate (256-bit)
    quantum_number_t z;                     // Z coordinate for Jacobian (256-bit)
    coordinate_system_t coordinate_system;  // Coordinate system type
    uint32_t point_flags;                   // Point flags
    quantum_number_t mathematical_signature; // Mathematical fingerprint
} quantix_ecc_point_t;

// Scalar representation for private keys and multipliers
typedef struct quantix_ecc_scalar {
    quantum_number_t value;                 // Scalar value (256-bit)
    uint32_t scalar_properties;             // Prime, generator order, etc.
    quantum_number_t modular_inverse;       // Cached inverse if computed
    uint64_t last_used;                     // LRU cache management
} quantix_ecc_scalar_t;

// secp256k1 curve parameters structure
typedef struct quantix_secp256k1_params {
    // Field prime p
    quantum_number_t field_prime;
    
    // Curve parameters (a=0, b=7 for secp256k1)
    quantum_number_t curve_a;
    quantum_number_t curve_b;
    
    // Generator point G
    quantix_ecc_point_t generator;
    
    // Order of the generator point
    quantum_number_t order;
    
    // Cofactor (1 for secp256k1)
    quantum_number_t cofactor;
    
    // Precomputed optimization tables
    quantix_ecc_point_t *precomputed_multiples;  // G, 2G, 3G, ..., 255G
    quantix_ecc_point_t *window_table;           // Windowed NAF table
    size_t precomputed_count;
    
    // Mathematical properties
    uint32_t curve_properties;
    uint64_t operation_complexity;
} quantix_secp256k1_params_t;

// Hash table for O(1) point lookups
typedef struct quantix_ecc_point_table {
    struct {
        quantum_number_t key_hash;           // Hash of point coordinates
        quantix_ecc_point_t point;           // Cached point
        uint64_t access_count;               // Access frequency
        uint64_t last_access;                // LRU timestamp
        uint32_t validation_status;          // Point validation cache
    } *entries;
    
    size_t table_size;                       // Hash table size (power of 2)
    size_t entry_count;                      // Current number of entries
    uint64_t total_lookups;                  // Performance statistics
    uint64_t successful_hits;                // Cache hit statistics
    
    // Hash function parameters
    quantum_number_t hash_seed;              // Cryptographic hash seed
    uint32_t hash_algorithm;                 // Hash algorithm selection
} quantix_ecc_point_table_t;

// Precomputed multiplication table for scalar multiplication
typedef struct quantix_ecc_precomp_table {
    // Windowed NAF precomputation: odd multiples 1G, 3G, 5G, ..., (2^w-1)G
    quantix_ecc_point_t *odd_multiples;     // Precomputed odd multiples
    size_t window_size;                      // Window size (typically 4-8)
    size_t table_entries;                    // Number of precomputed points
    
    // Montgomery ladder precomputation
    quantix_ecc_point_t *montgomery_table;   // Montgomery ladder points
    size_t montgomery_entries;               // Montgomery table size
    
    // Mathematical properties
    quantum_number_t table_signature;       // Mathematical signature
    uint64_t computation_time;               // Time to compute table
    uint32_t optimization_flags;             // Applied optimizations
} quantix_ecc_precomp_table_t;

// Field arithmetic lookup tables for modular operations
typedef struct quantix_field_lookup_table {
    // Modular inverse lookup for small values
    quantum_number_t *inverse_table;        // Precomputed inverses
    size_t inverse_table_size;               // Size of inverse table
    
    // Square root lookup table (Tonelli-Shanks optimization)
    quantum_number_t *sqrt_table;           // Precomputed square roots
    size_t sqrt_table_size;                  // Size of sqrt table
    
    // Montgomery reduction constants
    quantum_number_t montgomery_r;           // Montgomery R = 2^256 mod p
    quantum_number_t montgomery_r_squared;   // R² mod p
    quantum_number_t montgomery_n_prime;    // -p^(-1) mod 2^64
    
    // Barrett reduction constants
    quantum_number_t barrett_mu;             // μ = ⌊2^512 / p⌋
    uint32_t barrett_k;                      // Reduction parameter
} quantix_field_lookup_table_t;

// ECC operation context for mathematical computations
typedef struct quantix_ecc_context {
    quantix_secp256k1_params_t *curve_params;
    
    // Operation caches for O(1) lookups
    struct {
        quantix_ecc_point_t *point_cache;    // Recently computed points
        quantix_ecc_scalar_t *scalar_cache;  // Recently used scalars
        size_t cache_size;
        uint64_t cache_hits;
        uint64_t cache_misses;
    } performance_cache;
    
    // Mathematical optimization state
    struct {
        uint32_t optimization_level;         // 0-10 optimization level
        uint32_t precomputation_flags;       // What's precomputed
        uint32_t algorithm_selection;        // Montgomery ladder, NAF, etc.
    } optimization;
    
    // Security and validation
    struct {
        uint32_t validation_level;           // Point validation strictness
        uint32_t side_channel_protection;    // Constant-time guarantees
        uint32_t randomization_flags;        // Blinding, randomization
    } security;
} quantix_ecc_context_t;

// Core elliptic curve operations
ecc_result_t quantix_ecc_point_add(const quantix_ecc_point_t *p1,
                                   const quantix_ecc_point_t *p2,
                                   quantix_ecc_point_t *result,
                                   quantix_ecc_context_t *ctx);

ecc_result_t quantix_ecc_point_double(const quantix_ecc_point_t *point,
                                      quantix_ecc_point_t *result,
                                      quantix_ecc_context_t *ctx);

ecc_result_t quantix_ecc_scalar_mult(const quantix_ecc_scalar_t *scalar,
                                     const quantix_ecc_point_t *point,
                                     quantix_ecc_point_t *result,
                                     quantix_ecc_context_t *ctx);

// Field arithmetic operations
ecc_result_t quantix_field_add(const quantum_number_t *a,
                               const quantum_number_t *b,
                               quantum_number_t *result);

ecc_result_t quantix_field_mult(const quantum_number_t *a,
                                const quantum_number_t *b,
                                quantum_number_t *result,
                                quantix_field_lookup_table_t *table);

ecc_result_t quantix_field_inverse(const quantum_number_t *a,
                                   quantum_number_t *result,
                                   quantix_field_lookup_table_t *table);

// High-level cryptographic operations
ecc_result_t quantix_ecdsa_sign(const quantum_number_t *private_key,
                                const quantum_number_t *message_hash,
                                quantix_ecc_scalar_t *r,
                                quantix_ecc_scalar_t *s,
                                quantix_ecc_context_t *ctx);

ecc_result_t quantix_ecdsa_verify(const quantix_ecc_point_t *public_key,
                                  const quantum_number_t *message_hash,
                                  const quantix_ecc_scalar_t *r,
                                  const quantix_ecc_scalar_t *s,
                                  quantix_ecc_context_t *ctx);

// Key generation and management
ecc_result_t quantix_ecc_generate_keypair(quantix_ecc_scalar_t *private_key,
                                          quantix_ecc_point_t *public_key,
                                          quantix_ecc_context_t *ctx);

ecc_result_t quantix_ecc_derive_public_key(const quantix_ecc_scalar_t *private_key,
                                           quantix_ecc_point_t *public_key,
                                           quantix_ecc_context_t *ctx);

// Context management
ecc_result_t quantix_ecc_context_init(quantix_ecc_context_t **ctx);
ecc_result_t quantix_ecc_context_destroy(quantix_ecc_context_t *ctx);

// secp256k1 parameter initialization
ecc_result_t quantix_secp256k1_params_init(quantix_secp256k1_params_t **params);
quantix_secp256k1_params_t* quantix_get_secp256k1_params(void);

// Point utility functions
ecc_result_t quantix_ecc_point_is_infinity(const quantix_ecc_point_t *point);
ecc_result_t quantix_ecc_point_set_infinity(quantix_ecc_point_t *point);
ecc_result_t quantix_ecc_point_negate(const quantix_ecc_point_t *point,
                                      quantix_ecc_point_t *result);
ecc_result_t quantix_ecc_point_equals(const quantix_ecc_point_t *p1,
                                      const quantix_ecc_point_t *p2);

// Point validation
ecc_result_t quantix_ecc_validate_point(const quantix_ecc_point_t *point,
                                        quantix_ecc_context_t *ctx);

// O(1) lookup operations
ecc_result_t quantix_ecc_point_table_init(quantix_ecc_point_table_t **table,
                                          size_t initial_size);
ecc_result_t quantix_ecc_point_table_insert(quantix_ecc_point_table_t *table,
                                            const quantix_ecc_point_t *point);
ecc_result_t quantix_ecc_point_table_lookup(const quantix_ecc_point_table_t *table,
                                            const quantix_ecc_point_t *point,
                                            quantix_ecc_point_t **result);

// Precomputation table operations
ecc_result_t quantix_generate_precomp_table(const quantix_ecc_point_t *base_point,
                                            size_t window_size,
                                            quantix_ecc_precomp_table_t **table);

// Mathematical signature computation
ecc_result_t quantix_compute_ecc_signature(const quantix_ecc_point_t *point,
                                           quantum_number_t *signature);

// Constant-time operations for security
ecc_result_t quantix_montgomery_ladder_scalar_mult(const quantix_ecc_scalar_t *scalar,
                                                   const quantix_ecc_point_t *point,
                                                   quantix_ecc_point_t *result);

ecc_result_t quantix_windowed_naf_scalar_mult(const quantix_ecc_scalar_t *scalar,
                                              const quantix_ecc_point_t *point,
                                              quantix_ecc_point_t *result,
                                              const quantix_ecc_precomp_table_t *table);

// Field arithmetic optimization
ecc_result_t quantix_field_mult_optimized(const quantum_number_t *a,
                                          const quantum_number_t *b,
                                          quantum_number_t *result);

ecc_result_t quantix_secp256k1_reduce(const quantum_number_t *input,
                                      quantum_number_t *result);

// Security functions
ecc_result_t quantix_scalar_mult_with_blinding(const quantix_ecc_scalar_t *scalar,
                                               const quantix_ecc_point_t *point,
                                               quantix_ecc_point_t *result);

ecc_result_t quantix_randomize_point(quantix_ecc_point_t *point);

// Hash function for point lookup
uint64_t quantix_ecc_point_hash(const quantix_ecc_point_t *point);

#endif // QUANTIX_ECC_H
