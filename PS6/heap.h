#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>



#define MAXHEAP 4

bool debugflag;


// from Van Wyk, chapter 10, page 233++
typedef struct heapslot heapslot;
struct heapslot {
  int key;
  struct room * value;
} ;

struct heap {
    int size;           /* size of the pointer table */
    int n;              /* number of elements stored */
    struct heapslot ** heap;
};

typedef struct heap *Heap;

bool empty(Heap);

void demand(int cond, char * msg);

void printHeap(Heap);

struct room * findmin(Heap);

Heap initheap();

void swap(heapslot *s1, heapslot *s2);

void insert( Heap h, int key, struct room * r);

void deletemin( Heap h);

void destroyHeap( Heap h);

