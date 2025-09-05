; Copyright 2025 Arbitrary Number Project Team
; Licensed under the Apache License, Version 2.0
;
; Quantum Number Electron v2.0 - Assembly Unit Tests
; Tests for pure x64 assembly implementation
; Intel x64 Windows calling convention

section .data
    ; Test data structures
    test_qe1: times 4 dq 0    ; 32 bytes for 4 limbs
              db 0            ; 1 byte for flags
    
    test_qe2: times 4 dq 0    ; 32 bytes for 4 limbs  
              db 0            ; 1 byte for flags
    
    test_result: times 4 dq 0 ; 32 bytes for 4 limbs
                 db 0         ; 1 byte for flags
    
    ; Test constants
    test_val_42:   dq 42, 0, 0, 0
                   db 0
    
    test_val_1337: dq 1337, 0, 0, 0
                   db 0
    
    test_val_max:  dq 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF
                   db 0
    
    ; Test messages
    msg_test_start: db "Starting Quantum Electron Tests...", 13, 10, 0
    msg_test_zero: db "Testing qe_zero...", 13, 10, 0
    msg_test_one: db "Testing qe_one...", 13, 10, 0
    msg_test_copy: db "Testing qe_copy...", 13, 10, 0
    msg_test_add: db "Testing qe_add...", 13, 10, 0
    msg_test_subtract: db "Testing qe_subtract...", 13, 10, 0
    msg_test_multiply: db "Testing qe_multiply...", 13, 10, 0
    msg_test_compare: db "Testing qe_compare...", 13, 10, 0
    msg_test_is_zero: db "Testing qe_is_zero...", 13, 10, 0
    msg_test_negate: db "Testing qe_negate...", 13, 10, 0
    msg_test_passed: db "PASSED", 13, 10, 0
    msg_test_failed: db "FAILED", 13, 10, 0
    msg_all_tests_passed: db "All tests PASSED!", 13, 10, 0
    msg_some_tests_failed: db "Some tests FAILED!", 13, 10, 0
    
    ; Test counters
    tests_passed: dq 0
    tests_failed: dq 0

section .text
    ; External functions from quantum_number_electron.asm
    extern qe_zero
    extern qe_one
    extern qe_copy
    extern qe_add
    extern qe_subtract
    extern qe_multiply
    extern qe_compare
    extern qe_is_zero
    extern qe_set_sign
    extern qe_negate
    
    ; Windows API functions
    extern GetStdHandle
    extern WriteConsoleA
    extern ExitProcess
    
    global _start
    global main

; Constants
STD_OUTPUT_HANDLE equ -11

;==============================================================================
; _start - Entry point for the test program
;==============================================================================
_start:
main:
    ; Print test start message
    mov     rcx, msg_test_start
    call    print_string
    
    ; Run all tests
    call    test_qe_zero
    call    test_qe_one
    call    test_qe_copy
    call    test_qe_add
    call    test_qe_subtract
    call    test_qe_multiply
    call    test_qe_compare
    call    test_qe_is_zero
    call    test_qe_negate
    
    ; Print final results
    mov     rax, [tests_failed]
    test    rax, rax
    jz      .all_passed
    
    mov     rcx, msg_some_tests_failed
    call    print_string
    mov     rcx, 1
    jmp     .exit
    
.all_passed:
    mov     rcx, msg_all_tests_passed
    call    print_string
    mov     rcx, 0
    
.exit:
    call    ExitProcess

;==============================================================================
; print_string - Print null-terminated string to console
; Input: RCX = pointer to string
; Destroys: RAX, RCX, RDX, R8, R9
;==============================================================================
print_string:
    push    rbx
    push    rsi
    push    rdi
    
    mov     rsi, rcx        ; Save string pointer
    
    ; Get console handle
    mov     rcx, STD_OUTPUT_HANDLE
    call    GetStdHandle
    mov     rbx, rax        ; Save handle
    
    ; Calculate string length
    mov     rdi, rsi
    xor     rcx, rcx
.strlen_loop:
    cmp     byte [rdi + rcx], 0
    je      .strlen_done
    inc     rcx
    jmp     .strlen_loop
