#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"

int fd;
struct shm_buf *shm;


void init(void) {
  
  // delete the shared memory if it exists
  shm_unlink(SHM_NAME);
  
  printf("source: open shared memory, resize and map \r\n");
  fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
  ftruncate(fd, sizeof(struct shm_buf));

  printf("( shm file descriptor: %d ) \r\n", fd);
  
  shm = mmap(NULL, sizeof(struct shm_buf),
			 PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0);

  printf("source: init semaphores \r\n");
  sem_init(&shm->sem_wr, 1, 0);
  sem_init(&shm->sem_rd, 1, 0);

}

void deinit(void) {
  printf("source: finished, unlinking shared memory \r\n");
  shm_unlink(SHM_NAME);
}

int main(int argc, char *argv[]) {

  init();
  
  printf("source: do work \r\n");
  int i=0;
  while(i<SHM_SIZE) {
	shm->buf[i] = i;
	i = (i + 1) % 0x5555;
  }

  printf("source: post write semaphore \r\n");
  sem_post(&shm->sem_wr);
  
  printf("source: wait for read semaphore \r\n");
  sem_wait(&shm->sem_rd);

  deinit();
  
}
