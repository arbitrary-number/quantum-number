; PrintUtils.asm - 64-bit MASM style, no .model or .686

STD_OUTPUT_HANDLE EQU -11

EXTERN GetStdHandle : PROC
EXTERN WriteConsoleA : PROC

.data
    hStdOut QWORD 0

.code

; ------------------------------------------------------------
; InitPrint
; ------------------------------------------------------------
; Gets and stores handle to STD_OUTPUT_HANDLE in hStdOut
; Call this once before using PrintString
InitPrint PROC
    sub     rsp, 40             ; Shadow space + align stack
    mov     ecx, STD_OUTPUT_HANDLE
    call    GetStdHandle
    mov     hStdOut, rax
    add     rsp, 40
    ret
InitPrint ENDP

; ------------------------------------------------------------
; PrintString
; ------------------------------------------------------------
; Prints a null-terminated ASCII string to STDOUT
;
; Parameters:
;   RCX: pointer to null-terminated string
; ------------------------------------------------------------
PrintString PROC
    sub     rsp, 40             ; Shadow space + align stack
    push    rbx
    push    rsi
    push    rdi

    mov     rsi, rcx            ; rsi = pointer to string

    ; Find length of string (null-terminated)
    xor     rcx, rcx            ; rcx = length counter
find_len:
    cmp     BYTE PTR [rsi + rcx], 0
    je      done_len
    inc     rcx
    jmp     find_len
done_len:

    mov     rcx, hStdOut        ; hConsoleOutput
    mov     rdx, rsi            ; lpBuffer = string
    mov     r8, rcx             ; Number of chars to write (length)
    lea     r9, [rsp-8]         ; pointer to DWORD to receive chars written

    ; Call WriteConsoleA
    call    WriteConsoleA

    pop     rdi
    pop     rsi
    pop     rbx
    add     rsp, 40
    ret
PrintString ENDP

END
