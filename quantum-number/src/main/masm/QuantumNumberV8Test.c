#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#include "QuantumNumberV8.h"
#include <assert.h>

// Helper to print a 256-bit group (a1..a4) as hex
void print256bit(const char* label, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4) {
    // Print most significant first
    printf("%s: 0x%016llx%016llx%016llx%016llx\n",
        label, a1, a2, a3, a4);
}

// Recursive print for overflow nodes linked via leftPtr
void printQuantumNumber(const QuantumNumberV8* q, int depth) {
    if (!q) return;

    for (int i = 0; i < depth; i++) printf("  ");

    print256bit("a", q->a1, q->a2, q->a3, q->a4);
    for (int i = 0; i < depth; i++) printf("  ");
    print256bit("b", q->b1, q->b2, q->b3, q->b4);
    for (int i = 0; i < depth; i++) printf("  ");
    print256bit("c", q->c1, q->c2, q->c3, q->c4);
    for (int i = 0; i < depth; i++) printf("  ");
    print256bit("d", q->d1, q->d2, q->d3, q->d4);
    for (int i = 0; i < depth; i++) printf("  ");
    print256bit("e", q->e1, q->e2, q->e3, q->e4);
    for (int i = 0; i < depth; i++) printf("  ");
    print256bit("f", q->f1, q->f2, q->f3, q->f4);

    if (q->leftPtr) {
        for (int i = 0; i < depth; i++) printf("  ");
        printf("Left Overflow Node:\n");
        printQuantumNumber(q->leftPtr, depth + 1);
    }
}

int testMASMAdd() {
    QuantumNumberV8 q1 = {0};
    QuantumNumberV8 q2 = {0};

    // Simple add with no overflow
    q1.a4 = 100ULL;
    q2.a4 = 200ULL;

    printf("Test 1: No Overflow Addition with MASM\n");
    QuantumNumberV8* result1 = addMASM(&q1, &q2);
    printQuantumNumber(result1, 0);

    printf("\n");

    // Add with overflow from least to most significant 64-bit chunk (simulate max values)
    q1.a4 = 0xFFFFFFFFFFFFFFFFULL;
    q1.a3 = 0xFFFFFFFFFFFFFFFFULL;
    q1.a2 = 0xFFFFFFFFFFFFFFFFULL;
    q1.a1 = 0xFFFFFFFFFFFFFFFFULL;

    q2.a4 = 1ULL;

    printf("Test 2: Overflow Addition with MASM\n");
    QuantumNumberV8* result2 = addMASM(&q1, &q2);
    printQuantumNumber(result2, 0);

    // Free allocated nodes (not shown here, but you should implement a recursive free function)

    return 0;
}

int testMASMAdd2() {
    QuantumNumberV8 q1 = {0};
    QuantumNumberV8 q2 = {0};

    // q1 has a4 near max to cause carry, a3 with some pattern
    q1.a4 = 0xFFFFFFFFFFFFFFF0ULL;  // Close to max but with some space (for clarity)
    q1.a3 = 0x0000000000000005ULL;  // Just a small value for testing carry add

    // q2 small number to push a4 over max and cause carry
    q2.a4 = 0x20ULL;  // 32 decimal, sum with q1.a4 will overflow 64-bit a4
    q2.a3 = 0x0000000000000001ULL;  // Also add 1 to a3 to check carry addition

    printf("Test 3: Carry from a4 to a3 Addition with MASM\n");
    QuantumNumberV8* result = addMASM(&q1, &q2);
    printQuantumNumber(result, 0);

    // Remember to free result if dynamically allocated

    return 0;
}

