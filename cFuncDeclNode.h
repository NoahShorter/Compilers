#pragma once
//**************************************
// cFuncDeclNode.h
//
// Defines FuncDecl node for a lang program
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"
#include "cBlockNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"
#include "cDeclsNode.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        // param is the block that makes up the program
        cFuncDeclNode(cSymbol *name) : cDeclNode()
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

            AddChild(token);
        }

        void AddParamType(cSymbol *type, cDeclsNode *vardecls = nullptr)
        {
            AddChild(type->GetDecl());
            AddChild(vardecls);
        }

        void AddBlock(cBlockNode *block)
        {
            AddChild(block);
        }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
