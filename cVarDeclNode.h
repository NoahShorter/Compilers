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
        cVarDeclNode(cDeclNode * type, cSymbol * id) : cDeclNode()
        {
            cSymbol *token = g_symbolTable.GlobalLookup(id->GetName());
            if (token == nullptr)
            {
                token = id;
                g_symbolTable.Insert(id);
            }
            else
            {
                token = new cSymbol(id->GetName());
                g_symbolTable.Insert(token);
            }

            AddChild(type);
            AddChild(token);
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
