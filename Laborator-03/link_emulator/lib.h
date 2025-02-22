#ifndef LIB
#define LIB

#define MAX_LEN 1400

typedef struct {
  int len;
  char payload[MAX_LEN];
} msg;


typedef struct {
    char payload[1396];
    int parity;
} pkt;

void init(char* remote,int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);
int send_message(const msg* m);
int recv_message(msg* r);

#endif
