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

int digits|((0x|0X|0)digits)
id (underscore|letter)+(underscore|letter|digit)*
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
{type} {printf(":%s",yytext);return TYPE;}
{lp} {printf(":%s",yytext);return LP;}
{rp} {printf(":%s",yytext);return RP;}
{lb} {return LB;}
{rb} {return RB;}
{lc} {printf(":%s",yytext);return LC;}
{rc} {printf(":%s",yytext);return RC;}
{struct} {return STRUCT;}
{return} {printf(":%s",yytext);return RETURN;}
{if} {return IF;}
{else} {return ELSE;}
{break} {return BREAK;}
{cont} {return CONT;}
{for} {return FOR;}
{semi} {printf(":%s",yytext);return SEMI;}
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
{id} {printf(":%s",yytext);return ID;}
{int} {printf(":%s",yytext);//sscanf(yytext,"%d",&yylval);
	return INT;}
{whitespace} {;}
{newline} {linecount++;}
. {printf(":%s",yytext);}
{end} {;}
%%

int yywrap(){
	return 1;
}
