#ifndef QUANTUM_NUMBER_V8_H
#define QUANTUM_NUMBER_V8_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Define the QuantumNumberV8 structure — layout must match the MASM struct
typedef struct {
    uint64_t signs;
    uint64_t metadata1;
    uint64_t metadata2;
    uint64_t metadata3;

    uint64_t a1, a2, a3, a4;
    uint64_t b1, b2, b3, b4;
    uint64_t c1, c2, c3, c4;
    uint64_t d1, d2, d3, d4;
    uint64_t e1, e2, e3, e4;
    uint64_t f1, f2, f3, f4;
} QuantumNumberV8;

// Define the NumberNode structure matching the MASM layout
typedef struct NumberNode {
    QuantumNumberV8 value;
    struct NumberNode* leftPtr;
    struct NumberNode* rightPtr;
    struct NumberNode* upPtr;
    struct NumberNode* downPtr;
    struct NumberNode* inPtr;
    struct NumberNode* outPtr;
} NumberNode;

// Exported MASM procedures (callable from C/C++)
void __stdcall quantumAdd(QuantumNumberV8* q1, QuantumNumberV8* q2);
void __stdcall quantumDivide(QuantumNumberV8* numerator, QuantumNumberV8* denominator);

#ifdef __cplusplus
}
#endif

#endif // QUANTUM_NUMBER_V8_H
