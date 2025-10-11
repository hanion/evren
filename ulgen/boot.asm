org 0x7C00
jmp init


macro print string {
	mov si, string
	call bios_print
}
bios_print:
	lodsb
	or al, al  ; zero == end of str
	jz .done
	mov ah, 0x0E
	mov bh, 0
	int 0x10
	jmp bios_print
	.done:
		ret


KERNEL_SECTORS  equ 64
KERNEL_LOAD_SEG equ 0x1000
KERNEL_LOAD_OFF equ 0x0000
KERNEL_ADDR equ 0x10000

disk_error:
	print err_disk
	jmp $

short_pause:
	mov cx, 0xFFFF
	.pp:
		loop .pp
		ret

init:
	xor ax, ax
	mov ds, ax
	mov ss, ax
	mov sp, 0x7C00 ; stack in low memory
	cld

	;mov ax, 0xb800   ; text video memory
	;mov es, ax

	; enable A20
	in al, 0x92
	or al, 00000010b
	out 0x92, al

	; read kernel sectors to 0x10000 (ES:BX)
	mov bx, KERNEL_LOAD_OFF
	mov ax, KERNEL_LOAD_SEG


	mov es, ax
	mov ah, 0x02
	mov al, KERNEL_SECTORS
	mov ch, 0
	mov cl, 2
	mov dh, 0

	;mov dl, 0x80      ; first hard drive
	mov dl, 0x00      ; first floppy

	int 0x13
	jc disk_error

	; small delay, give time to BIOS
	print msg
	call short_pause

	lgdt [gdt_descriptor]

	cli ; disable interrupts

	; set PE bit in CR0
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	; far jump to flush prefetch and load CS
	jmp 0x08:protected_start


use32
protected_start:
	; data segments
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov eax, 0x00090000
	mov esp, eax

	jmp KERNEL_ADDR


gdt_start:
gdt_null:
	dq 0

gdt_code:           ; 0x08
	dw 0xFFFF       ; limit low
	dw 0x0000       ; base low
	db 0x00         ; base middle
	db 10011010b    ; access: present, ring 0, code, readable
	db 11001111b    ; granularity: 4KB blocks, 32-bit
	db 0x00         ; base high

gdt_data:           ; 0x10
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 10010010b    ; access: present, ring 0, data, writable
	db 11001111b
	db 0x00

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start



msg db "welcome to ulgen", 10, 0
err_disk db 'disk read error',0

times 510-($-$$) db 0
dw 0xAA55

