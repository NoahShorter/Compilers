#pragma once
//**************************************
// cVarExprNode.h
//
// Defines an AST node for an variables.
//
// Inherits from cExprNode so that variables can be used anywhere 
// expressions are used.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbolTable.h"

class cVarExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cVarExprNode(cSymbol *variable) : cExprNode()
        {
            AddChild(variable);
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
