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

#define  ARRET	-1

int main(int argc, char *argv[]) {
	int client_socket;
	struct sockaddr_in server_addr;
	struct hostent *host;
    unsigned short port;
    char *hostname;

	if(argc < 3) {
		puts("Argument missing, usage: <host> <port>...");
		exit(1);	
	}
    port = atoi(argv[2]);
    hostname = argv[1];
	
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
    }
	
	bzero(&host, sizeof(struct hostent));	
	if(!(host = gethostbyname(hostname))) {
		perror("host");
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

	int n, i;
	for(i = 0; i < TABLEN; i++) {
		n = write(client_socket, &objtab[i], sizeof(obj));	
		if(n < 0) {
			perror("write");
			exit(1);
		}
	}

	obj fin = {"arret", "arret", 0, 0, 0, ARRET};
	n = write(client_socket, &fin, sizeof(obj));
	if(n < 0) {
		perror("write");
		exit(1);
	}
	
	close(client_socket);
	return 0;
}
