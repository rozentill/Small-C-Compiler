#ifndef TREENODE_H
#define TREENODE_H
#include<stdio.h>

typedef struct treeNode{
	char * data;
	int childrenNum;
	struct treeNode ** children;
} Node; 

#endif
