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


//initialization
int returnNum;

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
				Node * extvars = root->children[1];
				while (strcmp(extvars->children[0]->data,"empty")!=0&&extvars!=NULL) {//dec exist
					Node * dec = extvars->children[0];

					if (dec->childrenNum==1) {//dec:var

						if (dec->children[0]->childrenNum==1) {//var:ID
							printf("@%s = common global i32 0, align 4\n",dec->children[0]->children[0]->data);
						}

						else{//var:var LB INT RB assume only one-dimensinal array
							Node * id = dec->children[0]->children[0]->children[0];
							Node * num = dec->children[0]->children[2];
							printf("@%s = common global [ %d x i32] zeroinitializer, align %d\n",id->data,atoi(num->data),atoi(num->data)*2);
						}

					}
					else{//dec:var ASSIGNOP init
						Node * init = dec->children[2];
						if (dec->childrenNum==1) {//var :ID
							printf("@%s = global i32 %d, align 4\n",dec->children[0]->children[0]->data,atoi(init->children[0]->children[0]->data));//init :exp,exp:ID
						}
						else{//var:var LB INTEGER RB
							Node * id = dec->children[0]->children[0]->children[0];
							Node * num = dec->children[0]->children[2];

							printf("@%s = global [%d x i32] [",id->data,atoi(num->data));
							//init : LC args RC
							Node * args = init->children[1];
							while (args) {
								printf("i32 %d",atoi(args->children[0]->children[0]->data));
								if (args->childrenNum==3) {//args:exp COMMA args
									args = args->children[2];
								}
								else{
									args = NULL;
								}
							}
							printf("], align %d\n",atoi(num->data)*2);
						}

					}
				}

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
							printf(",");
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
					while(strcmp(extvars->children[0]->data,"empty")!=0&&extvars!=NULL) {
						printf("@%s = common global %%struct.%s zeroinitializer, align 4\n",extvars->children[0]->children[0]->children[0]->data,stspec->children[1]->data);
						if (extvars->childrenNum==3) {
							extvars = extvars->children[2];
						}
						else{
							extvars = NULL;
						}
					}
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
		Node * dec=root->children[1]->children[0];

		if (dec->childrenNum==1) {//dec : var
			if (dec->children[0]->childrenNum==1) {//var:ID
				printf("  %%%s = alloca i32 , align 4\n",dec->children[0]->children[0]->data);
			}
			else{//var : var LB INT RB
				Node * id = dec->children[0]->children[0]->children[0];
				Node * num = dec->children[0]->children[2];
				printf("  %%%s = alloca [ %d x i32], align 4\n",id->data,atoi(num->data));
			}
		}
		else{//dec:var ASSIGNOP init
			Node * init = dec->children[2];
			if (dec->children[0]->childrenNum==1) {//var:ID
				printf("  %%%s = alloca i32 , align 4\n",dec->children[0]->children[0]->data);
				printf("  store i32 %d, i32* %s, align 4\n",atoi(init->children[0]->children[0]->data),dec->children[0]->children[0]->data);
			}
			else{//var : var LB INT RB ***数组定义赋值***
				Node * id = dec->children[0]->children[0]->children[0];
				Node * num = dec->children[0]->children[2];
				printf("  %%%s = alloca [ %d x i32], align 4\n",id->data,atoi(num->data));
				Node * exps =
			}
		}
	}
	else if (!strcmp(root->data,"stmts")) {
		if (!strcmp(root->children[0]->data,"empty")) {
			return;
		}
		else{
			translate(root->children[0]);//stmt
			translate(root->children[1]);//stmts
		}
	}
	else if (!strcmp(root->data,"stmt")) {
		if (!strcmp(root->children[0]->data,"exps")) {
			translate(root->children[0]);
		}
		else if (!strcmp(root->children[0]->data,"stmtblock") {
			// entryDepth++;
			translate(root->children[0]);
			// entryDepth--;
		}
		else if(!strcmp(root->children[0]->data,"return"))
		{

        char * tmp;
        tmp = translate_exp(root->child[1]);
				printf("  %%%d = load i32, i32* %s, align 4\n",returnNum,tmp);
				printf("  ret i32 %%%d\n",returnNum);
        returnNum++;
		}
		else if (!strcmp(root->children[0]->data,"if") {//IF LP exp RP stmt estmt
				if (strcmp(root->children[5]->children[0]->data,"empty")!=0) //ESTMT not null
        {
            char* tmp = translate_exp(root->children[2]);

            if (!strcmp(root->children[2]->children[1],"."))//exp:exp DOT ID
            {
                char num[10];
                sprintf(num, "%d", returnNum++);
                char* tmpReg;
                strcpy(tmpReg,"%%r");
                strcat(tmpReg,num);
								printf("  %s = icmp ne i32 %s, 0\n",tmpReg,tmp);
                strcpy(tmp,tmpReg);
            }
						//stop here

            fprintf(fout,"  br i1 %s, label %%if%d.then, label %%if%d.else\n\n",tmp, ifNum, ifNum);

            fprintf(fout,"if%d.then:\n",ifNum);
            stmt(p->child->brother->brother->brother->brother);
            fprintf(fout,"  br label %%if%d.end\n\n",ifNum);

            fprintf(fout,"if%d.else:\n",ifNum);
            stmt(p->child->brother->brother->brother->brother->brother->child->brother);
            fprintf(fout,"  br label %%if%d.end\n\n",ifNum);

            fprintf(fout,"if%d.end:\n",ifNum);

            ifNum++;
        }
				else
        {

            char* tmp = (char*)malloc(sizeof(char)*60);
            tmp = Exps(p->child->brother->brother);

            if (!strcmp(p->child->brother->brother->child->brother->name,"."))//DOT, special case
            {
                char num[10];
                sprintf(num, "%d", rNum++);
                char* tmpReg = (char*)malloc(sizeof(char)*60);
                strcpy(tmpReg,"%r");
                strcat(tmpReg,num);

                fprintf(fout,"  %s = icmp ne i32 %s, 0\n",tmpReg,tmp);
                strcpy(tmp,tmpReg);
            }


            fprintf(fout,"  br i1 %s, label %%if%d.then, label %%if%d.end\n\n",tmp, ifNum, ifNum);

            fprintf(fout,"if%d.then:\n",ifNum);
            stmt(p->child->brother->brother->brother->brother);
            fprintf(fout,"  br label %%if%d.end\n\n",ifNum);

            fprintf(fout,"if%d.end:\n",ifNum);

            ifNum++;

        }//if end
		}
		else if (!strcmp(root->children[0]->data,"for") {
			/* code */
		}
	}
	// else if (/* condition */) {
	// 	/* code */
	// }
	// else if (/* condition */) {
	// 	/* code */
	// }


}

char * translate_exp(Node * root){

}
