// This program is Fire.c which takes in a graph of rooms
// and runs methods that finds that burning room given 
// a starting roomkey. 
// Programmer: Briton Park

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
void bfs(Dict d, const char *key, bool best);
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
    			i++;
    		}
    	}
    	else{
    		fprintf(stderr,"Fatal error: unknown command line argument: %s\n",
    			argv[i]);
    		exit(0);
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
    bool in_graph = false;
    int total = 0; // total count of input rooms
	while(fgets(buffer, MAXSIZE +1 , stdin) != NULL){ // Loop through stdin
		if(buffer[strlen(buffer) -1] == '\n'){
			buffer[strlen(buffer) -1] = '\0';
		}
		
        token = strtok(buffer, " ");
        if(token != NULL){
        	if(!strcmp(token,roomkey)){
        		in_graph = true;
        	}
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

        	if(DictSearch(d,r->room) != 0){ // if room already in dict 
        		fprintf(stderr,"Room %s already in graph. ", r->room);
            	fprintf(stderr, "Replacing it.\n");
        		DictDelete(d, r->room);
        	}
        	else{
        		total++;
        	}
        	DictInsert(d, r->room, r);
        }
	}

	struct elt *e; // struct to go through dict
    for(int i=0; i<d->size; i++) {
      if (d->table[i]) {
		for (e = d->table[i];e != NULL; e = e->next){ // go through table
			int place = 0; // indices for following char array
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

    if(!in_graph){
    	fprintf(stderr,"Fatal error: room %s not included in graph\n",
    		roomkey);
    	DictDestroy(d);
    	exit(0);
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
		bfs(d, roomkey, false);
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
		bfs(d, roomkey, true);
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
// This function takes in a Dict d, char pointer key, int pointer, count,
// and boolean conn. It performs dfs if conn is true and just checks if
// the graph is connected if conn is false. This function uses the global
// variable success, so it knows when the burning room is found or not
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
// This function takes in a Dict d, const char key, and boolean best.
// It does breadth first search if best is false and best first search
// if best is true. It uses success (global variable) to see if the
// burning room is found
void bfs(Dict d, const char *key, bool best){
	extern bool success;
	Heap h;
	h = initheap();
	struct room *r = malloc(sizeof(struct room)); //create room to put in heap
	struct room *hold; //create pointer to room in dict
	hold = DictSearch(d, key); 
	hold->visited = 1;
	*r = *hold;
	int order = 0; // int key for order of rooms in heap
	if(r!=0){
		if(best){
			insert( h, -1*r->temp, r);
		}
		else{ //breadth first search
			insert(h, order ,r); 
		}
		order++;
		while(!empty(h)){
			struct room *v = malloc(sizeof(struct room)); 
			*v = *findmin(h);
			free(findmin(h));
			deletemin(h);
			printf(" %s", v->room);

			if(v->temp > 400){ // burning room found
				free(v);
				destroyHeap(h);
				printf("  SUCCESS!\n");
				success = true;
				return;
			}
			for(int i = 0; i < v->ncount;  i++){ // go through neighbors
				//create room to put in heap
				struct room *neigh = malloc(sizeof(struct room)); 
				struct room *h2; //create pointer to room in dict
				h2 = DictSearch(d,v->neighbors[i] );
				if(h2 != 0){
					*neigh = *h2;
					if(neigh->visited == 0){
						h2->visited = 1;
						if(best){
							insert( h, -1*neigh->temp, neigh);
						}
						else{ //breadth first search
							insert(h, order ,neigh); 
						}
						order++;
					}
					else{
						free(neigh);
					}
				}
				else{
					free(neigh);
				}	
			}
			free(v);
		}
	}
	destroyHeap(h);
}
// This function takes in a Dict d and returns 1 if the graph is
// undirected and 0 if the graph is directed
int undir(Dict d){
	struct elt *e; // struct to go through dict
	bool undirected = true; // bool to check if undirected or directed
    for(int i=0; i<d->size; i++) { // go through hash table
      if (d->table[i]) {
		for (e = d->table[i];e != NULL; e = e->next){ //go through linked list
			int place = 0; // Index variable for following 2D array (char)
			char badneighbors[10][4]; //2D array to hold neighbors not in dict
			memset(badneighbors, 0, sizeof(badneighbors));
			for(int j = 0; j< e->value->ncount; j++){  // go through neighbors
				struct room *r;
				r = DictSearch(d, e->value->neighbors[j]);
				int is_neighbor = 0;
				int repeat = 0;
				for(int l = 0; l < 10; l++){ // check if in 2D array
					if( !strcmp(badneighbors[l],e->value->neighbors[j] )){
						repeat = 1;
					}
				}
				if(repeat!= 1){
					strncpy(badneighbors[place], 
							e->value->neighbors[j], 4);
					badneighbors[place][3] = '\0';
					place++;
					if(r != 0){
						for( int k = 0; k < r->ncount; k++){ //look at neighbors
				 			if(!strcmp(r->neighbors[k],e->value->room )){
				 				is_neighbor = 1;
				 			}
				 		}
						if(is_neighbor == 0 && repeat == 0){ 
							// if both not neighbors then undirected
							undirected = false; 
							printf("Rooms %s and %s are not symmetric.\n", 
								r->room, e->key);
						}
					}
					else{ // if neighbor not in dictionary then undirected
						undirected = false;
						printf("Rooms %s and %s are not symmetric.\n", 
						e->value->neighbors[j], e->key);
					}		
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
// This funciton takes in a Dict d and marks all rooms in the
// dictionary as unvisited
void mark_unvisited(Dict d){
	struct elt *e; // elt struct to go through dict
	for(int i=0; i<d->size; i++) { // go through dict
      if (d->table[i]) {
		for (e = d->table[i];e != NULL; e = e->next){ //go through linked list
			e->value->visited = 0;
		}
      }
	}
}
