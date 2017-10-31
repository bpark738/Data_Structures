#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "btree.c"

void treeAdd(struct node * tree, char * key){
  struct node * newnode;

  printf("Adding %s\n", key);
  newnode = treeSearch(tree, key);
  if (newnode) {
    newnode->count++;
  } else {
    treeInsert(tree, key);
  }
}


int main(int argc, char **argv){

  printf("Welcome to btree!\n");

  struct node * root = makeNode("**root**");

  treeAdd(root, "a");
  printf("Tree height: %d Size: %d\n", treeHeight(root), treeSize(root));
  treeAdd(root, "b");
  printf("Tree height: %d Size: %d\n", treeHeight(root), treeSize(root));
  treeAdd(root, "z");
  printf("Tree height: %d Size: %d\n", treeHeight(root), treeSize(root));
  treeAdd(root, "d");

  treeAdd(root, "d");
  treeAdd(root, "b");
  treeAdd(root, "b");
  treeAdd(root, "b");


  printf("Tree height: %d Size: %d\n", treeHeight(root), treeSize(root));
  printTreePre(root);
  printf("\n");
  printTreeIn(root);
  printf("\n");
  printTreePost(root);
  printf("\n");

  treeDestroy(root);
}
