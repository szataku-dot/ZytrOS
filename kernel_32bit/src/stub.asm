; kernel_32bit/stub.asm
[bits 32]
section .text
global _start

multiboot_header:
    dd 0x1BADB002
    dd 0x00
    dd -(0x1BADB002 + 0x00)

_start:
    mov edi, 0xB8000
    mov ah, 0x0F
    mov esi, msg
.loop:
    lodsb
    cmp al, 0
    je .halt
    mov [edi], ax
    add edi, 2
    jmp .loop

.halt:
    cli
    hlt
    jmp .halt

msg db "Hello World!", 0