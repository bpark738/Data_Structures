#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "/c/cs223/hw6/dict.h"
#include "/c/cs223/hw6/heap.h"

#define MAXSIZE 1024 // Max size of input line through stdin

int undir(Dict d);
void best(Dict d, const char *key);
void bfs(Dict d, const char *key);
void dfs(Dict d, const char *key, int *count, bool conn);
void mark_unvisited(Dict d);

// global variable to check whether burning room found
bool success = false; 

int main(int argc, char *argv[]){
	char *roomkey; // variable to hold the room key from command line

    // Initialize command line flags
    bool dfs_flag = false;
    bool bfs_flag = false;
    bool best_flag = false;
    bool conn_flag = false;
    bool dir_flag = false;
    bool room_flag = false;
    // check if no methods given
    bool default_flag = true;
    extern bool success;

    for(int i = 1; i < argc; i++){ // go through command line arguments
    	if(!strcmp("-dfs", argv[i])){
    		dfs_flag = true;
    		default_flag = false;
    	}
    	else if(!strcmp("-bfs", argv[i])){
    		bfs_flag = true;
    		default_flag = false;
    	}
    	else if(!strcmp("-best", argv[i])){
    		best_flag = true;
    		default_flag = false;
    	}
    	else if(!strcmp("-conn", argv[i])){
    		conn_flag = true;
    		default_flag = false;
    	}
    	else if(!strcmp("-dir", argv[i])){
    		dir_flag = true;
    		default_flag = false;
    	}
    	else if(!strcmp("-room", argv[i])){
    		if(argc-1 == i){
    			fprintf(stderr, "Fatal error: no room given.\n");
    			fprintf(stderr, "Usage: Fire -room value ");
    			fprintf(stderr, "[-dfs | -bfs | -best | -conn | -dir]\n");
    			exit(0);
    		}
    		else{
    			room_flag = true;
    			roomkey = argv[i+1];
    		}
    	}
    }

    if(!room_flag){
    	fprintf(stderr, "Fatal error: no room given.\n");
    	fprintf(stderr, "Usage: Fire -room value ");
    	fprintf(stderr, "[-dfs | -bfs | -best | -conn | -dir]\n");
    	exit(0);
    }
    if(default_flag){ // if no commandline flags, dfs is the default method
    	dfs_flag = true;
    }
    struct dict *d; //dictionary for hash table
    d = DictCreate();

    char buffer[MAXSIZE+1];  // buffer array for fgets 
    char *token; // character pointer to extract tokens
    int index;

    int total = 0; // total count of input rooms
	while(fgets(buffer, MAXSIZE +1 , stdin) != NULL){ // Loop through stdin
		if(buffer[strlen(buffer) -1] == '\n'){
			buffer[strlen(buffer) -1] = '\0';
		}
		
        token = strtok(buffer, " ");
        index = 0;
        struct room *r = malloc(sizeof(struct room));

        r->visited = 0;
        r->ncount = 0;
		while( token != NULL ){ // Process tokenized input
			if(index == 0){ // get room key
				strncpy(r->room, token, 4);
				r->room[3] = '\0';
			}
			else if(index == 1){ // get temperature
				r->temp = atoi(token);
			}
			else{ //get neighbors
				strncpy(r->neighbors[index-2], token,4);
				r->neighbors[index-2][3] = '\0';
				r->ncount++;
			}
			index++;
			token = strtok(NULL," ");
        }

        if(DictSearch(d,r->room) != 0){ // if room already in dict from before
        	fprintf(stderr,"Room %s already in graph. ", r->room);
            fprintf(stderr, "Replacing it.\n");
        	DictDelete(d, r->room);
        }
        else{
        	total++;
        }
        DictInsert(d, r->room, r);
	}

	struct elt *e; // struct to go through dict
    for(int i=0; i<d->size; i++) {
      if (d->table[i]) {
		for (e = d->table[i];e != NULL; e = e->next){ // go through table
			int place = 0;
			char badneighbors[10][4]; //2D array to hold neighbors not in dict
			memset(badneighbors, 0, sizeof(badneighbors));
			for(int j = 0; j< e->value->ncount; j++){ // go through neighbors
				if(DictSearch(d, e->value->neighbors[j]) == 0){
					int repeat = 0;
					for(int l = 0; l < 10; l++){ // check if in 2D array
						if( !strcmp(badneighbors[l],e->value->neighbors[j] )){
							repeat = 1;
						}
					}
					if(repeat != 1){
						fprintf(stderr, "Warning: room %s has neighbor ", 
							e->value->room);
						fprintf(stderr,"%s which is not in the dictionary.\n",
							e->value->neighbors[j] );
						strncpy(badneighbors[place], 
							e->value->neighbors[j], 4);
						badneighbors[place][3] = '\0';
						place++;
					}					
				}
			}
		}
      }
    }
    int *count = malloc(sizeof(int)); // room count variable for dfs
    *count = 0;
	if(dfs_flag){
		bool conn = false; // boolean for dfs to run dfs and not conn
		printf("Starting depth first search:");
		dfs(d, roomkey, count, conn);
		if(!success){
			printf("  FAILED\n");
		}
		else{
			success = true;
		}
		mark_unvisited(d); // reset rooms in dict to be not visited
	}
	if(bfs_flag){
		printf("Starting breadth first search:");
		bfs(d, roomkey);
		if(!success){
			printf("  FAILED\n");
		}
		else{
			success = true;
		}
		mark_unvisited( d); // reset rooms in dict to be not visited
	}
	if(best_flag){
		printf("Starting best first search:");
		best(d, roomkey);
		if(!success){
			printf("  FAILED\n");
		}
		else{
			success = true;
		}
		mark_unvisited( d); // reset rooms in dict to be not visited
	}
	if(conn_flag){
		*count = 0;
		dfs(d, roomkey, count, conn_flag);
		if(*count == total){
			printf("Graph is connected.\n");
		}
		else{
			printf("Graph is NOT connected.\n");
		}
		mark_unvisited( d); // reset rooms in dict to be not visited
	}
	if(dir_flag){
		if (undir(d)){
			printf("Graph is undirected.\n");
		}
		else{
			printf("Graph is directed.\n");
		}
		mark_unvisited( d); // reset rooms in dict to be not visited
	}
	free(count);
	DictDestroy(d);
}

