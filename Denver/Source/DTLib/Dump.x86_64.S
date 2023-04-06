; This is XPX's Code
; File: Dump.x86_64.S
; Purpose: Dumping utilities.

global DumpStack
extern MemDumpStackInternal

section .text
    DumpStack:
        push rax
        push rbx
        push rcx
        push rdx
        push rsi
        push rdi
        push rbp
        push r8
        push r9
        push r10
        push r11
        push r12
        push r13
        push r14
        push r15

        mov rsp, rdi
        call MemDumpStackInternal

        pop r15
        pop r14
        pop r13
        pop r12
        pop r11
        pop r10
        pop r9
        pop r8
        pop rbp
        pop rdi
        pop rsi
        pop rdx
        pop rcx
        pop rbx
        pop rax

        ret