#pragma once
//**************************************
// cArrayDeclNode.h
//
// Defines ArrayDecl node for a lang program
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

class cArrayDeclNode : public cDeclNode
{
    public:
        // param is the block that makes up the program
        cArrayDeclNode(cSymbol *name, cSymbol *type, cDeclsNode *ranges) : cDeclNode()
        {
            name->SetDecl(this);
            name->SetIsType(true);
            g_symbolTable.Insert(name);
            AddChild(name);
            AddChild(type->GetDecl());
            AddChild(ranges);
        }

        virtual string NodeType() { return string("array"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
