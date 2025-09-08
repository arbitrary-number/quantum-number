#include <stdio.h>
#include "QuantumNumberV8.h"

int main() {
    QuantumNumberV8 q1 = {0};
    QuantumNumberV8 q2 = {0};

    // Initialize test values
    q1.a1 = 1;
    q2.a1 = 2;

    printf("=== QuantumNumberV8 Addition Diagnostic ===\n");
    printf("Initial values:\n");
    printf("q1.a1 = %llu\n", q1.a1);
    printf("q2.a1 = %llu\n", q2.a1);

    // Call the MASM addition function
    printf("Calling QN_Add(&q1, &q2)...\n");
    QN_Add(&q1, &q2);

    // Print values after addition
    printf("After QN_Add:\n");
    printf("q1.a1 = %llu\n", q1.a1);
    printf("q2.a1 (should be unchanged) = %llu\n", q2.a1);

    // Additional sanity check for other fields (should be zero)
    printf("Checking other fields in q1 (should be 0):\n");
    printf("q1.a2 = %llu\n", q1.a2);
    printf("q1.b1 = %llu\n", q1.b1);
    printf("q1.signs = %llu\n", q1.signs);

    return 0;
}
