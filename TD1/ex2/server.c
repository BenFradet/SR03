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
#include <sys/shm.h>
#include <sys/msg.h>

#define IPC_KEY				0x55
#define NB_MAX_CLT 			4
#define NB_MAX_TYP_OBJ		3

#define REQ_SRV	 			1

#define DEM_NUM_CLT 		101
#define DEM_CREATION_PANIER 102
#define DEM_LISTE_OBJETS	103
#define DEM_INFO_OBJ        104
#define DEM_QUIT_MAG        105

#define REP_CLT_PLEIN		200
#define REP_NUM_CLT 		201
#define REP_CREATION_PANIER	202
#define REP_LISTE_OBJETS	203
#define REP_INFO_OBJ        204
#define REP_SERVER_FULL		205
#define REP_QUIT_MAG        206

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

typedef struct _VP{
	bool 	mutex;
	int 	nb_client;
	int 	num_client;
	objet 	stockage[NB_MAX_TYP_OBJ];
	objet*	panier_list[NB_MAX_CLT];
	bool 	sous_processus_existe;
}VP;

void sigintHandler() {
	msgctl(id_msg,IPC_RMID,0);
	putchar('\n');
	exit(0);
}

void sousProcessusCompeleteHandler() {
	sous_processus_existe = false;
}

