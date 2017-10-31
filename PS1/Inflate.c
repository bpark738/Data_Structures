// Filename: Inflate.c
// This program copies text from standard input to standard output
// replacing integer values incremented by 1.
// Programmer: Briton Park 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ungetchar(c) ungetc(c, stdin) // Unread char read from stdin

bool isDigit(int c);
long increment(char c [], int sign, int base, int len);
bool isZero(int c);
bool checkSplice(int c);
void displayNum(int c, int sign, int base);
void putSymbols(long num, int sign, int base);
long toBase10(char c[], int base, int len);
bool isHex(int c);
bool isBinary(int c);
bool isOctal(int c);
void putDigits(long num, int base );

int main()
{
	int c;
	int sign = 0;  // Minus sign state
	while ((c = getchar()) != EOF){
			if ( c== '\\'){
				if(checkSplice(c)){ 
            		c = getchar();
        		}
        		else{           
        			putchar('\\');  
					c = getchar();
        		}
				if (c== '\"' ){  // Handle escaped quote case
					putchar(c);
				}
				else{    // Handle any other character after backslash
					ungetchar(c);
				}
			}
			else if (c == '\"'){        // Handle comments
				putchar('\"');
				c = getchar();
				while(c != '\"' && c != EOF){
					if (c == '\\'){ 
						putchar('\\');
						c= getchar();
						if ( c == '\"'){  // Handle escaped double quote
							putchar(c);
							c = getchar();
						}
						else{
							putchar(c);
							c = getchar();
						}
					}
					else{
						putchar(c);
						c = getchar();
					}
				}
				if(c != EOF){
					putchar(c);
				}
			} 
			else if( c == '-'){   // Save minus state
				sign = 1;
				if (!(isDigit(c = getchar() ))){  
					putchar('-');    // Handle negative sign without number
					sign = 0;
				}
				ungetchar(c);
			}
			else if( (isDigit(c) && !isZero(c))  ){ // Handle base 10 case
				displayNum(c,sign,10);
			}
			else if(isZero(c)){  // Handle 0 character case
				c = getchar();
				if(checkSplice(c)){ 
            		c = getchar();
        		}
				if (c == 'b' || c == 'B'){ // Handle binary case
					c = getchar();
					if(checkSplice(c)){ 
            			c = getchar();
        			}
					displayNum(c, sign, 2);
				}
				else if(c == 'x' || c == 'X'){  // Handle hexadecimal case
					c = getchar();
					if(checkSplice(c)){  
            			c = getchar();
        			}
					displayNum(c, sign, 16);
				}
				else if (isOctal(c)){ 			// Handle octal case
					displayNum(c, sign, 8);
				}
				else if(isDigit(c)){  // Handle 0 followed by 8 or 9
					displayNum(c, sign, 10);
				}
				else{   // Handle lone 0 character case
					sign = 0;
					ungetchar(c);
					putchar('1');
				}	
			}
			else{   // Handle other chracters
				putchar(c);
				sign = 0;
			}
	}
}
void displayNum(int c, int sign, int base){
	// Takes in input character c, sign of the number (sign), and the base
	// of the number (base). This function displays the incremented number
	// in the specified base.
	int len = 0;  // Length of number (number of digits)
	char digits[64] = {0};
	    // Get digits if valid respective to the base
		while( (isBinary(c) && base == 2) || (isDigit(c) && base == 10) ||
		       (isOctal(c) && base == 8) || (isHex(c) && base == 16)){
			digits[len] = c;
			c = getchar();
			if(checkSplice(c)){  // Handle line splices
            	c = getchar();
        	}
			len++;
		}
		ungetchar(c);			
		long numero = increment(digits, sign, base, len);
		putSymbols(numero ,sign ,base);
		putDigits(numero, base);
}
long increment(char digits[], int sign, int base, int len){
	// Takes in array of digits (digits), sign of the number (sign), and
	// the base of the number (base), and the number of digits
	// or length of the number (len). 
	// This function returns the incremented number.
	long numero = toBase10(digits, base, len);  // Number in base 10
	if(sign == 0){ 
		numero++;
	}
	else{   
		numero--;
	}
	return numero;
}
long toBase10(char digits[], int base, int len){
	// Takes in character array of digits (digits), base of the 
	// number (base), and the number of digits or length of the
	// number (len). It returns the number in base 10.
    long result = 0; // Resulting number in base 10
    for (int i=0; i< len; i++) {
        result = result*base;
        if (digits[i] <= '9' && digits[i] >= '0'){
        	result = result + (digits[i] - '0'); // Convert char to long int
        }
        else{ // Convert char to long int
        	if (digits[i] >= 'a' && digits[i] <= 'f' ){
        		result = result + (digits[i] - 'W'); 
        	}
        	else{ // Convert char to long int
        		result = result + (digits[i] - '7'); 
        	}
        }
    }
    return result;
}
void putSymbols(long num, int sign, int base){
	// Takes in incremented number (num), sign of the number (sign) and
	// the base of the number (base). The function displays the sign and 
	// symbols of the specified base.
	if (sign == 1 ){      // Negative number case
    		if(num != 0){
    			putchar('-');
    			// Put symbols for binary, octal, hex
    			if (base != 10)   
    			{	
					putchar('0');
					if (base == 2){
						putchar('b');
					}
					else if (base == 16){
						putchar('x');
					}
				}
    		}
    		else{  // 0 number case
    			putchar('0');
    			// Put symbols for binary and hex
    			if (base == 2){   
						putchar('b');
						putchar('0');
				}
				else if (base == 16){
						putchar('x');
						putchar('0');
				}
    		}
    }			
	else{   // Positive number case
		// Put symbols for binary, octal, hexadecimal
		if(base != 10){   
			putchar('0');
			if (base == 2){
				putchar('b');
			}
			else if (base == 16){
				putchar('x');
			}
		}
	}
}
void putDigits(long num, int base ){
	// Takes in incremented number (num) and base of the number (base)
	// and displays the digits in the specified base.
	int converted_number[64];
    char digits[16] ={'0', '1', '2', '3', '4', '5', '6',
                      '7','8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    int i =0;
    while(num != 0){  // Convert into different base
        converted_number[i] = num%base; // Note: number is written backwards
        num = num/base;
        i++;
    }
    i--;
    for(  ; i>=0; i--)  // Go backwards through array and display the digits
    {
       putchar(digits[converted_number[i]]);
    }
 }
bool isDigit(int c){
	// Takes in input character c and returns true if c is a digit
	if(c <= '9' && c >= '0'){
		return true;
	}
	return false;
}
bool isZero(int c){
	// Takes in input character c and returns true if c is 0
	if (c == '0'){
		return true;
	}
	return false;
}
bool isBinary(int c){
	// Takes in input character c, returns true if c is a binary digit
	if (c == '0' || c == '1'){
		return true;
	}
	return false;
}
bool isOctal(int c){
	// Takes in input character c, returns true if c is a octal digit
	for( int i = 0; i < 8; i++){
		if (i == (c- '0')){
			return true;
		}
	}
	return false;
}
bool isHex(int c){
	// Takes in input character c, returns true if it is hexadeimcal
 	char hex_digits[22] ={'0', '1', '2', '3', '4', '5', '6', '7',
 	                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 
 	                    'a', 'b', 'c', 'd', 'e', 'f'};
 	for (int i = 0; i < 22; i++)
 	{
 		if ( hex_digits[i] == c){
 			return true;
 		}
 	}
 	return false;
 }
bool checkSplice(int c){
	// Takes in input character c, returns true if there is a line splice
 	if(c == '\\'){
 		c = getchar();
 		if(c == '\n'){
 			return true;
 		}
 		ungetchar(c);
 	}
 	return false;
 }