void dfs(Dict d, const char *key, int *count, bool conn){
	struct room *r;
	r = DictSearch(d, key);
	extern bool success;

	if(r != 0){
		if(r->visited == 0){
			(*count)++;
			r->visited = 1;
			if(!success){ // print room if burning room not found yet
				if(!conn){
					printf(" %s", r->room);
				}
			}
			if(r->temp > 400 && !success ){ // if found for first time
				if(!conn){
					printf("  SUCCESS!\n");
				}
				success = true;
				return;
			}
			else{
				for(int h = 0; h < r->ncount; h++){ // go through neighbors
					dfs(d, r->neighbors[h],count, conn); // recursively
				}
			}
		}
	}
}

void bfs(Dict d, const char *key){
	extern bool success;
	Heap h;
	h = initheap();
	// create malloced room to put in heap
	struct room *r = malloc(sizeof(struct room)); 
	struct room *hold; //create pointer to room in dict
	hold = DictSearch(d, key); 
	hold->visited = 1;
	*r = *hold;
	int order = 0; // int key for order of rooms in heap
	if(r!=0){
		insert(h, order ,r); 
		order++;
		while(!empty(h)){
			struct room *v; // evaluate min-key room
			v = findmin(h);
			printf(" %s", v->room);

			if(v->temp > 400){ // burning room found
				destroyHeap(h);
				printf("  SUCCESS!\n");
				success = true;
				return;
			}
			for(int i = 0; i < v->ncount;  i++){ // go through neighbors
				//create malloced room to put in heap
				struct room *neigh = malloc(sizeof(struct room));
				struct room *h2; //create pointer to room in dict
				h2 = DictSearch(d,v->neighbors[i] );
				*neigh = *h2;
				if(neigh != 0 ){
					if(neigh->visited == 0){
						h2->visited = 1;
						insert( h, order, neigh);
						order++;
					}
					else{
						free(neigh);
					}
				}
			}
			free(v);
			deletemin(h);
		}
	}
	destroyHeap(h);
}

void best(Dict d, const char *key){
	extern bool success;
	Heap h;
	h = initheap();
	// create malloced room to put in heap
	struct room *r = malloc(sizeof(struct room));
	struct room *hold; //create pointer to room in dict
	hold = DictSearch(d, key);
	hold->visited = 1;
	*r = *hold;

	if(r!=0){
		insert(h, -1*r->temp ,r);
		while(!empty(h)){
			// evaluate min-key room
			struct room *v = malloc(sizeof(struct room)); 
			*v = *findmin(h);
			free(findmin(h));
			deletemin(h);
			printf(" %s", v->room);

			if(v->temp > 400){ //burning room
				free(v);
				destroyHeap(h);
				printf("  SUCCESS!\n");
				success = true;
				return;
			}
			for(int i = 0; i < v->ncount;  i++){ // go through neighbors
				// create malloced room to put in heap
				struct room *neigh = malloc(sizeof(struct room));
				struct room *h2; //create pointer to room in dict
				h2 = DictSearch(d,v->neighbors[i] );
				*neigh = *h2;
				if(neigh != 0 ){
					if(neigh->visited == 0){
						h2->visited = 1;
						// use negative temperature as key in heap
						insert( h, -1*neigh->temp, neigh);
					}
					else{
						free(neigh);
					}
				}
			}
			free(v);
		}
	}
	destroyHeap(h);
}

int undir(Dict d){
	struct elt *e; // struct to go through dict
	bool undirected = true; // bool to check if undirected or directed
    for(int i=0; i<d->size; i++) { // go through hash table
      if (d->table[i]) {
		for (e = d->table[i];e != NULL; e = e->next){ //go through linked list
			for(int j = 0; j< e->value->ncount; j++){  // go through neighbors
				struct room *r;
				r = DictSearch(d, e->value->neighbors[j]);
				int is_neighbor = 0;
				if(r != 0){
					for( int k = 0; k < r->ncount; k++){ // go through neighbors
				 		if(!strcmp(r->neighbors[k],e->value->room )){
				 			is_neighbor = 1;
				 		}
					}
				}
				else{ // if neighbor not in dictionary then undirected
					undirected = false;
					printf("Rooms %s and %s are not symmetric.\n", 
						r->room, e->key);
				}				
				if(is_neighbor == 0){ // if neighbor's list does not original 
					undirected = false; // then undirected
					printf("Rooms %s and %s are not symmetric.\n", 
						r->room, e->key);
				}
			}
		}
      }
    }
    if(undirected){
    	return 1;
    }
    return 0;
}

void mark_unvisited(Dict d){
	struct elt *e; // struct to go through dict
	for(int i=0; i<d->size; i++) { // go through dict
      if (d->table[i]) {
		for (e = d->table[i];e != NULL; e = e->next){ //go through linked list
			e->value->visited = 0;	
		}
      }
	}
}
