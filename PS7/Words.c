// The file name is Words.c and it implements a 2 word
// search and dynamic programming to see if a word
// can be broken up into smaller words using an online
// dictionary.
// Programmer: Briton Park
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "/c/cs223/hw7/hash.h"

#define MAXSIZE 1024 // Max size for input line from stdin

int strcmp(const char *str1, const char *str2);
int tolower(int c);
size_t strlen(const char *str);
void check( int col, int len, int matrix[len][len], bool *success);
void dp(Hash myHash, char *token,bool debug);
void two_words(Hash myHash, char *token);

int main(int argc, char *argv[]){
	bool debug_flag = false; // flag to check for debug flag in command line
	char *dictionary = "words";
	for(int i = 1; i < argc; i++){ // go through command line arguments
		if(!strcmp(argv[i], "-debug")){
			debug_flag = true;
		}
		else if(!strcmp(argv[i], "-dict")){
			if(i == (argc-1)){
				fprintf(stderr,"usage: Words [-dict filename | -debug]\n");
				exit(0);
			}
			else{
				dictionary = argv[i+1];
				i++;
			}
		}
		else{
			fprintf(stderr,"usage: Words [-dict filename | -debug]\n");
			exit(0);
		}
	}
	
	FILE *ptr_file; // Pointer file
	char buf[1000]; // character buffer
	ptr_file =fopen(dictionary, "r");
	if(ptr_file == 0){
		fprintf(stderr, "Fatal error: dictionary not found.\n");
		exit(0);
	}
	if(debug_flag){
		printf("Loading dictionary: %s\n", dictionary);
	}

	Hash myHash = HashCreate();
	char *point; // Pointer to get thorugh text file
	point = fgets(buf,1000, ptr_file);

	char buffer[MAXSIZE+1];  // buffer array for fgets 
    char *token; // character pointer to extract tokens
    int count = 0; //word count of dictionary

    while (point !=NULL){ // load hash table
    	count++;
		int index = 0;
		while (*(point+index) != '\0'){
			index++;
		}
		if (*(point+index-1) == '\n' ){ // Delete newline 
			*(point+index-1) = '\0';
			index--;
		}
		for(int k = 0; point[k] != '\0'; k++){
           	point[k] = tolower(point[k]);
        }
		HashInsert(myHash, point);
		
		point = fgets(buf,1000, ptr_file);
	}
	if(debug_flag){
		printf("Word Count: %d\n", count);
	}
    while(fgets(buffer, MAXSIZE +1 , stdin) != NULL){ // Loop through stdin
    	if(buffer[strlen(buffer) -1] == '\n'){
    		buffer[strlen(buffer) -1] = '\0';
    	}
    	if(debug_flag){
    		printf("Input: %s\n", buffer);
    	}
    	
        token = strtok(buffer, " ");
        
        if(token != 0){
        	bool error = false;
        	for(int k = 0; token[k]!= '\0'; k++){
        		if( !( (token[k] >= 97 && token[k] <= 122) || 
        			(token[k] >=65 && token[k] <= 90)  )){
        			fprintf(stderr, 
        				"Non-alpha character: [%c] in %s\n", token[k], token);
        			error = true;
        		}
        		token[k] = tolower(token[k]);
        	}
        	if(error){
        		fprintf(stderr, "Error: not processing %s\n", token);
        		printf("Two words: FAILURE\n");
        		printf("---\n");
        	}
        	else{
        		while( token != NULL ){ // Process tokenized input
        			two_words(myHash, token);
        			dp(myHash, token, debug_flag);
					token = strtok(NULL," ");
        		}
        	}
        }
        else{
        	printf("Two words: FAILURE\n");
        	printf("DP: SUCCESS \n\n");
        	printf("---\n");
        }
		
	}
	HashDestroy(myHash);
}
// This function takes in a hash (myHash) and a char pointer
// called token. It prints whether a word can be broken 
// down into two
void two_words(Hash myHash, char *token){
	printf("Two words:");
	bool success = false;
    for(int j = 1; j < strlen(token); j++){ // go through token
       	char firstHalf[j+1];
       	char secondHalf[strlen(token)-j];
       	strncpy(firstHalf, token, j);
       	strncpy(secondHalf, token + j, strlen(token)-j+1);
        firstHalf[j] = '\0';
        secondHalf[strlen(token)-j] = '\0';
        // if both halves in table
        if(HashSearch(myHash, firstHalf) && HashSearch(myHash, secondHalf)){
        	if(success){
        		printf(" %s %s.", firstHalf, secondHalf);
        	}
        	else{
        		printf(" SUCCESS:  %s %s.", firstHalf, secondHalf);
        		success = true;
        	}
        }
    }
    if(!success){
    	printf(" FAILURE");
    }
    printf("\n");

}
// This function takes in a Hash (myhash), a char pointer token, and
// a boolean debug. It prints output depending on if the word
// can be broken down into several words and prints the corresponding
// matrix.
void dp(Hash myHash, char *token, bool debug){
	int len = strlen(token); // int variable for token length
	int matrix[len][len]; // matrix for dynamic programmign method
	memset(matrix, -1, sizeof(matrix)); // clear memory for matrix

	for(int i = 1; i <= len; i++){  // index to go through matrix columns
		for( int j = 0; j < len ; j++ ){ // index for rows of matrix
			if( (i +j ) <= len){
				for(int k = j; k < i + j; k++){ // go through each section
					if(j == k && i ==1 ){ // search one character word in dict
						char strin[2];
						strin[0] = token[j];
						strin[1] = '\0';
						if(HashSearch(myHash, strin )){// set position to j
							matrix[j][j] = j; // if char is a word
						}
					}
					else{ 
						if(k == j ){
							char entire[i+1];
							strncpy(entire, token + j, i );
							entire[i] = '\0';
							if(HashSearch(myHash, entire)){
								matrix[j][i+j-1] = j;
							}
						}
						else if (matrix[j][k - 1] >= 0 && 
							matrix[k][i + j - 1] >= 0 ){
							if(matrix[j][i + j - 1] < 0){
								matrix[j][i + j - 1] = k; // set spot to break
							}	
						}
					}
				}
			}
		}
	}
	if(debug){ // print matrix
		printf("\n");
		for(int i = 0; i < len; i++){
			for(int j = 0; j < len; j++){
				printf("%d ", matrix[i][j]);
			}
			printf("\n");
		}
	}

	printf("DP: ");
	bool *success = malloc(sizeof(bool));
	*success = false;
	int breakpoint = 0;
	check(len-1, len, matrix, success);
	if(*success){ // print words
		printf("SUCCESS: \n");
		breakpoint = 0;

		for( int i = 0;  i < len; i++){
			if( i == matrix[i][len-1]){
				for(; i < len; i++) {
					printf("%c", token[i]);
				}
				break;
			}
			if(matrix[i][len-1] > breakpoint){
				for(int j = breakpoint; j < matrix[i][len-1]; j++) {
					printf("%c", token[j]);
				}
				printf(" ");
				breakpoint = matrix[i][len-1];
			}
		}
	}
	else{
		printf("FAILURE");
	}
	printf("\n---\n");
	free(success);
}

// This function takes in a integer col denoting the column of the 
// matrix of integers (matrix), an integer len (denoting the 
// size of the matrix), and a boolean pointer to see whether
// a word can be broken up into multiple words. It is recursive in nature.
void check(int col, int len, int matrix[len][len], bool *success){
	if(!*success){
		for(int i = 0; i < len; i++){
			if(matrix[i][col] >= 0){
				if(matrix[i][col] == 0){
					*success = true;
					return ;
				}
				else{
					check( matrix[i][col] - 1,  len, matrix, success);
				}
			}
		}
	}
}