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
    cSymbol * temp = g_symbolTable.Lookup(TooLower(token));
    cSymbol * Gtemp = g_symbolTable.GlobalLookup(TooLower(token));
    if(strcmp(TooLower(token), "begin") == 0)
        g_symbolTable.IncreaseScope();
    else if (strcmp(TooLower(token), "end") == 0)
        g_symbolTable.DecreaseScope();
    if(temp == NULL && Gtemp == NULL)
    {
        cSymbol * sym = new cSymbol(TooLower(token));
        yylval.symbol = g_symbolTable.Insert(sym);
        return g_symbolTable.Lookup(TooLower(token))->GetType();
    }
    else if (temp == NULL && Gtemp->GetType() == IDENTIFIER)
    {
        cSymbol * sym = new cSymbol(TooLower(token));
        yylval.symbol = g_symbolTable.Insert(sym);
        return g_symbolTable.Lookup(TooLower(token))->GetType();
    }
    else if (temp == NULL && Gtemp->GetType() != IDENTIFIER)
    {
        yylval.symbol = Gtemp;
        return Gtemp->GetType();
    }
    else
        yylval.symbol = temp;

    return temp->GetType();
}
