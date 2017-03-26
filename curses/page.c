#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "page.h"

static void set_row(struct page* p, int r) {
  if(r >= p->rows) { r = p->rows - 1; }
  if(r < 0) { r = 0; }
  p->row = r;
}

static void set_col(struct page* p, int c) {
  if(c >= p->cols) { c = p->cols - 1; }
  if(c < 0) { c = 0; }
  p->col = c;
}

struct page* page_init(int rows, int cols, int x, int y, int w, int h) {
  struct page* p = calloc(1, sizeof(struct page));
  p->rows = rows;
  p->cols = cols;
  p->pad = newpad(rows, cols);
  scrollok(p->pad, TRUE);
  p->panel = new_panel(p->pad);
  p->x = x;
  p->y = y;
  p->w = w;
  p->h = h;
  
  return p;
}

void page_deinit(struct page* p) {
  if(p == NULL) { return; }
  delwin(p->pad);
  free(p);
}

void page_append(struct page* p, const char* txt) {
  wprintw(p->pad, txt);
}

void page_refresh(struct page* p) {
  pnoutrefresh(p->pad, p->row, p->col, p->y, p->x, p->h, p->w);
}

void page_scroll(struct page* p, int row, int col) {
  set_row(p, row);
  set_col(p, col);
  page_refresh(p);
}

void page_scroll_up(struct page* p) {
  set_row(p, p->row - 1);
  page_refresh(p);
}

void page_scroll_down(struct page* p) {
  set_row(p, p->row + 1);
  page_refresh(p);
}

void page_scroll_forward(struct page* p) {
  set_col(p, p->col + 1);
  page_refresh(p);
}

void page_scroll_back(struct page* p) {
  set_col(p, p->col - 1);
  page_refresh(p);
}
