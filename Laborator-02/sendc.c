#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"
#include "my_msg.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc,char** argv) {
  init(HOST,PORT);
  msg msg1;
  my_msg msg2;
  int fd;
  int length;

  char *filename = "test1";
  if ((fd = open(filename, O_RDWR)) < 0) {
    perror("file does not exist");
  }

  sprintf(msg2.payload, "%s", filename);
  msg2.type = 0;
  memcpy(msg1.payload, &msg2, sizeof(int) + strlen(filename) + 1);
  msg1.len = sizeof(int) + strlen(filename) + 1;
  send_message(&msg1);

  if (recv_message(&msg1) < 0) {
    perror("receive error");
  } else {
    printf("[%s] Got reply with payload: %s\n", argv[0], msg1.payload);
  }

  if ((length = lseek(fd, 0, SEEK_END)) < 0) {
    perror("cannot open file");
  }
  lseek(fd, 0, SEEK_SET);

  memcpy(msg2.payload, &length, sizeof(int));
  msg2.type = 1;
  memcpy(msg1.payload, &msg2, sizeof(int) * 2);
  msg1.len = sizeof(int) * 2;
  send_message(&msg1);

  if (recv_message(&msg1) < 0) {
          perror("receive error");
  } else {
     printf("[%s] Got reply with payload: %s\n", argv[0], msg1.payload);
  }

  int x = 0;

  while ((x = read(fd, msg2.payload, 3)) > 0) {
    msg2.type = 2;
    memcpy(msg1.payload, &msg2, sizeof(int) + x);
    msg1.len = sizeof(int) + x;

    send_message(&msg1);

    if (recv_message(&msg1) < 0){
      perror("receive error");
    } else {
      printf("GOT ACK\n");
    }
  }
  printf("%d", x);

  return 0;
}
