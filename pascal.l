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

#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "pascalparse.h"

    // comment out the other line to generate debug info
#define DO_RETURN(a) return (a)
//#define DO_RETURN(a) return Return(a)
int Return(int val);
char * TooLower(char * str);
int ProccessIdentifier(char * token);
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here. PUNCTUATION is an example. */
PUNCTUATION         [+\-*/<=>\^\(\)\[\]\;,\.:] 
IDENTIFIERS         [a-zA-Z][a-zA-Z0-9]*
COMMENT             [{][^}\n]*[}]
WHITESPACE          [ \t\n]+
JUNK                .
INTEGER_CONST       [0-9]*
REAL_CONST          [0-9]+\.[0-9]+

%%
    // token definitions go here. "if" is an example.

{WHITESPACE}                    {}
{COMMENT}                       {}

{IDENTIFIERS}                   DO_RETURN(ProccessIdentifier(yytext));

"<="                            DO_RETURN(LE);
">="                            DO_RETURN(GE);
":="                            DO_RETURN(ASSIGN);
"<>"                            DO_RETURN(NOT_EQUAL);

{PUNCTUATION}                   DO_RETURN(yytext[0]);

{INTEGER_CONST}                 {   
                                    yylval.int_val = atoi(yytext);
                                    DO_RETURN(INT_VAL);
                                } 
{REAL_CONST}                    {
                                    yylval.real_val = atof(yytext);
                                    DO_RETURN(REAL_VAL);
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
    char * strcpy = str;
    if(strcpy != NULL)
    {
        for(int i = 0; i < strlen(strcpy); ++i)
        {
            strcpy[i] = tolower(strcpy[i]);
        }
    }
    return strcpy;
}

int ProccessIdentifier(char * token)
{
    // set the token to a lower case.
    char * lowerString = TooLower(token);

    // perform a global lookup.
    yylval.symbol = g_symbolTable.GlobalLookup(lowerString);

    // If the symbol is not in the table, create a new symbol
    if(yylval.symbol == nullptr)
        yylval.symbol = new cSymbol(TooLower(token));

    // Return the type.
    if( yylval.symbol->IsType())
        return TYPE_ID;
    return (yylval.symbol->GetType());
}
