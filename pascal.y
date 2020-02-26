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

static bool g_semanticErrorHappened;

#define CHECK_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; } }
#define PROP_ERROR() { if (g_semanticErrorHappened) \
    { g_semanticErrorHappened = false; YYERROR; } }


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
    cVarDeclsNode*  vardecls_node;
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
%type <decls_node> constdecl
%type <expr_node> constant
%type <decls_node> typedecls
%type <decls_node> typedecl
%type <decl_node> singleType
%type <decls_node> rangeList
%type <decl_node> range
%type <vardecls_node> goodvar
%type <vardecls_node> onevar
%type <vardecls_node> recorddef
%type <decls_node> vardecls
%type <vardecls_node> vardecl;
%type <decls_node> procdecls
%type <vardecls_node> paramSpec
%type <decl_node> procdecl
%type <vardecls_node> parlist
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
%type <symbol> varpart
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
                                  CHECK_ERROR();
                                }
header: PROGRAM IDENTIFIER ';'
                                { 
                                   g_symbolTable.IncreaseScope();
                                   $$ = $2;
                                   CHECK_ERROR(); 
                                }
block:  decls OPEN statements CLOSE
                                { 
                                    $$ = new cBlockNode($1, $3);
                                    CHECK_ERROR(); 
                                }

decls: constdecls typedecls vardecls procdecls
                                { 
                                    if( $1 != nullptr ||
                                        $2 != nullptr ||
                                        $3 != nullptr ||
                                        $4 != nullptr)
                                    {
                                    $$ = new cDeclsNode($1);
                                    $$->AddDecls($2);
                                    $$->AddDecls($3);
                                    $$->AddDecls($4);
                                    }
                                    else
                                        $$ = nullptr;
                                    CHECK_ERROR();
                                }
constdecls: CONST constdecl ';'
                                { $$ = $2; }
        | /*empty */
                                { $$ = nullptr; }
constdecl: constdecl ';' IDENTIFIER '=' constant 
                                {
                                    $$ = $1;
                                    $$->AddDecl(new cConstDeclNode($3, $5));
                                    CHECK_ERROR();
                                }
        |  IDENTIFIER '=' constant 
                                { 
                                    $$ = new cDeclsNode(
                                        new cConstDeclNode($1, $3));
                                    CHECK_ERROR(); 
                                }
typedecls: TYPE typedecl
                                { $$ = $2; }
        | /*empty */
                                { $$ = nullptr; }
typedecl: typedecl singleType
                                { 
                                    $$ = $1;
                                    $$->AddDecl($2);
                                    CHECK_ERROR();
                                }
        |  singleType
                                { 
                                    $$ = new cDeclsNode($1);
                                    CHECK_ERROR(); 
                                }
        |  error ';'
                                {}
singleType:  IDENTIFIER '=' recHeader recorddef ';'
                                { 
                                    $$ = new cRecordDeclNode($1, $4);
                                    CHECK_ERROR();
                                }
        | IDENTIFIER '=' ARRAY '[' rangeList ']' OF type ';'
                                { 
                                    $$ = new cArrayDeclNode($1, $8, $5);
                                    CHECK_ERROR(); 
                                }
rangeList: rangeList ',' range
                                { 
                                    $$ = $1;
                                    $$->AddDecl($3);
                                    CHECK_ERROR();
                                }
        |  range
                                { 
                                    $$ = new cDeclsNode($1);
                                    CHECK_ERROR(); 
                                }
range: INT_VAL '.' '.' INT_VAL
                                { 
                                    $$ = new cRangeDeclNode($1, $4);
                                    CHECK_ERROR(); 
                                }

vardecls: VAR vardecl
                                { $$ = $2; }
        | /* empty */
                                { $$ = nullptr; }
vardecl: vardecl onevar
                                { 
                                    $$ = $1;
                                    $$->AddVarDecls($2);
                                    CHECK_ERROR();
                                }
        | onevar
                                { 
                                    $$ = new cVarDeclsNode($1); 
                                    CHECK_ERROR();
                                }
onevar: goodvar ';'
                                { 
                                    $$ = $1;
                                    CHECK_ERROR(); 
                                }
        | error ';'
                                { }
goodvar: idlist ':' type
                                { 
                                    $$ = new cVarDeclsNode($3, $1);
                                    CHECK_ERROR(); 
                                }
procdecls: procdecls procdecl
                                { 
                                    $$ = new cDeclsNode($1);
                                    $$->AddDecl($2);
                                    CHECK_ERROR();
                                }
        | /* empty */
                                { $$ = nullptr; }

