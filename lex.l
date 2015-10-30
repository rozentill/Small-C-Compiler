%{

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
binaryop "+"|"*"|"/"|"%"|">"|"<"|">="|"<="|"=="|"!="|"&"|"^"|"|"|"&&"|"||"|"="|"+="|"-="|"*="|"/="|"&="|"^="|"|="|"<<="|">>="|
unaryop "++"|"--"|">>"|"<<"|"!"|"~"|
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
{binaryop} {return BINARYOP;}
{unaryop} {return UNARYOP;}
{sub} {return SUB;}
{assignop} {return ASSIGNOP;}
{id} {return ID;}
{int} {return INT;}

%%
void main(){

	yylex();
}
int yywrap(){
	return 1;
}
