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
    cExprListNode*  exprs_node;
    cBaseTypeNode*  baset_node;
    cIdListNode*    ids_node;
    cVarDeclsNode*  vardecl_node;
    cVarExprNode*   varexpr_node;
    cFuncDeclNode*  funcdecl_node;
    cProcDeclNode*  procdecl_node;
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
%type <symbol> type
%type <decls_node> decls
%type <decls_node> constdecls
%type <decl_node> constdecl
%type <ast_node> constant
%type <decls_node> typedecls
%type <decl_node> typedecl
%type <ast_node> singleType
%type <ast_node> rangeList
%type <ast_node> range
%type <decls_node> goodvar
%type <decls_node> onevar
%type <ast_node> recorddef
%type <decls_node> vardecls
%type <decls_node> vardecl;
%type <decls_node> procdecls
%type <decls_node> paramSpec
%type <decl_node> procdecl
%type <vardecl_node> parlist
%type <ids_node> idlist
%type <expr_node> func_call
%type <funcdecl_node> funcProto
%type <funcdecl_node> funcHeader
%type <procdecl_node> procHeader
%type <stmts_node> statements
%type <stmt_node> statement
%type <expr_node> expr
%type <expr_node> oneExpr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <varexpr_node> variable
%type <varexpr_node> varpart
%type <exprs_node> exprList
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
                                { 
                                   g_symbolTable.IncreaseScope();
                                   $$ = $2; 
                                }
block:  decls OPEN statements CLOSE
                                { $$ = new cBlockNode($1, $3); }

decls: constdecls typedecls vardecls procdecls
                                { 
                                    if( $3 != nullptr ||
                                        $4 != nullptr)
                                    {
                                    $$ = new cDeclsNode($3);
                                    $$->AddDecls($4);
                                    }
                                    else
                                        $$ = nullptr;
                                }
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
                                { $$ = $2; }
        | /* empty */
                                { $$ = nullptr; }
vardecl: vardecl onevar
                                { 
                                    $$ = $1;
                                    $$->AddDecls($2);
                                }
        | onevar
                                { $$ = new cDeclsNode($1); }
onevar: goodvar ';'
                                { $$ = $1; }
        | error ';'
                                { }
goodvar: idlist ':' type
                                { $$ = new cVarDeclsNode($3, $1); }
procdecls: procdecls procdecl
                                { 
                                    $$ = new cDeclsNode($1);
                                    $$->AddDecl($2);
                                }
        | /* empty */
                                { $$ = nullptr; }

procdecl: procHeader paramSpec ';' block ';'
                                { 
                                    $1->AddParamBlock($4, $2);
                                    g_symbolTable.DecreaseScope();
                                    $$ = $1;
                                }
        |  procHeader paramSpec ';' FORWARD ';'
                                { }
        |  funcProto ';' block ';'
                                { 
                                    $1->AddBlock($3); 
                                    g_symbolTable.DecreaseScope();
                                    $$ = $1;
                                }
        |  funcProto ';' FORWARD ';'
                                { }
        |  error ';' block ';'
                                { }
        |  error ';' FORWARD ';'
                                { }
procHeader: PROCEDURE IDENTIFIER 
                                { 
                                    $$ = new cProcDeclNode($2);
                                    g_symbolTable.IncreaseScope();
                                }
funcHeader: FUNCTION IDENTIFIER
                                { 
                                    $$ = new cFuncDeclNode($2); 
                                    g_symbolTable.IncreaseScope();
                                }
funcProto: funcHeader paramSpec ':' type
                                { $$->AddParamType($4, $2); }
paramSpec: '(' parlist ')'
                                { $$ = $2; }
        | /* empty */
                                { $$ = nullptr; }

idlist: idlist ',' IDENTIFIER
                                { 
                                    $$ = $1;
                                    $$->AddId($3);
                                }
    |    IDENTIFIER
                                { $$ = new cIdListNode($1); }

parlist: parlist ';' VAR idlist ':' type 
                                { 
                                    $$ = $1;
                                    $$->AddVarDecls($6, $4);
                                }
    |    parlist ';' idlist ':' type 
                                { 
                                    $$ = $1;
                                    $$->AddVarDecls($5, $3);
                                }
    |    VAR idlist ':' type
                                { $$ = new cVarDeclsNode($4, $2);}
    |    idlist ':' type
                                { $$ = new cVarDeclsNode($3, $1);}

