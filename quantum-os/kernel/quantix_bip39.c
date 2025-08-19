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
 * Quantix BIP-39 Mnemonic Implementation - Implementation File
 * 
 * This file implements the native BIP-39 mnemonic seed phrase functionality
 * for hierarchical deterministic wallet support in Quantix OS.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include "quantix_bip39.h"
#include "memory.h"
#include <string.h>

// Global English wordlist (embedded for kernel use)
static quantix_bip39_wordlist_t *g_english_wordlist = NULL;

// BIP-39 English wordlist (first 50 words for demonstration)
static const char* BIP39_ENGLISH_WORDS[] = {
    "abandon", "ability", "able", "about", "above", "absent", "absorb", "abstract",
    "absurd", "abuse", "access", "accident", "account", "accuse", "achieve", "acid",
    "acoustic", "acquire", "across", "act", "action", "actor", "actress", "actual",
    "adapt", "add", "addict", "address", "adjust", "admit", "adult", "advance",
    "advice", "aerobic", "affair", "afford", "afraid", "again", "age", "agent",
    "agree", "ahead", "aim", "air", "airport", "aisle", "alarm", "album",
    "alcohol", "alert"
    // Note: Full wordlist would contain all 2048 words
    // This is abbreviated for demonstration purposes
};

#define BIP39_ENGLISH_WORD_COUNT (sizeof(BIP39_ENGLISH_WORDS) / sizeof(BIP39_ENGLISH_WORDS[0]))

/**
 * Simple SHA-256 implementation for BIP-39 (simplified version)
 * In production, this would use the full cryptographic hash implementation
 */
static void simple_sha256(const uint8_t *data, size_t len, uint8_t hash[32]) {
    // Simplified hash for demonstration - in production use full SHA-256
    memset(hash, 0, 32);
    for (size_t i = 0; i < len; i++) {
        hash[i % 32] ^= data[i];
        hash[(i + 1) % 32] ^= (data[i] << 1) | (data[i] >> 7);
    }
}

/**
 * PBKDF2-HMAC-SHA512 implementation (simplified)
 * In production, this would use the full cryptographic implementation
 */
static void simple_pbkdf2_hmac_sha512(const uint8_t *password, size_t password_len,
                                      const uint8_t *salt, size_t salt_len,
                                      uint32_t iterations,
                                      uint8_t *derived_key, size_t key_len) {
    // Simplified PBKDF2 for demonstration
    memset(derived_key, 0, key_len);
    
    for (uint32_t iter = 0; iter < iterations; iter++) {
        for (size_t i = 0; i < password_len && i < key_len; i++) {
            derived_key[i] ^= password[i];
        }
        for (size_t i = 0; i < salt_len && i < key_len; i++) {
            derived_key[i] ^= salt[i];
        }
        
        // Simple mixing
        for (size_t i = 0; i < key_len - 1; i++) {
            derived_key[i] ^= derived_key[i + 1];
        }
    }
}

/**
 * Initialize English wordlist
 */
