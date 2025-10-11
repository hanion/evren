org 0x7C00
jmp init

include "print.asm"

init:
	xor ax, ax     ; make it zero
	mov ds, ax     ; DS=0
	mov ss, ax     ; stack starts at 0
	mov sp, 0x9c00 ; 2000h past code start
	cld

	mov ax, 0xb800   ; text video memory
	mov es, ax
	jmp start

start:
	call clear_screen

	mov si, msg   ; show text string
	call sprint

	cli            ; no interruptions
	mov bx, 0x09   ; hardware interrupt #
	shl bx, 2      ; multiply by 4
	xor ax, ax
	mov gs, ax     ; start of memory
	mov [gs:bx], word keyhandler
	mov [gs:bx+2], ds ; segment
	sti

	jmp hang


clear_screen:
	mov byte [ypos], 0
	mov byte [xpos], 0

	mov ah, 0x0F
	mov al, 0

	mov cl, TERM_HEIGHT
	.outer_loop:
		cmp cl, 0
		jz .done
	
		mov dl,TERM_WIDTH 
		.inner_loop:
			cmp dl, 0
			jz .inner_done
		
			push cx
			push dx
			call cprint
			pop dx
			pop cx
		
			dec dl
			jmp .inner_loop
		
		.inner_done:
			mov byte [xpos], 0
			add byte [ypos], 1
			dec cl
			jmp .outer_loop

	.done:
		mov byte [ypos], 0
		mov byte [xpos], 0
		ret

keyhandler:
	in al, 0x60   ; get key data
	mov bl, al    ; save it
	mov byte [port60], al

	in al, 0x61   ; keybrd control
	mov ah, al
	or al, 0x80    ; disable bit 7
	out 0x61, al   ; send it back
	xchg ah, al    ; get original
	out 0x61, al   ; send that back

	mov al, 0x20   ; End of Interrupt
	out 0x20, al   ;

	and bl, 0x80   ; key released
	jnz .done      ; don't repeat

	mov ax, [port60]
	mov  word [reg16], ax
	call printreg16

	.done:
		iret



hang:
	jmp hang

port60 dw 0

msg db "hello", 10, "world", 0

times 510-($-$$) db 0
dw 0xAA55