.strlen_done:
    
    ; Write to console
    mov     rcx, rbx        ; Console handle
    mov     rdx, rsi        ; String pointer
    mov     r8, rcx         ; String length (from above)
    mov     r9, 0           ; lpNumberOfCharsWritten (NULL)
    push    0               ; lpReserved (NULL)
    call    WriteConsoleA
    add     rsp, 8          ; Clean up stack
    
    pop     rdi
    pop     rsi
    pop     rbx
    ret

;==============================================================================
; assert_equal - Assert two quantum electrons are equal
; Input: RCX = qe1, RDX = qe2
; Output: Sets tests_passed or tests_failed
; Destroys: RAX, R8, R9, R10, R11
;==============================================================================
assert_equal:
    push    rcx
    push    rdx
    
    call    qe_compare
    test    rax, rax
    jz      .passed
    
    ; Test failed
    inc     qword [tests_failed]
    mov     rcx, msg_test_failed
    call    print_string
    jmp     .done
    
.passed:
    inc     qword [tests_passed]
    mov     rcx, msg_test_passed
    call    print_string
    
.done:
    pop     rdx
    pop     rcx
    ret

;==============================================================================
; assert_true - Assert condition is true
; Input: RAX = condition (0 = false, non-zero = true)
; Output: Sets tests_passed or tests_failed
;==============================================================================
assert_true:
    test    rax, rax
    jnz     .passed
    
    ; Test failed
    inc     qword [tests_failed]
    mov     rcx, msg_test_failed
    call    print_string
    ret
    
.passed:
    inc     qword [tests_passed]
    mov     rcx, msg_test_passed
    call    print_string
    ret

;==============================================================================
; Test Functions
;==============================================================================

;==============================================================================
; test_qe_zero - Test qe_zero function
;==============================================================================
test_qe_zero:
    mov     rcx, msg_test_zero
    call    print_string
    
    ; Initialize test quantum electron to non-zero
    mov     rcx, test_qe1
    mov     rax, 0xDEADBEEF
    mov     [rcx], rax
    mov     [rcx + 8], rax
    mov     [rcx + 16], rax
    mov     [rcx + 24], rax
    mov     byte [rcx + 32], 0xFF
    
    ; Call qe_zero
    call    qe_zero
    
    ; Verify result is zero
    mov     rax, [test_qe1]
    or      rax, [test_qe1 + 8]
    or      rax, [test_qe1 + 16]
    or      rax, [test_qe1 + 24]
    setz    al
    movzx   rax, al
    call    assert_true
    ret

;==============================================================================
; test_qe_one - Test qe_one function
;==============================================================================
test_qe_one:
    mov     rcx, msg_test_one
    call    print_string
    
    ; Call qe_one
    mov     rcx, test_qe1
    call    qe_one
    
    ; Verify result is one
    mov     rax, [test_qe1]
    cmp     rax, 1
    sete    al
    mov     rdx, [test_qe1 + 8]
    or      rdx, [test_qe1 + 16]
    or      rdx, [test_qe1 + 24]
    setz    dl
    and     al, dl
    movzx   rax, al
    call    assert_true
    ret

;==============================================================================
; test_qe_copy - Test qe_copy function
;==============================================================================
test_qe_copy:
    mov     rcx, msg_test_copy
    call    print_string
    
    ; Set up source with test value
    mov     rcx, test_qe1
    mov     rax, 0x123456789ABCDEF0
    mov     [rcx], rax
    mov     rax, 0xFEDCBA9876543210
    mov     [rcx + 8], rax
    mov     rax, 0x1111222233334444
    mov     [rcx + 16], rax
    mov     rax, 0x5555666677778888
    mov     [rcx + 24], rax
    mov     byte [rcx + 32], 0x42
    
    ; Copy to destination
    mov     rcx, test_qe2
    mov     rdx, test_qe1
    call    qe_copy
    
    ; Verify copy is equal to source
    mov     rcx, test_qe1
    mov     rdx, test_qe2
    call    assert_equal
    ret

