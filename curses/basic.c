#include <stdio.h>
#include <ncurses.h>

int main(void) {
  WINDOW* winA;
  WINDOW* winB;
  char ab = 1;
  char quit = 0;
  int c = 0;
  int i = 0;

  int w = 10;
  int h = 10;
  int x = 4;
  int y = 4;
  
  initscr();
  clear();
  cbreak();
  noecho();
  //  raw();
  //  timeout(0);

  WINDOW* winA_frame = newwin(12, 12, 3, 3);
  box(winA_frame, 0, 0);
  wrefresh(winA_frame);
  
  winA = newwin(10, 10, 4, 4);
  scrollok(winA, TRUE);
  box(winA, 0, 0);
  // ?? refreshing here destroys the box? odd
  wrefresh(winA);

  winB = newwin(10, 10, 14, 14);
  //  box(winB, 2, 2);
  scrollok(winB, TRUE);

  
  while(!quit) {
    c = getch();
    if(c == 'q') { quit = 1; }
	if(ab) { 
	  waddch(winA, (char)c);
	  wrefresh(winA);
	  ab = 0;
	} else {
	  waddch(winB, (char)c);
	  wrefresh(winB);
	  ab = 1;
	}
  }

  endwin();
  //  exit(0);
}
