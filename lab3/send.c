#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;
  pkt p;
  int fd;
  int size;
  // int WINDOW_SIZE = 1000 / (sizeof(msg) * 8); 


  fd = open("test.txt", O_RDONLY);
  for (int i = 0; i < 40; i++) {
    lseek(fd, 0, SEEK_SET);
    memset(t.payload, 0, MAX_LEN);
    size = read(fd, p.payload, 1396);

    if (size < 0) {
      perror("Unable to read from file\n");
    } else {
      t.len = size;
      p.parity = 0;

      for (int i = 0; i < size; i++) {
        for (int j = 0; j < 8; j++) {
          p.parity ^= (1 << j) & p.payload[i];
        }
      }

      memcpy(t.payload, p.payload, MAX_LEN);

      printf("[%s] Sending payload: %s\n", argv[0], t.payload);

      send_message(&t);
      recv_message(&t);

      printf("[%s] Received response: %s\n", argv[0], t.payload);
    }
  }

  close(fd);

  return 0;
}
