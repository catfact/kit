#include <stdio.h>
#include <ncurses.h>

const char filler[] =
#include "filler_text.c"
  ;

int main(void) {
  WINDOW* win;
  int c;
  char quit = 0;

  initscr();
  clear();
  cbreak();
  noecho();

  win = newwin(20, 20, 4, 4);
  scrollok(win, TRUE);
  wsetscrreg(win, 2, 18);
  idlok(win, TRUE);
  
  mvwaddstr(win, 0, 0, filler);
  waddstr(win, "\n\n");
  wrefresh(win);
  
  while(!quit) {
    c = getch();
    if(c == 'q') { quit = 1; }

	// f1, f2 scroll
	else if(c == 80) {
	  wscrl(win, 1);
	}
	else if (c == 81) {
	  wscrl(win, -1);
	}
	//	waddch(win, (char)c);
	else {
	  // wprintw(win, "%c (%d) ", (char)c, c);
	}
	wrefresh(win);
  }
  
  endwin();

}
