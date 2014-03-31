#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "defobj.h"
#include "defmeta.h"
#include "iniobj.h"

#define  ARRET 		-1

void recvHandler(ssize_t size) {
    if(size < 0) {
        perror("recv");
    }
}

void metaHandler(metadata meta, int client_socket) {
    if(strcmp(meta.type, "char") == 0) {
        char buffer[meta.qty];
        recvHandler(recv(client_socket, &buffer, sizeof(char) * meta.qty, 0));
        printf("%s modifiee vaut %s\n", meta.attribut, buffer);
    } else if(strcmp(meta.type, "int") == 0) {
        int buffer;
        recvHandler(recv(client_socket, &buffer, sizeof(int), 0));
        printf("%s modifiee vaut %d\n", meta.attribut, buffer);
    } else if(strcmp(meta.type, "double") == 0) {
        double buffer;
        recvHandler(recv(client_socket, &buffer, sizeof(double), 0));
        printf("%s modifiee vaut %f\n", meta.attribut, buffer);
    } else {
        printf("Type non reconnu\n");
    }
}

int main(int argc, char *argv[]) {
    int client_socket;
    struct sockaddr_in server_addr;
    struct hostent *host;
    unsigned short port;
    char *hostname;

	if(argc < 3) {
		puts("Argument missing, usage: localhost <port>...");
		exit(1);	
	}
    port = atoi(argv[2]);
    hostname = argv[1];

	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);	
	}
	
	//bzero(&host, sizeof(struct hostent));	
	if(!(host = gethostbyname(hostname))) {
		perror("server");
		exit(1);
    }
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	bcopy((char *)host->h_addr, 
		  (char *)&server_addr.sin_addr.s_addr,
  		  (size_t)(host->h_length));
	if(connect(client_socket, (struct sockaddr *)&server_addr,
			    sizeof(server_addr)) < 0) {
		perror("connect");
		exit(1);	
	}

    ssize_t size_sent;
    obj objet;
    int i;
	for(i = 0; i < TABLEN; i++) {
		size_sent = send(client_socket, &objtab[i], sizeof(obj), 0);	
		if(size_sent < 0) {
			perror("send");
			exit(1);
		}

        printf("Objet non modifie: {str1:%s,str2:%s,ii:%d,jj:%d,dd:%f}\n",
                objet.str1, objet.str2, objet.ii, objet.jj, objet.dd);
        metadata meta;
        int j;
        for(j = 0; j < 6; j++) {
            recvHandler(recv(client_socket, &meta, sizeof(meta), 0));
            printf("Lecture d'un %s ayant pour quantite %d ", 
                    meta.type, meta.qty);
            metaHandler(meta, client_socket);
        }
        
        //printf("Objet modifie: {str1:%s,str2:%s,ii:%d,jj:%d,dd:%f}\n",
        //       objet.str1, objet.str2, objet.ii, objet.jj, objet.dd);
    
        //size_received = recv(client_socket, &objet, sizeof(obj), 0);
        //if(size_received < 0) {
        //    perror("recv");
        //    exit(1);
        //} else {
        //    printf("Objet modifie: {str1:%s,str2:%s,ii:%d,jj:%d,dd:%f}\n",
        //            objet.str1, objet.str2, objet.ii, objet.jj, objet.dd);
        //}
	}

	obj fin = {"arret", "arret", 0, 0, 0.0, ARRET};
	size_sent = send(client_socket, &fin, sizeof(obj), 0);
	if(size_sent < 0) {
		perror("send");
		exit(1);
	}

	close(client_socket);
	return 0;
}
