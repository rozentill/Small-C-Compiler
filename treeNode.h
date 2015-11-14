#ifndef TREENODE_H
#define TREENODE_H
#include<stdio.h>

typedef struct treeNode{
	char * data;
	int childrenNum;
	struct treeNode ** children;
} Node; 

printRoot(Node * root){
	printf("%s\n",root->data);
}

printNodes(Node * node,int height){
	int i=height*4;
	while(i>0){
		printf("-");
		i--;
	}
	printf("|%s\n",node->data);
	int childNum=node->childrenNum;
	int j=0;
	for(;j<childNum;j++){
		printNodes(node->children[j],height+1);
	}
	free(node->children);
	free(node);
}

printParseTree(Node * root){
	printRoot(root);
	
	int rootChildNum=root->childrenNum;
	int i=0;
	
	for(;i<rootChildNum;i++){
		printNodes(root->children[i],1);
	}
	free(root->children);
	
}

#endif
