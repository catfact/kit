#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  char* line;
  size_t linesize;
  int ch;
  while(1) {
	#if 1
	getline(&line, &linesize, stdin);
	printf("%s", line, strlen(line));
	#else
	ch = getchar();
	printf("%d\n", ch);
	#endif
	fflush(stdout);
  }
  
  free(line);
}
