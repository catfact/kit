#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

WINDOW* pad;
int rows, cols;
int pad_rows;
int pad_cur_line = 0;

void quit(void) {
  endwin();
}

//-- test content
char text[] =
#include "loremipsum.c"
  ;
char* pline = text;
int nline = 0;

void text_init(void) { 
  pline = strtok(text, "\n");
}

char* text_get_line(void) {
  pline = strtok(NULL, "\n");
  return pline;
}

//--- pad refresh control
void wrap_pad_cur_line(void) {
  int l = pad_cur_line;
  int max = pad_rows - 1;
  if(l < 0) { l = 0; }
  if(l > max) { l = max; }
  pad_cur_line = l;
}

void inc_pad_cur_line(void) {
  pad_cur_line++;
  wrap_pad_cur_line();
}

void dec_pad_cur_line(void) {
  pad_cur_line--;
  wrap_pad_cur_line();
}

void pad_refresh(void) {
  prefresh(pad, pad_cur_line, 0, 0, 0, rows-1, cols-1);
}

//---- main function

int main(int argc, char **argv) {
  
  int ch;
  if(argc > 1) {
	pad_rows = atoi(argv[1]);
	printf("creating pad with %d rows.\n", pad_rows);
  } else {
	pad_rows = 0;
	printf("no argument for row count; using terminal dimentions.\n");
  }

  printf("'i' inserts more text; 'p' and 'n' scroll lines.\n");
  printf("press enter to continue...\n");
  ch = getchar();

  initscr();
  atexit(quit);
  clear();
  cbreak();
  
  getmaxyx(stdscr, rows, cols);
  curs_set(0);

  if(pad_rows == 0) { pad_rows = rows; }
  pad = newpad (pad_rows, cols);
  // hm...
  scrollok(pad, TRUE);
  
  if(pad == NULL) {
	perror("pad allocation");
	exit(1);
  }
	
  mvwprintw(pad, 0, 0, "hello world \n");
  pad_refresh();

  text_init();

  clear();
  refresh();

  return (0);
}
