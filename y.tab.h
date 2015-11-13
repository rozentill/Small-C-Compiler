/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TYPE = 258,
    LP = 259,
    RP = 260,
    LB = 261,
    RB = 262,
    LC = 263,
    RC = 264,
    STRUCT = 265,
    RETURN = 266,
    IF = 267,
    ELSE = 268,
    BREAK = 269,
    CONT = 270,
    FOR = 271,
    SEMI = 272,
    COMMA = 273,
    DOT = 274,
    UNARYOP = 275,
    ASSIGNOP = 276,
    ID = 277,
    INT = 278,
    BOP1 = 279,
    BOP2 = 280,
    BOP3 = 281,
    BOP4 = 282,
    BOP5 = 283,
    BOP6 = 284,
    BOP7 = 285,
    BOP8 = 286,
    BOP9 = 287,
    BOP10 = 288
  };
#endif
/* Tokens.  */
#define TYPE 258
#define LP 259
#define RP 260
#define LB 261
#define RB 262
#define LC 263
#define RC 264
#define STRUCT 265
#define RETURN 266
#define IF 267
#define ELSE 268
#define BREAK 269
#define CONT 270
#define FOR 271
#define SEMI 272
#define COMMA 273
#define DOT 274
#define UNARYOP 275
#define ASSIGNOP 276
#define ID 277
#define INT 278
#define BOP1 279
#define BOP2 280
#define BOP3 281
#define BOP4 282
#define BOP5 283
#define BOP6 284
#define BOP7 285
#define BOP8 286
#define BOP9 287
#define BOP10 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 28 "Small-C.y" /* yacc.c:1909  */

	int iValue;
	char sIndex;
	
	//Node * node;

#line 127 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
