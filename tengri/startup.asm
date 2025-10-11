format ELF

section '.text' executable

public _start

extrn kernel_main
extrn __bss_start
extrn __bss_end

_start:
	cld
	xor eax, eax
	mov edi, __bss_start
	mov ecx, __bss_end
	sub ecx, edi         ; size in bytes
	shr ecx, 2           ; convert to dwords
	rep stosd            ; zero .bss
	call kernel_main
	jmp $ ; never exit

