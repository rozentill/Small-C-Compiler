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
#include "avlTree.h"


//initialization
int returnNum;
int ifNum;
int forNum;
int callNum;
int arridxNum;
int ifLoad = 1;

AVLTree * symbolRoot;

char * translate(Node * root,ParaQueue * paraQueue) ;
char * translate_exp(Node * root);
void argsFunc(Node* root,ParaQueue * paraQueue);
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

char * translate(Node * root,ParaQueue * paraQueue){
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
							
							//symbol tree
							PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
							tmp->name = dec->children[0]->children[0]->data;
							tmp->type='g';
							symbolRoot=Insert(tmp,symbolRoot);

						}

						else{//var:var LB INT RB assume only one-dimensinal array
							Node * id = dec->children[0]->children[0]->children[0];
							Node * num = dec->children[0]->children[2];
							printf("@%s = common global [ %d x i32] zeroinitializer, align %d\n",id->data,atoi(num->data),atoi(num->data)*2);
							
							//symbol tree
							PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
							tmp->name = id->data;
							tmp->arrSize = atoi(num->data);
							tmp->type='g';
							symbolRoot=Insert(tmp,symbolRoot);
						}

					}
					else{//dec:var ASSIGNOP init
						Node * init = dec->children[2];
						if (dec->childrenNum==1) {//var :ID
							printf("@%s = global i32 %d, align 4\n",dec->children[0]->children[0]->data,atoi(init->children[0]->children[0]->data));//init :exp,exp:ID
							
							//symbol tree
							PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
							tmp->name = dec->children[0]->children[0]->data;
							tmp->type='g';
							symbolRoot=Insert(tmp,symbolRoot);
						}
						else{//var:var LB INTEGER RB
							Node * id = dec->children[0]->children[0]->children[0];
							Node * num = dec->children[0]->children[2];

							printf("@%s = global [%d x i32] [",id->data,atoi(num->data));
							
							//symbol tree
							PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
							tmp->name = id->data;
							tmp->arrSize = atoi(num->data);
							tmp->type='g';
							symbolRoot=Insert(tmp,symbolRoot);

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
					
					int strMem=0;
					printf("%%struct.%s = type { ",stspec->children[1]->children[0]->data);

					Node * defs = stspec->children[3];
					while (defs!=NULL) {
						printf("i32");
						
						//symbol tree
						PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
						tmp->name = defs->children[0]->children[1]->children[0]->children[0]->children[0]->data;
						tmp->strMem = strMem++;
						symbolRoot=Insert(tmp,symbolRoot);

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
						
						//symbol tree
						PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
						tmp->name = extvars->children[0]->children[0]->children[0]->data;
						tmp->strName = stspec->children[1]->data;
						tmp->type='g';
						symbolRoot=Insert(tmp,symbolRoot);

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
		//symbol tree
		PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
		tmp->name = root->children[1]->children[0]->data;
		tmp->type='a';
		symbolRoot=Insert(tmp,symbolRoot);

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
		printf("hello\n");
		if(!strcmp(root->children[0]->data,"empty")){
			return;
		}
		else{
			translate(root->children[0],paraQueue);
			translate(root->children[1],paraQueue);
		}
	}
	else if (!strcmp(root->data,"def")) {//def 	:spec decs SEMI
		Node * dec=root->children[1]->children[0];

		if (dec->childrenNum==1) {//dec : var
			if (dec->children[0]->childrenNum==1) {//var:ID
				printf("  %%%s = alloca i32 , align 4\n",dec->children[0]->children[0]->data);

				//symbol tree
				PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
				tmp->name = dec->children[0]->children[0]->data;
				tmp->type='l';
				symbolRoot=Insert(tmp,symbolRoot);


			}
			else{//var : var LB INT RB
				Node * id = dec->children[0]->children[0]->children[0];
				Node * num = dec->children[0]->children[2];
				printf("  %%%s = alloca [ %d x i32], align 4\n",id->data,atoi(num->data));

				//symbol tree
				PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
				tmp->name = id->data;
				tmp->type='l';
				tmp->arrSize = atoi(num->data);
				symbolRoot=Insert(tmp,symbolRoot);
			}
		}
		else{//dec:var ASSIGNOP init
			Node * init = dec->children[2];
			if (dec->children[0]->childrenNum==1) {//var:ID
				printf("  %%%s = alloca i32 , align 4\n",dec->children[0]->children[0]->data);
				printf("  store i32 %d, i32* %s, align 4\n",atoi(init->children[0]->children[0]->data),dec->children[0]->children[0]->data);
			
				//symbol tree
				PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
				tmp->name = dec->children[0]->children[0]->data;
				tmp->type='l';
				symbolRoot=Insert(tmp,symbolRoot);

			}
			else{//var : var LB INT RB ***数组定义赋值***
				Node * id = dec->children[0]->children[0]->children[0];
				Node * num = dec->children[0]->children[2];
				printf("  %%%s = alloca [ %d x i32], align 4\n",id->data,atoi(num->data));

				//symbol tree
				PAVLNode * tmp = (PAVLNode*)malloc(sizeof(PAVLNode));
				tmp->name = id->data;
				tmp->type='l';
				tmp->arrSize = atoi(num->data);
				symbolRoot=Insert(tmp,symbolRoot);

				char * arrName = id->data;
				int arrSize = tmp->arrSize;
				int arrPtr;
				if (init->childrenNum==1)
				{
					char * val = translate_exp(init->children[0]);
					printf("  %%arrayidx%d = getelementptr inbounds [%d x i32], [%d x i32]* %s, i64 0, i64 %d\n",arridxNum,arrSize,arrSize,arrName,arrPtr);
        			printf("  store i32 %s, i32* %%arrayidx%d, align 4\n",val,arridxNum);
        			arridxNum++;
        			arrPtr = 0;
				}
				else{
					Node * args = init->children[2];
					while(args){
						char * val = translate_exp(init->children[0]);
						printf("  %%arrayidx%d = getelementptr inbounds [%d x i32], [%d x i32]* %s, i64 0, i64 %d\n",arridxNum,arrSize,arrSize,arrName,arrPtr);
	        			printf("  store i32 %s, i32* %%arrayidx%d, align 4\n",val,arridxNum);
	        			arridxNum++;
	        			arrPtr++;
	        			if (args->childrenNum==3)
	        			{
		        			args = args->children[2];
	        			}
	        			else{
	        				args=NULL;
	        			}
					}
				}

				arrSize = 0;


			}
		}
	}
	else if (!strcmp(root->data,"stmts")) {
		if (!strcmp(root->children[0]->data,"empty")) {
			return NULL;
		}
		else{
			translate(root->children[0],paraQueue);//stmt
			translate(root->children[1],paraQueue);//stmts
		}
	}
	else if (!strcmp(root->data,"stmt")) {
		if (!strcmp(root->children[0]->data,"exps")) {
			translate(root->children[0],paraQueue);
		}
		else if (!strcmp(root->children[0]->data,"stmtblock")) {
			// entryDepth++;
			translate(root->children[0],paraQueue);
			// entryDepth--;
		}
		else if(!strcmp(root->children[0]->data,"return"))
		{

        		char * tmp;
        		tmp = translate(root->children[1],NULL);
				printf("  %%%d = load i32, i32* %s, align 4\n",returnNum,tmp);
				printf("  ret i32 %%%d\n",returnNum);
        		returnNum++;
		}
		else if (!strcmp(root->children[0]->data,"if")) {//IF LP exp RP stmt estmt
			if (strcmp(root->children[5]->children[0]->data,"empty")!=0) //ESTMT not null
	        {
	            char* tmp = translate_exp(root->children[2]);

	            if (!strcmp(root->children[2]->children[1]->data,"."))//exp:exp DOT ID
	            {
	                char num[10];
	                sprintf(num, "%d", returnNum++);
									char* tReg;
	                strcpy(tReg,"%%r");
	                strcat(tReg,num);
									printf("  %s = icmp ne i32 %s, 0\n",tReg,tmp);
	                strcpy(tmp,tReg);
	            }


	            printf("  br i1 %s, label %%if%d.then, label %%if%d.else\n\n",tmp, ifNum, ifNum);

	            printf("if%d.then:\n",ifNum);
							translate(root->children[4],paraQueue);
	            printf("  br label %%if%d.end\n\n",ifNum);

	            printf("if%d.else:\n",ifNum);
							translate(root->children[5]->children[1],paraQueue);
	            printf("  br label %%if%d.end\n\n",ifNum);

	            printf("if%d.end:\n",ifNum);

	            ifNum++;
	        }
			else
	        {

	            char* tmp=translate_exp(root->children[2]);

	            if (!strcmp(root->children[2]->children[1]->data,"."))//DOT, special case
	            {
	                char num[10];
	                sprintf(num, "%d", returnNum++);
	                char* tReg;
	                strcpy(tReg,"%%r");
	                strcat(tReg,num);
									printf("  %s = icmp ne i32 %s, 0\n",tReg,tmp);
	                strcpy(tmp,tReg);
	            }


	            printf("  br i1 %s, label %%if%d.then, label %%if%d.end\n\n",tmp, ifNum, ifNum);

	            printf("if%d.then:\n",ifNum);
							translate(root->children[4],NULL);
	            printf("  br label %%if%d.end\n\n",ifNum);
							printf("if%d.end:\n",ifNum);

	            ifNum++;

	        }//if end
		}
		else if (!strcmp(root->children[0]->data,"for")) {//stmt:FOR LP exps SEMI exps SEMI exps RP stmt
				//store i32 0, i32* %i, align 4
        //br label %for.cond
				translate(root->children[2],paraQueue);
        printf("  br label %%for%d.cond\n\n",forNum);
        printf("for%d.cond:\n",forNum);
        char* tmp ;

				Node * expsMiddle = root->children[4];

				// if(Exp2->child->brother == NULL)//ERROR checking.
				// {
				// 	fprintf(fout,"line%d:error! It will loop forever in the for(;;)!",p->Line);
				// 	exit(-1);
				// }

        tmp = translate(expsMiddle,paraQueue);

        if (!strcmp(expsMiddle->children[1]->data,"arrs")) //special case, ID ARRS
        {
            //%cmp = icmp sgt i32 %0, 16
            printf("  %%r%d = icmp ne i32 %s, 0\n",returnNum,tmp);
            printf("  br i1 %%r%d, label %%for%d.body, label %%for%d.end\n\n",returnNum,forNum,forNum);
            returnNum++;
        }
        else printf("  br i1 %s, label %%for%d.body, label %%for%d.end\n\n",tmp,forNum,forNum);

        printf("for%d.body:\n",forNum);
				translate(root->children[8],paraQueue);//stmt

        printf("  br label %%for%d.inc\n\n",forNum);
        printf("for%d.inc:\n",forNum);

				translate(root->children[6],paraQueue);

        printf("  br label %%for%d.cond\n\n",forNum);
        printf("for%d.end:\n",forNum);

        forNum++;
		}
		else if (!strcmp(root->children[0]->data,"write")) {//stmt:WRITE  LP exp RP SEMI
						char* tmp = translate_exp(root->children[2]);
            int trans;
            if (strlen(tmp)>1 && (tmp[0]=='0' || (tmp[0]=='-' && tmp[1]=='0')))
            {
                trans = strtol(tmp,NULL,0);
                printf("  %%call%d = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32 %d)\n",callNum,trans);
                callNum++;
                printf("  %%call%d = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8]* @.str1, i32 0, i32 0))\n",callNum);
                callNum++;
            }
            else
            {
                printf("  %%call%d = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32 %s)\n",callNum,tmp);
                callNum++;
                printf("  %%call%d = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8]* @.str1, i32 0, i32 0))\n",callNum);
                callNum++;
            }
		}
		else{//read
						char* tmp;
            ifLoad = 0;
            tmp = translate_exp(root->children[2]);
            ifLoad = 1;

            printf("  %%call%d = call i32 (i8*, ...)* @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8]* @.str, i32 0, i32 0), i32* %s)\n",callNum,tmp);
            callNum++;
		}
	}
	else if (!strcmp(root->data,"exps")) {//exps
		if (!strcmp(root->children[0]->data,"empty")) {//exps:empty
			return NULL;
		}
		else {
			return translate_exp(root->children[0]);//exp
		}
	}
	// else if (/* condition */) {
	// 	/* code */
	// }
	return NULL;

}

char * translate_exp(Node * root){
		if (root->childrenNum==1) //exp:INT
    {
        return root->children[0]->data;
    }
		else if (!strcmp(root->children[0]->data,"++")) //++
    {

        char* op ;
        ifLoad = 0;
        op = translate_exp(root->children[1]);
        ifLoad = 1;

        printf("  %%r%d = load i32, i32* %s, align 4\n",returnNum,op);
        printf("  %%r%d = add nsw i32 %%r%d, 1\n",returnNum+1,returnNum);
        printf("  store i32 %%r%d, i32* %s, align 4\n",returnNum+1,op);

        returnNum+=2;
        return NULL;
    }
		else if (!strcmp(root->children[0]->data,"--")) //--
    {

        char* op ;
        ifLoad = 0;
        op = translate_exp(root->children[1]);
        ifLoad = 1;

        printf("  %%r%d = load i32, i32* %s, align 4\n",returnNum,op);
        printf("  %%r%d = sub nsw i32 %%r%d, 1\n",returnNum-1,returnNum);
        printf("  store i32 %%r%d, i32* %s, align 4\n",returnNum-1,op);

        returnNum+=2;
        return NULL;
    }

		else if (!strcmp(root->children[0]->data,"-")) //-
    {

        char* op ;
        ifLoad = 0;
        op = translate_exp(root->children[1]);
        ifLoad = 1;

        printf("  %%r%d = load i32, i32* %s, align 4\n",returnNum,op);
        printf("  %%r%d = sub nsw i32 0, %%r%d\n",returnNum+1,returnNum);

        char num[10];
        sprintf(num, "%d", returnNum+1);
        char* tReg;
        strcpy(tReg,"%%r");
        strcat(tReg,num);

        returnNum+=2;
        return tReg;
    }
	else if (!strcmp(root->children[0]->data,"!")) //!
    {

        char* op = translate_exp(root->children[1]);

        char num[10];
        sprintf(num, "%d", returnNum++);
        char* tmpReg = (char*)malloc(sizeof(char)*60);
				char* tmpReg1 = (char*)malloc(sizeof(char)*60);
				char* retReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);
        printf("  %s = icmp ne i32 %s, 0\n",tmpReg,op);

				strcpy(tmpReg1,tmpReg);
        strcat(tmpReg1,"lnot");
				printf("  %s = xor i1 %s, true\n",tmpReg1,tmpReg);
				return tmpReg1;

    }
	else if (!strcmp(root->children[0]->data,"=")) //EXP->EXP ASSIGNOP EXP
    {

        char* op2 = translate_exp(root->children[2]);

        ifLoad = 0;
        char* op1 = translate_exp(root->children[0]);
        ifLoad = 1;

        printf("  store i32 %s, i32* %s, align 4\n",op2,op1);
        return NULL;

    }
    else if (!strcmp(root->children[0]->data,"(")) //LP EXP RP
    {
        return translate_exp(root->children[1]);
    }
	else if (!strcmp(root->children[1]->data,"arrs")) //ID arrs
    {
        //printf("%s, %c",symTable[0][0]->word,symTable[0][0]->type);
        //return symTable[0][0]->word;

        Node * arrs = root->children[1];
        if (arrs->childrenNum==1) //ARRS : empty, ID case
        {
            char * tmp= (char*)malloc(sizeof(char)*60);
	    	char* tmp1;
            Node * id = root->children[0];
	    	tmp1 = id->data;
			PAVLNode * tmpNode;
			tmpNode = Find(tmp1,symbolRoot);

			int i =0 ;
            switch (tmpNode->type)
            {
                case 'g':
					for (i=strlen(tmp1);i>=0;i--) tmp[i+1] = tmp1[i];
                	tmp[0] = '@';
                	break;

                case 'l':
                	tmp[0] = '%';
					strcat(tmp,tmp1);
                	break;

                case 'a':
                	tmp[0] = '%';
					strcat(tmp,tmp1);
              	  	strcat(tmp,".addr");
                	break;
            }
	    
            if (ifLoad)
            {
                char num[10];
                sprintf(num, "%d", returnNum++);
                char* tmpReg = (char*)malloc(sizeof(char)*60);
                strcpy(tmpReg,"%%r");
                strcat(tmpReg,num);

                printf("  %s = load i32, i32* %s, align 4\n",tmpReg,tmp);
                return tmpReg;
            }
            else return tmp;
        }
        else //we need to return arrindex
        {
            char* tmp = (char*)malloc(sizeof(char)*60);
            Node * nodeId = root->children[0];
            strcpy(tmp,nodeId->data);

            char* arrsIndex = (char*)malloc(sizeof(char)*60);
            if (ifLoad==0)
            {
                ifLoad = 1;
                arrsIndex = translate_exp(root->children[1]->children[1]); //what we obtained could be register or INT
                ifLoad = 0;
            }
            else arrsIndex = translate_exp(root->children[1]->children[1]);

            char* ret = (char*)malloc(sizeof(char)*60);
            strcpy(ret,"%%arrayidx");

            char num[10];
            sprintf(num, "%d", arridxNum++);
            strcat(ret,num);

            PAVLNode * tmpNode;
			tmpNode = Find(tmp,symbolRoot);

			int i;
            
            switch (tmpNode->type)
            {
                case 'g':
                for (i=strlen(tmp);i>=0;i--) tmp[i+1] = tmp[i];
                tmp[0] = '@';
                break;

                case 'l':
                for (i=strlen(tmp);i>=0;i--) tmp[i+1] = tmp[i];
                tmp[0] = '%';
                break;

                case 'a':
                for (i=strlen(tmp);i>=0;i--) tmp[i+1] = tmp[i];
                tmp[0] = '%';
                strcat(tmp,".addr");
                break;
            }

            //%arrayidx4 = getelementptr inbounds [2 x i32]* %d, i32 0, i32 1
            printf("  %s = getelementptr inbounds [%d x i32]* %s, i32 0, i32 %s\n",ret,tmpNode->arrSize,tmp,arrsIndex);

            if (ifLoad)
            {
                char num[10];
                sprintf(num, "%d", returnNum++);
                char* tmpReg = (char*)malloc(sizeof(char)*60);
                strcpy(tmpReg,"%%r");
                strcat(tmpReg,num);

                printf("  %s = load i32, i32* %s, align 4\n",tmpReg,ret);
                return tmpReg;
            }
            else return ret;
        }
    }

    else if (!strcmp(root->children[1]->data,".")) ////EXP->EXP DOT THEID
    {
        //%0 = load i32* getelementptr inbounds (%struct.doubleO* @T, i32 0, i32 0), align 4
        Node * nodeId = root->children[0]->children[0];
		
		PAVLNode * tmpNode = Find(nodeId->data,symbolRoot);
        
        char* op1 = (char*)malloc(sizeof(char)*200);
        strcpy(op1,nodeId->data);

        char* opStr = (char*)malloc(sizeof(char)*200);
        strcpy(opStr,tmpNode->strName); //opStr, doubleO


        nodeId = root->children[2];


        tmpNode = Find(nodeId->data,symbolRoot);
		
        int op2 = tmpNode->strMem; //op2, 0

        char* ret = (char*)malloc(sizeof(char)*200);
        strcpy(ret,"getelementptr inbounds (%struct.");
        strcat(ret,opStr);
        strcat(ret,"* @");
        strcat(ret,op1);
        strcat(ret,", i32 0, i32 ");
        char indTmp = '0'+op2;
        char* ind = (char*)malloc(sizeof(char)*70); ind[0] = indTmp; ind[1] = '\0';
        strcat(ret,ind);
        strcat(ret,")");

        if (ifLoad)
        {
            char num[10];
            sprintf(num, "%d", returnNum++);
            char* tmpReg = (char*)malloc(sizeof(char)*200);
            strcpy(tmpReg,"%%r");
            strcat(tmpReg,num);

            printf("  %s = load i32, i32* %s, align 4\n",tmpReg,ret);
            return tmpReg;
        }
        else return ret;
    }
    else if (!strcmp(root->children[1]->data,"==")) //EXP->EXP == EXP
    {
        
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        char num[10];
        sprintf(num, "%d", returnNum++);
		
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);
        printf("  %s = icmp eq i32 %s, %s\n",tmpReg,op1,op2);
		
		return tmpReg;
        
    }
    else if (!strcmp(root->children[1]->data,">")) //EXP->EXP GREATER EXP
    {
        
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        char num[10];
        sprintf(num, "%d", returnNum++);
		
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);
        printf("  %s = icmp sgt i32 %s, %s\n",tmpReg,op1,op2);
		return tmpReg;
        
    }
    else if (!strcmp(root->children[1]->data,"<")) //EXP->EXP LESS EXP
    {
        //%cmp = icmp sgt i32 %0, 16
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        char num[10];
        sprintf(num, "%d", returnNum++);
		
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);
        printf("  %s = icmp slt i32 %s, %s\n",tmpReg,op1,op2);
		return tmpReg;

    }
    else if (!strcmp(root->children[1]->data,"&&")) //EXP->EXP LOGICAND EXP
    {
        //%cmp = icmp eq i32 %0, %1
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        int reg1 = returnNum, reg2 = returnNum+1; returnNum+=2;
        printf("  %%r%d = icmp ne i32 %s, 0\n",reg1,op1);
        printf("  %%r%d = icmp ne i32 %s, 0\n",reg2,op2);

        int reg3 = returnNum; returnNum++;
        printf("  %%r%d = and i1 %%r%d, %%r%d\n",reg3,reg1,reg2);

        char num[10];
        sprintf(num, "%d", reg3);
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);

        return tmpReg;
    }
    else if (!strcmp(root->children[1]->data,"+")) //EXP ADD EXP
    {
        
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        char num[10];
        sprintf(num, "%d", returnNum++);
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);

        printf("  %s = add nsw i32 %s, %s\n",tmpReg,op1,op2);
        return tmpReg;
    }
    else if (!strcmp(root->children[1]->data,"-")) //EXP -> EXP - Exp
    {
		
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);

        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        char num[10];
        sprintf(num, "%d", returnNum++);
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);

        printf("  %s = sub nsw i32 %s, %s\n",tmpReg,op1,op2);
        return tmpReg;
    }
    else if (!strcmp(root->children[1]->data,"*")) //EXP MULT EXP
    {
        //%0 = load i32* %a, align 4
        //%1 = load i32* %b, align 4
        //%add = add nsw i32 %0, %1
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        char num[10];
        sprintf(num, "%d", returnNum++);
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);

        printf("  %s = mul nsw i32 %s, %s\n",tmpReg,op1,op2);
        return tmpReg;
    }
    else if (!strcmp(root->children[1]->data,"%%"))//MOD srem
    {
        
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        char num[10];
        sprintf(num, "%d", returnNum++);
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);

        printf("  %s = srem i32 %s, %s\n",tmpReg,op1,op2);
        return tmpReg;
    }

    else if (!strcmp(root->children[1]->data,"&"))//BITAND
    {
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        char num[10];
        sprintf(num, "%d", returnNum++);
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);

        printf("  %s = and i32 %s, %s\n",tmpReg,op1,op2);
  
        sprintf(num, "%d", returnNum++);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);

        printf("  %s = icmp ne i32 %%r%d, 0\n",tmpReg,returnNum-2);
        return  tmpReg;
    }
    else if (!strcmp(root->children[1]->data,"^"))//BITXOR
    {
        char* op1 = (char*)malloc(sizeof(char)*60);
        op1 = translate_exp(root->children[0]);
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

        char num[10];
        sprintf(num, "%d", returnNum++);
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%r");
        strcat(tmpReg,num);

        printf("  %s = xor i32 %s, %s\n",tmpReg,op1,op2);
        return tmpReg;
    }
    else if (!strcmp(root->children[1]->data,">>=")) //EXP SHIFTRA EXP
    {
     
        char* op1 = (char*)malloc(sizeof(char)*60);
        ifLoad = 0;
        op1 = translate_exp(root->children[0]);
        ifLoad = 1;
        char* op2 = (char*)malloc(sizeof(char)*60);
        op2 = translate_exp(root->children[2]);

       	printf("%%r%d = load i32, i32* %s, align 4\n",returnNum,op1);
        printf("  %%r%d = ashr i32 %%r%d, %s\n",returnNum+1,returnNum,op2);
        printf("  store i32 %%r%d, i32* %s, align 4\n",returnNum+1,op1);
        returnNum+=2;
        return NULL;
    }
    else if (!strcmp(root->children[2]->data,"args")) //ID LP ARGS RP
    {

    	ParaQueue * paraQueue = (ParaQueue*)malloc(sizeof(ParaQueue));
		paraQueue->start = 0;
		paraQueue->end = 0;
		paraQueue->size = 10;
		paraQueue->num = 0;
		paraQueue->queue = (char **)malloc(sizeof(char *)*10);

		translate(root,paraQueue);

		

        argsFunc(root->children[2],paraQueue);

        char num[10];
        sprintf(num, "%d", callNum++);
        char* tmpReg = (char*)malloc(sizeof(char)*60);
        strcpy(tmpReg,"%%call");
        strcat(tmpReg,num);

        Node * id = root->children[0];

        printf("  %s = call i32 @%s(",tmpReg,id->data);
		int i;

		printf("i32 %%%s",dequeue(paraQueue));
		while (paraQueue->end!=paraQueue->start) {
			
			printf(", i32 %%%s",dequeue(paraQueue));
		}

        
        printf(")\n");

        //free memory allocated
		free(paraQueue->queue);
		free(paraQueue);

        return tmpReg;
    }

}
void argsFunc(Node* root,ParaQueue * paraQueue) //ARGS for function call
{
    if (root->childrenNum == 1) //EXP
    {
        char* tmp = (char*)malloc(sizeof(char)*60);
        tmp = translate_exp(root->children[0]);
        enqueue(paraQueue,tmp);
    }
    else //EXP COMMA ARGS
    {
        char* tmp = (char*)malloc(sizeof(char)*60);
        tmp = translate_exp(root->children[0]);
        enqueue(paraQueue,tmp);


        argsFunc(root->children[2],paraQueue);
    }
}
