[bits 32]
extern _main
global _start

section .text
_start:
    call _main
    jmp $