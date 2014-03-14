#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define IPC_KEY				0x55
#define NB_MAX_TYP_OBJ		3
#define REQ_SRV	 			1

#define DEM_NUM_CLT 		101
#define DEM_CREATION_PANIER 102
#define DEM_LISTE_OBJETS	103
#define DEM_INFO_OBJ        104

#define REP_CLT_PLEIN		200
#define REP_NUM_CLT 		201
#define REP_CREATION_PANIER	202
#define REP_LISTE_OBJETS    203
#define REP_INFO_OBJ        204

typedef struct _objet{
	char   name[15];
	float  prix;
	int    nb;
} objet;

typedef struct
{
	long 	type;
	int 	req;
	int 	num_clt;
	int  	ret;
	objet 	panier[NB_MAX_TYP_OBJ];
} msg;

const int MSG_SIZE = sizeof(msg) - sizeof(long);

/*void afficherMenu(){
	puts("1.Creer un panier d'achat.");
	puts("2.La liste des types d'objets disponibles.");
	puts("3.L'état du stock et le prix d'un objet.");
	puts("4.Prélever des objets et les mettre dans un panier d'achat");
	puts("5.Enlever des objets du panier");
	puts("6.Vider le panier");
	puts("7.Acheter le contenu du panier.");
	puts("8.Fermer");
	printf("\nChoix? =");
}*/

int demandeCreerPanier(int msgId, msg *message, int num_client){
	message->type = REQ_SRV;
	message->req = DEM_CREATION_PANIER;
	msgsnd(msgId, message, MSG_SIZE, 0);
	msgrcv(msgId, message, MSG_SIZE, num_client, 0);
	return message->ret;
}

int demandeListeObjets(int msgId, msg *message, int num_client){
	message->type = REQ_SRV;
	message->req = DEM_LISTE_OBJETS;
	msgsnd(msgId, message, MSG_SIZE, 0);
	msgrcv(msgId, message, MSG_SIZE, num_client, 0);
	return message->ret;
}

int demandeInfoObjet(int msgId, msg* message, int num_client) {
    message->type = REQ_SRV;
    message->req = DEM_INFO_OBJ;
    msgsnd(msgId, message, MSG_SIZE, 0);
    msgrcv(msgId, message, MSG_SIZE, num_clt, 0);
    return message->ret;
}

int main()
{
	int id_msg;
	key_t cle;
	int mon_num_clt;
	//int choix;
	msg message;
	message.type = REQ_SRV;
	message.req = DEM_NUM_CLT;
	cle = ftok("sr03p012", IPC_KEY);

	//Check if the message queue exist
	if (msgget(cle, IPC_EXCL) < 0)
	{
		perror("msgget");
		exit(1);	
	}

	id_msg = msgget(cle, 0);

	//Demande client number
	msgsnd(id_msg, (void *)&message, MSG_SIZE, 0);
	msgrcv(id_msg, (void *)&message, MSG_SIZE, REP_NUM_CLT, 0);
	mon_num_clt = message.num_clt;

	if(demandeCreerPanier(id_msg, &message, mon_num_clt) == 0){
		puts("Panier est pret");
	}

	message.ret = -1;

	if(demandeListeObjets(id_msg, &message, mon_num_clt) == 0){
		int i;
		puts("Objets fournis:");
		for (i = 0; i < NB_MAX_TYP_OBJ; ++i)
		{
			printf("%d. %s\n", i+1, message.panier[i].name);
		}
	}

    if(demandeInfoObjet(id_msg, &message, mon_num_clt) == 0) {
        int i;
        for(i = 0; i < NB_MAX_TYP_OBJ; ++i) {
        }
    }

	return 0;
}
