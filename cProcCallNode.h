#pragma once
//**************************************
// cProcCallNode.h
//
// Defines ProcCall node for a lang program
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"
#include "cStmtNode.h"
#include "cSymbolTable.h"

class cProcCallNode : public cStmtNode
{
    public:
        // param is the block that makes up the program
        cProcCallNode(cSymbol *name, cExprListNode *args = nullptr) : cStmtNode()
        {
            AddChild(name);
            AddChild(args);
        }
        virtual string NodeType() { return string("procCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
