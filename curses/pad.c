#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

void quit(void) {
  endwin();
}

int main(int argc, char **argv)
{
  int rows, cols;
  WINDOW* pad;
  
  initscr();
  atexit(quit);
  clear();
  cbreak();
  
  getmaxyx(stdscr, rows, cols);
  curs_set(0);
  
  pad = newpad (rows, cols);
    if(pad == NULL) {
	perror("pad allocation");
	exit(1);
  }
	
  wprintw(pad, "hello world");
  prefresh(pad, 0, 0, 0, 0, rows, cols);
  
  int ch;
  while((ch = wgetch(pad)) != 'q') {
    ;;
  }
  
  delwin(pad);
  clear();
  refresh();

  return (0);
}
