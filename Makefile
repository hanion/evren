
.PHONY: all
all: build build/evren.bin


build/evren.bin: build/ulgen/boot.o build/tengri/kernel.o
	i686-elf-gcc -T ulgen/linker.ld -o build/evren.bin -ffreestanding -O2 -nostdlib build/ulgen/boot.o build/tengri/kernel.o -lgcc

build/ulgen/boot.o: ulgen/boot.s
	i686-elf-as ulgen/boot.s -o build/ulgen/boot.o

build/tengri/kernel.o: tengri/kernel.c
	i686-elf-gcc -c tengri/kernel.c -o build/tengri/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra


build:
	mkdir -p build
	mkdir -p build/ulgen
	mkdir -p build/tengri

clean:
	rm -rf build