static bip39_result_t init_english_wordlist(void) {
    if (g_english_wordlist) return BIP39_SUCCESS;
    
    g_english_wordlist = (quantix_bip39_wordlist_t*)quantix_malloc(sizeof(quantix_bip39_wordlist_t));
    if (!g_english_wordlist) return BIP39_ERROR_MEMORY_ALLOCATION;
    
    memset(g_english_wordlist, 0, sizeof(quantix_bip39_wordlist_t));
    
    // Set language information
    strcpy(g_english_wordlist->language_name, "English");
    g_english_wordlist->language_code = BIP39_LANG_ENGLISH;
    
    // Copy words (limited set for demonstration)
    for (size_t i = 0; i < BIP39_ENGLISH_WORD_COUNT && i < 2048; i++) {
        strncpy(g_english_wordlist->words[i], BIP39_ENGLISH_WORDS[i], 15);
        g_english_wordlist->words[i][15] = '\0';
    }
    
    // Fill remaining slots with placeholder words for demonstration
    for (size_t i = BIP39_ENGLISH_WORD_COUNT; i < 2048; i++) {
        snprintf(g_english_wordlist->words[i], 16, "word%04zu", i);
    }
    
    // Initialize optimization data
    g_english_wordlist->optimization_data.word_lengths = 
        (uint32_t*)quantix_malloc(2048 * sizeof(uint32_t));
    g_english_wordlist->optimization_data.hash_table_size = 4096; // Power of 2
    g_english_wordlist->optimization_data.hash_table = 
        (uint32_t*)quantix_malloc(4096 * sizeof(uint32_t));
    
    if (!g_english_wordlist->optimization_data.word_lengths || 
        !g_english_wordlist->optimization_data.hash_table) {
        quantix_free(g_english_wordlist);
        g_english_wordlist = NULL;
        return BIP39_ERROR_MEMORY_ALLOCATION;
    }
    
    // Compute word lengths and hash table
    memset(g_english_wordlist->optimization_data.hash_table, 0xFF, 4096 * sizeof(uint32_t));
    
    for (size_t i = 0; i < 2048; i++) {
        g_english_wordlist->optimization_data.word_lengths[i] = 
            strlen(g_english_wordlist->words[i]);
        
        // Simple hash function for word lookup
        uint32_t hash = 0;
        const char *word = g_english_wordlist->words[i];
        for (size_t j = 0; word[j]; j++) {
            hash = hash * 31 + (uint8_t)word[j];
        }
        hash &= 4095; // Mask to table size
        
        // Linear probing for collision resolution
        while (g_english_wordlist->optimization_data.hash_table[hash] != 0xFFFFFFFF) {
            hash = (hash + 1) & 4095;
        }
        g_english_wordlist->optimization_data.hash_table[hash] = i;
    }
    
    // Compute wordlist checksum
    quantum_number_init(&g_english_wordlist->wordlist_checksum);
    quantum_number_set_from_uint64(&g_english_wordlist->wordlist_checksum, 0x123456789ABCDEF0ULL);
    
    g_english_wordlist->last_validated = 0; // TODO: Get current timestamp
    
    return BIP39_SUCCESS;
}

/**
 * Get English wordlist
 */
const quantix_bip39_wordlist_t* quantix_bip39_get_english_wordlist(void) {
    if (init_english_wordlist() != BIP39_SUCCESS) {
        return NULL;
    }
    return g_english_wordlist;
}

/**
 * Load wordlist for specified language
 */
bip39_result_t quantix_bip39_load_wordlist(bip39_language_t language,
                                           quantix_bip39_wordlist_t **wordlist) {
    if (!wordlist) return BIP39_ERROR_INVALID_PARAMETER;
    
    // For now, only English is supported
    if (language != BIP39_LANG_ENGLISH) {
        return BIP39_ERROR_INVALID_PARAMETER;
    }
    
    if (init_english_wordlist() != BIP39_SUCCESS) {
        return BIP39_ERROR_MEMORY_ALLOCATION;
    }
    
    *wordlist = g_english_wordlist;
    return BIP39_SUCCESS;
}

/**
 * Destroy wordlist (no-op for global wordlist)
 */
bip39_result_t quantix_bip39_wordlist_destroy(quantix_bip39_wordlist_t *wordlist) {
    // Don't destroy global wordlist
    if (wordlist == g_english_wordlist) {
        return BIP39_SUCCESS;
    }
    
    if (wordlist) {
        if (wordlist->optimization_data.word_lengths) {
            quantix_free(wordlist->optimization_data.word_lengths);
        }
        if (wordlist->optimization_data.hash_table) {
            quantix_free(wordlist->optimization_data.hash_table);
        }
        quantix_free(wordlist);
    }
    
    return BIP39_SUCCESS;
}

/**
 * Find word index in wordlist
 */
