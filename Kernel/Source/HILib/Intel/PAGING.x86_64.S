global VirtualFlushTLB
global VirtualLoadCR3

section .text
    VirtualFlushTLB: ; Invalidates the page table in order to force the MMU to refresh it.
        invlpg [rdi]
        ret

    VirtualLoadCR3:
        push rbp
        mov rsp, rbp
        mov rdi, cr3
        mov rbp, rsp
        pop rbp
        ret