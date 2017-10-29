#include <assert.h>
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



struct ev_node {
  struct ev_node *next;
  struct ev_node *prev;
  int value;
};

struct ev_q {
  struct ev_node *head;
  struct ev_node *tail;
  int size;
};

void ev_q_init(struct ev_q *q) {
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
}

// add to tail
void q_push(struct ev_q *q, int val) {
  struct ev_node *evn = calloc(1, sizeof(struct ev_node));
  evn->value = val;
  if (q->size == 0) {
    printf("adding first element");
    insque(evn, NULL);
    q->head = evn;
  } else {
    insque(evn, q->tail);
  }
  q->tail = evn;
  q->size += 1;
}

int q_pop(struct ev_q *q) {
  struct ev_node *evn;
  evn = q->head;
  if(evn == NULL) {
    printf("no head! \n");
    return -1;
  }
  printf("q_pop evn: 0x%08x\n", evn);
  int value = evn->value;
  
  printf("updating head\n");
  q->head = evn->next;
  if(q->tail == evn) {
    printf("removing last entry\n");
    assert(q->size == 1);
    q->tail = NULL;
  }
  remque(evn);
  free(evn);
  q->size -= 1;
  return value;
}



int main() {
  struct ev_q q;
  ev_q_init(&q);
  
  for(int i=0; i<10; ++i) {
    printf("pushing value: %d\n", i);
    q_push(&q, i);
  }
  for(int i=0; i<10; ++i) {
    int val = q_pop(&q);
    printf("%d\n", val);
  }

}
