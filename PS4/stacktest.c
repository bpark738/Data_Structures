/*
 stacktest.c

 demonstrate stack functions
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "stack.c"


// struct token {
//   char * str;
//   int type;
//   double value;
// };


typedef struct token *Token;

// defined in stack.c
void * mymalloc(size_t s){
  void * ptr = malloc(s);
  memset(ptr, 0, s);
  if (debugflag) {
    printf("Mymalloc: %zu bytes: %p\n", s, ptr);
  }
  return ptr;
}


 // defined in stack.c 
 // and used in stackDestroy
void myfree(void * p){
  if (debugflag) {
    printf("Myfree: %p \n", p);
  }
  free(p);
}



char *copyString(char *str)
{
  int len = strlen(str);
  char *tmp = mymalloc(len + 1);
  if (tmp) {
    strcpy(tmp, str);
    //tmp[len] = '\0';
  }
  return tmp;
}




int main(int argc, char **argv){

  debugflag = true;
  if (argc == 1) {
    fprintf(stderr, "Usage: stacktest numbers+\n");
    exit(1);
  }

  Token tok;

  int stacksize = 10;
  stackT st;
  StackInit(&st, stacksize);
  char * strtmp;
  int val;
  
  for (int i=1; i < argc; i++) {
    tok = mymalloc(sizeof(struct token));
    tok->type = NUM;
    strtmp = argv[i];
    tok->str = copyString(strtmp);
    val = strtod(strtmp, NULL);
    tok->value = val;
    StackPush(&st, tok);
  }

  StackPrint(&st);

  tok = StackPop(&st);
  myfree(tok->str);
  myfree(tok);

  StackPrint(&st);

  StackDestroy(&st);
  
  return(0);
}