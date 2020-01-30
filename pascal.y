%{
//**************************************
// pascal.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <iostream>
#include "lex.h"
#include "astnodes.h"

%}

%locations

 /* union defines the type for lexical values */
%union{
    int             int_val;
    double          real_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cStmtsNode*     stmts_node;
    cStmtNode*      stmt_node;
    cExprNode*      expr_node;
    cSymbol*        symbol;
    cDeclsNode*     decls_node;
    cDeclNode*      decl_node;
    }

%{
    int yyerror(const char *msg);

    cAstNode *yyast_root;
%}

%start  program

%token PROGRAM PROCEDURE FUNCTION FORWARD
%token OPEN CLOSE
%token WRITE
%token WHILE IF THEN ELSE REPEAT UNTIL
%token FOR DO TO DOWNTO
%token CONST TYPE VAR
%token RECORD ARRAY OF
%token NIL
%token CHAR INTEGER REAL
%token JUNK_TOKEN

%token <symbol>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <real_val>  REAL_VAL
%token <int_val>   AND
%token <int_val>   OR
%token <int_val>   NOT
%token <int_val>   DIV
%token <int_val>   MOD
%token <int_val>   NOT_EQUAL
%token <int_val>   LE
%token <int_val>   GE
%token <int_val>   ASSIGN

%type <program_node> program
%type <symbol> header
%type <block_node> block
%type <ast_node> type
%type <ast_node> decls
%type <ast_node> constdecls
%type <ast_node> constdecl
%type <ast_node> constant
%type <ast_node> typedecls
%type <ast_node> typedecl
%type <ast_node> singleType
%type <ast_node> rangeList
%type <ast_node> range
%type <ast_node> goodvar
%type <ast_node> onevar
%type <ast_node> recorddef
%type <ast_node> vardecls
%type <ast_node> vardecl;
%type <ast_node> procdecls
%type <ast_node> paramSpec
%type <ast_node> procdecl
%type <ast_node> parlist
%type <ast_node> idlist
%type <ast_node> func_call
%type <ast_node> funcProto
%type <ast_node> funcHeader
%type <ast_node> procHeader
%type <stmts_node> statements
%type <stmt_node> statement
%type <ast_node> expr
%type <ast_node> oneExpr
%type <ast_node> addit
%type <ast_node> term
%type <ast_node> fact
%type <ast_node> variable
%type <ast_node> varpart
%type <ast_node> exprList
%type <ast_node> recHeader
%%

program: header block '.'
                                { $$ = new cProgramNode($1, $2);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                }
header: PROGRAM IDENTIFIER ';'
                                {  g_SymbolTable.IncreaseScope();
                                   $$ = $2; 
                                }
block:  decls OPEN statements CLOSE
                                { $$ = new cBlockNode(nullptr, $3); }

decls: constdecls typedecls vardecls procdecls
                                { }
constdecls: CONST constdecl ';'
                                { }
        | /*empty */
                                { }
constdecl: constdecl ';' IDENTIFIER '=' constant 
                                { }
        |  IDENTIFIER '=' constant 
                                { }
typedecls: TYPE typedecl
                                { }
        | /*empty */
                                { }
typedecl: typedecl singleType
                                { }
        |  singleType
                                { }
        |  error ';'
                                {}
singleType:  IDENTIFIER '=' recHeader recorddef ';'
                                { }
        | IDENTIFIER '=' ARRAY '[' rangeList ']' OF type ';'
                                { }
rangeList: rangeList ',' range
                                { }
        |  range
                                { }
range: INT_VAL '.' '.' INT_VAL
                                { }

vardecls: VAR vardecl
                                { }
        | /* empty */
                                { }
vardecl: vardecl onevar
                                { }
        | onevar
                                { }
onevar: goodvar ';'
                                { }
        | error ';'
                                { }
goodvar: idlist ':' type
                                { }
procdecls: procdecls procdecl
                                { }
        | /* empty */
                                { }

procdecl: procHeader paramSpec ';' block ';'
                                { }
        |  procHeader paramSpec ';' FORWARD ';'
                                { }
        |  funcProto ';' block ';'
                                { }
        |  funcProto ';' FORWARD ';'
                                { }
        |  error ';' block ';'
                                { }
        |  error ';' FORWARD ';'
                                { }
procHeader: PROCEDURE IDENTIFIER 
                                { }
funcHeader: FUNCTION IDENTIFIER
                                { }
funcProto: funcHeader paramSpec ':' type
                                { }
paramSpec: '(' parlist ')'
                                { }
        | /* empty */
                                { }

idlist: idlist ',' IDENTIFIER
                                { }
    |    IDENTIFIER
                                { }

parlist: parlist ';' VAR idlist ':' type 
                                { }
    |    parlist ';' idlist ':' type 
                                { }
    |    VAR idlist ':' type
                                { }
    |    idlist ':' type
                                { }

type: TYPE_ID
                                { }
recHeader: RECORD
                                { }
recorddef: vardecl CLOSE
                                { }
constant: INT_VAL
                                { }
    |   '-' INT_VAL
                                { }

statements: statements statement
                                { }
    |   statement
                                { $$ = new cStmtsNode($1); }

statement: variable ASSIGN expr ';'
                                { }
    |   IF expr THEN statement
                                { }
    |   IF expr THEN statement ELSE statement
                                { }
    |   REPEAT statements UNTIL expr ';'
                                { }
    |   WHILE expr DO statement
                                { }
    |   FOR IDENTIFIER ASSIGN expr TO expr DO statement
                                {}
    |   FOR IDENTIFIER ASSIGN expr DOWNTO expr DO statement
                                {}
    |   IDENTIFIER '(' exprList ')' ';'
                                { }
    |   IDENTIFIER ';'
                                { }
    |   WRITE '(' exprList ')' ';'
                                { }
    |   OPEN statements CLOSE
                                { }
    |   NIL ';'
                                { $$ = new cNilNode(); }
    |   error ';'
                                { }

exprList: exprList ',' oneExpr
                                { }
        | oneExpr
                                { }
        | /* empty */
                                { }
oneExpr: expr
                                { }

func_call:  IDENTIFIER '(' exprList ')'
                                { }

variable: variable '.' varpart
                                { }
        | variable '[' exprList ']'
                                { }
        | varpart
                                { }

varpart:  IDENTIFIER
                                { }

expr:       expr '=' addit
                                { }
        |   expr '>' addit
                                { }
        |   expr '<' addit
                                { }
        |   expr LE addit
                                { }
        |   expr GE addit
                                { }
        |   expr NOT_EQUAL addit
                                {}
        |   addit
                                { }

addit:      addit '+' term
                                { }
        |   addit '-' term
                                { }
        |   addit OR term
                                { }
        |   term
                                { }
        |   '-' term
                                { }

term:       term '*' fact
                                { }
        |   term '/' fact
                                { }
        |   term MOD fact
                                { }
        |   term DIV fact
                                { }
        |   term AND fact
                                { }
        |   fact
                                { }

fact:        '(' expr ')'
                                { }
        |   INT_VAL
                                { }
        |   REAL_VAL
                                { }
        |   variable
                                { }
        |   func_call
                                { }
        |   NOT fact
                                { }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cout << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}
