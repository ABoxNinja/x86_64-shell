[bits 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x1000
mov [BOOT_DRIVE], dl

mov bp,0x9000
mov sp,bp

call load_kernel
call switch_32

jmp $

%include "src/boot/disk.s"
%include "src/boot/gdt.s"
%include "src/boot/switch_32.s"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 2
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
begin_32:
    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE db 0

times 510-($-$$) db 0
dw 0xaa55