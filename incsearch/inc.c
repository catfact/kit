#include <curses.h>
#include <stdio.h>
#include <string.h>

#define NWORDS 13
#define STRSIZE 64

char* dict[NWORDS] = {
  "arb", "arp", "aard", "dog", "cat", "bar", "butt", "naz", "nazar", "balthazar", "computer", "computerus", "lingus"
};

int nsrc;
int nres;

char src[NWORDS][STRSIZE];
char res[NWORDS][STRSIZE];
char input[STRSIZE];

void input_init(void) {
  for(int i=0; i<64; i++) { input[i] = '\0'; }
}

void search_init(void) {
  for(int i=0; i<NWORDS; i++) {
    strncpy(src[i], dict[i], STRSIZE);
    res[i][0] = '\0';
  }
  nsrc = NWORDS;
  nres = 0;
}

void search_perform(char ch, int pos) {
  nres = 0;
  for(int i=0; i<nsrc; i++) {
    if(src[i][pos] == ch) {
      strcpy(res[nres], src[i]);
      nres++;
    }
  }
  // copy results back to source
  // (FIXME: should be able to swap pointers instead if needed)
  for(int i=0; i<nres; i++) {
    strncpy(src[i], res[i], STRSIZE);
  }
  nsrc = nres;
}

void search_print(void) {
  int x = 0;
  move(2, 0);
  clrtoeol();
  for(int i=0; i<nres; i++) {
    mvprintw(2, x, "%s ", res[i]);
    x += strlen(res[i]) + 1;
  }
}

int main(void) {

  int ch;
  int pos = 0;
  
  search_init();
  input_init();
  
  initscr();
  clear();
  noecho();
  cbreak(); // disable line buffering

  
  
  while(1) {
    
    ch = getch();

    if(ch == 32) {
      // spacebar: reset
      pos = 0;
      search_init();
      input_init();
      clear();

      /* } else if(ch == 0x7f) { */
      /* // FIXME: also need to rebuild search results */
      /*   // backspace: delete */
      /*   if(pos > 0) { */
      /* 	input[--pos] = '\0'; */
      /* 	mvprintw(0, 0, "%s ", input); */
      /* 	search_perform((char)ch, pos); */
      /*   } */
    } else {
      
      if(ch == -1 || pos >= 64) { 
	// no input
	// buffer is maxed
      } else {
	
	// search for input char at current pos
	input[pos] = (char)ch;
	mvprintw(0, 0, "%s ", input);
   
	search_perform((char)ch, pos);
	pos++;

	search_print();
	
      }    
    }
    mvprintw(32, 0, "%04x", ch);
  }
}
