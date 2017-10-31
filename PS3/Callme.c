// File name: Callme.c
// This program converts alphabetic input into numbers and converts numeric 
// numbers into words and checks the online dictionary if there are any
// matching words and prints them out.
// Programmer: Briton Park
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "hash.c"

int strcmp(const char *str1, const char *str2);
int tolower(int c);
size_t strlen(const char *str);
void getCombo(Hash h, int number [], int current_digit, char result[], int n);
void convertWord(char *s);

static bool match; // global flag if a word is matched in hash

int main(int argc, char *argv[]){
	extern bool debugflag; // Flag for -debug argument
	extern bool match;
	if (argc == 1 || argc > 3){ // Check for too little or too many arguments
		fprintf(stderr, "usage: Callme (digits | letters) [-debug]?\n");
		exit(0);
	}
	else if (argc == 3){ // Check for -debug argument if two inputs
		if (strcmp(*(argv+2), "-debug") == 0){
			debugflag = true;
		}
		else{
			fprintf(stderr,"Fatal error: invalid argument %s\n", *(argv + 2));
			exit(0);
		}
	}
	if ( argv[1][0] == 48 && strlen(*(argv+1)) == 1   ){ // Check for 0 case
		fprintf(stderr, "Fatal error: invalid argument %s\n", *(argv + 1));
		exit(0);
	}
	for(int j = 0; argv[1][j] != '\0'; j++){ 
		// If not letter or digit print error
		if(! (( tolower(argv[1][j]) >= 97 && tolower(argv[1][j]) <= 122 ) ||
							((argv[1][j])  >= 48 && (argv[1][j])  <= 57 ))){
			fprintf(stderr, "Fatal error: invalid argument %s\n", argv[1]);
			exit(0);
		}
	}
	// if first letter is a letter
	if (tolower(argv[1][0]) >= 97  && tolower(argv[1][0]) <= 122 ){ 
		for(int j = 0; argv[1][j] != '\0'; j++){ // If digit print error
			if( !(tolower(argv[1][j]) >= 97 && tolower(argv[1][j])<= 122)){
				fprintf(stderr,"Fatal error: invalid argument %s\n", argv[1]);
				exit(0);
			}
		} // Output for alphabetic input
		printf("alphabetic: %s => ", *(argv+1));
		convertWord(*(argv +1));
		printf("%s\n", *(argv +1));
	}
	else{ // Number input
		for(int j = 0; argv[1][j] != '\0'; j++){ 
			// If contains letter then print error
			if( tolower(argv[1][j]) >= 97  && tolower(argv[1][j]) <= 122){
				fprintf(stderr,"Fatal error: invalid argument %s\n", argv[1]);
				exit(0);
			}
		}
		FILE *ptr_file;
		char buf[1000];
		ptr_file =fopen("words.txt", "r");

		Hash myHash = HashCreate();
		char *point; // Pointer to get thorugh text file
		point = fgets(buf,1000, ptr_file);
		int len = strlen(*(argv+1)); // Length of number input
		if (debugflag){
			printf("Loading dictionary\n");
		}
		while (point !=NULL){
			int index = 0;
			while (*(point+index) != '\0'){
				index++;
			}
			if (*(point+index-1) == '\n'){ // Delete newline 
				*(point+index-1) = '\0';
				index--;
			}
			for(int k = 0; point[k] != '\0'; k++){
                point[k] = tolower(point[k]);
        	}

			if(index == len){
				HashInsert(myHash, point);
			}
			point = fgets(buf,1000, ptr_file);
		}
		fclose(ptr_file);
		
		if(debugflag){
			printf("Word Count: %lu\n", myHash->n);
		}

		int number[len]; // Array of digits to store numbere

		for (int k = 0; argv[1][k] != '\0'; k++){
			number[k] = argv[1][k] - 48; // Convert char number to int number
		}
		match = false; 
    	char result[len]; // Char array to hold resulting words
    	printf("numeric: %s =>", *(argv+1));
    	getCombo(myHash, number, 0, result, len-1);
    	if (!match){ // If no matches found print it out
    		printf(" ** no matches **");
    	}
    	printf("\n");
    	HashDestroy(myHash);
	}
}

// This recursive function takes in a hash table (H) of words,
// array of integers (number) to make  words out of, an indexing 
// int (current_digit),a character array (result) to store each word 
// per iteration, the length of the number array (n), 
// This function obtains the different combination of words that can be made,
// checks if the word is in the hash table, and prints it out if so.
// It also changes the global variable match to true if a word is matched.
void getCombo(Hash H, int number [], int current_digit, char result[], int n)
{
    const char letters[10][5] = {"", "", "abc", "def", "ghi", "jkl",
                               "mno", "pqrs", "tuv", "wxyz"};
    int end = 3; // Get number of possible letters for each digit
    if (number[current_digit] == 7 || number[current_digit] == 9){
        end = 4;
    }
    for(int j = 0; j< end; j++){
    	// Convert current digit to letter and store
        result[current_digit] = letters[number[current_digit]][j];  
        
        if(current_digit < n){ // Get next digit to letter
            getCombo(H, number, current_digit+1, result, n);
        }
        if (current_digit == n){ // If last digit store into combos
        	result[n+1] = '\0';
        	if (HashSearch(H, result)){
				printf(" %s", result);
				match = true;
			}
        }
    } 
}
// This function takes in a character pointer (*s) of a word,
// converts it to lower case, and replaces it with the 
// corresponding number character. If the character isn't a 
// valid letter, the function leaves it be.
void convertWord(char *s){
    for(int j = 0; *(s+j) != '\0'; j++){
    	*(s+j) = tolower(*(s+j));
        if (*(s+j) == 'a' ||*(s+j) == 'b' ||  *(s+j) == 'c'){
            *(s+j) = '2';
        }
        else if (*(s+j) == 'd' ||*(s+j) == 'e' ||  *(s+j) == 'f'){
            *(s+j) = '3';
        }
        else if (*(s+j) == 'g' ||*(s+j) == 'h' ||  *(s+j) == 'i'){
            *(s+j) = '4';
        }
        else if (*(s+j) == 'j' ||*(s+j) == 'k' ||  *(s+j) == 'l'){
            *(s+j) = '5';
        }
        else if (*(s+j) == 'm' ||*(s+j) == 'n' ||  *(s+j) == 'o'){
            *(s+j) = '6';
        }
        else if (*(s+j) == 'p'||*(s+j) == 'q'||*(s+j) == 'r'||*(s+j) == 's'){
            *(s+j) = '7';
        }
        else if (*(s+j) == 't' ||*(s+j) == 'u' ||  *(s+j) == 'v'){
            *(s+j) = '8';
        }
        else if (*(s+j) == 'w'||*(s+j) == 'x'|| *(s+j)== 'y'||*(s+j) == 'z'){
            *(s+j) = '9';
        }
    }
}