int testMASMAdd3() {
    QuantumNumberV8 q1 = {0};
    QuantumNumberV8 q2 = {0};

    // Initialize q1 with a pattern close to max in each part to cause overflow
    q1.a1 = 0xFFFFFFFFFFFFFFF0ULL;
    q1.a2 = 0xFFFFFFFFFFFFFFF1ULL;
    q1.a3 = 0xFFFFFFFFFFFFFFF2ULL;
    q1.a4 = 0xFFFFFFFFFFFFFFF3ULL;

    // Initialize q2 with values that will cause overflow in each corresponding part
    q2.a1 = 0x20ULL; // will overflow with q1.a1
    q2.a2 = 0x30ULL; // will overflow with q1.a2
    q2.a3 = 0x40ULL; // will overflow with q1.a3
    q2.a4 = 0x50ULL; // will overflow with q1.a4

    printf("Test 4: Full 'a' Group Addition with Overflow in Each Part\n");
    QuantumNumberV8* result = addMASM(&q1, &q2);
    printQuantumNumber(result, 0);

    // Expected results calculation (manually calculated carry propagation):
    // For a4: 0xFFFFFFFFFFFFFFF3 + 0x50 = 0x1000000000000043, carry 1
    // a4 = 0x43 (lower 64 bits)
    // carry to a3 = 1

    // For a3: 0xFFFFFFFFFFFFFFF2 + 0x40 + 1 = 0x1000000000000033, carry 1
    // a3 = 0x33

    // For a2: 0xFFFFFFFFFFFFFFF1 + 0x30 + 1 = 0x1000000000000022, carry 1
    // a2 = 0x22

    // For a1: 0xFFFFFFFFFFFFFFF0 + 0x20 + 1 = 0x1000000000000011, carry 1
    // a1 = 0x11

    // The carry after a1 is 1, so it should overflow to a higher block (leftPtr)
    assert(result->a4 == 0x43ULL);
    assert(result->a3 == 0x33ULL);
    assert(result->a2 == 0x22ULL);
    assert(result->a1 == 0x11ULL);

    // There should be an overflow node due to carry from a1
    assert(result->leftPtr != NULL);

    // Overflow node's a4 should be 1 (the carry)
    QuantumNumberV8* overflow = result->leftPtr;
    assert(overflow->a4 == 1ULL);

    // Other parts of overflow node should be zero
    assert(overflow->a1 == 0);
    assert(overflow->a2 == 0);
    assert(overflow->a3 == 0);
    assert(overflow->a1 == 0);
    assert(overflow->b1 == 0);
    assert(overflow->c1 == 0);
    // ... etc. (you can check as needed)

    printf("Test 4 passed!\n");

    // Remember to free result and overflow nodes if dynamically allocated

    return 0;
}

int main() {
    // Set console output code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);

    // Set stdout to normal text mode (NOT UTF-16 mode)
    _setmode(_fileno(stdout), _O_TEXT);

    printf("=== QuantumNumberV8 Addition Test — UTF-8 Demo × ÷ ===\n\n");

    // Initialize two QuantumNumberV8 values with some values
    QuantumNumberV8 q1 = {0};
    QuantumNumberV8 q2 = {0};

    // Simple add with no overflow
    q1.a4 = 100ULL;
    q2.a4 = 200ULL;

    printf("Test 1: No Overflow Addition\n");
    QuantumNumberV8* result1 = add(&q1, &q2);
    printQuantumNumber(result1, 0);

    printf("\n");

    // Add with overflow from least to most significant 64-bit chunk (simulate max values)
    q1.a4 = 0xFFFFFFFFFFFFFFFFULL;
    q1.a3 = 0xFFFFFFFFFFFFFFFFULL;
    q1.a2 = 0xFFFFFFFFFFFFFFFFULL;
    q1.a1 = 0xFFFFFFFFFFFFFFFFULL;

    q2.a4 = 1ULL;

    printf("Test 2: Overflow Addition\n");
    QuantumNumberV8* result2 = add(&q1, &q2);
    printQuantumNumber(result2, 0);

    // Free allocated nodes (not shown here, but you should implement a recursive free function)
    // testMASMAdd();
    testMASMAdd3();
    return 0;
}
