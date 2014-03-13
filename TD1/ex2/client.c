#define REQ_SRV 1

#define DEM_NUM_CLT 1
#define DEM_CREATION_PANIER 2

#define REP_NUM_CLT 2


typedef struct
{
	long type,
	int req;
	int num_clt;
} msg;

void main()
{
	key_t cle;
	int id_msg;
	int mon_num_clt;
	msg message;
	message.type = REQ_SRV;
	message.req = DEM_NUM_CLT;
	const int MSG_SIZE = sizeof(msg) - sizeof(long);
	cle = ftok("sr03p012", 0);
	id_msg = msgget(cle, 0);

	msgsnd(id_msg, (void*)&message, MSG_SIZE, 0);
	msgrcv(id_msg, (void*)&message, MSG_SIZE, REP_NUM_CLT);
	mon_num_clt = message.num_clt;

	message.type = REQ_SRV;
	message.req = DEM_CREATION_PANIER;
	message.num_clt = mon_num_clt;
	msgsnd(id_msg, (void*)&message, MSG_SIZE, 0);
	msgrcv(id_msg, (void*)&message, MSG_SIZE, mon_num_clt);
}
