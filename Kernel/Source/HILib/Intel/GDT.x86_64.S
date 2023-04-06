global LdGdt

section .text
    LdGdt:
        lgdt [rdi]
        iretq
