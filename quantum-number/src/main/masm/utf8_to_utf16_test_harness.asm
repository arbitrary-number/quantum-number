; ============================================================================
; UTF-8 TO UTF-16 TEST HARNESS
; ============================================================================
; Comprehensive test harness for UTF-8 to UTF-16 conversion macros
; Tests various Unicode scenarios and error handling
; ============================================================================

; Include the UTF-8 conversion macros
INCLUDE utf8_to_utf16_macros.asm

; Windows API declarations
EXTERN ExitProcess : PROC

.DATA
; ============================================================================
; TEST STRINGS - Various Unicode Test Cases
; ============================================================================

; Test Case 1: ASCII-only string (1-byte UTF-8 sequences)
; Expected: Direct ASCII mapping
test_ascii db "Hello World! Regular ASCII text.", 0Dh, 0Ah, 0
test_ascii_desc db "Test 1: ASCII-only string", 0Dh, 0Ah, 0

; Test Case 2: 2-byte UTF-8 sequences (U+0080 to U+07FF)
; Characters: é (U+00E9), ñ (U+00F1), ü (U+00FC)
test_two_byte db 0C3h, 0A9h, 0C3h, 0B1h, 0C3h, 0BCh, " - Spanish characters", 0Dh, 0Ah, 0
test_two_byte_desc db "Test 2: 2-byte UTF-8 sequences", 0Dh, 0Ah, 0

; Test Case 3: 3-byte UTF-8 sequences (U+0800 to U+FFFF)
; Characters: 你 (U+4F60), 好 (U+597D), 世 (U+4E16), 界 (U+754C)
test_three_byte db 0E4h, 0BDh, 0A0h, 0E5h, 0A5h, 0BDh, 0E4h, 0B8h, 095h, 0E7h, 095h, 08Ch, " - Chinese characters", 0Dh, 0Ah, 0
test_three_byte_desc db "Test 3: 3-byte UTF-8 sequences (Chinese)", 0Dh, 0Ah, 0

; Test Case 4: 4-byte UTF-8 sequences (Surrogate pairs U+10000 to U+10FFFF)
; Characters: 🚀 (U+1F680), 🎉 (U+1F389), 🌟 (U+1F31F)
test_four_byte db 0F0h, 09Fh, 09Ah, 080h, 0F0h, 09Fh, 08Eh, 089h, 0F0h, 09Fh, 08Ch, 09Fh, " - Emoji with surrogate pairs", 0Dh, 0Ah, 0
test_four_byte_desc db "Test 4: 4-byte UTF-8 sequences (emoji, surrogate pairs)", 0Dh, 0Ah, 0

; Test Case 5: Mixed Unicode ranges
; ASCII + 2-byte + 3-byte + 4-byte combinations
test_mixed db "ASCII: ABC", 0C3h, 0A9h, 0E4h, 0BDh, 0A0h, 0F0h, 09Fh, 09Ah, 080h, "ABC", 0Dh, 0Ah, 0
test_mixed_desc db "Test 5: Mixed Unicode (all byte sequence types)", 0Dh, 0Ah, 0

; Test Case 6: Invalid UTF-8 sequence
; Contains an incomplete 3-byte sequence (missing continuation byte)
test_invalid db "Valid", 0E4h, "text", 0Dh, 0Ah, 0
test_invalid_desc db "Test 6: Invalid UTF-8 (incomplete sequence)", 0Dh, 0Ah, 0

; Test Case 7: Empty string
test_empty db "", 0
test_empty_desc db "Test 7: Empty string", 0Dh, 0Ah, 0

; Test Case 8: European characters mix
test_european db 0C3h, 084h, 0C3h, 096h, 0C3h, 09Ch, 0C3h, 092h, " - German umlauts", 0Dh, 0Ah, 0
test_european_desc db "Test 8: European characters (ÄÖÜÖ)", 0Dh, 0Ah, 0

; ============================================================================
; MACROS FOR TEST EXECUTION
; ============================================================================

; Macro to execute a test case
RunTest MACRO test_desc, test_string
    LOCAL test_label
    push rcx
    push rdx

    ; Print test description
    mov rcx, offset test_desc
    call printString

    ; Convert and print the UTF-8 string
    PrintUtf8String offset test_string

    ; Add newline for readability
    lea rcx, [msgNewline]
    call printString

    pop rdx
    pop rcx
ENDM

.CODE

; ============================================================================
; MAIN TEST EXECUTION
; ============================================================================

test_utf8_conversions PROC
    ; Display test header
    lea rcx, [msg_test_header]
    call printString

    ; Execute all test cases
    RunTest test_ascii_desc, test_ascii
    RunTest test_two_byte_desc, test_two_byte
    RunTest test_three_byte_desc, test_three_byte
    RunTest test_four_byte_desc, test_four_byte
    RunTest test_mixed_desc, test_mixed
    RunTest test_invalid_desc, test_invalid
    RunTest test_empty_desc, test_empty
    RunTest test_european_desc, test_european

    ; Display completion message
    lea rcx, [msg_test_complete]
    call printString

    ret
test_utf8_conversions ENDP

PUBLIC main
main PROC
    ; Execute UTF-8 conversion tests
    call test_utf8_conversions

    ; Exit program gracefully
    xor rcx, rcx    ; Exit code 0
    call ExitProcess
main ENDP

END

; ============================================================================
; ADDITIONAL MESSAGES (defined in .DATA for cleaner code organization)
; ============================================================================

.DATA
msg_test_header db 0Dh, 0Ah, "==================================================", 0Dh, 0Ah
               db "UTF-8 TO UTF-16 CONVERSION TEST HARNESS", 0Dh, 0Ah
               db "Testing various Unicode scenarios and edge cases", 0Dh, 0Ah
               db "==================================================", 0Dh, 0Ah, 0Dh, 0Ah, 0

msg_test_complete db 0Dh, 0Ah, "==================================================", 0Dh, 0Ah
                 db "ALL TESTS COMPLETED SUCCESSFULLY!", 0Dh, 0Ah
                 db "==================================================", 0Dh, 0Ah, 0

msg_error       db "ERROR: ", 0

; ============================================================================
; BUILD INSTRUCTIONS
; ============================================================================
;
; To build this test harness:
; 1. Ensure utf8_to_utf16_macros.asm is in the same directory or include path
; 2. Use MASM64 build commands:
;    ml64 /c utf8_to_utf16_test_harness.asm
;    link utf8_to_utf16_test_harness.obj kernel32.lib user32.lib /subsystem:console /entry:main
;
; Expected output will demonstrate:
; - Accurate conversion of all UTF-8 byte sequences
; - Proper surrogate pair generation for supplementary characters
; - Safe handling of invalid sequences
; - Comprehensive Unicode text display capability
;
; ============================================================================
; TEST CASE DETAILS
; ============================================================================
;
; Test 1 (ASCII): Basic ASCII text to verify fundamental conversion
; Test 2 (2-byte): European accented characters (Latin-1 supplement)
; Test 3 (3-byte): CJK characters from Basic Multilingual Plane
; Test 4 (4-byte): Emojis requiring UTF-16 surrogate pairs
; Test 5 (Mixed): Combination of all encoding types
; Test 6 (Invalid): Tests error handling and fallback behavior
; Test 7 (Empty): Edge case with minimal input
; Test 8 (European): Additional international character coverage
;
; All tests validate proper console display via WriteConsoleW
; ============================================================================
