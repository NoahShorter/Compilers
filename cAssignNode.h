#pragma once
//**************************************
// cAssignNode.h
//
// Defines an AST node for an Assignment Statements.
//
// Inherits from cStmtNode so Assignment statments 
// can go in statement lists, and be
// used anywhere a statement is legal.
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cVarExprNode.h"

class cAssignNode : public cStmtNode
{
    public:
        // param is the value of the integer constant
        cAssignNode(cVarExprNode *var, cExprNode * expr) : cStmtNode()
        {
            AddChild(var);
            AddChild(expr);
        }

        cVarExprNode * GetLval()
        {
            return dynamic_cast<cVarExprNode *>(GetChild(0));
        }

        cExprNode * GetExpr()
        {
            return dynamic_cast<cExprNode *>(GetChild(1));
        }

        virtual string NodeType() { return string("assign"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
