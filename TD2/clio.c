#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "defobj.h"
#include "iniobj.h"

#define  ARRET	2

//3eme parametres est traite comme le message de fin
int main(int argc, char *argv[]) {
	if(argc < 3) {
		puts("Pas de hostname et port...");
		exit(1);	
	}
	
	int clientfd;
	struct sockaddr_in server_addr;
	struct hostent *server;

	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("socket");
			exit(1);	
	}
	
	bzero(&server, sizeof(struct sockaddr_in));	
	if(!(server = gethostbyname(argv[1]))) {
			perror("server");
			exit(1);	
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));

    //doesnt work
	bcopy((char *)server->h_addr, 
		  (char *)&server_addr.sin_addr.s_addr,
  		  (size_t)(server->h_length));

	if(connect(clientfd, (struct sockaddr *)&server_addr,
			    sizeof(server_addr)) < 0) {
			perror("connect");
			exit(1);	
	}

	int n, i;
	for(i = 0; i < TABLEN; i++) {
		n = write(clientfd, &objtab[i], sizeof(obj));	
		if(n < 0) {
			perror("write");
			exit(1);
		}
	}

	obj fin = {"ident_o1", "description_o1", 11, 12, 10.2345, 1};
	n = write(clientfd, &fin, sizeof(obj));

	obj arret;
	if(argc == 4 && (strcmp("arret", argv[3]) == 0)) {
		arret.ii = 0;
		arret.jj = 0;
		arret.dd = 0;
		arret.fin = ARRET;
		write(clientfd, &arret, sizeof(obj));
	}

	if(n < 0) {
			perror("write");
			exit(1);
	}
	
	close(clientfd);
	return 0;
}
