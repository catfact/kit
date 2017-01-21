#pragma once

#include <semaphore.h>

/*
  shared memory
  open with shm_open(name)
  use with mmap()
  
  unnamed semaphore provides sync
 */

// shared memory name
#define SHM_NAME "/shmem_test"
// shared memory size, in chars
#define SHM_SIZE (0x55)
//#define SHM_SIZE (1024 * 128)

struct shm_buf {
  sem_t sem_wr;
  sem_t sem_rd;
  char buf[SHM_SIZE];
};
