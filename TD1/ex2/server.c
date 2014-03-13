#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define IPC_KEY				0x55
#define NB_MAX_CLT 			4
#define NB_MAX_CLT_SIM		2
#define NB_MAX_TYP_OBJ		3

#define REQ_SRV	 			1

#define DEM_NUM_CLT 		101
#define DEM_CREATION_PANIER 102
#define DEM_LISTE_OBJETS	103

#define REP_CLT_PLEIN		200
#define REP_NUM_CLT 		201
#define REP_CREATION_PANIER	202
#define REP_LISTE_OBJETS	203

#define OBJET_UN			"Pomme"
#define OBJET_DEUX			"Banane"
#define OBJET_TROIS			"Poire"

typedef struct _objet{
	char   name[15];
	float  prix;
	int    nb;
} objet;


typedef struct
{
	long 	type;
	int  	req;
	int  	num_clt;
	int  	ret;
	objet 	panier[NB_MAX_TYP_OBJ];
} msg;

int id_msg;
static objet stokage[NB_MAX_TYP_OBJ];

void sigintHandler(){
	msgctl(id_msg,IPC_RMID,0);
	putchar('\n');
	exit(0);
}

int main()
{
	srand(time(0));
	const int MSG_SIZE = sizeof(msg) - sizeof(long);

	key_t cle;
	int nb_client;
	objet clientPanier[NB_MAX_TYP_OBJ];
	msg message;
	cle = ftok("sr03p012", IPC_KEY);

	//init stokage
	objet tmpObjet1 = {OBJET_UN, 2.5, 20};
	stokage[0] = tmpObjet1;
	objet tmpObjet2 = {OBJET_DEUX, 1.6, 10};
	stokage[1] = tmpObjet2;
	objet tmpObjet3 = {OBJET_TROIS, 2.0, 15};
	stokage[2] = tmpObjet3;

	signal(SIGINT, sigintHandler);
	
	if(cle == -1)
	{
		perror("ftok");
	}
	else
	{
		id_msg = msgget(cle, IPC_CREAT|IPC_EXCL|0666);
		if(id_msg == -1)
		{
			perror("msgget");
		}
		else
		{
			while(1){
				char tmpLog[255];
				msgrcv(id_msg, (void*)&message, MSG_SIZE, REQ_SRV, 0);
				switch(message.req)
				{
					case DEM_NUM_CLT:
						puts("cli_noid ---[demande numéro client]---> serveur     		 DEM_NUM_CLT");
						if (nb_client == NB_MAX_CLT)
						{
							message.type = REP_CLT_PLEIN;
							message.num_clt = -1;
							msgsnd(id_msg, &message, MSG_SIZE, 0);
							break;
						}
						message.type = REP_NUM_CLT;
						message.num_clt = rand()%100;
						msgsnd(id_msg, (void*)&message, MSG_SIZE, 0);
						puts("cli_noid <---[fournir numéro client]--- serveur          REP_NUM_CLT\n");
						nb_client++;
						break;
					case DEM_CREATION_PANIER:						
						sprintf(tmpLog, "cli_%d ---[Demande de creation de panier]---> serveur       DEM_CREATION_PANIER", message.num_clt);
						puts(tmpLog);
						memset(tmpLog, 0, sizeof(tmpLog));

						message.type = message.num_clt;
						message.req = REP_CREATION_PANIER;
						tmpObjet1.nb = 0;
						tmpObjet2.nb = 0;
						tmpObjet3.nb = 0;
						clientPanier[0] = tmpObjet1;
						clientPanier[1] = tmpObjet2;
						clientPanier[2] = tmpObjet3;
						message.ret = 0;
						msgsnd(id_msg, (void*)&message, MSG_SIZE, 0);
						
						sprintf(tmpLog, "cli_%d <---[Creation de panier]--- serveur       REP_CREATION_PANIER\n", message.num_clt);
						puts(tmpLog);
						memset(tmpLog, 0, sizeof(tmpLog));
						break;
					case DEM_LISTE_OBJETS:
						sprintf(tmpLog, "cli_%d ---[Demande la liste d'objet]---> serveur       DEM_LISTE_OBJETS", message.num_clt);
						puts(tmpLog);
						memset(tmpLog, 0, sizeof(tmpLog));

						message.type = message.num_clt;
						message.req = REP_LISTE_OBJETS;
						memcpy(message.panier, clientPanier, sizeof(message.panier));
						
						message.ret = 0;
						msgsnd(id_msg, (void *)&message, MSG_SIZE, 0);

						sprintf(tmpLog, "cli_%d <---[Fournir liste d'objet]--- serveur       REP_LISTE_OBJETS\n", message.num_clt);
						puts(tmpLog);
						memset(tmpLog, 0, sizeof(tmpLog));
						break;
				}
			}
		}
	}

	msgctl(id_msg,IPC_RMID,0);

	return 0;
}
