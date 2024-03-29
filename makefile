all: clam.bin run

kernel.bin: kernel-entry.o kernel.o
	ld -m i386pe -o kernel.elf -Ttext 0x1000 $^
	objcopy -O binary kernel.elf $@
kernel-entry.o: boot/kernel-entry.s
	nasm $< -f elf -o $@
kernel.o: kernel/kernel.c $(wildcard drivers/*.h)
	gcc -m32 -ffreestanding -c $< -o $@
boot_record.bin: boot/boot_record.s
	nasm $< -f bin -o $@
clam.bin: boot_record.bin kernel.bin
	copy /B boot_record.bin + kernel.bin clam.bin
run: clam.bin
	qemu-system-x86_64 -fda $<
clean:
	$(RM) *.bin *.o *.dis
