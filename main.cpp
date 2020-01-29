//**************************************
// main.cpp
//
// Main routine for PASCAL compiler.
// This version only runs the lexer
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Edited by: Noah Shorter
// noah.shorter@oit.edu
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "tokens.h"

#define test1
//#define test2

cSymbolTable g_symbolTable;
long long cSymbol::nextId = 0;
extern yylval_t yylval;

// Program to test a symbol table implementation
// Command line arguments:
//     Input file:  Default = stdin
//     Output file: Default = stdout
int main(int argc, char **argv)
{
    const char *outfile_name;
    int result = 0;
    int token;

    std::cout << "Noah Shorter" << std::endl;

    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL)
        {
            std::cerr << "Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    if (argc > 2)
    {
        outfile_name = argv[2];

        FILE *output = fopen(outfile_name, "w");
        output = fopen(outfile_name, "w");
        if (output == nullptr)
        {
            std::cerr << "ERROR: Unable to open file " << outfile_name << "\n";
            exit(-1);
        }

        // redirect stdout to the output file
        int output_fd = fileno(output);
        if (dup2(output_fd, 1) != 1)
        {
            std::cerr << "Unable to duplicate the file descriptor\n";
        }
    }

    #ifdef test2
    std::cout << "<program>\n";
    #endif

    token = yylex();
    while (token != 0)
    {
        // if we found an identifier, print it out
        #ifdef test2
            if (token == IDENTIFIER) 
                std::cout << yylval.symbol->ToString() << "\n";
        #endif
        #ifdef test1
            std::cout << token << ":" << yytext << "\n";
        #endif
        token = yylex();
    }
    
    #ifdef test2
    std::cout << "</program>\n";
    #endif

    return result;
}
