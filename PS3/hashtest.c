#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"
#include "hash.c"


int main(){

  Hash h = HashCreate();

  HashInsert(h,"one");
  HashInsert(h,"two");
  HashInsert(h,"three");
  HashInsert(h,"four");
  HashInsert(h,"five");
  HashInsert(h,"six");
  
  HashDisplay(h);


  bool item;
  item = HashSearch(h, "one");

  if(item) {
    printf("Element found: %d\n", item);
  }else {
    printf("Element not found\n");
  }

  item = HashSearch(h, "ten");

  if(item){
    printf("Element found: %d\n", item);
  }else {
    printf("Element not found\n");
  }
  HashDestroy(h);
}