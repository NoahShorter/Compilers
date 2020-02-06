// *************************************************************
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//
//**************************************************************
#include "cSymbolTable.h"
#include "astnodes.h"
#include "pascalparse.h"


cSymbolTable::cSymbolTable()
{
    IncreaseScope();
    InitTable();
}

// Increases scope
void cSymbolTable::IncreaseScope()
{
    unordered_map<string, cSymbol *> temp;
    SymbolTable.push_back(temp);
}

// Decreases scope
void cSymbolTable::DecreaseScope()
{
    SymbolTable.pop_back();
}

// Inserts str into the symbol table
cSymbol * cSymbolTable::Insert(cSymbol * symbol)
{
    SymbolTable.back().insert({symbol->GetName(), symbol});
    return symbol;
}

// Looks at symbol table from local scope then works out.
cSymbol * cSymbolTable::GlobalLookup(string str)
{
    for (auto rit=SymbolTable.rbegin(); rit!=SymbolTable.rend(); ++rit)
    {
        auto got = rit->find (str);
        if(got != rit->end())
        {
            return got->second;
        }
    }
    return nullptr;
}

// Only looks up current scope
cSymbol * cSymbolTable::Lookup(string str)
{

    auto got = SymbolTable.back().find (str);
    if(got != SymbolTable.back().end())
    {
        return got->second;
    }
    return nullptr;
}

// Inserts language keywords into the table.
void cSymbolTable::InitTable()
{
    cSymbol * keyword;
    keyword = new cSymbol("integer");
    Insert(keyword);
    keyword = new cSymbol("real");
    Insert(keyword);
    keyword = new cSymbol("char");
    Insert(keyword);
    keyword = new cSymbol("program", PROGRAM);
    Insert(keyword);
    keyword = new cSymbol("procedure", PROCEDURE);
    Insert(keyword);
    keyword = new cSymbol("function", FUNCTION);
    Insert(keyword);
    keyword = new cSymbol("begin", OPEN);
    Insert(keyword);
    keyword = new cSymbol("end", CLOSE);
    Insert(keyword);
    keyword = new cSymbol("if", IF);
    Insert(keyword);
    keyword = new cSymbol("then", THEN);
    Insert(keyword);
    keyword = new cSymbol("else", ELSE);
    Insert(keyword);
    keyword = new cSymbol("while", WHILE);
    Insert(keyword);
    keyword = new cSymbol("repeat", REPEAT);
    Insert(keyword);
    keyword = new cSymbol("until", UNTIL);
    Insert(keyword);
    keyword = new cSymbol("for", FOR);
    Insert(keyword);
    keyword = new cSymbol("do", DO);
    Insert(keyword);
    keyword = new cSymbol("to", TO);
    Insert(keyword);
    keyword = new cSymbol("downto", DOWNTO);
    Insert(keyword);
    keyword = new cSymbol("forward", FORWARD);
    Insert(keyword);
    keyword = new cSymbol("const", CONST);
    Insert(keyword);
    keyword = new cSymbol("type", TYPE);
    Insert(keyword);
    keyword = new cSymbol("var", VAR);
    Insert(keyword);
    keyword = new cSymbol("array", ARRAY);
    Insert(keyword);
    keyword = new cSymbol("record", RECORD);
    Insert(keyword);
    keyword = new cSymbol("of", OF);
    Insert(keyword);
    keyword = new cSymbol("nil", NIL);
    Insert(keyword);
    keyword = new cSymbol("or", OR);
    Insert(keyword);
    keyword = new cSymbol("div", DIV);
    Insert(keyword);
    keyword = new cSymbol("mod", MOD);
    Insert(keyword);
    keyword = new cSymbol("and", AND);
    Insert(keyword);
    keyword = new cSymbol("not", NOT);
    Insert(keyword);
    keyword = new cSymbol("write", WRITE);
    Insert(keyword);
}
