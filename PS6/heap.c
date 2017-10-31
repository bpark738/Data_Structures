// This file name is heap.c and it implements the functions
// outlined in heap.h
// Programmer: Briton Park

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "/c/cs223/hw6/heap.h"

#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)

// from Van Wyk, chapter 10, page 233++
// typedef struct heapslot heapslot;
// struct heapslot {
//   int key;
//   struct room * value;
// } ;

// struct heap {
//     int size;           /* size of the pointer table */
//     int n;              /* number of elements stored */
//     struct heapslot ** heap;
// };

// #define MAXHEAP 4

// bool debugflag;

//typedef struct heap *Heap;

void internaldestroyHeap( Heap h){
	for( int i = 0; i < h->size; i++){
		free(h->heap[i]);
	}
	free(h->heap);
	free(h);
}

Heap internalHeapCreate(int size){
	Heap myHeap = malloc(sizeof(struct heap));
	myHeap->size = size;
	myHeap->n = 0;
	myHeap->heap = malloc(sizeof(struct heapslot*)*myHeap->size);
	for(int i = 0; i < myHeap->size; i++){
		myHeap->heap[i] = malloc(sizeof(struct heapslot));
	}
	return myHeap;
}

void growHeap(Heap h){
	Heap h2; 
	struct heap swap;

	h2 = internalHeapCreate(h->size*GROWTH_FACTOR);

    for(int i = 0; i < h->n; i++) {
        insert(h2, h->heap[i]->key,h->heap[i]->value);
    }

    swap = *h;
    *h = *h2;
    *h2 = swap;
    internaldestroyHeap(h2);
}

bool empty(Heap h){
	if(h!=0){
		return h->n == 0;
	}
	return 0;
}

void demand(int cond, char * msg){
	if(cond){
		printf("%s\n", msg);
		return;
	}
}

void printHeap(Heap h){
	for(int i = 0 ; i < h->n; i++){
		printf("key: %d\n", h->heap[i]->key);
	}
}

struct room * findmin(Heap h){
	if(h!= 0){
		return h->heap[0]->value;
	}
	return 0;
}

Heap initheap(){
	Heap myHeap = malloc(sizeof(struct heap));
	myHeap->size = MAXHEAP;
	myHeap->n = 0;
	myHeap->heap = malloc(sizeof(struct heapslot*)*myHeap->size);
	for(int i = 0; i < myHeap->size; i++){
		myHeap->heap[i] = malloc(sizeof(struct heapslot));
	}
	return myHeap;
}

void swap(heapslot *s1, heapslot *s2){
	heapslot temp;
	temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

void insert( Heap h, int key, struct room * r){
	if(h != 0){
		int cur;
		int parent;
		cur = h->n;
		h->n++;
		h->heap[cur]->key = key;
		h->heap[cur]->value = r;
		parent = cur/2;
		while(h->heap[parent]->key > h->heap[cur]->key ){
			swap(h->heap[parent], h->heap[cur]);
			cur = parent;
			parent = cur/2;
		}

		if(h->n >= h->size * MAX_LOAD_FACTOR) {
        	growHeap(h);
    	}
	}
}

void deletemin(Heap h){
	if(h != 0){
		int cur;
		int child;
		*h->heap[0] = *h->heap[h->n-1];
		h->n--;
		cur = 0;
		child = 1;

		while(child<= h->n-1){
			if(child < h->n-1 && h->heap[child+1]->key < h->heap[child]->key){
				child++;
			}
			if(h->heap[cur]->key > h->heap[child]->key){
				swap(h->heap[cur], h->heap[child]);
				cur = child;
				child = 2*cur+1;
			}
			else{
				break;
			}
		}
	}
}

void destroyHeap( Heap h){
	if(h!=0){
		for( int i = 0; i < h->size; i++){
			if(i < h->n){ // only delete if occupied
				free(h->heap[i]->value);
			}
			free(h->heap[i]);
		}
		free(h->heap);
		free(h);
	}
}
