#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "iniobj.h"

void reception(int clientfd) {
	obj objet;
	read(clientfd, &objet, sizeof(obj));
	while(objet.fin == 0) {
		printf("Str1:%s, Str2:%s, ii:%d, jj:%d, dd:%f\n", 
                objet.str1, objet.str2, objet.ii, objet.jj, objet.dd);
		read(clientfd, &objet, sizeof(obj));
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		puts("Pas de port...");
		exit(1);
	}

	int sd, port, clientfd, clienlen;
	struct sockaddr_in server_addr, clien_addr;

	if(sd = socket(AF_INET, SOCK_STREAM, 0) < 0) {
			perror("socket");
			exit(1);	
	}
	
	port = atoi(argv[1]);
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sd, (struct sockaddr *)&server_addr,
			sizeof(struct sockaddr_in)) < 0){
			perror("bind");
			exit(1);
	}
	
	listen(sd, SOMAXCONN);
		
	clienlen = sizeof(struct sockaddr_in);
	while(1) {
			if(clientfd = accept(sd, 0, 0) < 0) {
				perror("accept");
				exit(1);	
			}
			pid_t pid;
			int status;
			if(pid = fork() == 0) {
					reception(clientfd);
					return 0;
			}
			waitpid(pid, &status, 0);
	}
}
