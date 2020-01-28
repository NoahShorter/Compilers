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
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include <string.h>

    // comment out the other line to generate debug info
#define DO_RETURN(a) return (a)
//#define DO_RETURN(a) return Return(a)
    int Return(int val);
    char * TooLower(char * str);
    int ProccessIdentifier(char * token);
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
REAL_CONST          [0-9]+.[0-9]+
A                   [Aa]
B                   [Bb]
C                   [Cc]
D                   [Dd]
E                   [Ee]
F                   [Ff]
G                   [Gg]
H                   [Hh]
I                   [Ii]
J                   [Jj]
K                   [Kk]
L                   [Ll]
M                   [Mm]
N                   [Nn]
O                   [Oo]
P                   [Pp]
Q                   [Qq]
R                   [Rr]
S                   [Ss]
T                   [Tt]
U                   [Uu]
V                   [Vv]
W                   [Ww]
X                   [Xx]
Y                   [Yy]
Z                   [Zz]

%%


    // token definitions go here. "if" is an example.

{PUNCTUATION}                   DO_RETURN(yytext[0]);
{INTEGER_CONST}                 DO_RETURN(INT_VAL);
{REAL_CONST}                    DO_RETURN(REAL_VAL);
{COMMENT}                       {}
{WHITESPACE}                    {}

"<="                            DO_RETURN(LE);
">="                            DO_RETURN(GE);
":="                            DO_RETURN(ASSIGN);
"<>"                            DO_RETURN(NOT_EQUAL);
{O}{R}                          DO_RETURN(OR);
{D}{I}{V}                       DO_RETURN(DIV);
{M}{O}{D}                       DO_RETURN(MOD);
{A}{N}{D}                       DO_RETURN(AND);
{N}{O}{T}                       DO_RETURN(NOT);

{P}{R}{O}{G}{R}{A}{M}           DO_RETURN(PROGRAM);
{P}{R}{O}{C}{E}{D}{U}{R}{E}     DO_RETURN(PROCEDURE);
{F}{U}{N}{C}{T}{I}{O}{N}        DO_RETURN(FUNCTION);
{B}{E}{G}{I}{N}                 {   g_symbolTable.IncreaseScope();
                                    return yytext[0];
                                    //DO_RETURN(OPEN);
                                }
{E}{N}{D}                       {   g_symbolTable.DecreaseScope();
                                    return yytext[0];
                                    //DO_RETURN(CLOSE);
                                }
{I}{F}                          DO_RETURN(IF);
{T}{H}{E}{N}                    DO_RETURN(THEN);
{E}{L}{S}{E}                    DO_RETURN(ELSE);
{C}{L}{O}{S}{E}                 DO_RETURN(CLOSE);
{W}{H}{I}{L}{E}                 DO_RETURN(WHILE);
{R}{E}{P}{E}{A}{T}              DO_RETURN(REPEAT);
{U}{N}{T}{I}{L}                 DO_RETURN(UNTIL);
{F}{O}{R}                       DO_RETURN(FOR);
{D}{O}                          DO_RETURN(DO);
{T}{O}                          DO_RETURN(TO);
{D}{O}{W}{N}{T}{O}              DO_RETURN(DOWNTO);
{F}{O}{R}{W}{A}{R}{D}           DO_RETURN(FORWARD);
{C}{O}{N}{S}{T}                 DO_RETURN(CONST);
{T}{Y}{P}{E}                    DO_RETURN(TYPE);
{V}{A}{R}                       DO_RETURN(VAR);
{A}{R}{R}{A}{Y}                 DO_RETURN(ARRAY);
{R}{E}{C}{O}{R}{D}              DO_RETURN(RECORD);
{O}{F}                          DO_RETURN(OF);
{N}{I}{L}                       DO_RETURN(NIL);
{W}{R}{I}{T}{E}                 DO_RETURN(WRITE);
{I}{N}{T}{E}{G}{E}{R}           DO_RETURN(INTEGER);
{R}{E}{A}{L}                    DO_RETURN(REAL);
{C}{H}{A}{R}                    DO_RETURN(CHAR);

{IDENTIFIERS}                   {   //ProccessIdentifier(yytext);
                                    //return IDENTIFIER;
                                    DO_RETURN(ProccessIdentifier(yytext));
                                }

{JUNK}                          DO_RETURN(JUNK_TOKEN);

%%

// This function allows us to do extra processing on each token
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

char * TooLower(char * str)
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

int ProccessIdentifier(char * token)
{
    if(g_symbolTable.Lookup(TooLower(token)) == NULL)
    {
        cSymbol * sym = new cSymbol(TooLower(token));
        yylval.symbol = g_symbolTable.Insert(sym);
    }
    else
        yylval.symbol = g_symbolTable.Lookup(TooLower(token));

    return g_symbolTable.Lookup(TooLower(token))->GetType();
}
