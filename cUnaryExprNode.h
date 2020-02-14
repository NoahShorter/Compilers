#pragma once
//*****************************************
// cUnaryExprNode.h
//
// Defines an AST node for an unnary expressions.
//
// Inherits from cExprNode so that unnary constants can be used anywhere 
// expressions are used.
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//
//

#include "cAstNode.h"
#include "cExprNode.h"

class cUnaryExprNode : public cExprNode
{
    public:
        // param is the value of the double constant
        cUnaryExprNode(int op, cExprNode *expr) : cExprNode()
        {
            AddChild(new cOpNode(op));
            AddChild(expr);
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string NodeType() { return string("unaryExpr"); }

        cDeclNode *GetType()
        {
            return dynamic_cast<cSymbol *>(GetChild(1))->GetDecl();
        }

    protected:
};