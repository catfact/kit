#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void write_loop(void) {
  int count;
  while(1) {
	printf("%d\n", count++);
	fflush(stdout);
	usleep(50000);
  }
}

int main(int argc, char** argv) {

  write_loop();  
}
