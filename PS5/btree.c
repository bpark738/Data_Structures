// This file is btree.c which implements the
// functions in btree.h
// Programmer: Briton Park

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "btree.h"

struct node *makeNode(char * key){
	struct node *newNode;
	newNode = malloc(sizeof(struct node));
	assert(newNode);

	newNode->key = malloc(strlen(key)+1);
	// set variables
	newNode->key[strlen(key)] = '\0';
	strcpy(newNode->key, key);
	newNode->count = 1;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->next = NULL;
	newNode->height = 0;
	newNode->parent = NULL;
	return newNode;
}

int treeSize(struct node *root){
	if(root == NULL){
		return 0;
	}
	else{
		return 1 + treeSize(root->left) + treeSize(root->right);
	}
}

int treeHeight(struct node *root){
	int lh;
	int rh;
	if(root ==NULL){
		return -1;
	}
	else{
		lh = treeHeight(root->left);
		rh = treeHeight(root->right);
		if(lh>rh){
			return 1 + lh;
		}
		else{
			return 1 + rh;
		}
	}
}

struct node * treeSearch(struct node *root, char * target){
	if( !root){
		return NULL;
	}
	if(strcmp(root->key, target) == 0){ // found
		return root;
	}
	else if (strcmp(root->key, target) > 0){
		return treeSearch(root->left, target);
	}
	else{
		return treeSearch(root->right, target);
	}
}

void treeInsert(struct node *root, char * key){
	if(root != NULL){
		struct node *newNode;
		newNode = makeNode(key);
		for(;;) {
        	if(strcmp(root->key, key) > 0) {
            	if(root->left) {
                	root = root->left;
            	}
            	else {
                	root->left = newNode;
                	return;
            	}
        	}
        	else {
            	if(root->right) {
                	root = root->right;
            	}
            	else {
                	root->right = newNode;
                	return;
            	}
        	}
    	}
	}
}

void printTreePre(struct node * tree){
  if(tree!= NULL){
  	printf("%s [%d / %d] ", tree->key, tree->count, treeHeight(tree));
  	printTreePre(tree->left);
  	printTreePre(tree->right);
  } 
}

void printTreeIn(struct node * tree){
	if(tree!= NULL){
		printTreeIn(tree->left);
		printf("%s [%d / %d] ", tree->key, tree->count, treeHeight(tree));
		printTreeIn(tree->right);
	}
}

void printTreePost(struct node * tree){
	if(tree!= NULL){
		printTreePost(tree->left);
		printTreePost(tree->right);
		printf("%s [%d / %d] ", tree->key, tree->count, treeHeight(tree));
	}
}

void treeDestroy(struct node * tree){
	if (tree != NULL){
		treeDestroy(tree->right);
		treeDestroy(tree->left);
		free(tree->key);
		free(tree);
	}
}
