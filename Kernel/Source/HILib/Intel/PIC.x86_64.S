global ISPicWait

section .text
ISPicWait:
     jmp .loop
    .loop:
        jmp .loop2
     .loop2:
        ; do nothing

     ret ; return
