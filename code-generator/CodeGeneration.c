/*
***************************************
Code Generation Function Implementation
***************************************
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "treeNode.h"
#include "paraQueue.c"

ParaQueue paraQueue;

void translate(Node * root) ;

void codeGen(Node * root,FILE * fout){

	translate(root);

}

void translate(Node * root){
	if(!strcmp(root->data,"program")){//program

		translate(root);

	}
	else if (!strcmp(root->data,"extdefs")) {//extdefs

		if (strcmp(root->children[0]->data,"empty")) {
			translate(root->children[0]);
			translate(root->children[1]);
		}

	}
	else if (!strcmp(root->data,"extdef")) {//extdef

		if (!strcmp(root->children[1]->data,"func")) {//spec func stmtblock
			translate(root->children[1]);//func
			translate(root->children[2]);//stmtblock
		}
		else{//spec extvars SEMI
			// translate
		}

	}
	else if (!strcmp(root->data,"func")) {//func

		printf("define i32 @%s(",root->children[0]->data);
		translate(root->children[2]);//paras
		printf(") #0 {\n");

	}
	else if (!strcmp(root->data,"paras")) {//paras

		if (!strcmp(root->children[0]->data,"empty")) {//empty
		}
		else if (root->childrenNum == 3) {//para COMMA paras
			translate(root->children[0]);
			printf(",");
			translate(root->children[2]);
		}
		else{//para
			translate(root->children[0]);
		}

	}
	else if (!strcmp(root->data,"para")) {//para ,only can be "int a" ,no "int [] a"

		printf("i32 %%%s",root->children[1]->children[0]->data);
		paraQueue.enqueue(root->children[1]->children[0]->data);

	}
	else if (!strcmp(root->data,"funcstmtblock")) {
		int paraNum=0;
		while (paraQueue.num>paraNum) {
			paraNum++;
			printf("%%%d = alloca i32, align 4\n",paraNum);
		}
		paraNum = 0;
		while (paraQueue.end!=paraQueue.start) {
			paraNum++;
			printf("store i32 %%%s, i32* %%%d, align 4\n",paraQueue.dequeue(),paraNum);
		}
	}
	// else if (/* condition */) {
	// 	/* code */
	// }
	// else if (/* condition */) {
	// 	/* code */
	// }
	// else if (/* condition */) {
	// 	/* code */
	// }
	// else if (/* condition */) {
	// 	/* code */
	// }
	// else if (/* condition */) {
	// 	/* code */
	// }
	// else if (/* condition */) {
	// 	/* code */
	// }


}
