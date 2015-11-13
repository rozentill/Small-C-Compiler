%{
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <ctype.h>
#include "treeNode.h"
Node * newNode(char * data,int size,...);

Node * parseTreeRoot;

FILE * yyin;
FILE * yyout;
%}
%union {
        char *  sValue;
        int iPunctuation;
	char * sIndex;
        char * sString;
	char * sOperator;
	struct treeNode * nNode;
}

%token <iPunctuation> SEMI COMMA LC RC
%token <sValue> INT
%token <sString> ID TYPE STRUCT RETURN IF ELSE BREAK CONT FOR 
%right <sOperator> ASSIGNOP 
%left <sOperator> BOP10
%left <sOperator> BOP9
%left <sOperator> BOP8
%left <sOperator> BOP7
%left <sOperator> BOP6
%left <sOperator> BOP5
%left <sOperator> BOP4
%left <sOperator> BOP3
%left <sOperator> BOP2
%left <sOperator> BOP1
%right <sOperator> UNARYOP
%left <iPunctuation> LP RP LB RB DOT 

%start program

%type <nNode> program extdefs extdef extvars spec stspec opttag var func paras para stmtblock stmts stmt estmt defs def decs dec init exp exps arrs args
%%
program :extdefs{parseTreeRoot=$$=newNode("program",1,$1);}
	;
extdefs :extdef extdefs{$$=newNode("extdefs",2,$1,$2);}
	|{$$=newNode("extdefs",1,newNode("empty",0));}
	;
extdef  :spec extvars SEMI{$$=newNode("extdef",2,$1,$2);}
	|spec func stmtblock{$$=newNode("extdef",3,$1,$2,$3);}
	;
extvars :dec{$$=newNode("extvars",1,$1);}
	|dec COMMA extvars{$$=newNode("extvars",3,$1,newNode(",",0),$3);}
	|{$$=newNode("extvars",1,newNode("empty",0));}
	;
spec    :TYPE{$$=newNode("spec",1,newNode($1,0));}
	|stspec{$$=newNode("spec",1,$1);}
	;
stspec  :STRUCT opttag LC defs RC{$$=newNode("stspec",5,newNode($1,0),$2,newNode("{",0),$4,newNode("}",0));}
	|STRUCT ID{$$=newNode("stspec",2,newNode($1,0),newNode($2,0));}
	;
opttag  :ID{$$=newNode("opttag",1,newNode($1,0));}
	|{$$=newNode("opttag",1,newNode("empty",0));}
	;
var 	:ID{$$=newNode("var",1,newNode($1,0));}
	|var LB INT RB{$$=newNode("var",4,$1,newNode("[",0),newNode($3,0),newNode("]",0));}
	;
func    :ID LP paras RP{$$=newNode("func",4,newNode($1,0),newNode("(",0),$3,newNode(")",0));}
	;
paras   :para COMMA paras{$$=newNode("paras",3,$1,newNode(",",0),$3);}
	|para{$$=newNode("paras",1,$1);}
	|{$$=newNode("paras",1,newNode("empty",0));}
	;
para    :spec var{$$=newNode("para",2,$1,$2);}
	;
stmtblock:LC defs stmts RC{$$=newNode("stmtblock",4,newNode("{",0),$2,$3,newNode("}",0));}
	;
stmts   :stmt stmts{$$=newNode("stmts",2,$1,$2);}
	|{$$=newNode("stmts",1,newNode("empty",0));}
	;
stmt    :exp SEMI{$$=newNode("stmt",1,$1);}
	|stmtblock{$$=newNode("stmt",1,$1);}
	|RETURN exps SEMI{$$=newNode("stmt",2,newNode($1,0),$2);}
	|IF LP exp RP stmt estmt{$$=newNode("stmt",6,newNode($1,0),newNode("(",0),$3,newNode(")",0),$5,$6);}
	|FOR LP exps SEMI exps SEMI exps RP stmt{$$=newNode("stmt",7,newNode($1,0),newNode("(",0),$3,$5,$7,newNode(")",0),$9);}
	|CONT SEMI{$$=newNode("stmt",1,newNode($1,0));}
	|BREAK SEMI{$$=newNode("stmt",1,newNode($1,0));}
	;
estmt   :ELSE stmt{$$=newNode("estmt",2,newNode($1,0),$2);}
	|{$$=newNode("estmt",1,newNode("empty",0));}
	;
defs    :def defs{$$=newNode("defs",2,$1,$2);}
	|{$$=newNode("defs",1,newNode("empty",0));}
	;
def 	:spec decs SEMI{$$=newNode("def",2,$1,$2);}
	;
decs	:dec COMMA decs{$$=newNode("decs",3,$1,newNode(",",0),$3);}
	|dec{$$=newNode("decs",1,$1);}
	;
dec 	:var{$$=newNode("dec",1,$1);}
	|var ASSIGNOP init{$$=newNode("dec",3,$1,newNode($2,0),$3);}
	;
init	:exp{$$=newNode("init",1,$1);}
	|LC args RC{$$=newNode("init",3,newNode("{",0),$2,newNode("}",0));}
	;
exp	:exp BOP1 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|exp BOP2 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|exp BOP3 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|exp BOP4 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|exp BOP5 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|exp BOP6 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|exp BOP7 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|exp BOP8 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|exp BOP9 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|exp BOP10 exp{$$=newNode("exp",3,$1,newNode($2,0),$3);}
	|UNARYOP exp{$$=newNode("exp",2,newNode($1,0),$2);}
	|LP exp RP{$$=newNode("exp",3,newNode("(",0),$2,newNode(")",0));}
	|ID LP args RP{$$=newNode("exp",4,newNode($1,0),newNode("(",0),$3,newNode(")",0));}
	|ID arrs{$$=newNode("exp",2,newNode($1,0),$2);}
	|exp DOT ID{$$=newNode("exp",3,$1,newNode(".",0),newNode($3,0));}
	|INT{$$=newNode("exp",1,newNode($1,0));}
	;
exps    :exp{$$=newNode("exps",1,$1);}
        |{$$=newNode("exps",1,newNode("empty",0));}
        ;
arrs	:LB exp RB arrs{$$=newNode("arrs",4,newNode("[",0),$2,newNode("]",0),$4);}
	|{$$=newNode("arrs",1,newNode("empty",0));}
	;
args	:exp COMMA args{$$=newNode("args",3,$1,newNode(",",0),$3);}
	|exp{$$=newNode("args",1,$1);}
	;
%%
Node * newNode(char*data,int size,...){
	Node * temp;
	temp=(Node*)malloc(sizeof(Node));
	//if(temp==null){printf("Memory Overflow!\n");}	
	temp->data=data; 
	temp->childrenNum=size;
	if(size>0){
		temp->children = (Node**)malloc(size*sizeof(Node*));
		//if(temp->children==null){printf("Memory Overflow!");}
		int i = size;
		va_list ap;
		va_start(ap,size);
		va_arg(ap,int);
		while(i>0){
				
			temp->children[size-i]=va_arg(ap,Node*);
			printf("%s..\n",temp->children[size-i]->data);
			i--;
		}
		va_end(ap);
	}
	return temp;
}
int main(int argc,char * argv[]){
	yyin = freopen(argv[1],"r",stdin);
	yyout = freopen(argv[2],"w",stdout);
	yyparse();
	printParseTree(parseTreeRoot);
	fclose(yyin);
	fclose(yyout);
	return 0;
}
int yyerror(char * msg){
	printf("%s.\n",msg);
}

