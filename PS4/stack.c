// This file contains the implementations of the methods
// outlined in the header file stack.h. The programmer
// name is Briton Park.

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "stack.h"
// This function takes in a pointer to a stackT type (stackP)
// and the maximum size of the stackT (maxSize) and initializes
// the stackT fields. It mallocs memory for contents and
// initializes each to be 0.
void StackInit(stackT *stackP, int maxSize){
	stackP->top = 0;
	stackP->count = 0;
	stackP->maxSize = maxSize;
	stackP->contents = malloc(sizeof(Token*)*maxSize);
	for(int j = 0; j<maxSize; j++){
		stackP->contents[j] = 0;
	}
}
// This function takes in a pointer to a stackT type (stackP) and
// frees its occupied contents.
void StackDestroy(stackT *stackP){
	for(int i =0; i<stackP->count; i++){
		free(stackP->contents[i]->str);
		free(stackP->contents[i]);
	}
	free(stackP->contents);
}
// This function takes in a pointer to a stackT type (stackP) and a
// Token type (element). If the stack is full then it returns an error
// but pushes the Token onto the stack if there is space and increments
// the count variable
void StackPush(stackT *stackP, Token element){
	if(!StackIsFull(stackP)){
		stackP->contents[stackP->count] = element;
		if (stackP->count != 0){
			stackP->top++;
		}
		stackP->count++;
	}
	else{
		fprintf(stderr, "Stack is full\n");
		exit(0);
	}
}
// This funciton takes in a pointer to a stackT type (stackP) and if
// the stack is empty, it prints an error and exits. If it is not empty
// it mallocs a Token (S) and copies the contents of the top of the stack
// into S and frees the top of the stack. It then returns S
Token StackPop(stackT *stackP){
	if (!StackIsEmpty(stackP)){
		Token S = malloc(sizeof(struct token));
		*S = *stackP->contents[stackP->top];
		free(stackP->contents[stackP->top]);
		stackP->contents[stackP->top] = NULL;
		stackP->count--;
		if(stackP->top != 0){
			stackP->top--;
		}
		return S;
	}
	fprintf(stderr, "Stack is empty\n");
	exit(0);
}
// This funciton takes in a pointer to a stackT type (stackP) and returns
// true if the count is 0 and false otherwise.
bool StackIsEmpty(stackT *stackP){
	if (stackP->count == 0){
		return true;
	}
	return false;
}
// This funciton takes in a pointer to a stackT type (stackP) and returns 
// true if the count of the stack is equal to its maxSize and false otherwise.
bool StackIsFull(stackT *stackP){
	if (stackP->maxSize == stackP->count){
		return true;
	}
	return false;
}
// This funciton takes in a pointer to a stackT type (stackP) and returns
// the count of the stack.
int StackCount(stackT *stackP){
	return stackP->count;
}
// This funciton takes in a pointer to a stackT type (stackP) and prints
// its type and content's str and value as a tuple.
void StackPrint(stackT *stackP){
	printf("Stack: size: %d :",stackP->count);
	for(int k = stackP->count ; k > 0; k--){
        printf("[%d %s %.1f] ", stackP->contents[k-1]->type, stackP->contents[k-1]->str, stackP->contents[k-1]->value);
    }
    printf("\n");
}
// This funciton takes in a pointer to a stackT type (stackP) and returns
// the top of the stack.
Token StackTop(stackT *stackP){
	return stackP->contents[stackP->top];
}