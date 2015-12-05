CC   = gcc -O2 -Wall $(INCS)
LIBS = -L/usr/local/lib/ -lftdi1
INCS = -I/usr/local/include/libftdi1/
RM = rm -f


all: atmega168

atmega168: atmega168.c
	$(CC) atmega168.c -o atmega168.run $(LIBS)
	chmod 755 atmega168.run

clean:
	${RM} *.bak *.o *~

mrproper: clean
	${RM} *.run