int traiteMessage(msg *message, VP* shmptr, bool is_sous_processus) {
	char tmpLog[255];
	if(!shmptr->mutex){
		shmptr->mutex = true;

		int i = 0;
		switch(message->req) {
			case DEM_NUM_CLT:
				puts("cli_noid ---[demande numéro client]---> serveur\
	                    DEM_NUM_CLT\n");
				if (shmptr->nb_client == NB_MAX_CLT) {
					message->type = REP_NUM_CLT;
					message->num_clt = -1;
					msgsnd(id_msg, message, MSG_SIZE, 0);
					puts("cli_noid <---[serveur occupe]--- serveur\
	                    REP_CLT_PLEIN\n");
					break;
				}
				message->type = REP_NUM_CLT;
	            
	            //enlever le rand pour eviter 2 clients avec le même
	            //numéro
				message->num_clt = shmptr->num_client;
				shmptr->num_client = (shmptr->num_client % 4) + 1;
				msgsnd(id_msg, message, MSG_SIZE, 0);
				puts("cli_noid <---[fournir numéro client]--- serveur\
	                    REP_NUM_CLT\n");
				shmptr->nb_client += 1;
				break;
			case DEM_CREATION_PANIER:						
				sprintf(tmpLog, "cli_%d ---[Demande de creation de panier]\
	                    ---> serveur DEM_CREATION_PANIER\n", message->num_clt);
				puts(tmpLog);
				memset(tmpLog, 0, sizeof(tmpLog));

				message->type = message->num_clt;
				message->req = REP_CREATION_PANIER;
				shmptr->panier_list[message->num_clt - 1] = (objet *)calloc(NB_MAX_TYP_OBJ,
																		sizeof(objet));
				memcpy(
					   shmptr->panier_list[message->num_clt - 1], 
					   shmptr->stockage, 
					   sizeof(shmptr->stockage)
				);
				
				for(i = 0;i < NB_MAX_TYP_OBJ;i++) {
					shmptr->panier_list[message->num_clt - 1][i].nb = 0;
	            }
	            
	            memcpy(
	            		message->panier, 
	            		shmptr->panier_list[message->num_clt - 1], 
	            		sizeof(message->panier)
	            );

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
				memcpy(message->panier, shmptr->stockage, sizeof(shmptr->stockage));

				for(i = 0;i < NB_MAX_TYP_OBJ;i++) {
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
	            memcpy(message->panier, shmptr->stockage, sizeof(shmptr->stockage));

	            message->ret = 0;
	            msgsnd(id_msg, message, MSG_SIZE, 0);

	            sprintf(tmpLog, "cli_%d <---[Fournir le stock et prix]---\
	                    REP_INFO_OBJ\n", message->num_clt);
	            puts(tmpLog);
	            break;
	        case DEM_QUIT_MAG:
	       		sprintf(tmpLog, "cli_%d ---[Demande quitter le magasin]-->\
	                    serveur DEM_QUIT_MAG\n", message->num_clt);
	            puts(tmpLog);
	            memset(tmpLog, 0, sizeof(tmpLog));

	            message->type = message->num_clt;
	            message->req = REP_QUIT_MAG;
	            message->ret = 0;

	            shmptr->nb_client -= 1;
	            free(shmptr->panier_list[message->num_clt - 1]);
	            shmptr->panier_list[message->num_clt] = NULL;

	            msgsnd(id_msg, message, MSG_SIZE, 0);
	            sprintf(tmpLog, "cli_%d <---[Permet de quitter le magasin]---\
	                    REP_INFO_OBJ\n", message->num_clt);
	            puts(tmpLog);
		}
		if (is_sous_processus) {
			shmptr->sous_processus_existe = false;
			shmptr->mutex = false;
		} else {
			shmptr->mutex = false;
			puts("Main over and sleep");
			sleep(5);
			puts("Main awake.");
		}

		return 0;

	} else{
		return 1;
	}
}

int main()
{
	srand(time(0));

	pid_t sp;
	key_t cle;
//	int nb_client;
	objet clientPanier[NB_MAX_TYP_OBJ];
	msg message;

	//Memoire partage
	int shmid;
	VP *shmptr = NULL;

	if(shmid = shmget(IPC_PRIVATE, sizeof(VP), IPC_CREAT | 0666) < 0){
		perror("shmget");
		exit(1);
	}

	//Initialisation de memoire partage
	shmptr = (VP *)shmat(shmid, NULL, 0);
	shmptr->mutex = false;
	shmptr->sous_processus_existe = false;
	shmptr->nb_client = 0;
	shmptr->num_client = 1;

	int i;
	for(i = 0; i < NB_MAX_CLT; i++)
		shmptr->panier_list[i] = NULL;
	
	objet tmpObjet1 = {OBJET_POMME, 2.5, 20};
	objet tmpObjet2 = {OBJET_BANANE, 1.6, 10};
	objet tmpObjet3 = {OBJET_POIRE, 2.0, 15};
	shmptr->stockage[0] = tmpObjet1;
	shmptr->stockage[1] = tmpObjet2;
	shmptr->stockage[2] = tmpObjet3;
	
	//Pour savoir si le serveur est en train de traiter 
    //le meme client en 2 ligne
	bool client_table[NB_MAX_CLT + 1] = {false, false, false, false, false};

    //numero client a retourner au client
//    int num_client = 1;
	cle = ftok("sr03p012", IPC_KEY);

	signal(SIGINT, sigintHandler);
//	signal(SIGUSR1, sousProcessusCompeleteHandler);
	
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

				if (!shmptr->sous_processus_existe) {
					sp = fork();
					if(sp == -1) {
						perror("Sous processus");
						exit(1);
					}
					if(sp == 0) {
						shmptr = (VP *)shmat(shmid, NULL, 0);
						msgrcv(id_msg, &message, MSG_SIZE, REQ_SRV, 0);
						puts("Sub Task:");
						while(traiteMessage(&message, shmptr, true) == 1){
							puts("Sub waiting...");
							sleep(1);
						}
						puts("Sub over.");	
						if (message.type == REP_NUM_CLT) {
							client_table[0] = false;
						} else {
							client_table[message.num_clt] = false;
						}
						shmdt(shmptr);
						return 0;
					}
				}

				shmptr->mutex = true;
				shmptr->sous_processus_existe = true;
				shmptr->mutex = false;
				
				puts("Main task:");
				
				while(traiteMessage(&message, shmptr, false) == 1){
					puts("Main waiting...");
					sleep(1);
				}

				if (message.type == REP_NUM_CLT) {
					client_table[0] = false;
				} else {
					client_table[message.num_clt] = false;
				}
			}
		}
	}
	shmdt(shmptr);
	msgctl(id_msg,IPC_RMID,0);

	return 0;
}
