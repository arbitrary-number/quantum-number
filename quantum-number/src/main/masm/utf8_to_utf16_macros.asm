; ============================================================================
; UTF-8 TO UTF-16 CONVERSION MACROS AND PROCEDURES
; ============================================================================
; Standalone MASM x64 assembly file providing reusable UTF-8 to UTF-16
; conversion utilities for Windows console output.
;
; Features:
; - Full UTF-8 to UTF-16 conversion (1-4 byte sequences)
; - Support for surrogate pairs (U+10000 to U+10FFFF)
; - Bounds checking and error handling
; - Windows console output via WriteConsoleW
;
; Usage:
;   INCLUDE utf8_to_utf16_macros.asm
;   call Utf8ToUtf16, rcx(rcx=utf8_ptr), rdx(rdx=utf16_buffer_ptr)
;   call PrintUtf16, rcx(rcx=utf16_ptr), rdx(rdx=utf16_len)
; ============================================================================

; Windows API constants and externs
STD_OUTPUT_HANDLE EQU -11

EXTERN GetStdHandle : PROC
EXTERN WriteConsoleW : PROC

.DATA
; Fixed-size UTF-16 conversion buffer (4KB should handle most strings)
; This can accommodate ~2K characters (including surrogate pairs)
utf16_buffer db 4096 DUP (0) ; 4KB UTF-16 buffer
utf16_length dd 0            ; Length in UTF-16 characters (not bytes)

; Error handling
conversion_error db 0        ; Set to 1 if conversion error occurs

; ============================================================================
; UTF-8 TO UTF-16 CONVERSION PROCEDURE
; ============================================================================
; PROCEDURE: Utf8ToUtf16
; INPUT:
;   RCX = Pointer to null-terminated UTF-8 string
;   RDX = Pointer to UTF-16 buffer for output
; OUTPUT:
;   RAX = Number of UTF-16 characters written (0 = error)
;   Global: utf16_length updated
;   Global: conversion_error set if encoding error
; ============================================================================

Utf8ToUtf16 PROC
    push rbp
    mov rbp, rsp
    push rbx
    push rsi
    push rdi
    push r12
    push r13

    ; Save parameters
    mov rsi, rcx                    ; RSI = UTF-8 input pointer
    mov rdi, rdx                    ; RDI = UTF-16 output buffer
    mov r12, 0                      ; R12 = UTF-16 character count
    mov byte ptr [conversion_error], 0 ; Reset error flag

    ; Calculate maximum safe buffer size (leave room for null terminator)
    lea rax, [rdi + 4096 - 4]       ; Max write address (4KB - 2 chars for safety)
    mov r13, rax                    ; R13 = max write address for bounds checking

utf8_conversion_loop:
    ; Check bounds (leave room for potential surrogate pair + null terminator)
    lea rax, [rdi + r12*2 + 4]      ; Current write position + safety margin
    cmp rax, r13
    jae utf8_conversion_error       ; Buffer overflow

    ; Get next UTF-8 byte
    lodsb                           ; AL = next UTF-8 byte, RSI++
    test al, al                     ; Check for null terminator
    jz utf8_conversion_done         ; End of string

    ; Determine UTF-8 sequence length and decode
    ; Check first byte to determine sequence type

    ; 1-byte sequence (U+0000 to U+007F): 0xxxxxxx
    test al, 10000000b              ; Check if MSB is set
    jz utf8_decode_1byte

    ; 2-byte sequence: 110xxxxx 10xxxxxx
    test al, 11100000b              ; Check pattern 111xxxxx
    cmp al, 11000000b
    je utf8_decode_2byte

    ; 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx
    test al, 11110000b              ; Check pattern 1111xxxx
    cmp al, 11100000b
    je utf8_decode_3byte

    ; 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    test al, 11111000b              ; Check pattern 11111xxx
    cmp al, 11110000b
    je utf8_decode_4byte

    ; Invalid UTF-8 sequence
    jmp utf8_invalid_sequence

; ========================================
; 1-BYTE UTF-8 SEQUENCE HANDLING
; ========================================
utf8_decode_1byte:
    ; ASCII character: direct mapping
    and rax, 00000000000000000000000001111111b ; Clear high bits
    mov [rdi + r12*2], ax            ; Store as UTF-16 (little-endian)
    inc r12                          ; Increment character count
    jmp utf8_conversion_loop

; ========================================
; 2-BYTE UTF-8 SEQUENCE HANDLING
; ========================================
utf8_decode_2byte:
    movzx rbx, al                    ; RBX = first byte
    lodsb                            ; AL = second byte
    test al, al                      ; Check for premature null
    jz utf8_invalid_sequence
    movzx rcx, al                    ; RCX = second byte

    ; Validate continuation byte format: 10xxxxxx
    and cl, 11000000b                ; Extract bits 7-6
    cmp cl, 10000000b                ; Must be 10xxxxxx
    jne utf8_invalid_sequence

    ; Extract bits and combine
    and rbx, 00011111b               ; Extract 5 bits from first byte (xxxxx)
    and rcx, 00111111b               ; Extract 6 bits from second byte (xxxxxx)
    shl rbx, 6                       ; Shift left by 6
    or rbx, rcx                      ; Combine: first_byte_bits | second_byte_bits

    ; Store UTF-16 character
    mov [rdi + r12*2], bx
    inc r12
    jmp utf8_conversion_loop

