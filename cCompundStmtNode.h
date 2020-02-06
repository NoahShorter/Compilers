#pragma once
//**************************************
// cCompundStmtNode.h
//
// Defines AST node for compund statements
//
// Inherits from cStmtNode so compund statments can go in statement lists, and be
// used anywhere a statement is legal.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprListNode.h"

class cCompoundStmtNode : public cStmtNode
{
    public:
        // param is the value to be printed
        cCompoundStmtNode(cStmtsNode *stmts) : cStmtNode()
        {
            AddAllChildren(stmts);
        }

        virtual string NodeType() { return string("compound"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
