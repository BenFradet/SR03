all:sero clio sernow

sero:sero.o
	gcc -o sero sero.o

sernow:sernow.o
	gcc -o sernow sernow.o

clio:clio.o
	gcc -o clio clio.o

sero.o:sero.c defobj.h
	gcc -c sero.c

clio.o:clio.c defobj.h iniobj.h
	gcc -c clio.c

sernow.o:sernow.c defobj.h
	gcc -c sernow.c

clean:
	rm *.o