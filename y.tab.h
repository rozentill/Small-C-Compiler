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
    SEMI = 258,
    COMMA = 259,
    LC = 260,
    RC = 261,
    INT = 262,
    ID = 263,
    TYPE = 264,
    STRUCT = 265,
    RETURN = 266,
    IF = 267,
    ELSE = 268,
    BREAK = 269,
    CONT = 270,
    FOR = 271,
    ASSIGNOP = 272,
    BOP10 = 273,
    BOP9 = 274,
    BOP8 = 275,
    BOP7 = 276,
    BOP6 = 277,
    BOP5 = 278,
    BOP4 = 279,
    BOP3 = 280,
    BOP2 = 281,
    BOP1 = 282,
    UNARYOP = 283,
    LP = 284,
    RP = 285,
    LB = 286,
    RB = 287,
    DOT = 288
  };
#endif
/* Tokens.  */
#define SEMI 258
#define COMMA 259
#define LC 260
#define RC 261
#define INT 262
#define ID 263
#define TYPE 264
#define STRUCT 265
#define RETURN 266
#define IF 267
#define ELSE 268
#define BREAK 269
#define CONT 270
#define FOR 271
#define ASSIGNOP 272
#define BOP10 273
#define BOP9 274
#define BOP8 275
#define BOP7 276
#define BOP6 277
#define BOP5 278
#define BOP4 279
#define BOP3 280
#define BOP2 281
#define BOP1 282
#define UNARYOP 283
#define LP 284
#define RP 285
#define LB 286
#define RB 287
#define DOT 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 15 "Small-C.y" /* yacc.c:1909  */

        char *  sValue;
        int iPunctuation;
	char * sIndex;
        char * sString;
	char * sOperator;
	struct treeNode * nNode;

#line 129 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
