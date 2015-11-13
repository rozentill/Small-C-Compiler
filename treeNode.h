#ifndef TREENODE_H
#define TREENODE_H
#include<stdio.h>

typedef struct Node{
	char * data;
	int childrenNum;
	struct Node ** children;
} Node; 

#endif
