/**
 * Quantix BIP-39 Mnemonic Implementation - Header File
 * 
 * This file defines the native BIP-39 mnemonic seed phrase implementation
 * for hierarchical deterministic wallet support in Quantix OS.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#ifndef QUANTIX_BIP39_H
#define QUANTIX_BIP39_H

#include <stdint.h>
#include <stddef.h>
#include "quantum_number.h"

// BIP-39 result codes
typedef enum {
    BIP39_SUCCESS = 0,
    BIP39_ERROR_INVALID_PARAMETER = 1,
    BIP39_ERROR_INVALID_ENTROPY_SIZE = 2,
    BIP39_ERROR_INVALID_WORD_COUNT = 3,
    BIP39_ERROR_INVALID_CHECKSUM = 4,
    BIP39_ERROR_WORD_NOT_FOUND = 5,
    BIP39_ERROR_MEMORY_ALLOCATION = 6,
    BIP39_ERROR_INVALID_MNEMONIC = 7,
    BIP39_ERROR_WORDLIST_NOT_LOADED = 8
} bip39_result_t;

// Supported entropy sizes (bits)
#define BIP39_ENTROPY_128_BITS  128  // 12 words
#define BIP39_ENTROPY_160_BITS  160  // 15 words
#define BIP39_ENTROPY_192_BITS  192  // 18 words
#define BIP39_ENTROPY_224_BITS  224  // 21 words
#define BIP39_ENTROPY_256_BITS  256  // 24 words

// Language identifiers
typedef enum {
    BIP39_LANG_ENGLISH = 0,
    BIP39_LANG_JAPANESE = 1,
    BIP39_LANG_KOREAN = 2,
    BIP39_LANG_SPANISH = 3,
    BIP39_LANG_CHINESE_SIMPLIFIED = 4,
    BIP39_LANG_CHINESE_TRADITIONAL = 5,
    BIP39_LANG_FRENCH = 6,
    BIP39_LANG_ITALIAN = 7,
    BIP39_LANG_CZECH = 8
} bip39_language_t;

// BIP-39 mnemonic structure
typedef struct quantix_bip39_mnemonic {
    // Entropy and mnemonic data
    uint8_t entropy[32];                    // 256-bit entropy (max size)
    size_t entropy_bits;                    // Actual entropy size in bits
    char mnemonic_words[24][16];            // Up to 24 words, 15 chars each + null
    size_t word_count;                      // 12, 15, 18, 21, or 24 words
    
    // Language and validation
    bip39_language_t language_id;           // Language identifier
    uint8_t checksum;                       // BIP-39 checksum
    uint32_t validation_status;             // Validation flags
    
    // Mathematical properties
    quantum_number_t entropy_signature;     // Mathematical signature of entropy
    uint64_t generation_complexity;         // Complexity measure
    
    // Security metadata
    struct {
        uint64_t generation_time;           // When mnemonic was generated
        uint32_t entropy_source;            // Source of entropy
        uint32_t security_level;            // Estimated security level
        uint32_t validation_flags;          // Validation performed
    } security_metadata;
} quantix_bip39_mnemonic_t;

// BIP-39 wordlist structure for multiple languages
typedef struct quantix_bip39_wordlist {
    char language_name[32];                 // Language name (English, Japanese, etc.)
    bip39_language_t language_code;         // Language code
    char words[2048][16];                   // 2048 words, max 15 chars each + null
    
    // Mathematical properties for optimization
    struct {
        uint32_t *word_lengths;             // Length of each word
        uint32_t *hash_table;               // Hash table for O(1) lookup
        uint32_t hash_table_size;           // Size of hash table
    } optimization_data;
    
    // Validation data
    quantum_number_t wordlist_checksum;     // Checksum of entire wordlist
    uint64_t last_validated;               // Last validation timestamp
} quantix_bip39_wordlist_t;

// Validation flags
#define BIP39_VALIDATION_ENTROPY_CHECKED    0x01
#define BIP39_VALIDATION_CHECKSUM_VERIFIED  0x02
#define BIP39_VALIDATION_WORDS_VERIFIED     0x04
#define BIP39_VALIDATION_LENGTH_VERIFIED    0x08
#define BIP39_VALIDATION_COMPLETE           0x0F

// Entropy source flags
#define BIP39_ENTROPY_HARDWARE_RNG          0x01
#define BIP39_ENTROPY_SOFTWARE_RNG          0x02
#define BIP39_ENTROPY_USER_PROVIDED         0x04
#define BIP39_ENTROPY_MIXED_SOURCES         0x08

// Core BIP-39 operations
bip39_result_t quantix_bip39_generate_mnemonic(size_t entropy_bits,
                                               quantix_bip39_mnemonic_t *mnemonic,
                                               const quantix_bip39_wordlist_t *wordlist);

bip39_result_t quantix_bip39_validate_mnemonic(const quantix_bip39_mnemonic_t *mnemonic,
                                               const quantix_bip39_wordlist_t *wordlist);

bip39_result_t quantix_bip39_mnemonic_to_seed(const quantix_bip39_mnemonic_t *mnemonic,
                                              const char *passphrase,
                                              uint8_t seed[64]);

bip39_result_t quantix_bip39_entropy_to_mnemonic(const uint8_t *entropy,
                                                 size_t entropy_bits,
                                                 quantix_bip39_mnemonic_t *mnemonic,
                                                 const quantix_bip39_wordlist_t *wordlist);

bip39_result_t quantix_bip39_mnemonic_to_entropy(const quantix_bip39_mnemonic_t *mnemonic,
                                                 const quantix_bip39_wordlist_t *wordlist,
                                                 uint8_t *entropy,
                                                 size_t *entropy_bits);

// Wordlist management
bip39_result_t quantix_bip39_load_wordlist(bip39_language_t language,
                                           quantix_bip39_wordlist_t **wordlist);

bip39_result_t quantix_bip39_wordlist_destroy(quantix_bip39_wordlist_t *wordlist);

bip39_result_t quantix_bip39_find_word_index(const quantix_bip39_wordlist_t *wordlist,
                                             const char *word,
                                             uint32_t *index);

bip39_result_t quantix_bip39_get_word_by_index(const quantix_bip39_wordlist_t *wordlist,
                                               uint32_t index,
                                               char *word,
                                               size_t word_size);

// Validation and verification
bip39_result_t quantix_bip39_verify_checksum(const quantix_bip39_mnemonic_t *mnemonic,
                                             const quantix_bip39_wordlist_t *wordlist);

bip39_result_t quantix_bip39_compute_checksum(const uint8_t *entropy,
                                              size_t entropy_bits,
                                              uint8_t *checksum);

// Utility functions
size_t quantix_bip39_entropy_to_word_count(size_t entropy_bits);
size_t quantix_bip39_word_count_to_entropy(size_t word_count);

bip39_result_t quantix_bip39_normalize_mnemonic(const char *input_mnemonic,
                                                char *normalized_mnemonic,
                                                size_t max_length);

// String parsing and formatting
bip39_result_t quantix_bip39_parse_mnemonic_string(const char *mnemonic_string,
                                                   quantix_bip39_mnemonic_t *mnemonic);

bip39_result_t quantix_bip39_format_mnemonic_string(const quantix_bip39_mnemonic_t *mnemonic,
                                                    char *mnemonic_string,
                                                    size_t max_length);

// Security and entropy analysis
bip39_result_t quantix_bip39_analyze_entropy_quality(const uint8_t *entropy,
                                                     size_t entropy_bits,
                                                     uint32_t *quality_score);

bip39_result_t quantix_bip39_estimate_security_level(const quantix_bip39_mnemonic_t *mnemonic,
                                                     uint32_t *security_bits);

// Mathematical signature computation
bip39_result_t quantix_bip39_compute_mnemonic_signature(const quantix_bip39_mnemonic_t *mnemonic,
                                                        quantum_number_t *signature);

// Default wordlist access
const quantix_bip39_wordlist_t* quantix_bip39_get_english_wordlist(void);

// Constants for word count validation
static inline int quantix_bip39_is_valid_word_count(size_t word_count) {
    return (word_count == 12 || word_count == 15 || word_count == 18 || 
            word_count == 21 || word_count == 24);
}

static inline int quantix_bip39_is_valid_entropy_bits(size_t entropy_bits) {
    return (entropy_bits == 128 || entropy_bits == 160 || entropy_bits == 192 || 
            entropy_bits == 224 || entropy_bits == 256);
}

#endif // QUANTIX_BIP39_H
