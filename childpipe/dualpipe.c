#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

//int run_child(const char* command, const char* msg) {
int run_child(void) {
  int pipe_in[2];
  int pipe_out[2];
  
  if (pipe(pipe_in) < 0) {
	perror("allocating pipe for child input redirect");
    return -1;
  }
  if (pipe(pipe_out) < 0) {
    close(pipe_in[PIPE_READ]);
    close(pipe_in[PIPE_WRITE]); 
    perror("allocating pipe for child output redirect");
    return -1;
  }

  pid_t child = fork();
  if (0 == child) {
    // child continues here

    // redirect stdin
    if (dup2(pipe_in[PIPE_READ], STDIN_FILENO) == -1) {
      perror("redirecting stdin");
      return -1;
    }

    // redirect stdout
    if (dup2(pipe_out[PIPE_WRITE], STDOUT_FILENO) == -1) {
      perror("redirecting stdout");
      return -1;
    }

    // redirect stderr
    if (dup2(pipe_out[PIPE_WRITE], STDERR_FILENO) == -1) {
      perror("redirecting stderr");
      return -1;
    }

    close(pipe_in[PIPE_READ]);
    close(pipe_in[PIPE_WRITE]);
    close(pipe_out[PIPE_READ]);
    close(pipe_out[PIPE_WRITE]); 

    // run child process image
	char* const argv[3] = { "/usr/bin/bc", "-i", NULL };
	int res = execv(argv[0], argv);

    // if we get here at all, an error occurred; we are in the child process
    perror("exec of the child process");
	return res;
	
  } else if (child > 0) {
    // parent continues here

    // close unused file descriptors, these are for child only
    close(pipe_in[PIPE_READ]);
    close(pipe_out[PIPE_WRITE]); 

	//	usleep(1000000);

	int quit = 0;
	char* buf;
	size_t len, dum;
	char ch;
	
	while(!quit) { 
	  getline(&buf, &dum, stdin);
	  len = strlen(buf);
	  printf("got line of length %d: %s", len, buf);
	  if(len == 2) {
		if(buf[0] = 'q') {
		  quit = 1;
		  continue;
		}
	  } else if (len > 0) {
		printf("writing to child \n");
		write(pipe_in[PIPE_WRITE], buf, len);
	  }
	  
	  while (read(pipe_out[PIPE_READ], &ch, 1) == 1) {
		//	  	printf("reading from child \n");
	  	write(STDOUT_FILENO, &ch, 1);
	  }

	  
	}
	
	  //	printf("writing to child stdin: %s\n", msg);
	/*   if (NULL != msg) { */
	/* 	write(pipe_in[PIPE_WRITE], msg, strlen(msg)); */
	/*   } */

	/*   //	printf("reading child stdout: \n"); */
	/*   char ch; */
	/*   while (read(pipe_out[PIPE_READ], &ch, 1) == 1) { */
	/* 	write(STDOUT_FILENO, &ch, 1); */
	/*   } */
	/* } */

	// .. keep writing to child..
	
    close(pipe_in[PIPE_WRITE]);
    close(pipe_out[PIPE_READ]);
  } else {
    // failed to create child
    close(pipe_in[PIPE_READ]);
    close(pipe_in[PIPE_WRITE]);
    close(pipe_out[PIPE_READ]);
    close(pipe_out[PIPE_WRITE]);
  }
  return child;
}

int main(int argc, char** argv) {
  /* char* msg = ""; */
  /* if(argc < 2) { */
  /* 	printf("usage: %s command <message>\n", argv[0]); */
  /* 	return 1; */
  /* } */

  /* if(argc > 2) { */
  /* 	msg = argv[2]; */
  /* } */
  
  /* printf("executing command: %s %s\n", argv[1], msg); */
  /* run_child(argv[1], "2+2\n"); */
  run_child();
}
