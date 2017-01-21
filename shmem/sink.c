#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"

int fd;
struct shm_buf *shm;

void init(void) {
  printf("sink: open shared memory and map \r\n");
  fd = shm_open(SHM_NAME, O_RDWR, 0);

  printf("( shm file descriptor: %d ) \r\n", fd);

  
  shm = mmap(NULL, sizeof(struct shm_buf),
			 PROT_READ | PROT_WRITE, MAP_SHARED , fd , 0);
}

void deinit(void) {
  // nothing to do
}

int main(int argc, char *argv[]) {
  init();
  
  printf("sink: wait for write semaphore \r\n");
  sem_wait(&shm->sem_wr);

  printf("sink: do work \r\n");
  for(int i=0; i<SHM_SIZE; i++) {
	printf("%04x \r\n", shm->buf[i]);
  }
  
  // signal semaphore
  printf("sink: post read semphore \r\n");
  sem_post(&shm->sem_rd);

  deinit();
}
