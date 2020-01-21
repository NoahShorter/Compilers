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

class cSymbolTable
{
    public:
        void IncreaseScope()
        {
            unordered_map<string, cSymbol *> temp;
            SymbolTable.push_back(temp);
        }
        void DecreaseScope()
        {
            SymbolTable.pop_back();
        }
        cSymbol * Insert(string str)
        {
            cSymbol * newSym = new cSymbol(str);
            SymbolTable.back().insert({str, newSym});
            return newSym;
        }
        cSymbol * GlobalLookup(string str)
        {
            for (auto rit=SymbolTable.rbegin(); rit!=SymbolTable.rend(); ++rit)
            {
                unordered_map<string,cSymbol *>::const_iterator got = rit->find (str);
                if(got != rit->end())
                {
                    return got->second;
                }
            }
            return NULL;
        }
        cSymbol * Lookup(string str)
        {

            unordered_map<string,cSymbol *>::const_iterator got = SymbolTable.back().find (str);
            if(got != SymbolTable.back().end())
            {
                return got->second;
            }
            return NULL;
        }

    private:
        int Scope = -1;
        list<unordered_map<string, cSymbol*>> SymbolTable;
};
