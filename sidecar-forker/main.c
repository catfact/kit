#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

const size_t CMD_CAPTURE_BYTES = 8192 * 8;
const size_t CMD_LINE_BYTES = 1024;

const char *url = "ipc:///tmp/test.ipc";

// allocates string buffer; caller takes ownership
char *sidecar_run_cmd(const char *cmd, int *sz) {
  const size_t CMD_LINE_CHARS = CMD_LINE_BYTES / sizeof(char);

  FILE *f = popen((char *)cmd, "r");
  if (f == NULL) {
    fprintf(stderr, "system_cmd: command failed\n");
    return NULL;
  }

  char *buf = (char *)malloc(CMD_CAPTURE_BYTES);
  buf[0] = '\0';
  size_t nb = fread(buf, CMD_CAPTURE_BYTES - 1, 1, f);
  buf[nb] = '\0';
  buf = realloc(buf, nb);
  *sz = nb;
  return buf;
}

int run_sidecar() {
  int fd = nn_socket(AF_SP, NN_REQ);
  if (fd < 0) {
    fprintf(stderr, "nn_socket: %s\n", nn_strerror(nn_errno()));
    return (-1);
  }

  if (nn_bind(fd, url) < 0) {
    fprintf(stderr, "nn_bind: %s\n", nn_strerror(nn_errno()));
    nn_close(fd);
    return (-1);
  }

  int nb;
  for (;;) {
    char *buf = NULL;
    nb = nn_recv(fd, &buf, NN_MSG, 0);
    if (nb < 0) { 
        fprintf(stderr, "nn_recv: %s\n", nn_strerror(nn_errno()));
        return -1;
    }
    int nbres;
    printf("sidecar received %d bytes\n", nb);
    printf("running cmd: %s\n", buf);
    //char *result = sidecar_run_cmd(buf, &nbres);
    //nn_send(fd, result, nbres, 0);
    //free(result);
    nn_freemsg(buf);
  }
  return 0;
}

int run_main() {
  char *line;
  char *buf;
  size_t len;
  size_t sz;

  int fd = nn_socket(AF_SP, NN_REQ);
  if (fd < 0) {
    fprintf(stderr, "nn_socket (main): %s\n", nn_strerror(nn_errno()));
    return (-1);
  }
  
  
  int res = nn_connect(AF_SP, url);
  if (res < 0) {
    fprintf(stderr, "nn_connect (main): %s\n", nn_strerror(nn_errno()));
    return (-1);
  }
  

  for (;;) {
    sz = getline(&line, &len, stdin);
    nn_send(fd, line, len, 0);
    if (sz < 0) { 
        fprintf(stderr, "nn_send (main): %s\n", nn_strerror(nn_errno()));
        return -1;
    }
    free(line);
    sz = nn_recv (fd, &buf, NN_MSG, 0);
    if (sz < 0) { 
        fprintf(stderr, "nn_recv (main): %s\n", nn_strerror(nn_errno()));
        return -1;
    }
    printf("rx:\n%s\n", buf);
    nn_freemsg(buf);
  }
}

int main(int argc, char **argv) {
  if (fork() != 0) {
    // parent process
    if (fork() != 0) {
        printf("running sidecar...\n");
      // second fork: sidecar
      run_sidecar();
    } else {
      // parent process
      // nothing to do..
      for (;;)
        ;
    }
  } else {
    // first fork: main
    printf("running main...\n");
    run_main();
    return 0;
  }
}
