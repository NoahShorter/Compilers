#pragma once

#include <unordered_map>
#include <list>
#include "cSymbol"

class cSymbolTable
{
    public:
        void IncreaseScope()
        {
            Scope++;
            SymbolTable[Scope].clear();
        }
        void DecreaseScope();
        {
            Scope--;
        }
        void insert(std::string str, cSymbol * sym)
        {
            SymbolTable[Scope].insert(std::make_pair<std::string, cSymbol *>(str, sym));
        }
        cSymbol * lookup(std::string str)
        {
            int scopeRemeber = Scope;
            bool found = false;
            while(!found)
            {
                std::unordered_map<std::string,cSymbol *>::const_iterator got = SymbolTable[Scope].find (str);
                if(got == SymbolTable[Scope].end())
                {
                    if(Scope != 0){Scope--;}
                }
                else
                {
                    Scope = scopeRemember;
                    return got->second();
                }
            }
        }

    private:
        int Scope = -1;
        std::list<std::unordered_map<std::string, cSymbol*>> SymbolTable;
}
