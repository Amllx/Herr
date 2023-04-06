%macro IntDecl 1
    dq DENVERInt%1
%endmacro

%macro IntExp 1
DENVERInt%1:
    push %1
    jmp HandleIrq
%endmacro

%macro IntNormal 1
DENVERInt%1:
    push  0
    push  %1
    jmp HandleIrq
%endmacro

; Now we need to actually handle the interrupts
; We handover them to IDT.c "HandleInterrupts" subroutine
; Last edited 07/07/22

extern HandleInterrupts
global pInterruptVectorTable
global LdIdt

section .text

HandleIrq:
    cld

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

    mov rdi, rsp
    call HandleInterrupts

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

    IntNormal 0
    IntNormal 1
    IntNormal 2
    IntNormal 3
    IntNormal 4
    IntNormal 5
    IntNormal 6
    IntNormal 7
    IntExp   8
    IntNormal 9
    IntExp   10
    IntExp   11
    IntExp   12
    IntExp   13
    IntExp   14
    IntNormal 15
    IntNormal 16
    IntExp 17
    IntNormal 18
    IntNormal 19
    IntNormal 20
    IntNormal 21
    IntNormal 22
    IntNormal 23
    IntNormal 24
    IntNormal 25
    IntNormal 26
    IntNormal 27
    IntNormal 28
    IntNormal 29
    IntExp   30
    IntNormal 31

    %assign i 32
    %rep 224
        IntNormal i
    %assign i i+1
    %endrep

pInterruptVectorTable:
    %assign i 0
    %rep 256
        IntDecl i
    %assign i i+1
    %endrep

    LdIdt:
      lidt [rdi]
      ret
