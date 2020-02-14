#pragma once
//*****************************************
// cBinaryExprNode.h
//
// Defines an AST node for an binary expressions.
//
// Inherits from cExprNode so that binary constants can be used anywhere 
// expressions are used.
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//
//

#include "cAstNode.h"
#include "cExprNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        // param is the value of the double constant
        cBinaryExprNode(cExprNode *left, cExprNode *right, int op) : cExprNode()
        {
            AddChild(left);
            AddChild(new cOpNode(op));
            AddChild(right);
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string NodeType() { return string("expr"); }

        cDeclNode *GetType()
        {
            return nullptr;
        }
    protected:
};