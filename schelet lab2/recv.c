#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"
#include "msg.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc, char** argv) {
  msg r, t;
  init(HOST, PORT);
  int l;
  int fd;
  char filename[100];

  if (recv_message(&r) < 0) {
    perror("receive error");
    return -1;
  }

  printf("filename: %s\n", r.payload + 4);

  sprintf(t.payload, "ACK(%s)", r.payload);
  t.len = strlen(t.payload + 1);
  send_message(&t);

  strcpy(filename, r.payload + sizeof(int));
  strcat(filename, ".out");
  if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0655)) < 0) {
    perror("file not found");
  }

  if (recv_message(&r) < 0) {
    perror("error size");
    return -1;
  }

  memcpy(&l, r.payload + sizeof(int), sizeof(int));
  printf("%d\n", l);

  sprintf(t.payload, "ACK(%s)", r.payload);
  t.len = strlen(t.payload + 1);
  send_message(&t);

  while (l > 0) {
    char buff[100] = "";
    if (recv_message(&r) < 0) {
      perror("error size");
      return -1;
    }

    memcpy(buff, r.payload + sizeof(int), r.len - sizeof(int));

    write(fd, buff, strlen(buff));
    l -= strlen(buff);

    sprintf(t.payload, "ACK(%s)", r.payload);
    t.len = strlen(t.payload + 1);
    send_message(&t);
  }

  return 0;
}
