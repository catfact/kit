#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>


pthread_t signaller_pid;
sem_t sem;

void* signaller_run(void* p) {
  usleep(1000000 * 4);
  // signal semaphore
  sem_post(&sem);
}


int main(int argc, char** argv) {
  pthread_attr_t attr;
  struct timespec ts;
  int res;

  // set up the semaphore
  printf(" set up the semaphore\n");
  if (sem_init(&sem, 0, 0) == -1) {
	printf("sem_init() error: %d\n", res);
	return 1;
  }

  // set up the timeout
  printf(" set up the timeout\n");
  res = clock_gettime(CLOCK_REALTIME, &ts);
  if(res != 0) {
	printf("clock_gettime() error: %d\n", res);
	return 2;
  }
  ts.tv_sec += 10;

  // set signalling thread attributes
  res = pthread_attr_init(&attr);
  if(res != 0) {
	printf("pthread_attr_init() error: %d\n", res);
	return 3;
  }
  
  // start the signalling thread
  printf("start the signalling thread\n");
  res = pthread_create(&signaller_pid, &attr, &signaller_run, NULL);
  if(res != 0) {
	printf("pthread_create() error: %d\n", res);
	return 4;
  }

  // wait on semaphore
  printf(" wait on semaphore\n");
  while ((res = sem_timedwait(&sem, &ts)) == -1 && errno == EINTR) {
	continue;       /* Restart if interrupted by handler */
  }

  if(res == -1) {
	if(errno == ETIMEDOUT) {
	  printf("sem_timedwait() timed out\n");
	}
	else {
	  printf("sem_timedwait error");
	}
  } else {
	printf("sem_timedwait() succeeded\n");
  }

  printf("hi! \n");

  
} 
