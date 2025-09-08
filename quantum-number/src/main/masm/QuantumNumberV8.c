#include "QuantumNumberV8.h"
#include <stdlib.h>
#include <string.h> // memset

extern void __fastcall addMASMInnerAsm(QuantumNumberV8* q1, QuantumNumberV8* q2, QuantumNumberV8* q3, QuantumNumberV8* q4);

// Add two uint64_t values plus carry-in, produce sum and carry-out
static uint64_t add64_carry(uint64_t a, uint64_t b, uint64_t carry_in, uint64_t* carry_out) {
    uint64_t sum = a + b + carry_in;
    // carry if sum < a or (carry_in && sum == a)
    *carry_out = (sum < a) || (carry_in && sum == a) ? 1 : 0;
    return sum;
}

static QuantumNumberV8* allocateNode() {
    QuantumNumberV8* node = (QuantumNumberV8*)malloc(sizeof(QuantumNumberV8));
    if (node) {
        memset(node, 0, sizeof(QuantumNumberV8));
    }
    return node;
}

// Add one 256-bit group: q1Group and q2Group are pointers to the 4 uint64_t chunks (a1..a4)
// destGroup is where the sum is stored (also 4 uint64_t)
// If overflow, sets *leftPtr_out to newly allocated node with the carry.
static void add_256bit_group(
    uint64_t* destGroup, const uint64_t* q1Group, const uint64_t* q2Group,
    QuantumNumberV8** leftPtr_out
) {
    uint64_t carry = 0;
    for (int i = 3; i >= 0; i--) {
        destGroup[i] = add64_carry(q1Group[i], q2Group[i], carry, &carry);
    }

    if (carry) {
        // Overflow carry beyond 256-bit boundary
        QuantumNumberV8* carryNode = allocateNode();
        if (carryNode) {
            carryNode->a4 = carry;  // Store the carry in least significant 64 bits of new node
            *leftPtr_out = carryNode;
        }
        // If allocation fails, just leave leftPtr as NULL (no overflow handled)
    }
}

// Wrapper to call the MASM addition routine
// q1 + q2 -> result
QuantumNumberV8* addMASM(QuantumNumberV8* q1, QuantumNumberV8* q2) {
    if (!q1 || !q2) return NULL;

    QuantumNumberV8* result = allocateNode();
    if (!result) return NULL;

    QuantumNumberV8* overflow = allocateNode();
    if (!overflow) {
        free(result);
        return NULL;
    }

    printf("q1: %p\nq2: %p\nresult: %p\noverflow: %p\n", (void*)q1, (void*)q2, (void*)&result, (void*)&overflow);

    addMASMInnerAsm(q1, q2, result, overflow);

    printf("q1: %p\nq2: %p\nresult: %p\noverflow: %p\n", (void*)q1, (void*)q2, (void*)&result, (void*)&overflow);

    printQuantumNumber(result, 0);
    printQuantumNumber(overflow, 0);
    

    // Check if overflow has any non-zero values
    int has_overflow = overflow->a1 || overflow->a2 || overflow->a3 || overflow->a4 ||
                       overflow->b1 || overflow->b2 || overflow->b3 || overflow->b4 ||
                       overflow->c1 || overflow->c2 || overflow->c3 || overflow->c4 ||
                       overflow->d1 || overflow->d2 || overflow->d3 || overflow->d4 ||
                       overflow->e1 || overflow->e2 || overflow->e3 || overflow->e4 ||
                       overflow->f1 || overflow->f2 || overflow->f3 || overflow->f4 ||
                       overflow->signs || overflow->metadata1 || overflow->metadata2 || overflow->metadata3;

    if (has_overflow) {
        result->leftPtr = overflow;
        overflow->rightPtr = NULL;
        overflow->upPtr = NULL;
        overflow->downPtr = NULL;
        overflow->inPtr = NULL;
        overflow->outPtr = NULL;
    } else {
        free(overflow);
    }

    // Set other pointers to NULL
    result->rightPtr = NULL;
    result->upPtr = NULL;
    result->downPtr = NULL;
    result->inPtr = NULL;
    result->outPtr = NULL;

    return result;
}

// q1 + q2 -> q3 + overflow in q4
// implement this in MASM ni QuantumNumberV8.asm
void addMASMInner(QuantumNumberV8* q1, QuantumNumberV8* q2, QuantumNumberV8* q3, QuantumNumberV8* q4) {
}

QuantumNumberV8* add(QuantumNumberV8* q1, QuantumNumberV8* q2) {
    if (!q1 || !q2) return NULL;

    QuantumNumberV8* result = allocateNode();
    if (!result) return NULL;

    add_256bit_group(&result->a1, &q1->a1, &q2->a1, &result->leftPtr);
    add_256bit_group(&result->b1, &q1->b1, &q2->b1, &result->leftPtr);
    add_256bit_group(&result->c1, &q1->c1, &q2->c1, &result->leftPtr);
    add_256bit_group(&result->d1, &q1->d1, &q2->d1, &result->leftPtr);
    add_256bit_group(&result->e1, &q1->e1, &q2->e1, &result->leftPtr);
    add_256bit_group(&result->f1, &q1->f1, &q2->f1, &result->leftPtr);

    // Clear or copy metadata and sign bits as needed here if you want
    // For now zeroed by allocateNode

    // Null other pointers
    result->rightPtr = NULL;
    result->upPtr = NULL;
    result->downPtr = NULL;
    result->inPtr = NULL;
    result->outPtr = NULL;

    return result;
}
