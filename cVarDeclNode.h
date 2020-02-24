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
            cSymbol *name = g_symbolTable.GlobalLookup(id->GetName());
            if (name == nullptr)
            {
                name = id;
                g_symbolTable.Insert(id);
            }
            else
            {
                cSymbol * sym = g_symbolTable.LocalLookup(id->GetName());
                if (sym == nullptr)
                {
                    name = new cSymbol(id->GetName());
                    g_symbolTable.Insert(name);
                }
                else
                {
                    SemanticParseError(
                        "Symbol " + sym->GetName() + 
                        " already exists in current scope");
                }
                
            }

            name->SetDecl(this);
            AddChild(type);
            AddChild(name);
        }

        cDeclNode * GetDeclType()
        {
            return dynamic_cast<cDeclNode *>(GetChild(0));
        }

        string GetName()
        {
            return GetDeclType()->GetName();
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
