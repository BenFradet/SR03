#include <stdlib.h>
#include <string.h>
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
#define DEM_QUIT_MAG        105

#define REP_CLT_PLEIN		200
#define REP_NUM_CLT 		201
#define REP_CREATION_PANIER	202
#define REP_LISTE_OBJETS    203
#define REP_INFO_OBJ        204
#define REP_SERVER_FULL     205
#define REP_QUIT_MAG        206

#define OBJET_POMME         "Pomme"
#define OBJET_BANANE        "Banane"
#define OBJET_POIRE         "Poire"

typedef struct _objet {
	char   name[15];
	float  prix;
	int    nb;
} objet;

typedef struct {
	long 	type;
	int 	req;
	int 	num_clt;
	int  	ret;
	objet 	panier[NB_MAX_TYP_OBJ];
} msg;

const int MSG_SIZE = sizeof(msg) - sizeof(long);

void afficherStockPrix(msg message, char objet_char) {
    char objet_str[15];
    switch(objet_char) {
        case 'a':
            strcpy(objet_str, "Pomme");
            break;
        case 'b':
            strcpy(objet_str, "Banane");
            break;
        case 'p':
            strcpy(objet_str, "Poire");
           break; 
    }
    int i;
    for(i = 0; i < NB_MAX_TYP_OBJ; ++i) {
        if(strcmp(objet_str, message.panier[i].name) == 0) {
            printf("Prix: %f\n", message.panier[i].prix);
            printf("Stock: %d\n", message.panier[i].nb);
            return;
        }
    }
}

int demandeCreerPanier(int msg_id, msg *message, int num_client) {
	message->type = REQ_SRV;
	message->req = DEM_CREATION_PANIER;
	msgsnd(msg_id, message, MSG_SIZE, 0);
	msgrcv(msg_id, message, MSG_SIZE, num_client, 0);
	return message->ret;
}

int demandeListeObjets(int msg_id, msg *message, int num_client) {
	message->type = REQ_SRV;
	message->req = DEM_LISTE_OBJETS;
	msgsnd(msg_id, message, MSG_SIZE, 0);
	msgrcv(msg_id, message, MSG_SIZE, num_client, 0);
	return message->ret;
}

int demandeInfoObjet(int msg_id, msg* message, int num_client) {
    message->type = REQ_SRV;
    message->req = DEM_INFO_OBJ;
    msgsnd(msg_id, message, MSG_SIZE, 0);
    msgrcv(msg_id, message, MSG_SIZE, num_client, 0);
    return message->ret;
}

int demandeQuitterMagasin(int msg_id, msg* message, int num_client){
    message->type = REQ_SRV;
    message->req = DEM_QUIT_MAG;
    msgsnd(msg_id, message, MSG_SIZE, 0);
    msgrcv(msg_id, message, MSG_SIZE, num_client, 0);
    return message->ret;
}

int main()
{
	int id_msg;
	key_t cle;
	int mon_num_clt;
	msg message;
	message.type = REQ_SRV;
	message.req = DEM_NUM_CLT;
	message.num_clt = 0;
	cle = ftok("sr03p012", IPC_KEY);

	//Check if the message queue exist
	if(msgget(cle, IPC_EXCL) < 0) {
		perror("Aucun server n'a pu etre contacte");
		exit(1);	
	}

	id_msg = msgget(cle, 0);

	//Demande client number
	msgsnd(id_msg, (void *)&message, MSG_SIZE, 0);
	msgrcv(id_msg, (void *)&message, MSG_SIZE, REP_NUM_CLT, 0);
	mon_num_clt = message.num_clt;

    if(mon_num_clt < 0){
        puts("Serveur a atteint le nombre de clients maximum...");
        return 0;
    }

    char rep[20] = "init";
	int ret_flag = 0;
    while(strcmp(rep, "0") != 0) {
        puts("\n\nChoisissez une des options suivantes:\n");
        puts("(1) pour creer un nouveau panier");
        puts("(2) pour demander la liste des objets disponibles");
        puts("(3) pour consuler le prix et stock d'un produit");
        puts("(0) pour sortir");
        scanf("%s", &rep);

        if(strcmp(rep, "1") == 0) {
            ret_flag = demandeCreerPanier(id_msg, &message, mon_num_clt);
            if(ret_flag == 0) {
                puts("Panier est pret");
            } else if(ret_flag == -1) {
                puts("Server occupe...");
            }
        } else if(strcmp(rep, "2") == 0) {
            ret_flag = demandeListeObjets(id_msg, &message, mon_num_clt);
            if(ret_flag == 0) {
                int i;
                puts("Objets fournis:");
                for(i = 0; i < NB_MAX_TYP_OBJ; ++i) {
                    printf("%d. %s\n", i+1, message.panier[i].name);
                }
            } else if(ret_flag == -1) {
                puts("Server occupe...");
            }
        } else if(strcmp(rep, "3") == 0) {
            ret_flag = demandeInfoObjet(id_msg, &message, mon_num_clt);
            if(ret_flag == 0) {
                char objet_char[20] = "init";
                while(strcmp(objet_char, "a") != 0 &&
                        strcmp(objet_char, "b") != 0 && 
                        strcmp(objet_char, "p") != 0) {
                    puts("Choisissez un type d'objet\n");
                    puts("(a) pour avoir le stock et prix des pommes");
                    puts("(b) pour avoir le stock et prix des bananes");
                    puts("(p) pour avoir le stock et prix des poires");
                    scanf("%s", &objet_char);
                }
                afficherStockPrix(message, objet_char[0]);
            } else if(ret_flag == -1) {
                puts("Serveur occupe...");
            } 
        } else if(strcmp(rep, "0") == 0) {
            demandeQuitterMagasin(id_msg, &message, mon_num_clt);
        } else{
            puts("Entrez une option valide");
        }
    }

	return 0;
}
