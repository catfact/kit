#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

const size_t CMD_CAPTURE_BYTES = 8192 * 8;
const size_t CMD_LINE_BYTES = 1024;

const char *url = "ipc:///tmp/test.ipc";

// allocates and returns a string buffer
int sidecar_run_cmd(char **result, const char *cmd, size_t *sz) {
  const size_t CMD_LINE_CHARS = CMD_LINE_BYTES / sizeof(char);
  FILE *f = popen((char *)cmd, "r");
  if (f == NULL) {
    fprintf(stderr, "popen() failed\n");
    return -1;
  }
  char *buf = (char *)malloc(CMD_CAPTURE_BYTES);
  buf[0] = '\0';
  size_t nb = fread(buf, 1, CMD_CAPTURE_BYTES-1, f);
  printf("captured %d bytes\n", nb);
  buf[nb] = '\0';
  buf = realloc(buf, nb);
  *result = buf;
  *sz = nb;
  return 0;
}

int run_sidecar() {
  int fd = nn_socket(AF_SP, NN_REP);
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
    size_t sz;
    printf("sidecar received %d bytes\n", nb);
    printf("running cmd: %s\n", buf);
    char *result;
    sidecar_run_cmd(&result, buf, &sz);
    nn_send(fd, result, sz, 0);
    free(result);
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

  int res = nn_connect(fd, url);
  if (res < 0) {
    fprintf(stderr, "nn_connect (main): %s\n", nn_strerror(nn_errno()));
    return (-1);
  }

  for (;;) {
    printf("> ");
    sz = getline(&line, &len, stdin);
    printf("received input: %s\n", line);
    sz = nn_send(fd, line, len, 0);
    if (sz < 0) {
      fprintf(stderr, "nn_send (main): %s\n", nn_strerror(nn_errno()));
      return -1;
    }
    free(line);
    sz = nn_recv(fd, &buf, NN_MSG, 0);
    if (sz < 0) {
      fprintf(stderr, "nn_recv (main): %s\n", nn_strerror(nn_errno()));
      return -1;
    }
    printf("main rx:\n%s\n", buf);
    nn_freemsg(buf);
  }
}

int main(int argc, char **argv) {
  if (fork() == 0) {
    // parent process
    if (fork() != 0) {
      // second fork
      sleep(1);
      fprintf(stderr, "running main...\n");
      run_main();
    } else {
      // parent process
      // nothing to do..
      for (;;)
        sleep(1);
    }
  } else {
    // first fork
    fprintf(stderr, "running sidecar...\n");
    run_sidecar();
    return 0;
  }
}