type: TYPE_ID
                                { $$ = $1; }
recHeader: RECORD
                                { }
recorddef: vardecl CLOSE
                                { }
constant: INT_VAL
                                { }
    |   '-' INT_VAL
                                { }

statements: statements statement
                                { 
                                    $$ = $1;
                                    $$->AddStmt($2);
                                }
    |   statement
                                { $$ = new cStmtsNode($1); }

statement: variable ASSIGN expr ';'
                                { $$ = new cAssignNode($1, $3); }
    |   IF expr THEN statement
                                { $$ = new cIfNode($2, $4); }
    |   IF expr THEN statement ELSE statement
                                { $$ = new cIfNode($2, $4, $6); }
    |   REPEAT statements UNTIL expr ';'
                                { }
    |   WHILE expr DO statement
                                { $$ = new cWhileNode($2, $4); }
    |   FOR IDENTIFIER ASSIGN expr TO expr DO statement
                                {}
    |   FOR IDENTIFIER ASSIGN expr DOWNTO expr DO statement
                                {}
    |   IDENTIFIER '(' exprList ')' ';'
                                { $$ = new cProcCallNode($1, $3); }
    |   IDENTIFIER ';'
                                { $$ = new cProcCallNode($1);}
    |   WRITE '(' exprList ')' ';'
                                { $$ = new cWriteNode($3); }
    |   OPEN statements CLOSE
                                { $$ = new cCompoundStmtNode($2); }
    |   NIL ';'
                                { $$ = new cNilNode(); }
    |   error ';'
                                { }

exprList: exprList ',' oneExpr
                                { 
                                    $$ = $1;
                                    $$->AddExpr($3);    
                                }
        | oneExpr
                                { $$ = new cExprListNode($1); }
        | /* empty */
                                { }
oneExpr: expr
                                { $$ = $1; }

func_call:  IDENTIFIER '(' exprList ')'
                                { $$ = new cFuncExprNode($1, $3); }

variable: variable '.' varpart
                                { }
        | variable '[' exprList ']'
                                { }
        | varpart
                                { $$ = $1; }

varpart:  IDENTIFIER
                                { $$ = new cVarExprNode($1); }

expr:       expr '=' addit
                                { $$ = new cBinaryExprNode($1, $3, '='); }
        |   expr '>' addit
                                { $$ = new cBinaryExprNode($1, $3, '>'); }
        |   expr '<' addit
                                { $$ = new cBinaryExprNode($1, $3, '<'); }
        |   expr LE addit
                                { $$ = new cBinaryExprNode($1, $3, LE); }
        |   expr GE addit
                                { $$ = new cBinaryExprNode($1, $3, GE); }
        |   expr NOT_EQUAL addit
                                { $$ = new cBinaryExprNode($1, $3, NOT_EQUAL); }
        |   addit
                                { $$ = $1; }

addit:      addit '+' term
                                { $$ = new cBinaryExprNode($1, $3, '+'); }
        |   addit '-' term
                                { $$ = new cBinaryExprNode($1, $3, '-'); }
        |   addit OR term
                                { $$ = new cBinaryExprNode($1, $3, OR);}
        |   term
                                { $$ = $1; }
        |   '-' term
                                { $$ = new cUnaryExprNode('-', $2); }

term:       term '*' fact
                                { $$ = new cBinaryExprNode($1, $3, '*'); }
        |   term '/' fact
                                { $$ = new cBinaryExprNode($1, $3, '/'); }
        |   term MOD fact
                                { $$ = new cBinaryExprNode($1, $3, MOD);}
        |   term DIV fact
                                { $$ = new cBinaryExprNode($1, $3, DIV);}
        |   term AND fact
                                { $$ = new cBinaryExprNode($1, $3, AND);}
        |   fact
                                { $$ = $1; }

fact:        '(' expr ')'
                                { $$ = $2; }
        |   INT_VAL
                                { $$ = new cIntExprNode($1); }
        |   REAL_VAL
                                { $$ = new cRealExprNode($1); }
        |   variable
                                { }
        |   func_call
                                { $$ = $1; }
        |   NOT fact
                                { $$ = new cUnaryExprNode(NOT, $2); }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cout << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}