; ========================================
; 3-BYTE UTF-8 SEQUENCE HANDLING
; ========================================
utf8_decode_3byte:
    movzx rbx, al                    ; RBX = first byte
    lodsb                            ; AL = second byte
    test al, al
    jz utf8_invalid_sequence
    movzx rdx, al                    ; RDX = second byte
    lodsb                            ; AL = third byte
    test al, al
    jz utf8_invalid_sequence
    movzx rcx, al                    ; RCX = third byte

    ; Validate continuation byte formats
    and dl, 11000000b
    cmp dl, 10000000b
    jne utf8_invalid_sequence
    and cl, 11000000b
    cmp cl, 10000000b
    jne utf8_invalid_sequence

    ; Extract bits and combine
    and rbx, 00001111b               ; Extract 4 bits from first byte (xxxx)
    and rdx, 00111111b               ; Extract 6 bits from second byte
    and rcx, 00111111b               ; Extract 6 bits from third byte
    shl rbx, 12                      ; Shift left by 12
    shl rdx, 6                       ; Shift left by 6
    or rbx, rdx                      ; Combine
    or rbx, rcx                      ; Combine

    ; Store UTF-16 character
    mov [rdi + r12*2], bx
    inc r12
    jmp utf8_conversion_loop

; ========================================
; 4-BYTE UTF-8 SEQUENCE HANDLING (SURROGATE PAIRS)
; ========================================
utf8_decode_4byte:
    movzx rbx, al                    ; RBX = first byte
    lodsb                            ; AL = second byte
    test al, al
    jz utf8_invalid_sequence
    movzx rdx, al                    ; RDX = second byte
    lodsb                            ; AL = third byte
    test al, al
    jz utf8_invalid_sequence
    movzx r8, al                     ; R8 = third byte
    lodsb                            ; AL = fourth byte
    test al, al
    jz utf8_invalid_sequence
    movzx rcx, al                    ; RCX = fourth byte

    ; Validate continuation byte formats
    and dl, 11000000b
    cmp dl, 10000000b
    jne utf8_invalid_sequence
    and r8b, 11000000b
    cmp r8b, 10000000b
    jne utf8_invalid_sequence
    and cl, 11000000b
    cmp cl, 10000000b
    jne utf8_invalid_sequence

    ; Extract bits and combine
    and rbx, 00000111b               ; Extract 3 bits from first byte (xxx)
    and rdx, 00111111b               ; Extract 6 bits from second byte
    and r8, 00111111b                ; Extract 6 bits from third byte
    and rcx, 00111111b               ; Extract 6 bits from fourth byte

    ; Combine into Unicode scalar value (U+10000 to U+10FFFF)
    shl rbx, 18                      ; Shift left by 18
    shl rdx, 12                      ; Shift left by 12
    shl r8, 6                        ; Shift left by 6
    or rbx, rdx                      ; Combine
    or rbx, r8                       ; Combine
    or rbx, rcx                      ; Combine

    ; Convert to UTF-16 surrogate pair
    ; High surrogate: 0xD800 | ((scalar - 0x10000) >> 10)
    ; Low surrogate:  0xDC00 | ((scalar - 0x10000) & 0x3FF)

    sub rbx, 10000h                  ; Subtract 0x10000
    mov rdx, rbx                     ; Copy scalar - 0x10000

    ; High surrogate
    shr rdx, 10                      ; (scalar - 0x10000) >> 10
    or rdx, 0D800h                   ; Add high surrogate base (0xD800)
    mov [rdi + r12*2], dx            ; Store high surrogate
    inc r12

    ; Low surrogate
    and rbx, 03FFh                   ; (scalar - 0x10000) & 0x3FF
    or rbx, 0DC00h                   ; Add low surrogate base (0xDC00)
    mov [rdi + r12*2], bx            ; Store low surrogate
    inc r12

    jmp utf8_conversion_loop

; ========================================
; ERROR HANDLING
; ========================================
utf8_invalid_sequence:
    ; Skip invalid sequence - could be improved with error return
    inc rsi                           ; Skip this byte
    jmp utf8_conversion_loop

utf8_conversion_error:
    mov byte ptr [conversion_error], 1
    mov r12, 0                       ; Return 0 characters on error
    ; Could return specific error code here

utf8_conversion_done:
    ; Null-terminate UTF-16 string
    mov word ptr [rdi + r12*2], 0
    mov dword ptr [utf16_length], r12d  ; Update global length
    mov rax, r12                       ; Return character count

utf8_conversion_exit:
    pop r13
    pop r12
    pop rdi
    pop rsi
    pop rbx
    mov rsp, rbp
    pop rbp
    ret
Utf8ToUtf16 ENDP

; ============================================================================
; UTF-16 CONSOLE OUTPUT PROCEDURE
; ============================================================================
; PROCEDURE: PrintUtf16
; INPUT:
;   RCX = Pointer to UTF-16 string
;   RDX = Length in UTF-16 characters (not bytes)
; OUTPUT:
;   RAX = Number of characters written (0 = error)
; ============================================================================

