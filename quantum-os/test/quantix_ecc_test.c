/**
 * Quantix ECC Mathematics Integration - Test Suite
 * 
 * This file contains comprehensive tests for the native secp256k1 elliptic
 * curve cryptography implementation in Quantix OS.
 * 
 * Copyright (c) 2025 Arbitrary Number Project Team
 * Licensed under the Apache License, Version 2.0
 */

#include "../kernel/quantix_ecc.h"
#include "../kernel/quantix_bip39.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Test result tracking
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Test assertion macro
#define TEST_ASSERT(condition, message) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf("✓ PASS: %s\n", message); \
    } else { \
        tests_failed++; \
        printf("✗ FAIL: %s\n", message); \
    } \
} while(0)

// Test section header
#define TEST_SECTION(name) printf("\n=== %s ===\n", name)

/**
 * Test secp256k1 parameter initialization
 */
void test_secp256k1_params_init(void) {
    TEST_SECTION("secp256k1 Parameter Initialization");
    
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    TEST_ASSERT(params != NULL, "secp256k1 parameters initialized");
    
    // Test field prime
    TEST_ASSERT(!quantum_number_is_zero(&params->field_prime), "Field prime is non-zero");
    
    // Test curve parameters
    TEST_ASSERT(quantum_number_is_zero(&params->curve_a), "Curve parameter a = 0");
    
    quantum_number_t seven;
    quantum_number_init(&seven);
    quantum_number_set_from_uint64(&seven, 7);
    TEST_ASSERT(quantum_number_equals(&params->curve_b, &seven), "Curve parameter b = 7");
    
    // Test generator point
    TEST_ASSERT(!quantum_number_is_zero(&params->generator.x), "Generator X coordinate is non-zero");
    TEST_ASSERT(!quantum_number_is_zero(&params->generator.y), "Generator Y coordinate is non-zero");
    TEST_ASSERT(params->generator.point_flags & ECC_POINT_VALIDATED, "Generator point is validated");
    
    // Test order
    TEST_ASSERT(!quantum_number_is_zero(&params->order), "Curve order is non-zero");
    
    // Test cofactor
    quantum_number_t one;
    quantum_number_init(&one);
    quantum_number_set_from_uint64(&one, 1);
    TEST_ASSERT(quantum_number_equals(&params->cofactor, &one), "Cofactor = 1");
}

/**
 * Test ECC context management
 */
void test_ecc_context_management(void) {
    TEST_SECTION("ECC Context Management");
    
    quantix_ecc_context_t *ctx = NULL;
    ecc_result_t result = quantix_ecc_context_init(&ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "ECC context initialization");
    TEST_ASSERT(ctx != NULL, "ECC context is not NULL");
    TEST_ASSERT(ctx->curve_params != NULL, "Context has curve parameters");
    
    result = quantix_ecc_context_destroy(ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "ECC context destruction");
}

/**
 * Test point utility functions
 */
void test_point_utilities(void) {
    TEST_SECTION("Point Utility Functions");
    
    quantix_ecc_point_t point;
    quantum_number_init(&point.x);
    quantum_number_init(&point.y);
    quantum_number_init(&point.z);
    quantum_number_init(&point.mathematical_signature);
    
    // Test point at infinity
    ecc_result_t result = quantix_ecc_point_set_infinity(&point);
    TEST_ASSERT(result == ECC_SUCCESS, "Set point to infinity");
    TEST_ASSERT(quantix_ecc_point_is_infinity(&point) == ECC_SUCCESS, "Point is at infinity");
    
    // Test point equality
    quantix_ecc_point_t point2;
    quantum_number_init(&point2.x);
    quantum_number_init(&point2.y);
    quantum_number_init(&point2.z);
    quantum_number_init(&point2.mathematical_signature);
    quantix_ecc_point_set_infinity(&point2);
    
    TEST_ASSERT(quantix_ecc_point_equals(&point, &point2) == ECC_SUCCESS, 
                "Two points at infinity are equal");
    
    // Test point negation
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    quantix_ecc_point_t neg_point;
    quantum_number_init(&neg_point.x);
    quantum_number_init(&neg_point.y);
    quantum_number_init(&neg_point.z);
    quantum_number_init(&neg_point.mathematical_signature);
    
    result = quantix_ecc_point_negate(&params->generator, &neg_point);
    TEST_ASSERT(result == ECC_SUCCESS, "Point negation");
    TEST_ASSERT(quantum_number_equals(&params->generator.x, &neg_point.x), 
                "Negated point has same X coordinate");
}

