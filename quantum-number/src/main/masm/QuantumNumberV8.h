#ifndef QUANTUM_NUMBER_V8_H
#define QUANTUM_NUMBER_V8_H

#include <stdint.h>

// Quantum number struct
typedef struct QuantumNumberV8 {
    uint64_t signs;
    uint64_t metadata1;
    uint64_t metadata2;
    uint64_t metadata3;

    uint64_t a1;
    uint64_t a2;
    uint64_t a3;
    uint64_t a4;

    uint64_t b1;
    uint64_t b2;
    uint64_t b3;
    uint64_t b4;

    uint64_t c1;
    uint64_t c2;
    uint64_t c3;
    uint64_t c4;

    uint64_t d1;
    uint64_t d2;
    uint64_t d3;
    uint64_t d4;

    uint64_t e1;
    uint64_t e2;
    uint64_t e3;
    uint64_t e4;

    uint64_t f1;
    uint64_t f2;
    uint64_t f3;
    uint64_t f4;

    struct QuantumNumberV8* leftPtr;
    struct QuantumNumberV8* rightPtr;
    struct QuantumNumberV8* upPtr;
    struct QuantumNumberV8* downPtr;
    struct QuantumNumberV8* inPtr;
    struct QuantumNumberV8* outPtr;

} QuantumNumberV8;

QuantumNumberV8* add(QuantumNumberV8* q1, QuantumNumberV8* q2);

#endif
