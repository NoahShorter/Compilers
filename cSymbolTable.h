#pragma once

#include <unordered_map>
#include <list>
#include "cSymbol.h"

using std::unordered_map;
using std::string;
using std::make_pair;
using std::list;

class cSymbolTable
{
    public:
        void IncreaseScope()
        {
            //Scope++;
            unordered_map<string, cSymbol *> temp;
            SymbolTable.push_back(temp);
        }
        void DecreaseScope()
        {
            //Scope--;
            SymbolTable.pop_back();
        }
        void insert(string str)
        {
            cSymbol newSym(str);
            SymbolTable.back().insert({str, &newSym});
        }
        cSymbol * lookup(string str)
        {
                for (auto rit=SymbolTable.rbegin(); rit!=SymbolTable.rend(); ++rit)
                {
                    unordered_map<string,cSymbol *>::const_iterator got = rit->find (str);
                    if(got != rit->end())
                    {
                        return got->second;
                    }
                }
        }

    private:
        int Scope = -1;
        list<unordered_map<string, cSymbol*>> SymbolTable;
};
