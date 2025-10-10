
.PHONY: all
all: build build/evren.bin


build/evren.bin: build/boot.o build/kernel.o
	i686-elf-gcc -T linker.ld -o build/evren.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o -lgcc

build/boot.o: boot.s
	i686-elf-as boot.s -o build/boot.o

build/kernel.o: kernel.c
	i686-elf-gcc -c kernel.c -o build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra


build:
	mkdir -p build

clean:
	rm -rf build

