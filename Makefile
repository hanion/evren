KERNEL_SECTORS := 64

.PHONY: all run clean
all: build build/evren.img run

build/boot.o: ulgen/boot.asm
	fasm ulgen/boot.asm build/boot.o

build/kernel.bin: tengri/kernel.c
	fasm tengri/startup.asm build/startup.o
	i686-elf-gcc -c -m32 -std=c99 -ffreestanding -O2 -Wall -Wextra tengri/kernel.c -o build/kernel.o
	i686-elf-ld -T tengri/linker.ld -o build/kernel.elf build/startup.o build/kernel.o
	objcopy -O binary build/kernel.elf build/kernel.bin

build/evren.img: build/boot.o build/kernel.bin
	dd if=/dev/zero of=build/evren.img bs=512 count=2880
	dd if=build/boot.o of=build/evren.img conv=notrunc
	dd if=build/kernel.bin of=build/evren.img bs=512 seek=1 conv=notrunc


run: build/evren.img
	qemu-system-i386 -fda build/evren.img -boot a

build:
	mkdir -p build

clean:
	rm -rf build






