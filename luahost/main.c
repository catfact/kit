#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glue.h"


int main(int argc, char** argv) {

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  l_init();

  int quit = 0;
  while ( !quit) {
	printf("> ");
	read = getline(&line, &len, stdin);
	//	printf("got line: \"%s\"\n(length %d)\n", line, strlen(line));
	// zap the newline..
	line[strlen(line)-1] = '\0';
	if( (strcmp(line,"quit") == 0)
		|| (strcmp(line,"exit") == 0)
		|| (strcmp(line,"q") == 0)
		|| (strcmp(line,"bye") == 0)
		) {
	  quit = 1;
	  printf("bye \r\n\r\n");
	}
	else {
		l_run_code(line);
		printf("%s\n", l_get_stack_buf());
	  }
  }
  free(line);
}