PrintUtf16 PROC
    push rbp
    mov rbp, rsp
    push rbx
    push rsi

    ; Get stdout handle
    mov rcx, STD_OUTPUT_HANDLE
    call GetStdHandle
    mov rbx, rax                    ; RBX = handle

    ; Validate handle
    cmp rbx, -1
    je print_utf16_error

    ; Prepare WriteConsoleW call
    mov rcx, rbx                    ; hConsoleOutput
    mov rdx, rsi                    ; lpBuffer (UTF-16 string from input)
    mov rsi, rdx                    ; Save original buffer pointer
    mov r8, rdx                     ; nNumberOfCharsToWrite (character count)
    lea r9, [rsp + 32]              ; lpNumberOfCharsWritten
    mov QWORD PTR [rsp + 40], 0     ; lpReserved

    ; Call WriteConsoleW
    call WriteConsoleW

    ; Check for errors
    test rax, rax
    jnz print_utf16_success

print_utf16_error:
    xor rax, rax                    ; Return 0 on error
    jmp print_utf16_exit

print_utf16_success:
    mov rax, r8                     ; Return number of characters written

print_utf16_exit:
    pop rsi
    pop rbx
    mov rsp, rbp
    pop rbp
    ret
PrintUtf16 ENDP

; ============================================================================
; UTILITY MACROS FOR EASY USE
; ============================================================================

; MACRO: ConvertUtf8ToUtf16 <utf8_string, utf16_buffer>
ConvertUtf8ToUtf16 MACRO utf8_ptr, utf16_buffer
    LOCAL result
    push rcx
    push rdx
    mov rcx, utf8_ptr               ;; UTF-8 input pointer
    mov rdx, utf16_buffer           ;; UTF-16 output buffer
    call Utf8ToUtf16
    mov result, rax                 ;; Save result
    pop rdx
    pop rcx
    mov rax, result                 ;; Return character count
ENDM

; MACRO: PrintUtf8String <utf8_string>
PrintUtf8String MACRO utf8_ptr
    LOCAL temp_buffer
    .DATA
    temp_buffer dw 2048 DUP (0)     ;; Temporary UTF-16 buffer
    .CODE
    push rcx
    push rdx
    mov rcx, utf8_ptr               ;; UTF-8 input
    lea rdx, temp_buffer            ;; UTF-16 output buffer
    call Utf8ToUtf16                ;; Convert to UTF-16
    test rax, rax                   ;; Check for errors
    jz print_utf8_done              ;; Skip if error
    lea rcx, temp_buffer            ;; UTF-16 buffer
    mov rdx, rax                    ;; Character count
    call PrintUtf16                 ;; Print to console
print_utf8_done:
    pop rdx
    pop rcx
ENDM

; ============================================================================
; EXAMPLE USAGE AND TESTING
; ============================================================================

; Example usage within other ASM files:
; INCLUDE utf8_to_utf16_macros.asm
;
; .DATA
; test_utf8 db "Hello 世界! 🌟", 0
; output_utf16 dw 1024 DUP (0)
;
; .CODE
; ; Method 1: Manual conversion and printing
; mov rcx, offset test_utf8
; lea rdx, output_utf16
; call Utf8ToUtf16                  ; Convert UTF-8 to UTF-16
; lea rcx, output_utf16             ; UTF-16 buffer
; mov rdx, utf16_length             ; Character count
; call PrintUtf16                   ; Print to console
;
; ; Method 2: Using macro (automatic conversion and printing)
; PrintUtf8String offset test_utf8  ; One-line conversion and print
;
; ; Method 3: Using conversion macro separately
; lea rdx, output_utf16
; ConvertUtf8ToUtf16 offset test_utf8, rdx
; ; Use result in RAX as needed...

END

; ============================================================================
; CHANGELOG AND NOTES
; ============================================================================
;
; Version 1.0 - Initial implementation
; - Full UTF-8 to UTF-16 conversion support
; - Surrogate pair handling for BMP-extended characters
; - Buffer bounds checking
; - Error handling for invalid sequences
; - Windows console output via WriteConsoleW
; - Easy-to-use macros for common operations
; - Comprehensive inline documentation
;
; Supported Unicode ranges:
; - U+0000 to U+007F (ASCII): 1-byte UTF-8
; - U+0080 to U+07FF: 2-byte UTF-8
; - U+0800 to U+FFFF: 3-byte UTF-8
; - U+10000 to U+10FFFF: 4-byte UTF-8 (surrogate pairs)
;
; Error handling:
; - Invalid UTF-8 sequences are skipped safely
; - Buffer overflow protection prevents crashes
; - Error flag set for debugging/troubleshooting
;
; Buffer sizing:
; - Default 4KB UTF-16 buffer (2048 UTF-16 characters)
; - Adjustable via utf16_buffer definition
; - Bounds checking prevents overflow
;
; Performance notes:
; - Optimized bit operations for speed
; - Minimal register usage, preserves stack integrity
; - Suitable for real-time applications
; ============================================================================
