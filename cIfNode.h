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

        cExprNode * GetCondition()
        {
            return dynamic_cast<cExprNode *>(GetChild(0));
        }

        cStmtNode * GetIfStmt()
        {
            return dynamic_cast<cStmtNode *>(GetChild(1));
        }

        cStmtNode * GetElseStmt()
        {
            return dynamic_cast<cStmtNode *>(GetChild(2));
        }

        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
