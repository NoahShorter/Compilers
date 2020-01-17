%{
//**************************************
// lang.l
//
// Scanner definition file for CST 320 project
//
// Author: Phil Howard 
// phil.howard@oit.edu
// Edited by: Noah Shorter
// noah.shorter@oit.edu
//
// Date: Dec. 6, 2017
//

#include "tokens.h"
#include "cSymbolTable.h"
#include "lex.h"
#include <string.h>

    // comment out the other line to generate debug info
#define DO_RETURN(a) return (a)
//#define DO_RETURN(a) return Return(a)
int Return(int val);
char * tooLower(char * str);
extern cSymbolTable g_symbolTable;
yylval_t yylval;
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here. PUNCTUATION is an example. */
PUNCTUATION         [-.;(),+*/=^<>]|"["|"]"
IDENTIFIERS         [a-zA-Z][a-zA-Z0-9]*
COMMENT             [{][^}\n]*[}]
WHITESPACE          [ \t\n]+
JUNK                .
INTEGER_CONST       [0-9]*
REAL_CONST          [1-9][0-9]*.[0-9]*[1-9]

%%


    // token definitions go here. "if" is an example.

{PUNCTUATION}           DO_RETURN(yytext[0]);
{INTEGER_CONST}         DO_RETURN(INT_VAL);
{REAL_CONST}            DO_RETURN(REAL_VAL);
{COMMENT}               {}
{WHITESPACE}            {}

"<="                    DO_RETURN(LE);
">="                    DO_RETURN(GE);
":="                    DO_RETURN(ASSIGN);
"<>"                    DO_RETURN(NOT_EQUAL);
OR                      DO_RETURN(OR);
DIV                     DO_RETURN(DIV);
MOD                     DO_RETURN(MOD);
AND                     DO_RETURN(AND);
NOT                     DO_RETURN(NOT);

PROGRAM                 DO_RETURN(PROGRAM);
PROCEDURE               DO_RETURN(PROCEDURE);
FUNCTION                DO_RETURN(FUNCTION);
BEGIN                   {   g_symbolTable.IncreaseScope();
                            return yytext[0];
                            //DO_RETURN(OPEN);
                        }
END                     {   g_symbolTable.DecreaseScope();
                            return yytext[0];
                            //DO_RETURN(CLOSE);
                        }
IF                      DO_RETURN(IF);
THEN                    DO_RETURN(THEN);
ELSE                    DO_RETURN(ELSE);
CLOSE                   DO_RETURN(CLOSE);
WHILE                   DO_RETURN(WHILE);
REPEAT                  DO_RETURN(REPEAT);
UNTIL                   DO_RETURN(UNTIL);
FOR                     DO_RETURN(FOR);
DO                      DO_RETURN(DO);
TO                      DO_RETURN(TO);
DOWNTO                  DO_RETURN(DOWNTO);
FORWARD                 DO_RETURN(FORWARD);
CONST                   DO_RETURN(CONST);
TYPE                    DO_RETURN(TYPE);
VAR                     DO_RETURN(VAR);
ARRAY                   DO_RETURN(ARRAY);
RECORD                  DO_RETURN(RECORD);
OF                      DO_RETURN(OF);
NIL                     DO_RETURN(NIL);
WRITE                   DO_RETURN(WRITE);
"integer"               DO_RETURN(INTEGER);
"real"                  DO_RETURN(REAL);
"char"                  DO_RETURN(CHAR);

{IDENTIFIERS}           {   
                            if(g_symbolTable.lookup(tooLower(yytext)) == NULL)
                                yylval.symbol = g_symbolTable.insert(tooLower(yytext));
                            else
                                yylval.symbol = g_symbolTable.lookup(tooLower(yytext));
                            return IDENTIFIER;
                            //DO_RETURN(IDENTIFIER);
                        }

{JUNK}                  DO_RETURN(JUNK_TOKEN);

%%

// This function allows us to do extra processing on each token
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

char * tooLower(char * str)
{
    if(str != NULL)
    {
        for(int i = 0; i < strlen(str); ++i)
        {
            str[i] = tolower(str[i]);
        }
    }
    return str;
}




