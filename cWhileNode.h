#pragma once
//**************************************
// cWhileNode.h
//
// Defines AST node for WHILE statements
//
// Inherits from cStmtNode so WHILE statments can go in statement lists, and be
// used anywhere a statement is legal.
//
// Author: Noah Shorter 
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprListNode.h"

class cWhileNode : public cStmtNode
{
    public:
        // param is the value to be printed
        cWhileNode(cExprNode *expr, cStmtNode *stmt) : cStmtNode()
        {
            AddChild(expr);
            AddChild(stmt);
        }

        cExprNode * GetCondition()
        {
            return dynamic_cast<cExprNode *>(GetChild(0));
        }

        cStmtNode * GetStmt()
        {
            return dynamic_cast<cStmtNode *>(GetChild(1));
        }

        virtual string NodeType() { return string("while"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
