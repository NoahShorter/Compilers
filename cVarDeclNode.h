#pragma once
//**************************************
// cVarDeclNode.h
//
// Defines a class to represent a variable declaration.
//
// Author: Noah Shorter 
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cVarDeclNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cVarDeclNode(cBaseTypeNode * type, cSymbol * id) : cDeclNode()
        {
            cSymbol *token = g_symbolTable.GlobalLookup(id->GetName());
            if (token == nullptr)
                g_symbolTable.Insert(id);

            AddChild(type);
            AddChild(id);
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
