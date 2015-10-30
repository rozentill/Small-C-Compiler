%{
#include<stdio.h>
#include<string.h>
//#include<y.tab.h>
int linecount=1;
%}
digit [0-9]
digits {digit}+
letter [a-zA-Z]
letters {letter}+
underscore "_"

int digits|((0x|0X|0)digits)
id (underscore|letter)(underscore|letter|digit)*
semi ";"
comma ","

dot "."
binaryop "+"|"*"|"/"|"%"|">"|"<"|">="|"<="|"=="|"!="|"&"|"^"|"|"|"&&"|"||"|"+="|"-="|"*="|"/="|"&="|"^="|"|="|"<<="|">>="
unaryop "++"|"--"|">>"|"<<"|"!"|"~"
sub "-"

assignop "="
type int
lp "(" 
rp ")"
lb "["
rb "]"
lc "{"
rc "}"
struct struct
return return
if if 
else else 
break break
cont continue
for for
newline [\n] 
delium [" "\t]
whitespace {delium}+ 
end [ \t]+$
%%
{type} {printf("TYPE.\n");return TYPE;}
{lp} {printf("LP.\n");return LP;}
{rp} {printf("RP.\n");return RP;}
{lb} {printf("LB.\n");return LB;}
{rb} {printf("RB.\n");return RB;}
{lc} {printf("LC.\n");return LC;}
{rc} {printf("RC.\n");return RC;}
{struct} {printf("STRUCT.\n");return STRUCT;}
{return} {printf("RETURN.\n");return RETURN;}
{if} {printf("IF.\n");return IF;}
{else} {printf("ELSE.\n");return ELSE;}
{break} {printf("BREAK.\n");return BREAK;}
{cont} {return CONT;}
{for} {return FOR;}
{semi} {return SEMI;}
{comma} {return COMMA;}
{dot} {return DOT;}
{binaryop} {return BINARYOP;}
{unaryop} {return UNARYOP;}
{sub} {return SUB;}
{assignop} {return ASSIGNOP;}
{id} {return ID;}
{int} {return INT;}
{whitespace} {;}
{newline} {linecount++;}
{end} {;}
. {yyerror("Error!");}  
%%
void main(){

	yylex();
}
int yywrap(){
	return 1;
}
