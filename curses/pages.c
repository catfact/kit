#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "page.h"

#define CATFACT 0xc47f4c75
#define NUM_PAGES 4

static char text[] =
#include "loremipsum.c"
  ;
static int textlen;

struct page* pages[NUM_PAGES];
struct page* page_cur;

int page_nrows = 512;
int page_ncols = 80;

WINDOW* win_stat;

void quit(void) {
  
  for(int i=0; i<NUM_PAGES; i++) {
	page_deinit(pages[i]);
  }
  
  endwin();
}

void page_junk(struct page* p) {
  int off = rand() % (textlen/2);
  int len = rand() % (textlen/4) + 8;
  char* str = calloc(len, sizeof(char));
  snprintf(str, len, "%s", text + off);
  wprintw(p->pad, "%s\n\n", str);
  page_refresh(p);
  free(str);
  doupdate();
}

void select_page(int i) {
  page_cur = pages[i];
  top_panel(page_cur->panel);
  mvwprintw(win_stat, 0, 0, " --- %d ---        ", i);
  wnoutrefresh(win_stat);
  page_refresh(page_cur);
  doupdate();
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  
  int ch;

  // set up ncurses
  initscr();
  atexit(quit);
  clear();
  cbreak();

  // set up junk text generator
  srand(CATFACT);
  textlen = strlen(text);

  // set up pages 
  int mrows, mcols;
  getmaxyx(stdscr, mrows, mcols);
  page_ncols = mcols;

  for(int i=0; i<NUM_PAGES; i++) {
	pages[i] = page_init(page_nrows, page_ncols, 0, 1, mcols-1, mrows-2);
  }

  // status bar
  win_stat = newwin(1, mcols-1, 0, 0);  
  wbkgd(win_stat, A_STANDOUT);

  select_page(0);
  
  // loop ui
  while((ch = wgetch(page_cur->pad)) != 'q') {
    switch(ch) {
	case '1':
	  select_page(0);
	  break;	  
	case '2':
	  select_page(1);
	  break;
	case '3':
	  select_page(2);
	  break;
	case 'i':
	  page_junk(page_cur);
	  break;
	case 'n':
	  page_scroll_down(page_cur);
	  doupdate();
	  break;
	case 'p':
	  page_scroll_up(page_cur);
	  doupdate();
	  break;
	}
  }
 
}
