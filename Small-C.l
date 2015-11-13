%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"y.tab.h"

int linecount=1;

%}
digit [0-9]
digits {digit}+
letter [a-zA-Z]
letters {letter}+
underscore "_"

int {digits}|(0[0-7]+)|((0x|0X)[0-9a-fA-F]+)
id ({underscore}|{letter})+({underscore}|{letter}|{digit})*
semi ";"
comma ","
string \"[^\"]*\"

dot "."
bop1 "*"|"/"|"%"
bop2 "+"|"-"
bop3 "<<"|">>"
bop4 ">"|"<"|">="|"<="
bop5 "=="|"!="
bop6 "&"
bop7 "^"
bop8 "|"
bop9 "&&"
bop10 "||"
unaryop "++"|"--"|"!"|"~"|"-"


assignop "="|"+="|"-="|"*="|"/="|"&="|"^="|"|="|"<<="|">>="
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
delium [ \t\v\f]
whitespace {delium}+ 
end [ \t]+$
%%
{type} {return TYPE;}
{lp} {return LP;}
{rp} {return RP;}
{lb} {return LB;}
{rb} {return RB;}
{lc} {return LC;}
{rc} {return RC;}
{struct} {return STRUCT;}
{return} {return RETURN;}
{if} {return IF;}
{else} {return ELSE;}
{break} {return BREAK;}
{cont} {return CONT;}
{for} {return FOR;}
{semi} {return SEMI;}
{comma} {return COMMA;}
{dot} {return DOT;}
{bop1} {return BOP1;}
{bop2} {return BOP2;}
{bop3} {return BOP3;}
{bop4} {return BOP4;}
{bop5} {return BOP5;}
{bop6} {return BOP6;}
{bop7} {return BOP7;}
{bop8} {return BOP8;}
{bop9} {return BOP9;}
{bop10} {return BOP10;}
{unaryop} {return UNARYOP;}
{assignop} {return ASSIGNOP;}
{id} {return ID;}
{int} {return INT;}
{whitespace} {;}
{newline} {linecount++;}
{end} {;}
%%

int yywrap(){
	return 1;
}
