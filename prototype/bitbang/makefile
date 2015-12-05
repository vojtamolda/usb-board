CC   = gcc -O2 -Wall $(INCS)
LIBS = -L/usr/local/lib/ -lftdi1
INCS = -I/usr/local/include/libftdi1/
RM = rm -f


all: bitbang

bitbang: bitbang.c
	$(CC) bitbang.c -o bitbang.run $(LIBS)
	chmod 755 bitbang.run

clean:
	${RM} *.bak *.o *~

mrproper: clean
	${RM} *.run
