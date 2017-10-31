#include <stdbool.h>

/*  
 * This is the interface for a stack of tokens.
 * This is the type of the objects entered in the stack.
 */

struct token {
  int type;
  double value;
  char * str;
};

typedef struct token *Token;

// enumerated types for tokens:
enum {NUM, OP1, OP2, LPAR, RPAR};

// global variable for debugging
bool debugflag;

/*
 * Type: stackT
 * --------------
 * This is the type for a stack, i.e., it is a type that
 * holds the information necessary to keep track of a stack.
 * It has a pointer `contents' to a dynamically-allocated
 * array (used to hold the contents of the stack), an integer
 * `maxSize' that holds the size of this array (i.e., the
 * maximum number of things that can be held in the stack),
 * and another integer `top,' which stores the array index of
 * the element at the top of the stack.
 * count keeps track of how many items are on the stack.
 */

typedef struct {
  Token *contents;
  int maxSize;
  int top;
  int count;
} stackT;

/*
 * Function: StackInit
 * Usage: StackInit(&stack, maxSize);
 * -------------------------
 * A new stack variable is initialized.  The initialized
 * stack is made empty.  MaxSize is used to determine the
 * maximum number of character that can be held in the
 * stack.
 */

void StackInit(stackT *stackP, int maxSize);

/* Function: StackDestroy
 * Usage: StackDestroy(&stack);
 * -----------------------
 * This function frees all memory associated with the stack.
 * The `stack' variable may not be used again unless
 * StackInit(&stack, maxSize) is first called on the stack.
 */

void StackDestroy(stackT *stackP);

/*
 * Functions: StackPush, StackPop
 * Usage: StackPush(&stack, element); element = StackPop(&stack);
 * --------------------------------------------
 * These are the fundamental stack operations that add an element to
 * the top of the stack and remove an element from the top of the stack.
 * A call to StackPop on an empty stack or to StackPush on a full stack
 * is an error.  Make use of StackIsEmpty()/StackIsFull() (see below)
 * to avoid these errors.
 */

void StackPush(stackT *stackP, Token element);
Token StackPop(stackT *stackP);

/*
 * Functions: StackIsEmpty, StackIsFull
 * Usage: if (StackIsEmpty(&stack)) ...
 * -----------------------------------
 * These return a true value if the stack is empty
 * or full (respectively).
 */

bool StackIsEmpty(stackT *stackP);
bool StackIsFull(stackT *stackP);

// number of elements on the stack
int StackCount(stackT *stackP);

// for debugging
void StackPrint(stackT *stackP);

// Token at the top of stack
Token StackTop(stackT *stackP);