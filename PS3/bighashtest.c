#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"
#include "hash.c"

char * rand_string(char * str, int size)
{
  const char charset[26] = "abcdefghijklmnopqrstuvwxyz";
  if (size) {
    --size;
    for (size_t n = 0; n < size; n++) {
      int key = rand() % 26;
      str[n] = charset[key];
    }
    str[size] = '\0';
  }
  return str;
}

int main(){

  Hash h = HashCreate();

  int size = 5;
  char * str = malloc(size);
  for (int i=0; i < 1000; i++){
    HashInsert(h, rand_string(str, size));
  }
  free(str);

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