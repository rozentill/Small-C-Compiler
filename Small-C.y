%{
#include <stdio.h>
//#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <ctype.h>
#include "treeNode.h"

%}
%start program
%token TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE BREAK CONT FOR SEMI COMMA DOT UNARYOP ASSIGNOP ID INT
%token BOP1 BOP2 BOP3 BOP4 BOP5 BOP6 BOP7 BOP8 BOP9 BOP10

%right ASSIGNOP 
%left BOP10
%left BOP9
%left BOP8
%left BOP7
%left BOP6
%left BOP5
%left BOP4
%left BOP3
%left BOP2
%left BOP1
%right UNARYOP
%left LP RP LB RB DOT 

%union {
	int iValue;
	char sIndex;
	
	//Node * node;
};

%%
program :extdefs
	;
extdefs :extdef extdefs
	|
	;
extdef  :spec extvars SEMI
	|spec func stmtblock
	;
extvars :dec
	|dec COMMA extvars
	|
	;
spec    :TYPE
	|stspec
	;
stspec  :STRUCT opttag LC defs RC
	|STRUCT ID
	;
opttag  :ID
	|
	;
var 	:ID
	|var LB INT RB
	;
func    :ID LP paras RP 
	;
paras   :para COMMA paras
	|para
	|
	;
para    :spec var
	;
stmtblock:LC defs stmts RC
	;
stmts   :stmt stmts
	|
	;
stmt    :exp SEMI
	|stmtblock
	|RETURN exp SEMI
	|IF LP exps RP stmt estmt
	|FOR LP exp SEMI exp SEMI exp RP stmt
	|CONT SEMI
	|BREAK SEMI
	;
estmt   :ELSE stmt
	|{}
	;
defs    :def defs{}
	|{}
	;
def 	:spec decs SEMI{}
	;
decs	:dec COMMA decs{}
	|dec{}
	;
dec 	:var{}
	|var ASSIGNOP init{}
	;
init	:exp{}
	|LC args RC{}
	;
exp	:exps
	|
	;
exps	:exp BOP1 exp{}
	|exp BOP2 exp{}
	|exp BOP3 exp{}
	|exp BOP4 exp{}
	|exp BOP5 exp{}
	|exp BOP6 exp{}
	|exp BOP7 exp
	|exp BOP8 exp
	|exp BOP9 exp
	|exp BOP10 exp
	|UNARYOP exp{}
	|LP exp RP{}
	|ID LP args RP{}
	|ID arrs{}
	|exp DOT ID{}
	|INT{}
	;
arrs	:LB exp RB arrs{}
	|{}
	;
args	:exp COMMA args{}
	|exp{}
	;
%%
Node * newNode(int size,Node*child,...){
	Node * temp;
	va_list marker;
	va_start(marker,child);
	va_end(marker);
	return temp;
}
int main(int argc,char * argv[]){
	FILE * in;
	FILE * out;
	in = freopen(argv[1],"r",stdin);
	out = freopen(argv[2],"w",stdout); 
	yyparse();
	fclose(in);
	fclose(out);
	return 0;
}
int yyerror(char * msg){
	printf("%s.\n",msg);
}

