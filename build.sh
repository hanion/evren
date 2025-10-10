#!/usr/bin/env bash
set -e

# build
make

# check multiboot
if grub-file --is-x86-multiboot build/evren.bin; then
	echo multiboot confirmed
else
	echo the file is not multiboot
fi

# create iso
mkdir -p build/isodir/boot/grub
cp build/evren.bin build/isodir/boot/evren.bin
cp grub.cfg build/isodir/boot/grub/grub.cfg
grub-mkrescue -o build/evren.iso build/isodir

# run in qemu
qemu-system-i386 -cdrom build/evren.iso