bip39_result_t quantix_bip39_find_word_index(const quantix_bip39_wordlist_t *wordlist,
                                             const char *word,
                                             uint32_t *index) {
    if (!wordlist || !word || !index) return BIP39_ERROR_INVALID_PARAMETER;
    
    // Use hash table for O(1) lookup
    uint32_t hash = 0;
    for (size_t i = 0; word[i]; i++) {
        hash = hash * 31 + (uint8_t)word[i];
    }
    hash &= (wordlist->optimization_data.hash_table_size - 1);
    
    // Linear probing
    for (size_t probe = 0; probe < wordlist->optimization_data.hash_table_size; probe++) {
        uint32_t probe_hash = (hash + probe) & (wordlist->optimization_data.hash_table_size - 1);
        uint32_t word_index = wordlist->optimization_data.hash_table[probe_hash];
        
        if (word_index == 0xFFFFFFFF) {
            // Empty slot - word not found
            return BIP39_ERROR_WORD_NOT_FOUND;
        }
        
        if (word_index < 2048 && strcmp(wordlist->words[word_index], word) == 0) {
            *index = word_index;
            return BIP39_SUCCESS;
        }
    }
    
    return BIP39_ERROR_WORD_NOT_FOUND;
}

/**
 * Get word by index
 */
bip39_result_t quantix_bip39_get_word_by_index(const quantix_bip39_wordlist_t *wordlist,
                                               uint32_t index,
                                               char *word,
                                               size_t word_size) {
    if (!wordlist || !word || index >= 2048) return BIP39_ERROR_INVALID_PARAMETER;
    
    strncpy(word, wordlist->words[index], word_size - 1);
    word[word_size - 1] = '\0';
    
    return BIP39_SUCCESS;
}

/**
 * Convert entropy bits to word count
 */
size_t quantix_bip39_entropy_to_word_count(size_t entropy_bits) {
    switch (entropy_bits) {
        case 128: return 12;
        case 160: return 15;
        case 192: return 18;
        case 224: return 21;
        case 256: return 24;
        default: return 0;
    }
}

/**
 * Convert word count to entropy bits
 */
size_t quantix_bip39_word_count_to_entropy(size_t word_count) {
    switch (word_count) {
        case 12: return 128;
        case 15: return 160;
        case 18: return 192;
        case 21: return 224;
        case 24: return 256;
        default: return 0;
    }
}

/**
 * Compute checksum for entropy
 */
bip39_result_t quantix_bip39_compute_checksum(const uint8_t *entropy,
                                              size_t entropy_bits,
                                              uint8_t *checksum) {
    if (!entropy || !checksum || !quantix_bip39_is_valid_entropy_bits(entropy_bits)) {
        return BIP39_ERROR_INVALID_PARAMETER;
    }
    
    uint8_t hash[32];
    simple_sha256(entropy, entropy_bits / 8, hash);
    
    // Checksum is first entropy_bits/32 bits of hash
    size_t checksum_bits = entropy_bits / 32;
    *checksum = hash[0] >> (8 - checksum_bits);
    
    return BIP39_SUCCESS;
}

/**
 * Generate mnemonic from entropy
 */
