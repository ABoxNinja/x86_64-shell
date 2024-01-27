all: phi.bin run

kernel.bin: kernel-entry.o kernel.o
	ld -m i386pe -o kernel.elf -Ttext 0x1000 $^
	objcopy -O binary kernel.elf $@
kernel-entry.o: boot/kernel-entry.s
	nasm $< -f elf -o $@
kernel.o: kernel/kernel.c
	gcc -m32 -ffreestanding -c $< -o $@
boot_record.bin: boot/boot_record.s
	nasm $< -f bin -o $@
phi.bin: boot_record.bin kernel.bin
	copy /B boot_record.bin + kernel.bin phi.bin
run: phi.bin
	qemu-system-x86_64 -fda $<
clean:
	$(RM) *.bin *.o *.dis