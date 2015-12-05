CC   = gcc -O2 -Wall $(INCS)
LIBS = -L/usr/local/lib/ -lftdi1
INCS = -I/usr/local/include/libftdi1/
RM = rm -f


all: discover

discover: discover.c
	$(CC) discover.c -o discover.run $(LIBS)
	chmod 755 discover.run

clean:
	${RM} *.bak *.o *~

mrproper: clean
	${RM} *.run
