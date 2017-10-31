// file: Pack.c
// Programmer: Briton Park
// This program solves the bin packing problem by 
// implementing the next, first, best, ffd, and bfd algorithms
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

int next(float sizes[], int len, int trace);
int first(float sizes[], int len, int trace);
int best( float sizes[], int len, int trace);
int strcmp(const char *str1, const char *str2);
bool isValid(char *param);
double atof(const char *s);
float nround (float n, unsigned int c);
void exit(int status);
void sort(float *nums, int len);

int main( int argc, char *argv[] )  {
	// Indicators for -trace and each bin packing algorithm
	int trace = 0;
	int next_flag = 0;
	int first_flag = 0;
	int best_flag= 0;
	int ffd_flag=0;
	int bfd_flag = 0;
	for (int j = 0; j < argc; j++){ 
		if (strcmp((*(argv + j)), "-trace") ==0){
			trace = 1;
		}
	}
	if (argc == 1){  
		fprintf(stderr, "usage: Pack [sizes]* [-next ");
		fprintf(stderr, "| -first | -best | -ffd | -bfd]+ -trace*\n");
	}
	else{
		int i = 1; 
		while((atof(*(argv+i))) != 0 || (strcmp((*(argv+i)), "-trace") == 0)){
			// If number is less than 0 or greater than 1 then print error
			// Don't print error if string is -trace
			if ( ((atof(*(argv+i)) <= 0) || (nround(atof(*(argv+i)), 5) > 1)) ) {
				// Check if parameter not trace or a method
					if (!isValid(*(argv+i))){ 
						fprintf(stderr, "Fatal error: ");
						// Print size error if number invalid
						if ((atof(*(argv+i)) != 0)){
							fprintf(stderr, "Invalid size: %f\n", 
							atof(*(argv+i)));
							exit(0);
						}
						// Print bad argument error if argument not valid
					  	fprintf(stderr, "bad argument: %s\n", *(argv+i));
						exit(0);
					}  	
			}
			// Check if number is too small and if so print error
			if( nround(atof(*(argv+i)),5) == 0){
				fprintf(stderr, "Fatal error: bad argument: %s\n", *(argv+i));
				exit(0);
			}
			i++;

			// If argument is the last one in argv, print error and exit
			if ( i == argc ){
				fprintf(stderr, "Fatal error: no algorithm specified.\n");
				exit(0);
			}
		}
		// If last argument isn't trace or a bin packing method, print error
		if(!isValid(*(argv+i))){
			fprintf(stderr, "Fatal error: bad argument: %s\n", *(argv+i));
		}
		else{
			// Indicator variable to check whether number is greater than 0
			// and less than or equal to 1.
			int valid = 0;
			float numbers [i-1];  
			float sorted_numbers[i-1]; 
			//  Store each size in argv into numbers and sorted_numbers
			for ( int j = 1; j < i; j++){ 
				numbers[j-1] = atof(*(argv+j));
				numbers[j-1] = nround(numbers[j-1], 5);
				sorted_numbers[j-1] = numbers[j-1];
				// If number in valid range, note it
				if (numbers[j-1] != 0 && numbers[j-1]<=1){
					valid = 1;
				}
			}
			sort(sorted_numbers,i-1);
			// Check for numbers and if so, print error and exit
			for(int k = i; k < argc; k++){ 
				if ( atof(*(argv + k)) != 0){
					if ((atof(*(argv+k)) <= 0) || (atof(*(argv+k)) > 1)  ){
						fprintf(stderr, "Fatal error: Invalid size: ");
						fprintf(stderr, "%f\n", atof(*(argv + k)));
						exit(0);
					}
					fprintf(stderr, "Fatal error: Size option");
					fprintf(stderr, " out of order: %f\n", atof(*(argv + k)));
					exit(0);
				}
			}
			// Flags to check whether each algorithm is called
			int next_bin, first_bin, best_bin, ffd_bin, bfd_bin = 0;
			// Array of chars to store in order which algorithm is called
			char pos [argc-i]; 
			int p = 0; 
			for(int k = i; k < argc; k++){   
				if (strcmp((*(argv + k)), "-next" ) == 0){
					pos[p] = 'n';
					p++;
					next_flag = 1;
				}
				else if (strcmp((*(argv + k)), "-first" ) == 0 ){
					pos[p] = 'f';
					p++;
					first_flag = 1;
				}
				else if (strcmp((*(argv + k)), "-best" ) == 0 ){
					pos[p] = 'b';
					p++;
					best_flag = 1;
				}
				else if (strcmp((*(argv + k)), "-ffd" ) == 0 ){
					pos[p] = 'd';
					p++;
					ffd_flag = 1;
				}
				else if (strcmp((*(argv + k)), "-bfd" ) == 0){
					pos[p] = 'e';
					p++;
					bfd_flag = 1;
				}
				else if(!isValid(*(argv + k))){ //
					fprintf(stderr, "Fatal error: ");
					fprintf(stderr, "bad argument: %s\n", *(argv +k));
					exit(0);
				}
			}
			// Check flags for each method, if so run method and store
			// the number of bins
			printf("%d\n", next_flag);
			if (next_flag == 1){
				if (trace == 1 && valid == 1){
					printf("Trace -next\n");
				}
				next_bin = next(numbers,i, trace);
			}
			if (first_flag == 1){
				if (trace == 1 && valid == 1){
					printf("Trace -first\n");
				}
				first_bin = first(numbers,i,trace);
			}
			if(best_flag == 1){
				if (trace == 1 && valid == 1){
					printf("Trace -best\n");
				}
				best_bin = best(numbers,i,trace);
			}
			if (ffd_flag == 1){
				if (trace == 1 && valid == 1){
					printf("Trace -ffd\n");
				}
				ffd_bin = first(sorted_numbers,i, trace);
			}
			if (bfd_flag == 1){
				if (trace == 1 && valid ==1){
					printf("Trace -bfd\n");
				}
				bfd_bin = best(sorted_numbers,i, trace);
			}
			// Go through array of char that stores whether each
			// method is called and print the result if called
			for (int y = 0; y < p ; y++){
				switch(pos[y]) {
					case 'n':
						printf("-next: %d\n",  next_bin);
						break;
					case 'f':
						printf("-first: %d\n",  first_bin);
						break;
					case 'b':
						printf("-best: %d\n",  best_bin);
						break;
					case 'd':
						printf("-ffd: %d\n", ffd_bin);
						break;
					case 'e':
						printf("-bfd: %d\n", bfd_bin);
						break;
					default:
						break;
				}
			}
		}
	}
}
// This function takes in a pointer (param) that points to an array 
// of string parameters. It returns true if the strings are "-next", 
// "-first", "-best", "-ffd", "-bfd", or "-trace" and false otherwise.
bool isValid(char *param){
	if ( (strcmp(param, "-next") == 0) || (strcmp(param, "-first") == 0)) {
		return true;
	}
	else if ((strcmp(param, "-best") == 0) || (strcmp(param, "-ffd") == 0)){
		return true;
	}
	else if ( (strcmp(param, "-bfd") == 0) || (strcmp(param, "-trace") == 0)){
		return true;
	}
	return false;
}
// This function takes in a array of floats (sizes), the length of the
// of the array (len), and an indicator variable for -trace (trace)
// and returns the number of bins needed to pack sizes. The algorithm
// checks if the number fits in the same bin as the last item. If none
// fit a new bin is started.
int next(float sizes[], int len, int trace){
	int isValue = 0; // Indicator variable if valid number is present
	int countBins = 0;  // Number of bins used
	float currentBin = 0; 
	int count = -1; 
	for ( int i = 0; i < len-1; i++){ // Loop through sizes
		if(sizes[i] != 0)
		{
			count++;
			isValue = 1;
			currentBin += sizes[i];
			if (nround(currentBin,5) > 1.0){ 
				countBins++;
				currentBin = 0;
				i--; // Stay on current bin if fits
				count--;
			}
			else{ // Print results for the size after fitted
				if (trace == 1){
					printf("arg: %d val: %f bin: %d total: %f\n", 
						count, sizes[i], countBins, currentBin );
				}
			}
		}
	}
	if(isValue == 1){
		countBins++;
	}
	return (countBins);
}
// This function takes in a array of floats (sizes), the length of the
// of the array (len), and an indicator variable for -trace (trace)
// and returns the number of bins needed to pack sizes. The algorithm
// processes each number and places it into the first bin that fits.
// If none fit, a new bin is started.
int first(float sizes[], int len, int trace){
	int countBins = 0;  // Number of bins used
	float bins[len]; 
	int count = -1;  
	for( int i = 0; i < len; i++){
		bins[i] = 0;
	}
	// Loop through sizes
	for ( int i = 0; i < len-1; i++){
		if (sizes[i] != 0){
			count++;
			int j = 0;
			while( nround(bins[j] + sizes[i],5) > 1.0 ){
				j++;
			}
			bins[j] += sizes[i];
			if (trace == 1 && sizes[i] !=0 ){
				printf("arg: %d val: %f bin: %d total: %f\n", 
					count, sizes[i], j, bins[j] );
			}
		}
	}
	for(int i = 0; i < len; i++){
		if (bins[i] > 0){
			countBins++;
		}
	}
	return countBins;
}
// This function takes in a array of floats (sizes), the length of the
// of the array (len), and an indicator variable for -trace (trace)
// and returns the number of bins needed to pack sizes. The algorithm
// processes each number and places it into the one with tightest fit.
// If none fit, then a new bin is started.
int best( float sizes[], int len, int trace){
	int countBins = 0; // Number of bins used
	float bins[len];
	int count = -1;
	for( int i = 0; i < len; i++){
		bins[i] = 0;
	}
	for ( int i = 0; i < len-1; i++){  // Loop through sizes
		int j = 0; 
		int h = 0; // Index variable to identify best bin
		int best = 0;
		float min = 1.0;
		if (sizes[i] != 0)
		{
			count++;
			// Loop through bins to check for tightest fit
			while (bins[j] != 0){  
				if ( (nround(1 - sizes[i] - bins[j],5) <= min) &&
				 (nround(sizes[i] + bins[j],5) < 1.0) ){ 
					min = 1 - sizes[i] - bins[j];
					h = j;
					best = 1;
				}
				j++;
			}
			if (best == 1){
				bins[h] += sizes[i];
				if (trace == 1 && sizes[i] !=0 ){
					printf("arg: %d val: %f bin: %d total: %f\n",
					 count, sizes[i], h, bins[h] );
				}
			}
			else{ 
				bins[j] += sizes[i];
				if (trace == 1 && sizes[i] !=0 ){
					printf("arg: %d val: %f bin: %d total: %f\n", 
						count, sizes[i], j, bins[j] );
				}
			}
		}
	}
	for(int i = 0; i < len; i++){
		if (bins[i] > 0){
			countBins++;
		}
	}
	return countBins;
}
// This function takes in an array of floats (nums) and the 
// length of the array (len) and sorts the array in decreasing order
void sort(float *nums, int len){
	bool swaps = true;
	float temp;
	while (swaps){ // Loop until no swaps occur in array
		swaps = false;
		// Loop through nums and swap consecutive elements if needed
		for(int i = 0; i < len-1; i++){
			if( *(nums+i) < *(nums+i+1)){
				swaps = true;
				temp = *(nums+i);
				*(nums+i) = *(nums+i+1);
				*(nums+i+1) = temp;
			}
		}
	}
}
// This function takes in an double value (n) and an integer signifying
// how many decimal places to round to (dec) and returns the rounded number
float nround (float n, unsigned int dec)
{
    float power = pow (10, dec);
    float product = n * power;
    float rounded_num = round (product) / power;
    return rounded_num;
}
