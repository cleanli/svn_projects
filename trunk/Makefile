ASFLAG=-D__ASSEMBLY__
CFLAG=-g  -Os   -fno-strict-aliasing  -fno-common -ffixed-r8 -msoft-float -mshort-load-bytes -fno-builtin -ffreestanding -nostdinc -isystem -pipe  -DCONFIG_ARM -D__ARM__ -march=armv4 -mapcs-32 -mno-thumb-interwork -I/home/clean/arm-linux/arm-tools/lib/gcc-lib/arm-linux/3.3.6/include/
LDFLAG=-Bstatic -L /home/clean/arm-linux/arm-tools/bin/../lib/gcc-lib/arm-linux/3.3.6 -lgcc -lm /home/clean/arm-linux/arm-tools/lib/gcc-lib/arm-linux/3.3.6/libgcc.a

all:	cboot2440.bin

asm2440.o:	asm2440.S
	arm-linux-gcc -c -o asm2440.o asm2440.S $(CFLAG) $(ASFLAG)

m.o:	m.c
	arm-linux-gcc -c -o m.o m.c $(CFLAG)
s3c2440.o:	s3c2440.c
	arm-linux-gcc -c -o s3c2440.o s3c2440.c $(CFLAG)
cmd.o:	cmd.c
	arm-linux-gcc -c -o cmd.o cmd.c $(CFLAG)
print.o:	print.c
	arm-linux-gcc -c -o print.o print.c $(CFLAG)
xmodem.o:	xmodem.c
	arm-linux-gcc -c -o xmodem.o xmodem.c $(CFLAG)

asm2440.elf:	asm2440.o asm2440.lds m.o s3c2440.o print.o cmd.o xmodem.o
	arm-linux-ld -T asm2440.lds -Map asm2440.map asm2440.o m.o cmd.o xmodem.o print.o s3c2440.o $(LDFLAG) -o asm2440.elf

cboot2440.bin:	asm2440.elf
	arm-linux-objcopy --gap-fill=0xff -O binary asm2440.elf cboot2440.bin	
	arm-linux-objdump -d asm2440.elf > asm2440.antiasm
	cp cboot2440.bin /var/ftp/pub

clean:
	rm -rf *.o *.elf *.map *.bin *.antiasm
