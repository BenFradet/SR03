#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define IPC_KEY				0x55
#define NB_MAX_CLT 			4
#define NB_MAX_TYP_OBJ		3

#define REQ_SRV	 			1

#define DEM_NUM_CLT 		101
#define DEM_CREATION_PANIER 102
#define DEM_LISTE_OBJETS	103
#define DEM_INFO_OBJ        104

#define REP_CLT_PLEIN		200
#define REP_NUM_CLT 		201
#define REP_CREATION_PANIER	202
#define REP_LISTE_OBJETS	203
#define REP_INFO_OBJ        204
#define REP_SERVER_FULL		205

#define OBJET_POMME			"Pomme"
#define OBJET_BANANE	    "Banane"
#define OBJET_POIRE			"Poire"

typedef struct _objet {
	char   name[15];
	float  prix;
	int    nb;
} objet;


typedef struct {
	long 	type;
	int  	req;
	int  	num_clt;
	int  	ret;
	objet 	panier[NB_MAX_TYP_OBJ];
} msg;

/*Global variables*/
int id_msg;
static objet stockage[NB_MAX_TYP_OBJ];
static bool  sous_processus_existe = false;
const int MSG_SIZE = sizeof(msg) - sizeof(long);

void sigintHandler() {
	msgctl(id_msg,IPC_RMID,0);
	putchar('\n');
	exit(0);
}

void sousProcessusCompeleteHandler() {
	sous_processus_existe = false;
}

void traiteMessage(msg *message, int *nbClient, objet *clientPanier,
       int *num_client, bool is_sous_processus) {
	char tmpLog[255];
	switch(message->req) {
		case DEM_NUM_CLT:
			puts("cli_noid ---[demande numéro client]---> serveur\
                    DEM_NUM_CLT\n");
			if (*nbClient == NB_MAX_CLT) {
				message->type = REP_CLT_PLEIN;
				message->num_clt = -1;
				msgsnd(id_msg, message, MSG_SIZE, 0);
				break;
			}
			message->type = REP_NUM_CLT;
            //enlever le rand pour eviter 2 clients avec le même
            //numéro
			message->num_clt = *num_client;
			(*num_client) = (*num_client % 4) + 1;
			msgsnd(id_msg, message, MSG_SIZE, 0);
			puts("cli_noid <---[fournir numéro client]--- serveur\
                    REP_NUM_CLT\n");
			(*nbClient)++;
			break;
		case DEM_CREATION_PANIER:						
			sprintf(tmpLog, "cli_%d ---[Demande de creation de panier]\
                    ---> serveur DEM_CREATION_PANIER\n", message->num_clt);
			puts(tmpLog);
			memset(tmpLog, 0, sizeof(tmpLog));

			message->type = message->num_clt;
			message->req = REP_CREATION_PANIER;
			memcpy(clientPanier, stockage, sizeof(stockage));
			
			int i = 0;
			for(;i < NB_MAX_TYP_OBJ;i++) {
				clientPanier[i].nb = 0;
            }
            
            memcpy(message->panier, clientPanier, sizeof(message->panier));
			message->ret = 0;
			msgsnd(id_msg, message, MSG_SIZE, 0);
			
			sprintf(tmpLog, "cli_%d <---[Creation de panier]--- serveur\
                    REP_CREATION_PANIER\n", message->num_clt);
			puts(tmpLog);
			break;
		case DEM_LISTE_OBJETS:
			sprintf(tmpLog, "cli_%d ---[Demande la liste d'objet]---> serveur\
                    DEM_LISTE_OBJETS\n", message->num_clt);
			puts(tmpLog);
			memset(tmpLog, 0, sizeof(tmpLog));

			message->type = message->num_clt;
			message->req = REP_LISTE_OBJETS;
			memcpy(message->panier, stockage, sizeof(stockage));
			for(;i < NB_MAX_TYP_OBJ;i++) {
				message->panier[i].nb = 0;
            }
			
			message->ret = 0;
			msgsnd(id_msg, message, MSG_SIZE, 0);

			sprintf(tmpLog, "cli_%d <---[Fournir liste d'objet]--- serveur\
                    REP_LISTE_OBJETS\n", message->num_clt);
			puts(tmpLog);
			break;
        case DEM_INFO_OBJ:
            sprintf(tmpLog, "cli_%d ---[Demande du stock et du prix]-->\
                    serveur DEM_INFO_OBJ\n", message->num_clt);
            puts(tmpLog);
            memset(tmpLog, 0, sizeof(tmpLog));

            message->type = message->num_clt;
            message->req = REP_INFO_OBJ;
            memcpy(message->panier, stockage, sizeof(message->panier));

            message->ret = 0;
            msgsnd(id_msg, message, MSG_SIZE, 0);

            sprintf(tmpLog, "cli_%d <---[Fournir le stock et prix]---\
                    REP_INFO_OBJ\n", message->num_clt);
            puts(tmpLog);
            break;
	}
	if (is_sous_processus) {
		kill(getppid(), SIGUSR1);
	} else {
		puts("Main over and sleep");
		sleep(20);
		puts("Main awake.");
	}
}

int main()
{
	srand(time(0));

	pid_t sp;
	key_t cle;
	int nb_client;
	objet clientPanier[NB_MAX_TYP_OBJ];
	msg message;
	//Pour savoir si le serveur est en train de traiter 
    //le meme client en 2 ligne
	bool client_table[NB_MAX_CLT + 1] = {false, false, false, false, false};

    //numero client a retourner au client
    int num_client = 1;
	cle = ftok("sr03p012", IPC_KEY);

	//init stockage
	objet tmpObjet1 = {OBJET_POMME, 2.5, 20};
	stockage[0] = tmpObjet1;
	objet tmpObjet2 = {OBJET_BANANE, 1.6, 10};
	stockage[1] = tmpObjet2;
	objet tmpObjet3 = {OBJET_POIRE, 2.0, 15};
	stockage[2] = tmpObjet3;

	signal(SIGINT, sigintHandler);
	signal(SIGUSR1, sousProcessusCompeleteHandler);
	
	if(cle == -1) {
		perror("ftok");
	} else {
		id_msg = msgget(cle, IPC_CREAT|IPC_EXCL|0666);
		if(id_msg == -1) {
			perror("msgget");
		} else {
			while(1) {
				msgrcv(id_msg, &message, MSG_SIZE, REQ_SRV, 0);
				if (!client_table[message.num_clt])	{
					client_table[message.num_clt] = true;
				} else {
					message.type = message.num_clt;
					message.req = REP_SERVER_FULL;
					message.ret = -1;
					msgsnd(id_msg, &message, MSG_SIZE, 0);
					continue;
				}

				if (!sous_processus_existe) {
					sp = fork();
					if(sp == -1) {
						perror("Sous processus");
						exit(1);
					}
					if(sp == 0) {
						msgrcv(id_msg, &message, MSG_SIZE, REQ_SRV, 0);
						puts("Sub Task:");
						traiteMessage(&message, &nb_client, clientPanier, 
                                &num_client, true);
						puts("Sub over.");	
						if (message.type == REP_NUM_CLT) {
							client_table[0] = false;
						} else {
							client_table[message.num_clt] = false;
						}
						return 0;
					}
				}

				sous_processus_existe = true;
				puts("Main task:");
				traiteMessage(&message, &nb_client, clientPanier, &num_client,
                        false);
				if (message.type == REP_NUM_CLT) {
					client_table[0] = false;
				} else {
					client_table[message.num_clt] = false;
				}
			}
		}
	}

	msgctl(id_msg,IPC_RMID,0);

	return 0;
}
