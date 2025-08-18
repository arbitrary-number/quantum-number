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
 * @file quantix_twisted_curves.h
 * @brief Twisted Curve Mathematics Implementation for Quantix OS
 * 
 * This module provides comprehensive support for twisted Edwards curves used
 * in Solana (Ed25519) and Monero (Ed25519/Curve25519) cryptocurrencies.
 * Includes native curve operations, point arithmetic, scalar multiplication,
 * and integration with the Quantix filesystem for security research.
 * 
 * Key Features:
 * - Ed25519 twisted Edwards curve implementation
 * - Curve25519 Montgomery curve support
 * - Point addition, doubling, and scalar multiplication
 * - Signature generation and verification (EdDSA)
 * - Key exchange operations (ECDH)
 * - Integration with G-node filesystem
 * - Security research and analysis tools
 * - Performance optimization for 2^256 operations
 * 
 * @author Arbitrary Number Project Team
 * @version 1.0.0
 * @date 2024
 * 
 * @warning This implementation is for educational and research purposes only.
 *          Critical for evaluating cryptocurrency security before institutional adoption.
 */

#ifndef QUANTIX_TWISTED_CURVES_H
#define QUANTIX_TWISTED_CURVES_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "quantum_number.h"
#include "quantix_native_ecc.h"

