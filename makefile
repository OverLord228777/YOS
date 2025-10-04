# Компиляторы
ASM=nasm
CC=gcc
LD=ld

# Флаги компиляции
ASMFLAGS=-f bin
CFLAGS=-ffreestanding -nostdlib -nostartfiles -Wall -Wextra
LDFLAGS=-T linker.ld

# Цели
all: os-image

# Загрузчик
boot.bin: boot.asm
	$(ASM) $(ASMFLAGS) boot.asm -o boot.bin

# Ядро
kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: kernel.o
	$(LD) $(LDFLAGS) -o kernel.bin kernel.o

# Образ ОС
os-image: boot.bin kernel.bin
	cat boot.bin kernel.bin > os-image
	# Заполняем до 1.44MB (стандартный размер дискеты)
	truncate -s 1474560 os-image

# Очистка
clean:
	rm -f *.bin *.o os-image

# Запуск в QEMU
run: os-image
	qemu-system-i386 -fda os-image

.PHONY: all clean run