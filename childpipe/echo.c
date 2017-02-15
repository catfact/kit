#include <stdio.h>

int main(int argc, char** argv) {
  int ch;
  while(1) {
	ch = getchar();
	printf("%d\n",ch);
	fflush(stdout);
  }

}
