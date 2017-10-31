// File name: hash.c
// This program contains structures and functions to be
// used in Callme.c
// Programmer: Briton Park
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include "hash.h"
static void grow(Hash d);
char *strdup(const char *s);
int tolower(int c);

// elt is a node in the linked list which
// contains a pointer to the next element in the 
// linked list (*next) and a pointer to the
// word of the node (*key)
struct elt { 
  struct elt *next; 
  char *key; 
};
// hash is a hash table structure which contains
// the number of spots in the hash table (size),
// the number of elements in the hash table (n),
// the number of spots occupied (buckets), and
// a pointer of pointers (**table) which contains
// linked lists in each spot.
struct hash {
  int size;           
  unsigned long int n;              
  int buckets;
  struct elt **table;
};

Hash HashCreate(void){
	Hash h;
	h = malloc(sizeof(*h));
	h->size = INITIAL_SIZE;
	h->n = 0;
    h->buckets = 0;
	h->table = malloc(sizeof(struct elt *) *h->size); 

	assert(h->table != 0);
	for (int i = 0; i < h->size; i++){
		h->table[i] = 0;
	}
	return h;
}

void HashDestroy(Hash h){
	struct elt *d;
	struct elt *next;

	for (int i = 0; i<h->size; i++){
		for ( d = h->table[i]; d!= 0; d = next){
			next = d->next;
			free(d->key);
			free(d);
		}
	}
	free(h->table);
	free(h);
}

Hash internalHashCreate(int size)
{
    Hash d;
    d = malloc(sizeof(*d));

    assert(d != 0);

    d->size = size;
    d->n = 0;
    d->buckets = 0;
    d->table = malloc(sizeof(struct elt *) * d->size);

    assert(d->table != 0);
    for(int i = 0; i < d->size; i++){
    	d->table[i] = 0;
    }
    return d;
}
// This function takes in a constant character pointer
// (*str) which points to a word and returns a number.
// Taken online (djb2 by Dan Bernstein)
unsigned long hash_function( const char *str){ 
    unsigned long h = 5381;
    int c;

    while ((c = *str++))
        h = ((h << 5) + h) + c;
    return h;
}

void HashInsert(Hash h, const char *key){
	struct elt *e;
	e = malloc(sizeof(*e));
	assert(e);
	e->key = strdup(key);

	unsigned long hf_out = hash_function(key) % h->size;
    if (h->table[hf_out] == 0){
        h->buckets++;
    }

	e->next = h->table[hf_out];

	h->table[hf_out] = e;
	h->n++;
	if(h->n >= h->size*MAX_LOAD_FACTOR){
		grow(h);
	}
}

static void grow(Hash d){
    Hash d2;
    struct hash swap;
    struct elt *e;
    extern bool debugflag;
    if (debugflag){
        printf("Growing to size: %d. n: %lu. ", d->size*GROWTH_FACTOR,d->n);
        printf("Used buckets: %d. ", d->buckets);
        printf("Occupancy rate: %.2f\n",  (float)d->buckets/d->n);
    }

    d2 = internalHashCreate(d->size * GROWTH_FACTOR);
    for(int i = 0; i < d->size; i++) {
        for(e = d->table[i]; e != '\0'; e = e->next) {
            HashInsert(d2, e->key);
        }
    }
    swap = *d;
    *d = *d2;
    *d2 = swap;
    
    HashDestroy(d2);
}

bool HashSearch(Hash d,  char *key)
{
    struct elt *e;

    for(e = d->table[hash_function(key) % d->size]; e != 0; e = e->next) {
        if(!strcmp(e->key, key)) {
            return true;
        }
    }
    return false;
}

void HashDisplay(Hash d){
	struct elt *e;
	for (int i = 0; i < d->size; i++){
		for (e = d->table[i]; e!=0; e=e->next){
			printf("%s in place %d\n", e->key,i );
		}
	}
}
