#include "QuantumNumberV8.h"

// These declarations tell the C compiler that the functions are defined elsewhere,
// in the MASM object file or linked static/dynamic library.

extern void __stdcall quantumAdd(QuantumNumberV8* q1, QuantumNumberV8* q2);
extern void __stdcall quantumDivide(QuantumNumberV8* numerator, QuantumNumberV8* denominator);

// Optionally: You can wrap these MASM routines with simplified C-style functions
// if needed for clearer interfacing (especially with JNA).

// For example, wrapper with no __stdcall for simpler JNA signature:
void QN_Add(QuantumNumberV8* q1, QuantumNumberV8* q2) {
    quantumAdd(q1, q2);
}

void QN_Divide(QuantumNumberV8* numerator, QuantumNumberV8* denominator) {
    quantumDivide(numerator, denominator);
}