;==============================================================================
; test_qe_add - Test qe_add function
;==============================================================================
test_qe_add:
    mov     rcx, msg_test_add
    call    print_string
    
    ; Set up operands: 42 + 1337 = 1379
    mov     rcx, test_qe1
    mov     rdx, test_val_42
    call    qe_copy
    
    mov     rcx, test_qe2
    mov     rdx, test_val_1337
    call    qe_copy
    
    ; Perform addition
    mov     rcx, test_result
    mov     rdx, test_qe1
    mov     r8, test_qe2
    call    qe_add
    
    ; Verify result is 1379
    mov     rax, [test_result]
    cmp     rax, 1379
    sete    al
    mov     rdx, [test_result + 8]
    or      rdx, [test_result + 16]
    or      rdx, [test_result + 24]
    setz    dl
    and     al, dl
    movzx   rax, al
    call    assert_true
    ret

;==============================================================================
; test_qe_subtract - Test qe_subtract function
;==============================================================================
test_qe_subtract:
    mov     rcx, msg_test_subtract
    call    print_string
    
    ; Set up operands: 1337 - 42 = 1295
    mov     rcx, test_qe1
    mov     rdx, test_val_1337
    call    qe_copy
    
    mov     rcx, test_qe2
    mov     rdx, test_val_42
    call    qe_copy
    
    ; Perform subtraction
    mov     rcx, test_result
    mov     rdx, test_qe1
    mov     r8, test_qe2
    call    qe_subtract
    
    ; Verify result is 1295
    mov     rax, [test_result]
    cmp     rax, 1295
    sete    al
    mov     rdx, [test_result + 8]
    or      rdx, [test_result + 16]
    or      rdx, [test_result + 24]
    setz    dl
    and     al, dl
    movzx   rax, al
    call    assert_true
    ret

;==============================================================================
; test_qe_multiply - Test qe_multiply function
;==============================================================================
test_qe_multiply:
    mov     rcx, msg_test_multiply
    call    print_string
    
    ; Set up operands: 42 * 1337 = 56154
    mov     rcx, test_qe1
    mov     rdx, test_val_42
    call    qe_copy
    
    mov     rcx, test_qe2
    mov     rdx, test_val_1337
    call    qe_copy
    
    ; Perform multiplication
    mov     rcx, test_result
    mov     rdx, test_qe1
    mov     r8, test_qe2
    call    qe_multiply
    
    ; Verify result is 56154
    mov     rax, [test_result]
    cmp     rax, 56154
    sete    al
    movzx   rax, al
    call    assert_true
    ret

;==============================================================================
; test_qe_compare - Test qe_compare function
;==============================================================================
test_qe_compare:
    mov     rcx, msg_test_compare
    call    print_string
    
    ; Test equal values
    mov     rcx, test_qe1
    mov     rdx, test_val_42
    call    qe_copy
    
    mov     rcx, test_qe2
    mov     rdx, test_val_42
    call    qe_copy
    
    mov     rcx, test_qe1
    mov     rdx, test_qe2
    call    qe_compare
    
    ; Should return 0 for equal
    test    rax, rax
    setz    al
    movzx   rax, al
    call    assert_true
    ret

;==============================================================================
; test_qe_is_zero - Test qe_is_zero function
;==============================================================================
test_qe_is_zero:
    mov     rcx, msg_test_is_zero
    call    print_string
    
    ; Test with zero value
    mov     rcx, test_qe1
    call    qe_zero
    
    mov     rcx, test_qe1
    call    qe_is_zero
    
    ; Should return 1 for zero
    cmp     rax, 1
    sete    al
    movzx   rax, al
    call    assert_true
    ret

;==============================================================================
; test_qe_negate - Test qe_negate function
;==============================================================================
test_qe_negate:
    mov     rcx, msg_test_negate
    call    print_string
    
    ; Set up test value
    mov     rcx, test_qe1
    mov     rdx, test_val_42
    call    qe_copy
    
    ; Negate it
    mov     rcx, test_result
    mov     rdx, test_qe1
    call    qe_negate
    
    ; Add original and negated - should be zero
    mov     rcx, test_qe2
    mov     rdx, test_qe1
    mov     r8, test_result
    call    qe_add
    
    ; Check if result is zero
    mov     rcx, test_qe2
    call    qe_is_zero
    
    call    assert_true
    ret
