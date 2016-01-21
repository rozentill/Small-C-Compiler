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
#include "paraQueue.h"

void translate(Node * root,ParaQueue * paraQueue) ;

void codeGen(Node * root,FILE * fout){

	//initialize paraQueue
	ParaQueue * paraQueue = (ParaQueue*)malloc(sizeof(ParaQueue));
	paraQueue->start = 0;
	paraQueue->end = 0;
	paraQueue->size = 10;
	paraQueue->num = 0;
	paraQueue->queue = (char **)malloc(sizeof(char *)*10);

	translate(root,paraQueue);

	//free memory allocated
	free(paraQueue->queue);
	free(paraQueue);
}

void translate(Node * root,ParaQueue * paraQueue){
	if(!strcmp(root->data,"program")){//program
		printf("program is fine\n");
		translate(root->children[0],paraQueue);

	}
	else if (!strcmp(root->data,"extdefs")) {//extdefs

		if (strcmp(root->children[0]->data,"empty")) {
			translate(root->children[0],paraQueue);
			translate(root->children[1],paraQueue);
		}

	}
	else if (!strcmp(root->data,"extdef")) {//extdef

		if (!strcmp(root->children[1]->data,"func")) {//spec func stmtblock
			translate(root->children[1],paraQueue);//func
			translate(root->children[2],paraQueue);//stmtblock
		}
		else{//spec extvars SEMI
			if(!strcmp(root->children[0]->children[0]->data,"TYPE")){//spec : TYPE

			}
			else if (!strcmp(root->children[0]->children[0]->data,"stspec")){//spec:stspec
				Node * stspec = root->children[0]->children[0];
				Node * extvars = root->children[1];

				if (stspec->childrenNum==5)//stspec:STRUCT opttag LC defs RC
				{
					printf("%%struct.%s = type { ",stspec->children[1]->children[0]->data);

					Node * defs = stspec->children[3];
					while (defs!=NULL) {
						printf("i32");
						if (defs->childrenNum==2)//defs:def defs
						{
							defs=defs->children[1];
						}
						else{
							defs = NULL;
						}
					}

					printf(" } \n");

					while(strcmp(extvars->children[0]->data,"empty")!=0&&extvars!=NULL) {
						printf("@%s = common global %%struct.%s zeroinitializer, align 4\n",extvars->children[0]->children[0]->children[0]->data,stspec->children[1]->children[0]->data);
						if (extvars->childrenNum==3) {
							extvars = extvars->children[2];
						}
						else{
							extvars = NULL;
						}
					}
				}
				else{//stspec:STRUCT ID
					@%s = common global %%struct.%s zeroinitializer, align 4\n
				}
			}
		}

	}
	else if (!strcmp(root->data,"func")) {//func

		printf("define i32 @%s(",root->children[0]->data);
		translate(root->children[2],paraQueue);//paras
		printf(") #0 {\n");

	}
	else if (!strcmp(root->data,"paras")) {//paras

		if (!strcmp(root->children[0]->data,"empty")) {//empty
		}
		else if (root->childrenNum == 3) {//para COMMA paras
			translate(root->children[0],paraQueue);
			printf(",");
			translate(root->children[2],paraQueue);
		}
		else{//para
			translate(root->children[0],paraQueue);
		}

	}
	else if (!strcmp(root->data,"para")) {//para ,only can be "int a" ,no "int [] a"

		printf("i32 %%%s",root->children[1]->children[0]->data);
		enqueue(paraQueue,root->children[1]->children[0]->data);

	}
	else if (!strcmp(root->data,"funcstmtblock")) {//funcstmtblock:LC defs stmts RC
		int paraNum=0;

		while (paraQueue->num>paraNum) {
			paraNum++;
			printf("  %%%d = alloca i32, align 4\n",paraNum);
		}

		paraNum = 0;

		while (paraQueue->end!=paraQueue->start) {
			paraNum++;
			printf("  store i32 %%%s, i32* %%%d, align 4\n",dequeue(paraQueue),paraNum);
		}

		translate(root->children[1],paraQueue);//defs
		translate(root->children[2],paraQueue);//stmts

		printf("}\n");
	}
	else if (!strcmp(root->data,"defs")) {//defs:def defs
		if(!strcmp(root->children[0]->data,"empty")){
			return;
		}
		else{
			translate(root->children[0],NULL);
			translate(root->children[1],NULL);
		}
	}
	else if (!strcmp(root->data,"def")) {//def 	:spec decs SEMI
		translate(root->children[1])
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


}