procdecl: procHeader paramSpec ';' block ';'
                                { 
                                    $1->AddParamBlock($4, $2);
                                    g_symbolTable.DecreaseScope();
                                    $$ = $1;
                                    CHECK_ERROR();
                                }
        |  procHeader paramSpec ';' FORWARD ';'
                                { 
                                    $1->AddParam($2);
                                    g_symbolTable.DecreaseScope();
                                    $$ = $1;
                                    CHECK_ERROR();
                                }
        |  funcProto ';' block ';'
                                { 
                                    $1->AddBlock($3); 
                                    g_symbolTable.DecreaseScope();
                                    $$ = $1;
                                    CHECK_ERROR();
                                }
        |  funcProto ';' FORWARD ';'
                                { 
                                    $1->AddBlock(nullptr); 
                                    g_symbolTable.DecreaseScope();
                                    $$ = $1;
                                    CHECK_ERROR();
                                }
        |  error ';' block ';'
                                { }
        |  error ';' FORWARD ';'
                                { }
procHeader: PROCEDURE IDENTIFIER 
                                { 
                                    $$ = new cProcDeclNode($2);
                                    g_symbolTable.IncreaseScope();
                                    CHECK_ERROR();
                                }
funcHeader: FUNCTION IDENTIFIER
                                { 
                                    $$ = new cFuncDeclNode($2); 
                                    g_symbolTable.IncreaseScope();
                                    CHECK_ERROR();
                                }
funcProto: funcHeader paramSpec ':' type
                                { 
                                    $$->AddParamType($4, $2);
                                    CHECK_ERROR(); 
                                }
paramSpec: '(' parlist ')'
                                { $$ = $2; }
        | /* empty */
                                { $$ = nullptr; }

idlist: idlist ',' IDENTIFIER
                                { 
                                    $$ = $1;
                                    $$->AddId($3);
                                    CHECK_ERROR();
                                }
    |    IDENTIFIER
                                { 
                                    $$ = new cIdListNode($1);
                                    CHECK_ERROR(); 
                                }

parlist: parlist ';' VAR idlist ':' type 
                                { 
                                    $$ = $1;
                                    $$->AddVarDecls($6, $4);
                                    CHECK_ERROR();
                                }
    |    parlist ';' idlist ':' type 
                                { 
                                    $$ = $1;
                                    $$->AddVarDecls($5, $3);
                                    CHECK_ERROR();
                                }
    |    VAR idlist ':' type
                                { 
                                    $$ = new cVarDeclsNode($4, $2);
                                    CHECK_ERROR();
                                }
    |    idlist ':' type
                                { 
                                    $$ = new cVarDeclsNode($3, $1);
                                    CHECK_ERROR();
                                }

type: TYPE_ID
                                { $$ = $1; }
recHeader: RECORD
                                { g_symbolTable.IncreaseScope(); }
recorddef: vardecl CLOSE
                                { 
                                    $$ = $1;
                                    g_symbolTable.DecreaseScope(); 
                                }
constant: INT_VAL
                                { 
                                    $$ = new cIntExprNode($1);
                                    CHECK_ERROR(); 
                                }
    |   '-' INT_VAL
                                { 
                                    $$ = new cUnaryExprNode('-', 
                                        new cIntExprNode($2));
                                    CHECK_ERROR();
                                }

statements: statements statement
                                { 
                                    $$ = $1;
                                    $$->AddStmt($2);
                                    CHECK_ERROR();
                                }
    |   statement
                                { 
                                    $$ = new cStmtsNode($1);
                                    CHECK_ERROR(); 
                                }

statement: variable ASSIGN expr ';'
                                { 
                                    $$ = new cAssignNode($1, $3);
                                    CHECK_ERROR(); 
                                }
    |   IF expr THEN statement
                                { 
                                    $$ = new cIfNode($2, $4);
                                    CHECK_ERROR(); 
                                }
    |   IF expr THEN statement ELSE statement
                                { 
                                    $$ = new cIfNode($2, $4, $6);
                                    CHECK_ERROR(); 
                                }
    |   REPEAT statements UNTIL expr ';'
                                { }
    |   WHILE expr DO statement
                                { 
                                    $$ = new cWhileNode($2, $4);
                                    CHECK_ERROR(); 
                                }
    |   FOR IDENTIFIER ASSIGN expr TO expr DO statement
                                {}
    |   FOR IDENTIFIER ASSIGN expr DOWNTO expr DO statement
                                {}
    |   IDENTIFIER '(' exprList ')' ';'
                                { 
                                    $$ = new cProcCallNode($1, $3);
                                    CHECK_ERROR(); 
                                }
    |   IDENTIFIER ';'
                                { 
                                    $$ = new cProcCallNode($1);
                                    CHECK_ERROR();
                                }
    |   WRITE '(' exprList ')' ';'
                                { 
                                    $$ = new cWriteNode($3);
                                    CHECK_ERROR();
                                }
    |   OPEN statements CLOSE
                                { 
                                    $$ = new cCompoundStmtNode($2);
                                    CHECK_ERROR(); 
                                }
    |   NIL ';'
                                { 
                                    $$ = new cNilNode();
                                    CHECK_ERROR(); 
                                }
    |   error ';'
                                { }

