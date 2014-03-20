all:sero clio sernow server client clean

sero:sero.o
	gcc -o sero sero.o

sernow:sernow.o
	gcc -o sernow sernow.o

clio:clio.o
	gcc -o clio clio.o

server:server.o
	gcc -o server server.o

client:client.o
	gcc -o client client.o

sero.o:sero.c defobj.h
	gcc -c sero.c

clio.o:clio.c defobj.h iniobj.h
	gcc -c clio.c

sernow.o:sernow.c defobj.h
	gcc -c sernow.c

server.o:server.c defobj.h
	gcc -c server.c

client.o:client.c defobj.h iniobj.h
	gcc -c client.c

clean:
	rm *.o

cleanEx:
	rm clio client server sernow sero
