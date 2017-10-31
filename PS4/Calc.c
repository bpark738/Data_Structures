// This file is Calc.c which takes in input in prefix notation
// and uses the shunting algorithm to convert it to postfix notation
// and then evaluates it. The programmer name is Briton Park.

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "stack.c"
#define MAXSIZE 1024 // This is the maximum a stack can hold

double atof(const char *str);
bool is_numeric(char * s);
bool is_operator(char * s);
bool is_operator1(char* s);
bool is_operator2(char* s);
bool is_left_paren(char * s);
bool is_right_paren(char * s);
bool is_error(char *s);
char *strcpy(char *dest, const char *src);
int strcmp(const char *str1, const char *str2);

int main(int argc, char *argv[]){
   extern bool debugflag; // This is a boolean variable to check for -debug
   debugflag = false;
   if (argc == 2){
      if(strcmp(argv[argc-1], "-debug") == 0){
         debugflag = true;
      }
      else{
         fprintf(stderr, "Usage: Calc [-debug]\n");
         exit(0);
      }
   }
   else if(argc > 2){
      fprintf(stderr, "Usage: Calc [-debug]\n");
      exit(0);
   }
   stackT myStack; // This is used to represent the stack
   StackInit(&myStack, MAXSIZE);
   Token *Queue; // This Token pointer stores all the tokens from input
   // This token is used to represent the 1st argument when evaluating 
   // expressions.
   Token arg1; 
   int place = 0; // This int variable tracks the place of the Queue array
   char buffer[MAXSIZE]; // This char array is a buffer for taking in input
   char *token; // This char pointer stores the string of the input tokens
   int i; // This int variable stores the size of the token list

   while(fgets(buffer, MAXSIZE , stdin) != NULL)
   {
      buffer[strlen(buffer) -1] = '\0';
      printf("Input: %s",buffer);
      Token *Tok_list = malloc(sizeof(struct token)*(MAXSIZE));
      Queue = malloc(sizeof(struct token)*(MAXSIZE));
      place = 0;
      token = strtok(buffer, " ");
      i = 0;
      while( token != NULL ) // Create and place tokens in Queue
      {
         if(is_error(token)){
            printf("\n");
            StackDestroy(&myStack);
            for (int k = 0; k < i; i++){
               free(Tok_list[k]);
            }
            free(Tok_list);
            free(Queue);
            fprintf(stderr, "Fatal Error. Bad token: %s\n", token);
            exit(0);
         }
         Token T = malloc(sizeof(struct token));
         T->str = malloc(sizeof(char)*strlen(token) +1);
         strcpy(T->str, token);
         T->str[strlen(token)] = '\0';
         T->value = 0;
         if(is_numeric(token)){
            T->type = NUM;
            T->value = atof(token);
         }
         else if(is_operator1(token)){
            T->type = OP1;
         }
         else if(is_operator2(token)){
            T->type = OP2;
         }
         else if(is_left_paren(token)){
            T->type = LPAR;
         }
         else if (is_right_paren(token)){
            T->type = RPAR;
         }
         Tok_list[i] = T;
         i++;
         token = strtok(NULL," ");
      }
      printf("\n");
      if(debugflag){
         StackPrint(&myStack);
      }
      for (int j = 0; j< i; j++){ // Shunting yard algorithm implementation
         if(debugflag){
            printf("Token:%s: type: %d ",Tok_list[j]->str,Tok_list[j]->type);
            printf("value: %.2f\n",Tok_list[j]->value );
         }
         if(Tok_list[j]->type == NUM){
            Queue[place] = Tok_list[j];
            place++;
         }
         else if(Tok_list[j]->type == OP1){
            while(!StackIsEmpty(&myStack)){ 
               if (StackTop(&myStack)->type == OP1 || 
                  StackTop(&myStack)->type == OP2){
                  Queue[place] = StackPop(&myStack);
                  place++;
               }
               else{
                  break;
               }
            }
            StackPush(&myStack, Tok_list[j]);
         }
         else if (Tok_list[j]->type == OP2){
            while(!StackIsEmpty(&myStack)){ 
               if (StackTop(&myStack)->type == OP2){
                  Queue[place] = StackPop(&myStack);
                  place++;
               } 
               else{
                  break;
               }
            }
            StackPush(&myStack, Tok_list[j]);
         }
         else if(Tok_list[j]->type == LPAR){
           StackPush(&myStack, Tok_list[j]);
         }
         else if (Tok_list[j]->type == RPAR){
            free(Tok_list[j]->str);
            free(Tok_list[j]);
            bool lpar = false; // Flag for if left parenthesis comes up
            while(!StackIsEmpty(&myStack)){
               if (is_left_paren(StackTop(&myStack)->str)){
                  lpar = true;
                  Token free_this = StackPop(&myStack);
                  free(free_this->str);
                  free(free_this);
                  break;
               }   
               Queue[place] = StackPop(&myStack);
               place++;
            }
            if(!lpar){
               for (int k = j; k < i; k++){
                  free(Tok_list[k]);
               }
               for (int k = 0; k < place; k++){
                  free(Queue[k]);
               }
               StackDestroy(&myStack);
               free(Queue);
               fprintf(stderr, "Fatal error: missing left paran\n");
               exit(0);
            }
         }
         if(debugflag){
            StackPrint(&myStack);
         }
      }
      while(!StackIsEmpty(&myStack)){
         if(StackTop(&myStack)->type == LPAR || 
            StackTop(&myStack)->type == RPAR){
            fprintf(stderr, "Fatal error: mismatched paran\n");
            exit(0);
         }
         Queue[place] = StackPop(&myStack);
         place++;
      }
      if(debugflag){
         printf("OUTPUT:\n");
      }
      // This stores the token of the 2nd argument when evaluating functions
      Token arg2; 
      for(int j = 0; j<place; j++){ // Evaluate post fix notation
         if(debugflag){
            StackPrint(&myStack);
            printf("Token:%s: type: %d ", Queue[j]->str, Queue[j]->type);
            printf("value: %.2f\n", Queue[j]->value);
         }
         if(Queue[j]->type == NUM){
            StackPush(&myStack,Queue[j]);
         }
         else if(is_operator(Queue[j]->str)){
            if(myStack.count < 2){
               StackDestroy(&myStack);
               free(Tok_list);
               free(Queue);
               fprintf(stderr, "Fatal error: fewer than ");
               fprintf(stderr, "2 operands available.\n");
               exit(0);
            }
            else{
               arg1 = StackPop(&myStack);
               arg2 = StackPop(&myStack);
               if(strcmp(Queue[j]->str,"+") == 0){
                  arg1->value = arg2->value + arg1->value;
               }
               else if(strcmp(Queue[j]->str,"-") == 0){
                  arg1->value = arg2->value - arg1->value;
               }
               else if(strcmp(Queue[j]->str,"*") == 0){
                  arg1->value = arg2->value * arg1->value;
               }
               else if(strcmp(Queue[j]->str,"/") == 0){
                  arg1->value = arg2->value / arg1->value;
               }
               free(arg2->str);
               free(arg2);
               free(Queue[j]->str);
               free(Queue[j]);
               StackPush(&myStack, arg1);
            }
         }
      }
      if(myStack.count == 1){
         printf("Result: %.2f\n", StackTop(&myStack)->value);
      }
      else{
         StackDestroy(&myStack);
         free(Tok_list);
         free(Queue);
         fprintf(stderr, "Fatal error: Too many operands\n");
         exit(0);
      }
      Token free_this = StackPop(&myStack);
      free(free_this->str);
      free(free_this);
      free(Tok_list);
      free(Queue);
   }
   StackDestroy(&myStack);
}
// This function takes in a char pointer (s) and returns true if
// it is a left parenthesis and false otherwise.
bool is_left_paren(char * s){
   if(strcmp(s,"(") == 0){
      return true;
   }
   return false;
}
// This function takes in a char pointer (s) and 
// returns true if it is a right parenthesis and false otherwise.
bool is_right_paren(char * s){
   if(strcmp(s,")") == 0){
      return true;
   }
   return false;
}
// This function takes in a char pointer (s) and returns true
// if each char is a number or a decimal point.
// It returns false if at least one of the char isn't.
bool is_numeric(char * s){
   int dot_count = 0;
   for(int i = 0; s[i]!='\0'; i++){
      if(s[i] == '-' && i == 0 && strlen(s)!= 1){
         continue;
      }
      if(s[i] == '.'){ // count decimal
         dot_count++;
      }
      if( !(s[i] >= '0' && s[i] <= '9')  ){
         if( !(s[i] == '.' && dot_count == 1)){ 
            return false; // return false if dot appeared more than once
         }
      }
   }
   return true;
}
// This function takes in a char pointer (s) and returns true
// if it is a operator 1 type or operator 2 type and false otherwise.
bool is_operator(char * s){
   if (is_operator1(s) || is_operator2(s) ){
      return true;
   }
   return false;
}
// This function takes in a char pointer (s) and returns true
// if it is + or - symbols and false otherwise.
bool is_operator1(char * s){
   if (strcmp(s,"+") == 0 || strcmp(s,"-") == 0){
      return true;
   }
   return false;
}
// This function takes in a character pointer (s) and returns
// true if it is the * or / symbols and false otherwise.
bool is_operator2(char * s){ 
   if (strcmp(s,"*") == 0 || strcmp(s,"/") == 0){
      return true;
   }
   return false;
}
// This function takes in a char pointer (s) and returns true if it is not
// numeric, not a operator, and no parenthesis and false otherwise.
bool is_error(char *s){
   if(!is_numeric(s) && !is_operator(s) 
      && !is_right_paren(s) && !is_left_paren(s)){
      return true;
   }
   return false;
}
