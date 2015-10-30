%{

%}
DIGIT [0-9]
DIGITS {DIGIT}+
LETTER [a-zA-Z]
LETTERS {LETTER}+
UNDERSCORE "_"

INT DIGITS|((0x|0X|0)DIGITS)
ID (UNDERSCORE|LETTER)(UNDERSCORE|LETTER|DIGIT)*
SEMI ";"
COMMA ","
DOT "."
BINARYOP "+"|"-"|"*"|"/"|"%"|">"|"<"|">="|"<="|""|""|""|""|
UNARYOP ++
ASSIGNOP 
TYPE 
LP ( 
RP )
LB [
RB ]
LC {
RC }
STRUCT struct
RETURN return
IF if 
ELSE else 
BREAK break
CONT continue
FOR for 

%%

%%
