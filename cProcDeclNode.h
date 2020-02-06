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
            g_symbolTable.Insert(name);

            AddChild(name);
        }

        void AddParamBlock(cBlockNode *block, cDeclsNode *vardecls = nullptr)
        {
            AddChild(vardecls);
            AddChild(block);
        }
        
        virtual string NodeType() { return string("proc"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
