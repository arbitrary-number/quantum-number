; PrintUtilsTest.asm - 64-bit MASM style test for PrintUtils.asm

STD_OUTPUT_HANDLE EQU -11

EXTERN InitPrint : PROC
EXTERN PrintString : PROC
EXTERN ExitProcess : PROC

.data
    msg1 db "=== PrintUtilsTest.asm ===", 13, 10, 0
    msg2 db "Hello from MASM!", 13, 10, 0
    msg3 db "Printing works ✔️", 13, 10, 0

.code

PUBLIC main                 ; <-- Add this line

main PROC
    sub     rsp, 40             ; shadow space + stack alignment

    call    InitPrint

    lea     rcx, msg1
    call    PrintString

    lea     rcx, msg2
    call    PrintString

    lea     rcx, msg3
    call    PrintString

    mov     ecx, 0             ; Exit code 0
    call    ExitProcess

    add     rsp, 40            ; (not reached, but for completeness)
    ret
main ENDP

END
