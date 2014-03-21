#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

#include "defobj.h"

#define  ARRET 		2
#define	 FERME		3

int sd, clientfd;

int reception(int clientfd) {
	obj objet;
	int ferme_flag = 0;
	while(1){
		read(clientfd, &objet, sizeof(obj));
		while(objet.fin == 0 && objet.fin != FERME) {
			//Log
			printf("Modifer objet:{str1:%s, str2:%s, ii:%d, jj:%d, dd:%f}\n", 
	                objet.str1, objet.str2, objet.ii, objet.jj, objet.dd);

			//Modifier objet et le renvoyer
			char strbuffer[24];
			sprintf(strbuffer, "M_%s", objet.str1);
			memset(objet.str1, 0, sizeof(objet.str1));
			memcpy(objet.str1, strbuffer, sizeof(objet.str1));

			memset(strbuffer, 0, sizeof(strbuffer));
			sprintf(strbuffer, "M_%s", objet.str2);
			memset(objet.str2, 0, sizeof(objet.str2));
			memmove(objet.str2, strbuffer, sizeof(objet.str2));
			
			write(clientfd, &objet, sizeof(obj));

			//Lecture de donne
			read(clientfd, &objet, sizeof(obj));
		}
		if(objet.fin == FERME){
			puts("Yes");
			break;
		}
		else
			write(clientfd, &objet, sizeof(obj));	
	}
	putchar('\n');

	//sleep(1);

	//Detecter l'arret
	int n = read(clientfd, &objet, sizeof(obj));
	if(n > 0 && objet.fin == ARRET){
		return 1;
	}

	return 0;
}

void sigchldHandler(int n){
	int status;
	waitpid(-1, &status, WNOHANG);
	if(WEXITSTATUS(status) == 1){
		close(sd);
		close(clientfd);
		exit(0);
	}
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		puts("Pas de port...");
		exit(1);
	}

	int port, clienlen;

	signal(SIGCHLD, sigchldHandler);

	struct sockaddr_in server_addr, clien_addr;

	if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("socket");
			exit(1);	
	}
	
	port = atoi(argv[1]);
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(	bind(sd, (struct sockaddr *)&server_addr,
			sizeof(struct sockaddr_in)) < 0){
			perror("bind");
			exit(1);
	}
	
	listen(sd, SOMAXCONN);
		
	clienlen = sizeof(struct sockaddr_in);
	while(1) {
		if((clientfd = accept(sd, 0, 0)) < 0) {
			perror("accept");
			exit(1);	
		}
		
		pid_t pid = fork();
		if(pid == 0) {
			int ret = reception(clientfd);
			puts("Client demande la fermeture");
			puts("--------------------------------------------------------");
			close(clientfd);
			exit(ret);
		}
	}

	close(sd);

	return 0;
}