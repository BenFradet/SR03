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

#define  ARRET -1

int reception(int client_socket) {
	obj objet;
    int n;
	n = read(client_socket, &objet, sizeof(obj));
    if(n < 0) {
        perror("read");
        return 1;
    }
	while(objet.fin != ARRET) {
		printf("Str1:%s, Str2:%s, ii:%d, jj:%d, dd:%f\n", 
                objet.str1, objet.str2, objet.ii, objet.jj, objet.dd);
		n = read(client_socket, &objet, sizeof(obj));
        if(n < 0) {
            perror("read");
            return 1;
        }
	}
	putchar('\n');
	sleep(1);
	return 0;
}

void sigchldHandler(int n) {
    pid_t pid;
	int status;
    pid = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)) {
        printf("Child %d done\n", (int)pid);
    } else {
        perror("waitpid child");
    }
}

int main(int argc, char *argv[]) {
    int server_socket, client_socket;
	int sockaddr_size;
	struct sockaddr_in server_addr;
    unsigned short port;

	if(argc < 2) {
		puts("Argument missing, usage: <port>...");
		exit(1);
	}
	port = atoi(argv[1]);

	signal(SIGCHLD, sigchldHandler);

	if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);	
	}
	
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(server_socket, (struct sockaddr *)&server_addr,
                sizeof(struct sockaddr_in)) < 0) {
		perror("bind");
		exit(1);
	}
	
	if(listen(server_socket, SOMAXCONN) < 0) {
        perror("listen");
        exit(1);
    }
		
	sockaddr_size = sizeof(struct sockaddr_in);
	while(1) {
		if((client_socket = accept(server_socket, 0, 0)) < 0) {
			perror("accept");
			exit(1);	
		}
		
		pid_t pid = fork();
        if(pid == -1) {
            perror("fork");
            exit(1);
        } else if(pid == 0) {
			int ret = reception(client_socket);
			close(client_socket);
			exit(ret);
		}
	}
	close(server_socket);
	return 0;
}