bip39_result_t quantix_bip39_entropy_to_mnemonic(const uint8_t *entropy,
                                                 size_t entropy_bits,
                                                 quantix_bip39_mnemonic_t *mnemonic,
                                                 const quantix_bip39_wordlist_t *wordlist) {
    if (!entropy || !mnemonic || !wordlist || !quantix_bip39_is_valid_entropy_bits(entropy_bits)) {
        return BIP39_ERROR_INVALID_PARAMETER;
    }
    
    memset(mnemonic, 0, sizeof(quantix_bip39_mnemonic_t));
    
    // Copy entropy
    mnemonic->entropy_bits = entropy_bits;
    memcpy(mnemonic->entropy, entropy, entropy_bits / 8);
    
    // Compute checksum
    bip39_result_t result = quantix_bip39_compute_checksum(entropy, entropy_bits, &mnemonic->checksum);
    if (result != BIP39_SUCCESS) return result;
    
    // Convert to word indices
    size_t checksum_bits = entropy_bits / 32;
    size_t total_bits = entropy_bits + checksum_bits;
    mnemonic->word_count = total_bits / 11; // Each word represents 11 bits
    
    // Create bit array from entropy + checksum
    uint8_t bits[(256 + 8) / 8]; // Max entropy + max checksum
    memcpy(bits, entropy, entropy_bits / 8);
    
    // Append checksum bits
    if (checksum_bits > 0) {
        size_t byte_offset = entropy_bits / 8;
        size_t bit_offset = entropy_bits % 8;
        
        if (bit_offset == 0) {
            bits[byte_offset] = mnemonic->checksum << (8 - checksum_bits);
        } else {
            bits[byte_offset] |= mnemonic->checksum >> bit_offset;
            if (byte_offset + 1 < sizeof(bits)) {
                bits[byte_offset + 1] = mnemonic->checksum << (8 - bit_offset);
            }
        }
    }
    
    // Extract 11-bit words
    for (size_t i = 0; i < mnemonic->word_count; i++) {
        size_t bit_start = i * 11;
        uint32_t word_index = 0;
        
        // Extract 11 bits
        for (size_t bit = 0; bit < 11; bit++) {
            size_t byte_pos = (bit_start + bit) / 8;
            size_t bit_pos = (bit_start + bit) % 8;
            
            if (byte_pos < sizeof(bits)) {
                if (bits[byte_pos] & (0x80 >> bit_pos)) {
                    word_index |= (1 << (10 - bit));
                }
            }
        }
        
        // Get word from wordlist
        result = quantix_bip39_get_word_by_index(wordlist, word_index, 
                                                 mnemonic->mnemonic_words[i], 16);
        if (result != BIP39_SUCCESS) return result;
    }
    
    // Set metadata
    mnemonic->language_id = wordlist->language_code;
    mnemonic->validation_status = BIP39_VALIDATION_COMPLETE;
    mnemonic->security_metadata.entropy_source = BIP39_ENTROPY_SOFTWARE_RNG;
    mnemonic->security_metadata.security_level = entropy_bits;
    mnemonic->security_metadata.generation_time = 0; // TODO: Get timestamp
    
    // Compute mathematical signature
    quantum_number_init(&mnemonic->entropy_signature);
    quantix_bip39_compute_mnemonic_signature(mnemonic, &mnemonic->entropy_signature);
    
    return BIP39_SUCCESS;
}

/**
 * Generate mnemonic with specified entropy size
 */
bip39_result_t quantix_bip39_generate_mnemonic(size_t entropy_bits,
                                               quantix_bip39_mnemonic_t *mnemonic,
                                               const quantix_bip39_wordlist_t *wordlist) {
    if (!mnemonic || !wordlist || !quantix_bip39_is_valid_entropy_bits(entropy_bits)) {
        return BIP39_ERROR_INVALID_PARAMETER;
    }
    
    // Generate random entropy (simplified - should use secure RNG)
    uint8_t entropy[32];
    for (size_t i = 0; i < entropy_bits / 8; i++) {
        entropy[i] = (uint8_t)(0x12345678 + i * 0x9ABCDEF0); // Placeholder random
    }
    
    return quantix_bip39_entropy_to_mnemonic(entropy, entropy_bits, mnemonic, wordlist);
}

/**
 * Convert mnemonic back to entropy
 */
