#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>

#include "glue.h"


void* repl_run(void* p) {
  
  l_init();

  size_t len, dum;
  char* rxbuf;
  int quit = 0;
  
  printf("repl says hello \n");
  
  while(!quit) {
	printf("\n ... "); fflush(stdout);
	getline(&rxbuf, &dum, stdin);
	len = strlen(rxbuf);
	if(len == 2) {
	  if(rxbuf[0] == 'q') { 
		quit = 1;
	  }
	}
	else if (len > 0) {
	  l_run_code(rxbuf);
	}
  }
  printf("repl says bye \r\n");
  free(rxbuf);
  
}



int main(int argc, char** argv) {
  int s;
  pthread_attr_t attr;
  pthread_t pid;
  void* res;
  
  s = pthread_attr_init(&attr);
  if (s != 0) { printf("error: %d\n", s);  }

  printf("creating the repl thread... \n");
  s = pthread_create(&pid, &attr, &repl_run, NULL);
  if (s != 0) { printf("error: %d\n", s);  }
  
  pthread_join(pid, NULL);
 
  return 0;
}
