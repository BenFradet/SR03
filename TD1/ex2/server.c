#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define REQ_SRV 1
#define DEM_NUM_CLT 1
#define DEM_CREATION_PANIER 2

#define REP_NUM_CLT 2

void main()
{
	int id_msg;
	key_t cle;
	msg message;
	const int MSG_SIZE = sizeof(msg) - sizeof(long);
	cle = ftok("sr03p012", 0);
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
			msgrcv(id_msg, (void*)&message, MSG_SIZE, REQ_SRV, 0);
			switch(message.req)
			{
				case DEM_NUM_CLT:
					message.type = REP_NUM_CLT;
					message.num_clt = 
					msgsnd(id_msg, (void*)&message, MSG_SIZE, 0);
					break;
				case DEM_CREATION_PANIER:
					message.type = message.num_clt;
					msgsnd(id_msg, (void*)&message, MSG_SIZE, 0);
					break;
			}
		}
	}
}