/**
 * Test field arithmetic operations
 */
void test_field_arithmetic(void) {
    TEST_SECTION("Field Arithmetic Operations");
    
    quantum_number_t a, b, result;
    quantum_number_init(&a);
    quantum_number_init(&b);
    quantum_number_init(&result);
    
    // Test field addition
    quantum_number_set_from_uint64(&a, 123456);
    quantum_number_set_from_uint64(&b, 789012);
    
    ecc_result_t ecc_result = quantix_field_add(&a, &b, &result);
    TEST_ASSERT(ecc_result == ECC_SUCCESS, "Field addition");
    
    // Test field multiplication
    ecc_result = quantix_field_mult(&a, &b, &result, NULL);
    TEST_ASSERT(ecc_result == ECC_SUCCESS, "Field multiplication");
    
    // Test field inversion
    quantum_number_set_from_uint64(&a, 12345);
    ecc_result = quantix_field_inverse(&a, &result, NULL);
    TEST_ASSERT(ecc_result == ECC_SUCCESS, "Field inversion");
}

/**
 * Test point validation
 */
void test_point_validation(void) {
    TEST_SECTION("Point Validation");
    
    quantix_ecc_context_t *ctx;
    quantix_ecc_context_init(&ctx);
    
    // Test generator point validation
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    ecc_result_t result = quantix_ecc_validate_point(&params->generator, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Generator point is valid");
    
    // Test point at infinity validation
    quantix_ecc_point_t infinity_point;
    quantum_number_init(&infinity_point.x);
    quantum_number_init(&infinity_point.y);
    quantum_number_init(&infinity_point.z);
    quantum_number_init(&infinity_point.mathematical_signature);
    quantix_ecc_point_set_infinity(&infinity_point);
    
    result = quantix_ecc_validate_point(&infinity_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Point at infinity is valid");
    
    quantix_ecc_context_destroy(ctx);
}

/**
 * Test point doubling
 */
void test_point_doubling(void) {
    TEST_SECTION("Point Doubling");
    
    quantix_ecc_context_t *ctx;
    quantix_ecc_context_init(&ctx);
    
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    quantix_ecc_point_t doubled_point;
    quantum_number_init(&doubled_point.x);
    quantum_number_init(&doubled_point.y);
    quantum_number_init(&doubled_point.z);
    quantum_number_init(&doubled_point.mathematical_signature);
    
    // Test doubling generator point
    ecc_result_t result = quantix_ecc_point_double(&params->generator, &doubled_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Point doubling");
    TEST_ASSERT(quantix_ecc_point_is_infinity(&doubled_point) != ECC_SUCCESS, 
                "Doubled generator is not at infinity");
    
    // Validate doubled point is on curve
    result = quantix_ecc_validate_point(&doubled_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Doubled point is valid");
    
    quantix_ecc_context_destroy(ctx);
}

/**
 * Test point addition
 */
void test_point_addition(void) {
    TEST_SECTION("Point Addition");
    
    quantix_ecc_context_t *ctx;
    quantix_ecc_context_init(&ctx);
    
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    quantix_ecc_point_t doubled_point, sum_point;
    quantum_number_init(&doubled_point.x);
    quantum_number_init(&doubled_point.y);
    quantum_number_init(&doubled_point.z);
    quantum_number_init(&doubled_point.mathematical_signature);
    quantum_number_init(&sum_point.x);
    quantum_number_init(&sum_point.y);
    quantum_number_init(&sum_point.z);
    quantum_number_init(&sum_point.mathematical_signature);
    
    // First double the generator
    quantix_ecc_point_double(&params->generator, &doubled_point, ctx);
    
    // Test G + G = 2G
    ecc_result_t result = quantix_ecc_point_add(&params->generator, &params->generator, &sum_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Point addition G + G");
    
    // Test G + 2G = 3G
    result = quantix_ecc_point_add(&params->generator, &doubled_point, &sum_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Point addition G + 2G");
    
    // Validate result is on curve
    result = quantix_ecc_validate_point(&sum_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Sum point is valid");
    
    quantix_ecc_context_destroy(ctx);
}

/**
 * Test scalar multiplication
 */
void test_scalar_multiplication(void) {
    TEST_SECTION("Scalar Multiplication");
    
    quantix_ecc_context_t *ctx;
    quantix_ecc_context_init(&ctx);
    
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    quantix_ecc_scalar_t scalar;
    quantix_ecc_point_t result_point;
    quantum_number_init(&scalar.value);
    quantum_number_init(&scalar.modular_inverse);
    quantum_number_init(&result_point.x);
    quantum_number_init(&result_point.y);
    quantum_number_init(&result_point.z);
    quantum_number_init(&result_point.mathematical_signature);
    
    // Test scalar multiplication with small scalar
    quantum_number_set_from_uint64(&scalar.value, 3);
    ecc_result_t result = quantix_ecc_scalar_mult(&scalar, &params->generator, &result_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Scalar multiplication 3*G");
    
    // Validate result is on curve
    result = quantix_ecc_validate_point(&result_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "3*G is valid point");
    
    // Test scalar multiplication with zero
    quantum_number_set_zero(&scalar.value);
    result = quantix_ecc_scalar_mult(&scalar, &params->generator, &result_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Scalar multiplication 0*G");
    TEST_ASSERT(quantix_ecc_point_is_infinity(&result_point) == ECC_SUCCESS, 
                "0*G is point at infinity");
    
    quantix_ecc_context_destroy(ctx);
}

/**
 * Test key generation
 */
void test_key_generation(void) {
    TEST_SECTION("Key Generation");
    
    quantix_ecc_context_t *ctx;
    quantix_ecc_context_init(&ctx);
    
    quantix_ecc_scalar_t private_key;
    quantix_ecc_point_t public_key;
    quantum_number_init(&private_key.value);
    quantum_number_init(&private_key.modular_inverse);
    quantum_number_init(&public_key.x);
    quantum_number_init(&public_key.y);
    quantum_number_init(&public_key.z);
    quantum_number_init(&public_key.mathematical_signature);
    
    // Test keypair generation
    ecc_result_t result = quantix_ecc_generate_keypair(&private_key, &public_key, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Keypair generation");
    TEST_ASSERT(!quantum_number_is_zero(&private_key.value), "Private key is non-zero");
    TEST_ASSERT(quantix_ecc_point_is_infinity(&public_key) != ECC_SUCCESS, 
                "Public key is not at infinity");
    
    // Validate public key is on curve
    result = quantix_ecc_validate_point(&public_key, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Generated public key is valid");
    
    quantix_ecc_context_destroy(ctx);
}

/**
 * Test ECDSA signature operations
 */
void test_ecdsa_operations(void) {
    TEST_SECTION("ECDSA Signature Operations");
    
    quantix_ecc_context_t *ctx;
    quantix_ecc_context_init(&ctx);
    
    // Generate keypair
    quantix_ecc_scalar_t private_key;
    quantix_ecc_point_t public_key;
    quantum_number_init(&private_key.value);
    quantum_number_init(&private_key.modular_inverse);
    quantum_number_init(&public_key.x);
    quantum_number_init(&public_key.y);
    quantum_number_init(&public_key.z);
    quantum_number_init(&public_key.mathematical_signature);
    
    quantix_ecc_generate_keypair(&private_key, &public_key, ctx);
    
    // Create message hash
    quantum_number_t message_hash;
    quantum_number_init(&message_hash);
    quantum_number_set_from_uint64(&message_hash, 0x123456789ABCDEF0ULL);
    
    // Test signature generation
    quantix_ecc_scalar_t r, s;
    quantum_number_init(&r.value);
    quantum_number_init(&r.modular_inverse);
    quantum_number_init(&s.value);
    quantum_number_init(&s.modular_inverse);
    
    ecc_result_t result = quantix_ecdsa_sign(&private_key.value, &message_hash, &r, &s, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "ECDSA signature generation");
    TEST_ASSERT(!quantum_number_is_zero(&r.value), "Signature r component is non-zero");
    TEST_ASSERT(!quantum_number_is_zero(&s.value), "Signature s component is non-zero");
    
    // Test signature verification
    result = quantix_ecdsa_verify(&public_key, &message_hash, &r, &s, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "ECDSA signature verification");
    
    // Test verification with wrong message
    quantum_number_t wrong_hash;
    quantum_number_init(&wrong_hash);
    quantum_number_set_from_uint64(&wrong_hash, 0xFEDCBA9876543210ULL);
    
    result = quantix_ecdsa_verify(&public_key, &wrong_hash, &r, &s, ctx);
    TEST_ASSERT(result == ECC_ERROR_VERIFICATION_FAILED, "ECDSA verification fails with wrong message");
    
    quantix_ecc_context_destroy(ctx);
}

/**
 * Test O(1) lookup table operations
 */
void test_lookup_tables(void) {
    TEST_SECTION("O(1) Lookup Table Operations");
    
    quantix_ecc_point_table_t *table;
    ecc_result_t result = quantix_ecc_point_table_init(&table, 64);
    TEST_ASSERT(result == ECC_SUCCESS, "Point table initialization");
    TEST_ASSERT(table != NULL, "Point table is not NULL");
    
    // Insert generator point
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    result = quantix_ecc_point_table_insert(table, &params->generator);
    TEST_ASSERT(result == ECC_SUCCESS, "Point table insertion");
    
    // Lookup generator point
    quantix_ecc_point_t *found_point;
    result = quantix_ecc_point_table_lookup(table, &params->generator, &found_point);
    TEST_ASSERT(result == ECC_SUCCESS, "Point table lookup");
    TEST_ASSERT(found_point != NULL, "Found point is not NULL");
    
    // Test lookup of non-existent point
    quantix_ecc_point_t fake_point;
    quantum_number_init(&fake_point.x);
    quantum_number_init(&fake_point.y);
    quantum_number_init(&fake_point.z);
    quantum_number_init(&fake_point.mathematical_signature);
    quantum_number_set_from_uint64(&fake_point.x, 999999);
    quantum_number_set_from_uint64(&fake_point.y, 888888);
    
    result = quantix_ecc_point_table_lookup(table, &fake_point, &found_point);
    TEST_ASSERT(result == ECC_ERROR_POINT_NOT_FOUND, "Non-existent point not found");
}

/**
 * Test precomputation table generation
 */
void test_precomputation_tables(void) {
    TEST_SECTION("Precomputation Table Generation");
    
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    quantix_ecc_precomp_table_t *table;
    
    ecc_result_t result = quantix_generate_precomp_table(&params->generator, 4, &table);
    TEST_ASSERT(result == ECC_SUCCESS, "Precomputation table generation");
    TEST_ASSERT(table != NULL, "Precomputation table is not NULL");
    TEST_ASSERT(table->window_size == 4, "Window size is correct");
    TEST_ASSERT(table->table_entries == 8, "Table entries count is correct (2^(4-1))");
    TEST_ASSERT(table->odd_multiples != NULL, "Odd multiples array is allocated");
    
    // Verify first entry is the base point
    TEST_ASSERT(quantix_ecc_point_equals(&table->odd_multiples[0], &params->generator) == ECC_SUCCESS,
                "First precomputed point is the generator");
}

/**
 * Test BIP-39 mnemonic operations
 */
void test_bip39_operations(void) {
    TEST_SECTION("BIP-39 Mnemonic Operations");
    
    const quantix_bip39_wordlist_t *wordlist = quantix_bip39_get_english_wordlist();
    TEST_ASSERT(wordlist != NULL, "English wordlist loaded");
    
    // Test mnemonic generation
    quantix_bip39_mnemonic_t mnemonic;
    bip39_result_t result = quantix_bip39_generate_mnemonic(128, &mnemonic, wordlist);
    TEST_ASSERT(result == BIP39_SUCCESS, "BIP-39 mnemonic generation");
    TEST_ASSERT(mnemonic.word_count == 12, "128-bit entropy generates 12 words");
    TEST_ASSERT(mnemonic.entropy_bits == 128, "Entropy bits correctly set");
    
    // Test mnemonic validation
    result = quantix_bip39_validate_mnemonic(&mnemonic, wordlist);
    TEST_ASSERT(result == BIP39_SUCCESS, "BIP-39 mnemonic validation");
    
    // Test seed generation
    uint8_t seed[64];
    result = quantix_bip39_mnemonic_to_seed(&mnemonic, "test_passphrase", seed);
    TEST_ASSERT(result == BIP39_SUCCESS, "BIP-39 seed generation");
    
    // Test entropy conversion
    uint8_t recovered_entropy[32];
    size_t recovered_bits;
    result = quantix_bip39_mnemonic_to_entropy(&mnemonic, wordlist, recovered_entropy, &recovered_bits);
    TEST_ASSERT(result == BIP39_SUCCESS, "BIP-39 entropy recovery");
    TEST_ASSERT(recovered_bits == 128, "Recovered entropy bits match");
    
    // Test string formatting
    char mnemonic_string[512];
    result = quantix_bip39_format_mnemonic_string(&mnemonic, mnemonic_string, sizeof(mnemonic_string));
    TEST_ASSERT(result == BIP39_SUCCESS, "BIP-39 string formatting");
    TEST_ASSERT(strlen(mnemonic_string) > 0, "Formatted string is not empty");
}

/**
 * Test mathematical signature computation
 */
void test_mathematical_signatures(void) {
    TEST_SECTION("Mathematical Signature Computation");
    
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    quantum_number_t signature;
    quantum_number_init(&signature);
    
    // Test ECC point signature
    ecc_result_t result = quantix_compute_ecc_signature(&params->generator, &signature);
    TEST_ASSERT(result == ECC_SUCCESS, "ECC point signature computation");
    TEST_ASSERT(!quantum_number_is_zero(&signature), "Signature is non-zero");
    
    // Test BIP-39 mnemonic signature
    const quantix_bip39_wordlist_t *wordlist = quantix_bip39_get_english_wordlist();
    quantix_bip39_mnemonic_t mnemonic;
    quantix_bip39_generate_mnemonic(128, &mnemonic, wordlist);
    
    quantum_number_t mnemonic_signature;
    quantum_number_init(&mnemonic_signature);
    bip39_result_t bip_result = quantix_bip39_compute_mnemonic_signature(&mnemonic, &mnemonic_signature);
    TEST_ASSERT(bip_result == BIP39_SUCCESS, "BIP-39 mnemonic signature computation");
    TEST_ASSERT(!quantum_number_is_zero(&mnemonic_signature), "Mnemonic signature is non-zero");
}

/**
 * Test security functions
 */
void test_security_functions(void) {
    TEST_SECTION("Security Functions");
    
    quantix_ecc_context_t *ctx;
    quantix_ecc_context_init(&ctx);
    
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    quantix_ecc_scalar_t scalar;
    quantix_ecc_point_t result_point, blinded_result;
    quantum_number_init(&scalar.value);
    quantum_number_init(&scalar.modular_inverse);
    quantum_number_init(&result_point.x);
    quantum_number_init(&result_point.y);
    quantum_number_init(&result_point.z);
    quantum_number_init(&result_point.mathematical_signature);
    quantum_number_init(&blinded_result.x);
    quantum_number_init(&blinded_result.y);
    quantum_number_init(&blinded_result.z);
    quantum_number_init(&blinded_result.mathematical_signature);
    
    // Test scalar multiplication with blinding
    quantum_number_set_from_uint64(&scalar.value, 12345);
    ecc_result_t result = quantix_scalar_mult_with_blinding(&scalar, &params->generator, &blinded_result);
    TEST_ASSERT(result == ECC_SUCCESS, "Scalar multiplication with blinding");
    
    // Compare with regular scalar multiplication
    result = quantix_ecc_scalar_mult(&scalar, &params->generator, &result_point, ctx);
    TEST_ASSERT(result == ECC_SUCCESS, "Regular scalar multiplication");
    
    // Results should be the same (blinding should not affect result)
    TEST_ASSERT(quantix_ecc_point_equals(&result_point, &blinded_result) == ECC_SUCCESS,
                "Blinded and regular results are equal");
    
    // Test point randomization
    quantix_ecc_point_t randomized_point = params->generator;
    result = quantix_randomize_point(&randomized_point);
    TEST_ASSERT(result == ECC_SUCCESS, "Point randomization");
    
    quantix_ecc_context_destroy(ctx);
}

/**
 * Test constant-time operations
 */
void test_constant_time_operations(void) {
    TEST_SECTION("Constant-Time Operations");
    
    quantix_ecc_scalar_t scalar;
    quantix_ecc_point_t result_point;
    quantum_number_init(&scalar.value);
    quantum_number_init(&scalar.modular_inverse);
    quantum_number_init(&result_point.x);
    quantum_number_init(&result_point.y);
    quantum_number_init(&result_point.z);
    quantum_number_init(&result_point.mathematical_signature);
    
    quantix_secp256k1_params_t *params = quantix_get_secp256k1_params();
    
    // Test Montgomery ladder scalar multiplication
    quantum_number_set_from_uint64(&scalar.value, 54321);
    ecc_result_t result = quantix_montgomery_ladder_scalar_mult(&scalar, &params->generator, &result_point);
    TEST_ASSERT(result == ECC_SUCCESS, "Montgomery ladder scalar multiplication");
    
    // Validate result is on curve
    result = quantix_ecc_validate_point(&result_point, NULL);
    TEST_ASSERT(result == ECC_SUCCESS, "Montgomery ladder result is valid");
    
    // Test windowed NAF scalar multiplication
    quantix_ecc_precomp_table_t *table;
    quantix_generate_precomp_table(&params->generator, 4, &table);
    
    result = quantix_windowed_naf_scalar_mult(&scalar, &params->generator, &result_point, table);
    TEST_ASSERT(result == ECC_SUCCESS, "Windowed NAF scalar multiplication");
}

/**
 * Run all tests
 */
int main(void) {
    printf("Quantix ECC Mathematics Integration Test Suite\n");
    printf("==============================================\n");
    
    // Run all test categories
    test_secp256k1_params_init();
    test_ecc_context_management();
    test_point_utilities();
    test_field_arithmetic();
    test_point_validation();
    test_point_doubling();
    test_point_addition();
    test_scalar_multiplication();
    test_key_generation();
    test_ecdsa_operations();
    test_lookup_tables();
    test_precomputation_tables();
    test_bip39_operations();
    test_mathematical_signatures();
    test_security_functions();
    test_constant_time_operations();
    
    // Print test summary
    printf("\n=== Test Summary ===\n");
    printf("Tests Run:    %d\n", tests_run);
    printf("Tests Passed: %d\n", tests_passed);
    printf("Tests Failed: %d\n", tests_failed);
    
    if (tests_failed == 0) {
        printf("\n🎉 All tests passed! ECC implementation is working correctly.\n");
        return 0;
    } else {
        printf("\n❌ %d test(s) failed. Please review the implementation.\n", tests_failed);
        return 1;
    }
}
