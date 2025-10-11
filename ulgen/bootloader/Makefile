.PHONY: all run
all: run

boot.bin: boot.asm
	fasm boot.asm boot.bin

run: boot.bin
	qemu-system-x86_64 -drive format=raw,file=boot.bin
