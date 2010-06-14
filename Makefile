ASFLAG=-D__ASSEMBLY__
CFLAG=-g  -Os   -fno-strict-aliasing  -fno-common -ffixed-r8 -load-bytes -fno-builtin -ffreestanding -nostdinc -isystem -pipe  -DCONFIG_ARM -D__ARM__ -march=armv4 -mno-thumb-interwork -I/usr/local/arm//4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2/include/ -I../u-boot/include
LDFLAG=-Bstatic -L /home/clean/arm-linux/arm-tools/bin/../lib/gcc-lib/arm-linux/3.3.6 -lgcc -lm -L/usr/local/arm//4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2/armv4t/ -L/usr/local/arm//4.3.2/arm-none-linux-gnueabi/libc/usr/lib/

CC=arm-linux-gcc
all:	minicb.bin

asm2440.o:	asm2440.S
	$(CC) -c -o asm2440.o asm2440.S $(CFLAG) $(ASFLAG)

m.o:	m.c
	$(CC) -c -o m.o m.c $(CFLAG)
s3c2440.o:	s3c2440.c
	$(CC) -c -o s3c2440.o s3c2440.c $(CFLAG)
cmd.o:	cmd.c
	$(CC) -c -o cmd.o cmd.c $(CFLAG)
print.o:	print.c
	$(CC) -c -o print.o print.c $(CFLAG)
xmodem.o:	xmodem.c
	$(CC) -c -o xmodem.o xmodem.c $(CFLAG)

asm2440.elf:	asm2440.o asm2440.lds m.o s3c2440.o print.o cmd.o xmodem.o
	arm-linux-ld -T asm2440.lds -Map asm2440.map asm2440.o m.o cmd.o xmodem.o print.o s3c2440.o $(LDFLAG) -o asm2440.elf

minicb.bin:	asm2440.elf
	arm-linux-objcopy --gap-fill=0xff -O binary asm2440.elf minicb.bin	
	arm-linux-objdump -d -S asm2440.elf > asm2440.antiasm
	tftp -p -l minicb.bin 192.168.58.43

clean:
	rm -rf *.o *.elf *.map *.bin *.antiasm