#ifdef __cplusplus
extern "C" {
#endif

// Twisted curve constants
#define TWISTED_CURVE_FIELD_SIZE 32
#define TWISTED_CURVE_SCALAR_SIZE 32
#define TWISTED_CURVE_POINT_SIZE 32
#define TWISTED_CURVE_SIGNATURE_SIZE 64
#define TWISTED_CURVE_PUBLIC_KEY_SIZE 32
#define TWISTED_CURVE_PRIVATE_KEY_SIZE 32

// Ed25519 specific constants
#define ED25519_BASEPOINT_ORDER_SIZE 32
#define ED25519_COFACTOR 8
#define ED25519_SIGNATURE_SIZE 64

// Curve25519 specific constants
#define CURVE25519_KEY_SIZE 32
#define CURVE25519_SHARED_SECRET_SIZE 32

// Maximum cache sizes for performance
#define TWISTED_CURVE_CACHE_SIZE 8192
#define TWISTED_CURVE_PRECOMP_SIZE 256
#define TWISTED_CURVE_AST_NODES 65536

// Twisted curve types
typedef enum {
    TWISTED_CURVE_ED25519 = 0,
    TWISTED_CURVE_CURVE25519,
    TWISTED_CURVE_ED448,
    TWISTED_CURVE_X448,
    TWISTED_CURVE_CUSTOM,
    TWISTED_CURVE_COUNT
} twisted_curve_type_t;

// Point representation for twisted Edwards curves
typedef struct {
    quantum_number_t x;
    quantum_number_t y;
    quantum_number_t z;
    quantum_number_t t;
    bool is_extended;
    bool is_infinity;
    twisted_curve_type_t curve_type;
} twisted_point_t;

// Montgomery curve point representation
typedef struct {
    quantum_number_t x;
    quantum_number_t z;
    bool is_infinity;
} montgomery_point_t;

// Twisted curve scalar
typedef struct {
    quantum_number_t value;
    bool is_reduced;
    bool is_valid;
    twisted_curve_type_t curve_type;
    uint32_t derivation_path[8];
    uint32_t derivation_depth;
} twisted_scalar_t;

// EdDSA signature
typedef struct {
    quantum_number_t r;
    quantum_number_t s;
    uint8_t signature_bytes[TWISTED_CURVE_SIGNATURE_SIZE];
    bool is_canonical;
    twisted_curve_type_t curve_type;
} twisted_signature_t;

// Curve parameters structure
typedef struct {
    twisted_curve_type_t type;
    quantum_number_t p;     // Field prime
    quantum_number_t d;     // Curve parameter d
    quantum_number_t a;     // Curve parameter a (usually -1)
    quantum_number_t l;     // Order of base point
    quantum_number_t h;     // Cofactor
    twisted_point_t base_point;
    char curve_name[32];
    uint32_t security_level;
} twisted_curve_params_t;

// Precomputed point table for fast scalar multiplication
typedef struct {
    twisted_point_t points[TWISTED_CURVE_PRECOMP_SIZE];
    uint32_t point_count;
    uint32_t window_size;
    bool is_initialized;
} twisted_precomp_table_t;

// AST node for twisted curve operations
typedef struct twisted_ast_node {
    quantum_number_t key;
    twisted_point_t point;
    twisted_scalar_t scalar;
    struct twisted_ast_node* left;
    struct twisted_ast_node* right;
    struct twisted_ast_node* parent;
    uint32_t height;
    uint64_t access_count;
    uint64_t last_access_time;
    bool is_gnode;
    char gnode_path[256];
    twisted_curve_type_t curve_type;
} twisted_ast_node_t;

// G-node structure for twisted curve filesystem integration
typedef struct {
    quantum_number_t g_number;
    twisted_point_t public_key;
    twisted_scalar_t private_key;
    char filesystem_path[512];
    char solana_address[64];
    char monero_address[128];
    uint64_t creation_time;
    uint64_t last_modified;
    uint32_t permissions;
    twisted_curve_type_t curve_type;
    bool is_wallet_derived;
    bool is_stealth_address;
} twisted_gnode_t;

// Twisted curve context
typedef struct {
    twisted_curve_params_t curve_params;
    twisted_precomp_table_t precomp_table;
    
    // AST tree for O(1) lookups
    twisted_ast_node_t* ast_root;
    uint32_t ast_node_count;
    
    // G-node cache
    twisted_gnode_t gnode_cache[TWISTED_CURVE_CACHE_SIZE];
    uint32_t gnode_cache_count;
    
    // Performance statistics
    uint64_t point_operations;
    uint64_t scalar_operations;
    uint64_t signature_operations;
    uint64_t key_exchange_operations;
    uint64_t ast_lookups;
    uint64_t cache_hits;
    uint64_t cache_misses;
    
    // Configuration
    bool enable_precomputation;
    bool enable_constant_time;
    bool enable_side_channel_protection;
    bool enable_gnode_integration;
    uint32_t optimization_level;
    
    // Synchronization
    pthread_mutex_t context_mutex;
    pthread_rwlock_t ast_lock;
    pthread_rwlock_t cache_lock;
} twisted_curve_context_t;

// Solana-specific structures
typedef struct {
    twisted_point_t public_key;
    twisted_scalar_t private_key;
    char base58_address[64];
    uint64_t lamports_balance;
    uint32_t program_accounts;
    bool is_program_derived;
} solana_account_t;

// Monero-specific structures
typedef struct {
    twisted_point_t spend_public_key;
    twisted_point_t view_public_key;
    twisted_scalar_t spend_private_key;
    twisted_scalar_t view_private_key;
    char primary_address[128];
    char integrated_address[128];
    uint32_t subaddress_count;
    bool is_stealth;
} monero_account_t;

// Key exchange result
typedef struct {
    quantum_number_t shared_secret;
    uint8_t shared_bytes[CURVE25519_SHARED_SECRET_SIZE];
    bool is_valid;
    twisted_curve_type_t curve_type;
    uint64_t computation_time_us;
} twisted_key_exchange_t;

// Function declarations

/**
 * @brief Initialize twisted curve context
 * @param ctx Context to initialize
 * @param curve_type Type of twisted curve
 * @return 0 on success, negative on error
 */
int twisted_curve_init(twisted_curve_context_t* ctx, twisted_curve_type_t curve_type);

/**
 * @brief Cleanup twisted curve context
 * @param ctx Context to cleanup
 */
void twisted_curve_cleanup(twisted_curve_context_t* ctx);

/**
 * @brief Create twisted Edwards point
 * @param point Output point
 * @param x X coordinate
 * @param y Y coordinate
 * @param curve_type Curve type
 * @return 0 on success, negative on error
 */
int twisted_point_create(twisted_point_t* point, const quantum_number_t* x, 
                        const quantum_number_t* y, twisted_curve_type_t curve_type);

/**
 * @brief Check if point is on twisted Edwards curve
 * @param ctx Curve context
 * @param point Point to validate
 * @return true if on curve, false otherwise
 */
bool twisted_point_is_valid(const twisted_curve_context_t* ctx, const twisted_point_t* point);

/**
 * @brief Add two twisted Edwards points
 * @param ctx Curve context
 * @param result Output point (P + Q)
 * @param p First point
 * @param q Second point
 * @return 0 on success, negative on error
 */
int twisted_point_add(const twisted_curve_context_t* ctx, twisted_point_t* result,
                     const twisted_point_t* p, const twisted_point_t* q);

/**
 * @brief Double a twisted Edwards point
 * @param ctx Curve context
 * @param result Output point (2P)
 * @param p Point to double
 * @return 0 on success, negative on error
 */
int twisted_point_double(const twisted_curve_context_t* ctx, twisted_point_t* result,
                        const twisted_point_t* p);

/**
 * @brief Scalar multiplication on twisted Edwards curve
 * @param ctx Curve context
 * @param result Output point (k*P)
 * @param scalar Scalar multiplier
 * @param point Base point
 * @return 0 on success, negative on error
 */
int twisted_point_multiply(const twisted_curve_context_t* ctx, twisted_point_t* result,
                          const twisted_scalar_t* scalar, const twisted_point_t* point);

/**
 * @brief Generate Ed25519 key pair
 * @param ctx Curve context
 * @param public_key Output public key
 * @param private_key Output private key
 * @param seed Optional seed (32 bytes)
 * @return 0 on success, negative on error
 */
int twisted_generate_ed25519_keypair(const twisted_curve_context_t* ctx,
                                     twisted_point_t* public_key,
                                     twisted_scalar_t* private_key,
                                     const uint8_t* seed);

/**
 * @brief Generate Curve25519 key pair for ECDH
 * @param ctx Curve context
 * @param public_key Output public key
 * @param private_key Output private key
 * @param seed Optional seed (32 bytes)
 * @return 0 on success, negative on error
 */
int twisted_generate_curve25519_keypair(const twisted_curve_context_t* ctx,
                                        montgomery_point_t* public_key,
                                        twisted_scalar_t* private_key,
                                        const uint8_t* seed);

/**
 * @brief Sign message using EdDSA
 * @param ctx Curve context
 * @param signature Output signature
 * @param message Message to sign
 * @param message_len Length of message
 * @param private_key Private key for signing
 * @param public_key Public key (for verification)
 * @return 0 on success, negative on error
 */
int twisted_eddsa_sign(const twisted_curve_context_t* ctx, twisted_signature_t* signature,
                      const uint8_t* message, size_t message_len,
                      const twisted_scalar_t* private_key,
                      const twisted_point_t* public_key);

/**
 * @brief Verify EdDSA signature
 * @param ctx Curve context
 * @param signature Signature to verify
 * @param message Message that was signed
 * @param message_len Length of message
 * @param public_key Public key for verification
 * @return true if valid, false otherwise
 */
bool twisted_eddsa_verify(const twisted_curve_context_t* ctx,
                         const twisted_signature_t* signature,
                         const uint8_t* message, size_t message_len,
                         const twisted_point_t* public_key);

/**
 * @brief Perform Curve25519 key exchange (ECDH)
 * @param ctx Curve context
 * @param result Output shared secret
 * @param private_key Our private key
 * @param public_key Their public key
 * @return 0 on success, negative on error
 */
int twisted_curve25519_ecdh(const twisted_curve_context_t* ctx,
                           twisted_key_exchange_t* result,
                           const twisted_scalar_t* private_key,
                           const montgomery_point_t* public_key);

/**
 * @brief Convert Ed25519 point to Curve25519 point
 * @param montgomery_point Output Montgomery point
 * @param edwards_point Input Edwards point
 * @return 0 on success, negative on error
 */
int twisted_ed25519_to_curve25519(montgomery_point_t* montgomery_point,
                                  const twisted_point_t* edwards_point);

/**
 * @brief Convert Curve25519 point to Ed25519 point
 * @param edwards_point Output Edwards point
 * @param montgomery_point Input Montgomery point
 * @return 0 on success, negative on error
 */
int twisted_curve25519_to_ed25519(twisted_point_t* edwards_point,
                                  const montgomery_point_t* montgomery_point);

/**
 * @brief Create Solana account from key pair
 * @param account Output Solana account
 * @param public_key Public key
 * @param private_key Private key (optional)
 * @return 0 on success, negative on error
 */
int twisted_create_solana_account(solana_account_t* account,
                                  const twisted_point_t* public_key,
                                  const twisted_scalar_t* private_key);

/**
 * @brief Create Monero account from key pairs
 * @param account Output Monero account
 * @param spend_public Public spend key
 * @param view_public Public view key
 * @param spend_private Private spend key (optional)
 * @param view_private Private view key (optional)
 * @return 0 on success, negative on error
 */
int twisted_create_monero_account(monero_account_t* account,
                                  const twisted_point_t* spend_public,
                                  const twisted_point_t* view_public,
                                  const twisted_scalar_t* spend_private,
                                  const twisted_scalar_t* view_private);

/**
 * @brief Generate Monero stealth address
 * @param stealth_address Output stealth address
 * @param stealth_size Size of address buffer
 * @param account Monero account
 * @param transaction_public_key Transaction public key
 * @param output_index Output index
 * @return 0 on success, negative on error
 */
int twisted_generate_monero_stealth_address(char* stealth_address, size_t stealth_size,
                                            const monero_account_t* account,
                                            const twisted_point_t* transaction_public_key,
                                            uint32_t output_index);

/**
 * @brief Create G-node for twisted curve filesystem integration
 * @param gnode Output G-node
 * @param g_number G-number identifier
 * @param public_key Public key
 * @param curve_type Curve type
 * @param filesystem_path Path in filesystem
 * @return 0 on success, negative on error
 */
int twisted_gnode_create(twisted_gnode_t* gnode, const quantum_number_t* g_number,
                        const twisted_point_t* public_key, twisted_curve_type_t curve_type,
                        const char* filesystem_path);

/**
 * @brief Insert G-node into AST tree
 * @param ctx Curve context
 * @param gnode G-node to insert
 * @return 0 on success, negative on error
 */
int twisted_gnode_insert(twisted_curve_context_t* ctx, const twisted_gnode_t* gnode);

/**
 * @brief Lookup G-node by public key
 * @param ctx Curve context
 * @param public_key Public key to search
 * @return Pointer to G-node if found, NULL otherwise
 */
twisted_gnode_t* twisted_gnode_lookup(const twisted_curve_context_t* ctx,
                                      const twisted_point_t* public_key);

/**
 * @brief Initialize precomputation table for fast scalar multiplication
 * @param ctx Curve context
 * @param base_point Base point for precomputation
 * @param window_size Window size for precomputation
 * @return 0 on success, negative on error
 */
int twisted_precompute_table(twisted_curve_context_t* ctx,
                            const twisted_point_t* base_point,
                            uint32_t window_size);

/**
 * @brief Compress twisted Edwards point
 * @param compressed Output compressed point (32 bytes)
 * @param point Input point
 * @return 0 on success, negative on error
 */
int twisted_point_compress(uint8_t* compressed, const twisted_point_t* point);

/**
 * @brief Decompress twisted Edwards point
 * @param point Output point
 * @param compressed Input compressed point (32 bytes)
 * @param curve_type Curve type
 * @return 0 on success, negative on error
 */
int twisted_point_decompress(twisted_point_t* point, const uint8_t* compressed,
                             twisted_curve_type_t curve_type);

/**
 * @brief Encode scalar to bytes
 * @param bytes Output byte array (32 bytes)
 * @param scalar Input scalar
 * @return 0 on success, negative on error
 */
int twisted_scalar_encode(uint8_t* bytes, const twisted_scalar_t* scalar);

/**
 * @brief Decode scalar from bytes
 * @param scalar Output scalar
 * @param bytes Input byte array (32 bytes)
 * @param curve_type Curve type
 * @return 0 on success, negative on error
 */
int twisted_scalar_decode(twisted_scalar_t* scalar, const uint8_t* bytes,
                         twisted_curve_type_t curve_type);

/**
 * @brief Perform security analysis on twisted curve operations
 * @param ctx Curve context
 * @param analysis_type Type of analysis to perform
 * @param result Output analysis result
 * @param result_size Size of result buffer
 * @return 0 on success, negative on error
 */
int twisted_security_analysis(const twisted_curve_context_t* ctx,
                             const char* analysis_type,
                             char* result, size_t result_size);

/**
 * @brief Export twisted curve data for research
 * @param ctx Curve context
 * @param filename Output filename
 * @param export_type Type of data to export
 * @return 0 on success, negative on error
 */
int twisted_export_research_data(const twisted_curve_context_t* ctx,
                                const char* filename, const char* export_type);

/**
 * @brief Get twisted curve statistics
 * @param ctx Curve context
 * @param stats Output statistics structure
 * @return 0 on success, negative on error
 */
int twisted_get_statistics(const twisted_curve_context_t* ctx,
                          struct twisted_curve_stats* stats);

/**
 * @brief Benchmark twisted curve operations
 * @param ctx Curve context
 * @param operation_type Type of operation to benchmark
 * @param iterations Number of iterations
 * @return Operations per second, negative on error
 */
double twisted_benchmark_operations(const twisted_curve_context_t* ctx,
                                   const char* operation_type, uint32_t iterations);

/**
 * @brief Validate twisted curve implementation against test vectors
 * @param ctx Curve context
 * @param test_vector_file File containing test vectors
 * @return Number of tests passed, negative on error
 */
int twisted_validate_implementation(const twisted_curve_context_t* ctx,
                                   const char* test_vector_file);

// Error codes
#define TWISTED_SUCCESS                    0
#define TWISTED_ERROR_INVALID_PARAM       -1
#define TWISTED_ERROR_INVALID_POINT       -2
#define TWISTED_ERROR_INVALID_SCALAR      -3
#define TWISTED_ERROR_OUT_OF_MEMORY       -4
#define TWISTED_ERROR_NOT_ON_CURVE        -5
#define TWISTED_ERROR_POINT_AT_INFINITY   -6
#define TWISTED_ERROR_SIGNATURE_INVALID   -7
#define TWISTED_ERROR_KEY_EXCHANGE_FAILED -8
#define TWISTED_ERROR_COMPRESSION_FAILED  -9
#define TWISTED_ERROR_DECOMPRESSION_FAILED -10
#define TWISTED_ERROR_AST_FULL            -11
#define TWISTED_ERROR_GNODE_NOT_FOUND     -12
#define TWISTED_ERROR_PRECOMP_FAILED      -13
#define TWISTED_ERROR_CONTEXT_NOT_INIT    -14
#define TWISTED_ERROR_IO                  -15

// Statistics structure
struct twisted_curve_stats {
    uint64_t total_point_operations;
    uint64_t total_scalar_operations;
    uint64_t total_signatures;
    uint64_t total_verifications;
    uint64_t total_key_exchanges;
    uint64_t total_ast_lookups;
    uint64_t cache_hits;
    uint64_t cache_misses;
    double cache_hit_rate;
    double average_operation_time_us;
    uint32_t gnode_count;
    uint32_t precomp_table_size;
    twisted_curve_type_t curve_type;
};

// Curve parameter constants
extern const twisted_curve_params_t TWISTED_CURVE_ED25519_PARAMS;
extern const twisted_curve_params_t TWISTED_CURVE_CURVE25519_PARAMS;
extern const twisted_curve_params_t TWISTED_CURVE_ED448_PARAMS;

#ifdef __cplusplus
}
#endif

#endif // QUANTIX_TWISTED_CURVES_H