exprList: exprList ',' oneExpr
                                { 
                                    $$ = $1;
                                    $$->AddExpr($3);
                                    CHECK_ERROR();
                                }
        | oneExpr
                                { 
                                    $$ = new cExprListNode($1);
                                    CHECK_ERROR(); 
                                }
        | /* empty */
                                { }
oneExpr: expr
                                { $$ = $1; }

func_call:  IDENTIFIER '(' exprList ')'
                                { 
                                    $$ = new cFuncExprNode($1, $3);
                                    CHECK_ERROR(); 
                                }

variable: variable '.' varpart
                                { 
                                    $$ = $1;
                                    $$->AddVar($3);
                                    CHECK_ERROR();
                                }
        | variable '[' exprList ']'
                                { 
                                    $$ = $1;
                                    $$->AddVar($3);
                                    CHECK_ERROR();
                                }
        | varpart
                                { 
                                    $$ = new cVarExprNode($1);
                                    CHECK_ERROR(); 
                                }

varpart:  IDENTIFIER
                                { $$ = $1; }

expr:       expr '=' addit
                                { 
                                    $$ = new cBinaryExprNode($1, $3, '='); 
                                    CHECK_ERROR();
                                }
        |   expr '>' addit
                                { 
                                    $$ = new cBinaryExprNode($1, $3, '>'); 
                                    CHECK_ERROR();
                                }
        |   expr '<' addit
                                { 
                                    $$ = new cBinaryExprNode($1, $3, '<'); 
                                    CHECK_ERROR();
                                }
        |   expr LE addit
                                { 
                                    $$ = new cBinaryExprNode($1, $3, LE); 
                                    CHECK_ERROR();
                                }
        |   expr GE addit
                                { 
                                    $$ = new cBinaryExprNode($1, $3, GE);
                                    CHECK_ERROR(); 
                                }
        |   expr NOT_EQUAL addit
                                { 
                                    $$ = new cBinaryExprNode($1, $3, NOT_EQUAL);
                                    CHECK_ERROR(); 
                                }
        |   addit
                                { $$ = $1; }

addit:      addit '+' term
                                { 
                                    $$ = new cBinaryExprNode($1, $3, '+');
                                    CHECK_ERROR(); 
                                }
        |   addit '-' term
                                { 
                                    $$ = new cBinaryExprNode($1, $3, '-');
                                    CHECK_ERROR(); 
                                }
        |   addit OR term
                                { 
                                    $$ = new cBinaryExprNode($1, $3, OR);
                                    CHECK_ERROR();
                                }
        |   term
                                { $$ = $1; }
        |   '-' term
                                { 
                                    $$ = new cUnaryExprNode('-', $2);
                                    CHECK_ERROR(); 
                                }

term:       term '*' fact
                                { 
                                    $$ = new cBinaryExprNode($1, $3, '*');
                                    CHECK_ERROR(); 
                                }
        |   term '/' fact
                                { 
                                    $$ = new cBinaryExprNode($1, $3, '/');
                                    CHECK_ERROR();
                                }
        |   term MOD fact
                                { 
                                    $$ = new cBinaryExprNode($1, $3, MOD);
                                    CHECK_ERROR();
                                }
        |   term DIV fact
                                { 
                                    $$ = new cBinaryExprNode($1, $3, DIV);
                                    CHECK_ERROR();
                                }
        |   term AND fact
                                { 
                                    $$ = new cBinaryExprNode($1, $3, AND);
                                    CHECK_ERROR();
                                }
        |   fact
                                { $$ = $1; }

fact:        '(' expr ')'
                                { $$ = $2; }
        |   INT_VAL
                                { 
                                    $$ = new cIntExprNode($1);
                                    CHECK_ERROR(); 
                                }
        |   REAL_VAL
                                { 
                                    $$ = new cRealExprNode($1);
                                    CHECK_ERROR(); 
                                }
        |   variable
                                { $$ = $1; }
        |   func_call
                                { $$ = $1; }
        |   NOT fact
                                { 
                                    $$ = new cUnaryExprNode(NOT, $2);
                                    CHECK_ERROR(); 
                                }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cout << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}

// Function that gets called when a semantic error happens
void SemanticParseError(std::string error)
{
    std::cout << "ERROR: " << error << " on line " 
              << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}
