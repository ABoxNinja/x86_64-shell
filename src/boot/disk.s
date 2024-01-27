disk_load:
    pusha
    push dx

    mov ah, 0x02    ; read mode
    mov al, dh  ; read dh no. of sectors
    mov cl, 0x02    ; start from sect 2
                    ; sector 1 is the boot sector
    mov ch, 0x00    ; cylinder 0
    mov dh, 0x00    ; head 0

    int 0x13
    jc disk_error   ; check carry bit

    pop dx  ; origina no. of sectors
    cmp al, dh  ; of sectors

    jne sectors_error
    popa
    ret

disk_error:
    jmp disk_loop

sectors_error:
    jmp disk_loop

disk_loop:
    jmp $