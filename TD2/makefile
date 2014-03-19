all:sero clio

sero:sero.o
	gcc -o sero sero.o

clio:clio.o
	gcc -o clio clio.o

sero.o:sero.c defobj.h iniobj.h
	gcc -c sero.c

clio.o:clio.c defobj.h iniobj.h
	gcc -c clio.c

clean:
	rm *.o