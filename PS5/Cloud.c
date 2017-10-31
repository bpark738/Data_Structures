// This program is Cloud.c and it takes in input from
// standard input and it tabulates the frequency
// of words in the input and output those over a threshold
// Programmer: Briton Park

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "btree.h"

#define MAXSIZE 1024  // maximum size of input

bool is_int(char *s);
bool non_alpha(char *s);
int atoi(const char *str);
void to_lower(char *s);
void treeAdd(struct node * tree, char * key);

int main(int argc, char *argv[]){
	// flags for command line argument
	bool preorder = false;
	bool postorder = false;
	bool inorder = false;
	bool html = false;
	bool debugflag;
	debugflag = false;
	int threshold; // stores threshold value from command line input
	threshold = 5;

	for( int i = 1; i < argc ; i++){ // loop through command line input
		if(!strcmp(argv[i], "-threshold")){
			if(i == (argc-1)){
			   fprintf(stderr,"Missing threshold argment on command line.\n");
			}
			else if (atoi(argv[i+1]) == 1){
				fprintf(stderr, "Invalid threshold value: %s\n", argv[i+1]);
				i +=1; // Skip next input
			}
			else if( !is_int(argv[i+1])){
				fprintf(stderr, "Invalid threshold value: %s\n", argv[i+1]);
				i+=1; // Skip next input
			}
			else{
				threshold = atoi(argv[i+1]);
			}
		}
		else if(!strcmp(argv[i], "-html")){
			html = true;
		}
		else if(!strcmp(argv[i], "-debug")){
			debugflag = true;
		}
		else if(!strcmp(argv[i], "-preorder")){
			preorder = true;
		}
		else if(!strcmp(argv[i], "-inorder")){
			inorder = true;
		}
		else if(!strcmp(argv[i], "-postorder")){
			postorder = true;
		}
		else if(is_int(argv[i])){
			if (strcmp(argv[i-1], "-threshold")){
				fprintf(stderr, 
			      "Fatal error: invalid command line argument: %s\n",argv[i]);
				exit(0);
			}
		}
		else{
			fprintf(stderr, 
				"Fatal error: invalid command line argument: %s\n", argv[i]);
			exit(0);
		}
	}
	// character buffer that stores input from fgets
	char buffer[MAXSIZE+1]; 
    char *token; // character pointer to extract tokens
    struct node *cloud; // Linkedlist cloud node
    cloud = NULL;

    struct node * root = makeNode("**root**");

	while(fgets(buffer, MAXSIZE +1 , stdin) != NULL){ // Loop through stdin
        if(debugflag){
            printf("Input: %s",buffer);
        }
		buffer[strlen(buffer) -1] = '\0';
        token = strtok(buffer, " ");
		while( token != NULL ){ // Process tokenized input
			if( ! non_alpha(token)){
        		to_lower(token);
				treeAdd(root, token);
				if(treeSearch(root, token)->count >= threshold){
					if(cloud == NULL){ 
						cloud = treeSearch(root,token);;
						cloud->next = NULL;
					}
					else{
						struct node *e;
						bool present = false;
						// Check if node in linked list
						for(e = cloud; e != NULL; e = e->next){
							if(!strcmp(e->key, token)){
								present = true;
							}
						}
						if(!present){ // If not present, put in front of list
							treeSearch(root,token)->next = cloud;
							cloud = treeSearch(root,token);
						}
					}
				}
        	}
        	token = strtok(NULL," ");
        }
	}
	// Print output messages
    if(debugflag){
        printf("Tree height: %d\n", treeHeight(root));
        printf("Tree size: %d\n",treeSize(root));
    }
    if(preorder){
        printf("PREORDER\n");
        printTreePre(root);
        printf("\n");
    }
    if(inorder){
        printf("INORDER\n");
        printTreeIn(root);
        printf("\n");
    }
    if(postorder){
        printf("POSTORDER\n");
        printTreePost(root);
        printf("\n");
    }
    if(cloud == NULL){
    	printf("No words seen %d times.\n", threshold);
    }
    else{
    	struct node *e;
    	int i = 0;
    	if(!html){
    		printf("The Word Cloud:\n");
    	}
    	for(e = cloud; e != NULL; e = e->next){
       		if(!html){ // print regular output
        		printf("[%d] %s [%d]\n", i, e->key, e->count);
        	}
        	else{ // print html output
        		printf("<div style=\"font-size: %dpx\"> %s </div>\n",
        			e->count, e->key);
       		}	
        	i++;
    	}
    }
    treeDestroy(root);
}

// This function takes in a character pointer s and returns true
// if it is an integer and false otherwise.
bool is_int(char *s){
	if(atoi(s) == 0){
		return false;
	}
	if(s[0] == '-'){
		return false;
	}
	return true;
}
// This function takes in a character pointer s and returns true
// if it is not a non_alpha character and false otherwise.
bool non_alpha(char *s){
	for(int i = 0; i < strlen(s); i++){
		if( !(s[i] <= 'z' && s[i] >= 'a') && !(s[i] <= 'Z' && s[i] >= 'A')){
			return true;
		}
	}
	return false;
}
// This function takes in a character pointer s and converts 
// the string to upper case form.
void to_lower(char *s){
	for(int i = 0; i < strlen(s); i++){
		if( (s[i] <= 'Z' && s[i] >= 'A')){
			s[i] = s[i] - ('A' - 'a');
		}
	}
}
// This function takes in a pointer to a node (tree) and a character
// pointer (key). It creates a new node from key and calls treeSearch to
// see if it is in the tree already. If so it increments the nodes count
// and if not, it inserts the node into the tree using treeInsert.
void treeAdd(struct node * tree, char * key){
  struct node * newnode;
  newnode = treeSearch(tree, key);

  if (newnode) {
    newnode->count++;
  }
  else {
    treeInsert(tree, key);
  }
}
