#pragma once
//**************************************
// cProcDeclNode.h
//
// Defines ProcDecl node for a lang program
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

class cProcDeclNode : public cDeclNode
{
    public:
        // param is the block that makes up the program
        cProcDeclNode(cSymbol *name) : cDeclNode()
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

        void AddParamBlock(cBlockNode *block, cDeclsNode *vardecls = nullptr)
        {
            AddChild(vardecls);
            AddChild(block);
        }

        void AddParam(cDeclsNode *vardecls)
        {
            AddChild(vardecls);
        }
        
        virtual string NodeType() { return string("proc"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