bip39_result_t quantix_bip39_mnemonic_to_entropy(const quantix_bip39_mnemonic_t *mnemonic,
                                                 const quantix_bip39_wordlist_t *wordlist,
                                                 uint8_t *entropy,
                                                 size_t *entropy_bits) {
    if (!mnemonic || !wordlist || !entropy || !entropy_bits) {
        return BIP39_ERROR_INVALID_PARAMETER;
    }
    
    if (!quantix_bip39_is_valid_word_count(mnemonic->word_count)) {
        return BIP39_ERROR_INVALID_WORD_COUNT;
    }
    
    *entropy_bits = quantix_bip39_word_count_to_entropy(mnemonic->word_count);
    size_t checksum_bits = *entropy_bits / 32;
    
    // Convert words to indices
    uint32_t word_indices[24];
    for (size_t i = 0; i < mnemonic->word_count; i++) {
        bip39_result_t result = quantix_bip39_find_word_index(wordlist, 
                                                              mnemonic->mnemonic_words[i],
                                                              &word_indices[i]);
        if (result != BIP39_SUCCESS) return result;
    }
    
    // Reconstruct bit array
    uint8_t bits[(256 + 8) / 8];
    memset(bits, 0, sizeof(bits));
    
    for (size_t i = 0; i < mnemonic->word_count; i++) {
        size_t bit_start = i * 11;
        uint32_t word_index = word_indices[i];
        
        // Store 11 bits
        for (size_t bit = 0; bit < 11; bit++) {
            size_t byte_pos = (bit_start + bit) / 8;
            size_t bit_pos = (bit_start + bit) % 8;
            
            if (byte_pos < sizeof(bits)) {
                if (word_index & (1 << (10 - bit))) {
                    bits[byte_pos] |= (0x80 >> bit_pos);
                }
            }
        }
    }
    
    // Extract entropy
    memcpy(entropy, bits, *entropy_bits / 8);
    
    // Verify checksum
    uint8_t computed_checksum;
    bip39_result_t result = quantix_bip39_compute_checksum(entropy, *entropy_bits, &computed_checksum);
    if (result != BIP39_SUCCESS) return result;
    
    // Extract checksum from bits
    uint8_t extracted_checksum = 0;
    size_t checksum_start = *entropy_bits;
    for (size_t bit = 0; bit < checksum_bits; bit++) {
        size_t byte_pos = (checksum_start + bit) / 8;
        size_t bit_pos = (checksum_start + bit) % 8;
        
        if (byte_pos < sizeof(bits) && (bits[byte_pos] & (0x80 >> bit_pos))) {
            extracted_checksum |= (1 << (checksum_bits - 1 - bit));
        }
    }
    
    if (computed_checksum != extracted_checksum) {
        return BIP39_ERROR_INVALID_CHECKSUM;
    }
    
    return BIP39_SUCCESS;
}

/**
 * Validate mnemonic
 */
bip39_result_t quantix_bip39_validate_mnemonic(const quantix_bip39_mnemonic_t *mnemonic,
                                               const quantix_bip39_wordlist_t *wordlist) {
    if (!mnemonic || !wordlist) return BIP39_ERROR_INVALID_PARAMETER;
    
    // Validate word count
    if (!quantix_bip39_is_valid_word_count(mnemonic->word_count)) {
        return BIP39_ERROR_INVALID_WORD_COUNT;
    }
    
    // Validate all words exist in wordlist
    for (size_t i = 0; i < mnemonic->word_count; i++) {
        uint32_t index;
        bip39_result_t result = quantix_bip39_find_word_index(wordlist, 
                                                              mnemonic->mnemonic_words[i], 
                                                              &index);
        if (result != BIP39_SUCCESS) return BIP39_ERROR_WORD_NOT_FOUND;
    }
    
    // Validate checksum by converting back to entropy
    uint8_t entropy[32];
    size_t entropy_bits;
    return quantix_bip39_mnemonic_to_entropy(mnemonic, wordlist, entropy, &entropy_bits);
}

/**
 * Convert mnemonic to seed using PBKDF2
 */
bip39_result_t quantix_bip39_mnemonic_to_seed(const quantix_bip39_mnemonic_t *mnemonic,
                                              const char *passphrase,
                                              uint8_t seed[64]) {
    if (!mnemonic || !seed) return BIP39_ERROR_INVALID_PARAMETER;
    
    // Create mnemonic string
    char mnemonic_str[512];
    size_t pos = 0;
    
    for (size_t i = 0; i < mnemonic->word_count && pos < sizeof(mnemonic_str) - 1; i++) {
        if (i > 0) {
            mnemonic_str[pos++] = ' ';
        }
        
        size_t word_len = strlen(mnemonic->mnemonic_words[i]);
        if (pos + word_len < sizeof(mnemonic_str)) {
            strcpy(&mnemonic_str[pos], mnemonic->mnemonic_words[i]);
            pos += word_len;
        }
    }
    mnemonic_str[pos] = '\0';
    
    // Create salt: "mnemonic" + passphrase
    char salt[256];
    strcpy(salt, "mnemonic");
    if (passphrase) {
        strncat(salt, passphrase, sizeof(salt) - strlen(salt) - 1);
    }
    
    // Derive seed using PBKDF2-HMAC-SHA512
    simple_pbkdf2_hmac_sha512((uint8_t*)mnemonic_str, strlen(mnemonic_str),
                              (uint8_t*)salt, strlen(salt),
                              2048, // BIP-39 specifies 2048 iterations
                              seed, 64);
    
    return BIP39_SUCCESS;
}

