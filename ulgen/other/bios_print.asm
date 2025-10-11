org 0x7C00

macro print string {
	mov si, string
	call bios_print
}

start:
	xor ax, ax
	mov ds, ax
	cld

	print msgg
	print msg

	jmp hang


bios_print:
	lodsb
	or al, al  ; zero == end of str
	jz done
	mov ah, 0x0E
	mov bh, 0
	int 0x10
	jmp bios_print
done:
	ret


hang:
	jmp hang


msg  db 'hello', 13, 10, 0
msgg db 'world', 13, 10, 0

times 510-($-$$) db 0
dw 0xAA55
