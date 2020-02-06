//**********************************************
// This class takes care of the stack of hash tables.
//
// Functions:
//  IncreaseScope
//  DecreaseScope
//  Insert
//  GlobalLookup
//  Lookup
//
// Author: Noah Shorter
//**********************************************
#pragma once

#include <unordered_map>
#include <list>
#include "cSymbol.h"

using std::unordered_map;
using std::string;
using std::list;

typedef unordered_map<string, cSymbol*> symbolTable_t;
    
class cSymbolTable
{
    public:

        cSymbolTable();

        void IncreaseScope();
        void DecreaseScope();

        cSymbol * Insert(cSymbol * symbol);

        cSymbol * GlobalLookup(string str);
        cSymbol * Lookup(string str);

        void InitTable();
    private:
        int Scope = -1;
        list<unordered_map<string, cSymbol*>> SymbolTable;
};

extern cSymbolTable g_symbolTable;