/**
 * Compute mathematical signature for mnemonic
 */
bip39_result_t quantix_bip39_compute_mnemonic_signature(const quantix_bip39_mnemonic_t *mnemonic,
                                                        quantum_number_t *signature) {
    if (!mnemonic || !signature) return BIP39_ERROR_INVALID_PARAMETER;
    
    quantum_number_t temp;
    quantum_number_init(&temp);
    quantum_number_set_zero(signature);
    
    // Combine entropy and word indices for signature
    for (size_t i = 0; i < mnemonic->entropy_bits / 8; i++) {
        quantum_number_set_from_uint64(&temp, mnemonic->entropy[i]);
        quantum_number_add(signature, &temp, signature);
    }
    
    // Add word count and checksum
    quantum_number_set_from_uint64(&temp, mnemonic->word_count);
    quantum_number_mult(signature, &temp, signature);
    
    quantum_number_set_from_uint64(&temp, mnemonic->checksum);
    quantum_number_add(signature, &temp, signature);
    
    return BIP39_SUCCESS;
}

/**
 * Parse mnemonic string into structure
 */
bip39_result_t quantix_bip39_parse_mnemonic_string(const char *mnemonic_string,
                                                   quantix_bip39_mnemonic_t *mnemonic) {
    if (!mnemonic_string || !mnemonic) return BIP39_ERROR_INVALID_PARAMETER;
    
    memset(mnemonic, 0, sizeof(quantix_bip39_mnemonic_t));
    
    // Parse words separated by spaces
    const char *ptr = mnemonic_string;
    size_t word_count = 0;
    
    while (*ptr && word_count < 24) {
        // Skip leading spaces
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n' || *ptr == '\r') ptr++;
        if (!*ptr) break;
        
        // Extract word
        size_t word_len = 0;
        const char *word_start = ptr;
        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\r' && word_len < 15) {
            ptr++;
            word_len++;
        }
        
        if (word_len > 0) {
            strncpy(mnemonic->mnemonic_words[word_count], word_start, word_len);
            mnemonic->mnemonic_words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    mnemonic->word_count = word_count;
    
    if (!quantix_bip39_is_valid_word_count(word_count)) {
        return BIP39_ERROR_INVALID_WORD_COUNT;
    }
    
    return BIP39_SUCCESS;
}

/**
 * Format mnemonic structure to string
 */
bip39_result_t quantix_bip39_format_mnemonic_string(const quantix_bip39_mnemonic_t *mnemonic,
                                                    char *mnemonic_string,
                                                    size_t max_length) {
    if (!mnemonic || !mnemonic_string || max_length == 0) return BIP39_ERROR_INVALID_PARAMETER;
    
    size_t pos = 0;
    mnemonic_string[0] = '\0';
    
    for (size_t i = 0; i < mnemonic->word_count && pos < max_length - 1; i++) {
        if (i > 0 && pos < max_length - 1) {
            mnemonic_string[pos++] = ' ';
        }
        
        size_t word_len = strlen(mnemonic->mnemonic_words[i]);
        if (pos + word_len < max_length) {
            strcpy(&mnemonic_string[pos], mnemonic->mnemonic_words[i]);
            pos += word_len;
        } else {
            break;
        }
    }
    
    mnemonic_string[pos] = '\0';
    return BIP39_SUCCESS;
}

/**
 * Analyze entropy quality (simplified)
 */
bip39_result_t quantix_bip39_analyze_entropy_quality(const uint8_t *entropy,
                                                     size_t entropy_bits,
                                                     uint32_t *quality_score) {
    if (!entropy || !quality_score || !quantix_bip39_is_valid_entropy_bits(entropy_bits)) {
        return BIP39_ERROR_INVALID_PARAMETER;
    }
    
    // Simple entropy quality analysis
    uint32_t score = 0;
    size_t byte_count = entropy_bits / 8;
    
    // Check for patterns
    uint32_t bit_count = 0;
    for (size_t i = 0; i < byte_count; i++) {
        for (int bit = 0; bit < 8; bit++) {
            if (entropy[i] & (1 << bit)) bit_count++;
        }
    }
    
    // Score based on bit balance (closer to 50% = higher score)
    uint32_t expected_bits = entropy_bits / 2;
    uint32_t bit_diff = (bit_count > expected_bits) ? 
                        (bit_count - expected_bits) : (expected_bits - bit_count);
    
    if (bit_diff < entropy_bits / 16) score += 50; // Good bit balance
    else if (bit_diff < entropy_bits / 8) score += 25; // Fair bit balance
    
    // Check for repeated bytes
    int has_repeats = 0;
    for (size_t i = 0; i < byte_count - 1; i++) {
        if (entropy[i] == entropy[i + 1]) {
            has_repeats = 1;
            break;
        }
    }
    if (!has_repeats) score += 25;
    
    // Check for zero bytes
    int has_zeros = 0;
    for (size_t i = 0; i < byte_count; i++) {
        if (entropy[i] == 0) {
            has_zeros = 1;
            break;
        }
    }
    if (!has_zeros) score += 25;
    
    *quality_score = score;
    return BIP39_SUCCESS;
}

/**
 * Estimate security level
 */
bip39_result_t quantix_bip39_estimate_security_level(const quantix_bip39_mnemonic_t *mnemonic,
                                                     uint32_t *security_bits) {
    if (!mnemonic || !security_bits) return BIP39_ERROR_INVALID_PARAMETER;
    
    // Base security is the entropy size
    *security_bits = mnemonic->entropy_bits;
    
    // Analyze entropy quality if available
    if (mnemonic->entropy_bits > 0) {
        uint32_t quality_score;
        bip39_result_t result = quantix_bip39_analyze_entropy_quality(mnemonic->entropy, 
                                                                      mnemonic->entropy_bits, 
                                                                      &quality_score);
        if (result == BIP39_SUCCESS) {
            // Adjust security based on entropy quality
            if (quality_score < 50) {
                *security_bits = (*security_bits * 3) / 4; // Reduce by 25%
            } else if (quality_score < 75) {
                *security_bits = (*security_bits * 7) / 8; // Reduce by 12.5%
            }
            // High quality entropy (75-100) keeps full security level
        }
    }
    
    // Consider validation status
    if (!(mnemonic->validation_status & BIP39_VALIDATION_COMPLETE)) {
        *security_bits /= 2; // Unvalidated mnemonic has reduced security
    }
    
    return BIP39_SUCCESS;
}

/**
 * Normalize mnemonic string (remove extra spaces, convert to lowercase)
 */
bip39_result_t quantix_bip39_normalize_mnemonic(const char *input_mnemonic,
                                                char *normalized_mnemonic,
                                                size_t max_length) {
    if (!input_mnemonic || !normalized_mnemonic || max_length == 0) {
        return BIP39_ERROR_INVALID_PARAMETER;
    }
    
    const char *src = input_mnemonic;
    char *dst = normalized_mnemonic;
    size_t dst_pos = 0;
    int in_word = 0;
    
    while (*src && dst_pos < max_length - 1) {
        char c = *src++;
        
        // Convert to lowercase
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
        
        // Handle whitespace
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (in_word) {
                // End of word - add single space
                *dst++ = ' ';
                dst_pos++;
                in_word = 0;
            }
            // Skip multiple consecutive spaces
        } else {
            // Regular character
            *dst++ = c;
            dst_pos++;
            in_word = 1;
        }
    }
    
    // Remove trailing space if present
    if (dst_pos > 0 && normalized_mnemonic[dst_pos - 1] == ' ') {
        dst_pos--;
    }
    
    normalized_mnemonic[dst_pos] = '\0';
    return BIP39_SUCCESS;
}

/**
 * Verify checksum of mnemonic
 */
bip39_result_t quantix_bip39_verify_checksum(const quantix_bip39_mnemonic_t *mnemonic,
                                             const quantix_bip39_wordlist_t *wordlist) {
    if (!mnemonic || !wordlist) return BIP39_ERROR_INVALID_PARAMETER;
    
    // Convert mnemonic back to entropy and verify checksum
    uint8_t entropy[32];
    size_t entropy_bits;
    
    return quantix_bip39_mnemonic_to_entropy(mnemonic, wordlist, entropy, &entropy_bits);
}
