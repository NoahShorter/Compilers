#pragma once
//**************************************
// cConstDeclNode.h
//
// Defines a class to represent a const declaration.
//
// Author: Noah Shorter 
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"

extern cSymbolTable g_symbolTable;

class cConstDeclNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cConstDeclNode(cSymbol *name, cExprNode * value) : cDeclNode()
        {
            cSymbol *token = g_symbolTable.GlobalLookup(name->GetName());
            if (token == nullptr)
            {
                token = name;
                g_symbolTable.Insert(name);
            }
            else
            {
                token = new cSymbol(name->GetName());
                g_symbolTable.Insert(token);
            }

            token->SetDecl(this);
            AddChild(token);
            AddChild(value);
        }

        virtual string NodeType() { return string("const_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
