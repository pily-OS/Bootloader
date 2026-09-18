# 중요: Makefile에서 명령어 앞의 공백은 반드시 'Tab' 키로 띄워야 합니다!
CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m elf_i386 -T linker.ld

all: mykernel.bin

boot.o: boot.asm
	$(AS) -f elf32 boot.asm -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

mykernel.bin: boot.o kernel.o
	$(LD) $(LDFLAGS) -o mykernel.bin boot.o kernel.o

run: mykernel.bin
	qemu-system-x86_64 -kernel mykernel.bin

clean:
	rm -f *.o *.bin
