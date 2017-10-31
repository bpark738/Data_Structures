#include <stdio.h>
#include <string.h>
 
const char hashTable[10][5] = {"", "", "abc", "def", "ghi", "jkl",
                               "mno", "pqrs", "tuv", "wxyz"};
 

void  printWordsUtil(int number [], int current_digit, char output[], int n)
{
    int i = current_digit;
    int end = 3;
    if (number[current_digit] == 9){
        end = 4;
    }

    for(int j = 0; j< end; j++){
        output[i] = hashTable[number[i]][j];  
        
        if(i < n){
            printWordsUtil(number, i+1, output, n);
        }
        if (i == n){
            printf("%s\n", output);
        }
    } 
}
 
int main(void)
{
    int number[] = {9,2,2};
    int n = 3;
    char result[n+1];
    result[n] ='\0';
    printWordsUtil(number, 0, result, n-1);
    return 0;
}