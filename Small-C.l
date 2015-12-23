/*
This is the lex file,it defines how to match the input and convey the value to yacc file.In particularly,since "-" can be a binaryop or a unaryop,I define it alone as sub.
*/
%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"y.tab.h"


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
sub "-"
bop1 "*"|"/"|"%"
bop2 "+"
bop3 "<<"|">>"
bop4 ">"|"<"|">="|"<="
bop5 "=="|"!="
bop6 "&"
bop7 "^"
bop8 "|"
bop9 "&&"
bop10 "||"
unaryop "++"|"--"|"!"|"~"


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
{sub} {yylval.sOperator=strdup(yytext);return SUB;}
{bop1} {yylval.sOperator=strdup(yytext);return BOP1;}
{bop2} {yylval.sOperator=strdup(yytext);return BOP2;}
{bop3} {yylval.sOperator=strdup(yytext);return BOP3;}
{bop4} {yylval.sOperator=strdup(yytext);return BOP4;}
{bop5} {yylval.sOperator=strdup(yytext);return BOP5;}
{bop6} {yylval.sOperator=strdup(yytext);return BOP6;}
{bop7} {yylval.sOperator=strdup(yytext);return BOP7;}
{bop8} {yylval.sOperator=strdup(yytext);return BOP8;}
{bop9} {yylval.sOperator=strdup(yytext);return BOP9;}
{bop10} {yylval.sOperator=strdup(yytext);return BOP10;}
{unaryop} {yylval.sOperator=strdup(yytext);return UNARYOP;}
{assignop} {yylval.sOperator=strdup(yytext);return ASSIGNOP;}
{id} {yylval.sIndex=strdup(yytext);return ID;}
{int} {yylval.sValue=strdup(yytext);return INT;}
{whitespace} {;}
{newline} {yylineno++;}
{end} {;}
%%

int yywrap(){
	return 1;
}
