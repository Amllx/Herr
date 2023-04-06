global SSEEnable
global AVXEnable
global SSETest
global DisablePIC
global X87Test

section .text

[bits 32]

DisablePIC:
    mov al, 0xff
    out 0xa1, al
    out 0x21, al

SSEEnable:
    mov eax, cr0
    and ax, 0xFFFB
    or ax, 0x2
    mov cr0, eax
    mov eax, cr4
    or ax, 3 << 9
    mov eax, cr4
    ret

[bits 64]

AVXEnable:
    push rax
    push rcx
    push rdx

    xor rcx, rcx
    xgetbv ;Load XCR0 register
    or eax, 7 ;Set AVX, SSE, X87 bits
    xsetbv ;Save back to XCR0

    pop rdx
    pop rcx
    pop rax
    ret

SSETest:
    mov eax, 0x1
    cpuid
    test edx, 1 << 25
    mov edx, eax
    ret

X87Test:
    mov eax, 0
    cpuid
    test edx, 1 << 0
    mov edx, eax
    ret