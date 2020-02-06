#pragma once
//**************************************
// cIfNode.h
//
// Defines AST node for IF statements
//
// Inherits from cStmtNode so IF statments can go in statement lists, and be
// used anywhere a statement is legal.
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprListNode.h"

class cIfNode : public cStmtNode
{
    public:
        // param is the value to be printed
        cIfNode(cExprNode *condition, cStmtNode * ifStmt, 
            cStmtNode *elseStmt = nullptr) : cStmtNode()
        {
            AddChild(condition);
            AddChild(ifStmt);
            AddChild(elseStmt);
        }

        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
