#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>

#include <unistd.h>

#define SOCK_NAME "./test_sock"

#define CLIENT_BUFSIZE 256

// child process to run the target executable
pid_t client_pid;
// child process to handle socket connections
pid_t connection_pid;

// utility
void init_sock(struct sockaddr_un *addr) {
  memset(addr, 0, sizeof(struct sockaddr_un));
  addr->sun_family = AF_UNIX;
  sprintf(addr->sun_path, SOCK_NAME);
}

// client: passes stdin to the server,
// echos responses to stdout
void run_client(void) {  
  struct sockaddr_un addr;
  int sock;
  char rxbuf[CLIENT_BUFSIZE];
  char txbuf[CLIENT_BUFSIZE];
  int res;
  int nb;
  int ch;
  int newline = 0;
  
  sock = socket(PF_UNIX, SOCK_STREAM, 0);
  if(sock < 0) {
	printf("socket() failed\n");
	return;
  }

  init_sock(&addr);
  // wait a bit for the server
  usleep(100000);
  
  res = connect(sock,
				(struct sockaddr*)&addr,
				sizeof(struct sockaddr_un));
  if(res != 0) { printf("connect() failed \n"); return; }

  while(1) {
	nb = 0;
	txbuf[0] = '\0';
	newline = 0;
	printf("waiting for input\n");
	// read from stdin
	//	getline(&line, &linesize, stdin);
	while(newline != 1) {
	  if(nb < (CLIENT_BUFSIZE-1)) {
		ch = getchar();
		txbuf[nb++] = (char)ch;
		if(ch == 10) { newline = 1; }
	  } else {
		newline = 1;
	  }
	}
	txbuf[nb] = '\0';
	
	// send to server
	//	printf("sending %dB to server: \n%s\n", nb, txbuf);
	printf("sending %dB to server\n", nb);
	write(sock, txbuf, nb);
	// print server response
	nb = read(sock, rxbuf, CLIENT_BUFSIZE);
	//	printf("client rx: %d bytes\n", nb);
	printf("client rx: %s \n", rxbuf);
	//	write(sock, "ack", 4);
  }
  //  free(line);
}

// handle connection on server
void handle_connection(int sock) {
  //  char buf[BUFSIZE];
  //  int nb;
  //  int count = 0;

  dup2( sock, STDOUT_FILENO );  /* duplicate socket on stdout */
  dup2( sock, STDERR_FILENO );  /* duplicate socket on stderr too */
  // hm...
  dup2( sock, STDIN_FILENO);
  
  close( sock );  /* can close the original after it's duplicated */
  
  /* #if 0 */
  /* // write/read forever */
  /* while(1) { */
  /* 	printf("%d\n", count++); */
  /* 	//	nb = read(sock, buf, 256); */
  /* 	//	printf("server rx: %d bytes: %s\n", nb, buf); */
  /* 	usleep(100000); */
  /* } */
  /* #else */
  // ok.. try running an executable with the redirected stdout... 
  	char* const argv[] = { "./echo", NULL };
	int res = execv(argv[0], argv);
	// exec only returns on error
	printf("exec() failed \n");

	//  #endif
}

// run server: 
void run_server(void) {
  struct sockaddr_un addr;
  int sock, connect;
  int res;
  socklen_t addr_len;
  
  sock = socket(PF_UNIX, SOCK_STREAM, 0);
  if(sock <0) { printf("socket() failed \n"); return; }
  
  unlink(SOCK_NAME);
  init_sock(&addr);

  res = bind(sock,
			 (struct sockaddr *) &addr, 
			 sizeof(struct sockaddr_un));
  if(res != 0) { printf("bind() failed \n"); return; }

  res = listen(sock, 5);
  if(res!= 0) { printf("listen() failed\n"); return; }

  while((connect = accept(sock,
						  (struct sockaddr *) &addr,
						  &addr_len)) > -1) {
	connection_pid = fork();
	if(connection_pid == 0) {
	  handle_connection(connect);
	  return;
	} else {
	  close(connect);
	}
  }
}


int main(int argc, char** argv) {
  
  client_pid = fork();
   if(client_pid < 0) { 
	 printf("fork() returned an error\n");
	 return 1;
   } 
  
  if(client_pid == 0) {
	printf("main(): run_client()\n");
	run_server();
  } else {
	printf("main(): run_server()\n");
	run_client();
  }

  // wait for all child processes to finish
  wait(0);

}
