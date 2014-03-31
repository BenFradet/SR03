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
#include "defmeta.h"

#define  ARRET 		-1

void sendHandler(ssize_t size) {
    if(size < 0) {
        perror("send");
    }
}

int reception(int client_socket) {
	obj objet;
	int ferme_flag = 0;
    ssize_t size_received, size_sent;
    do {
        size_received = recv(client_socket, &objet, sizeof(objet), 0);
        if(size_received < 0) {
            perror("recv");
            return 1;
        } else {
            if(objet.fin != ARRET) {
                printf("Modifer objet: {str1:%s,str2:%s,ii:%d,jj:%d,dd:%f}\n", 
                        objet.str1, objet.str2, objet.ii, objet.jj, objet.dd);
                //Modifie objet et le renvoyer
                char strbuffer[24];
                sprintf(strbuffer, "M_%s", objet.str1);
                memset(objet.str1, 0, sizeof(objet.str1));
                memcpy(objet.str1, strbuffer, sizeof(objet.str1));

                memset(strbuffer, 0, sizeof(strbuffer));
                sprintf(strbuffer, "M_%s", objet.str2);
                memset(objet.str2, 0, sizeof(objet.str2));
                memmove(objet.str2, strbuffer, sizeof(objet.str2));

                objet.ii += 10;
                objet.jj += 10;
                objet.dd += 10.0;

                metadata meta;
                sprintf(meta.type, "char");
                meta.qty = sizeof(objet.str1);
                sprintf(meta.attribut, "str1");
                sendHandler(send(client_socket, &meta, sizeof(meta), 0));
                sendHandler(send(client_socket, &objet.str1, 
                            sizeof(objet.str1), 0));

                meta.qty = sizeof(objet.str2);
                sprintf(meta.attribut, "str2");
                sendHandler(send(client_socket, &meta, sizeof(meta), 0));
                sendHandler(send(client_socket, &objet.str2,
                            sizeof(objet.str2), 0));

                sprintf(meta.type, "int");
                meta.qty = 1;
                sprintf(meta.attribut, "ii");
                sendHandler(send(client_socket, &meta, sizeof(meta), 0));
                sendHandler(send(client_socket, &objet.ii, 
                            sizeof(objet.ii), 0));

                sprintf(meta.attribut, "jj");
                sendHandler(send(client_socket, &meta, sizeof(meta), 0));
                sendHandler(send(client_socket, &objet.jj, 
                        sizeof(objet.jj), 0));

                sprintf(meta.attribut, "fin");
                sendHandler(send(client_socket, &meta, sizeof(meta), 0));
                sendHandler(send(client_socket, &objet.fin, 
                            sizeof(objet.fin), 0));

                sprintf(meta.type, "double");
                sprintf(meta.attribut, "dd");
                sendHandler(send(client_socket, &meta, sizeof(meta), 0));
                sendHandler(send(client_socket, &objet.dd,
                            sizeof(objet.dd), 0));

                //size_sent = send(client_socket, &objet, sizeof(objet), 0);
                //if(size_sent < 0) {
                //    perror("send");
                //    return 1;
                //}
                printf("Objet modifie et envoye: {str1:%s,str2:%s,ii:%d,jj:%d\
                        ,dd:%f}\n", objet.str1, objet.str2, objet.ii, objet.jj,
                        objet.dd);
            }
        }
    } while(objet.fin != ARRET);
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
