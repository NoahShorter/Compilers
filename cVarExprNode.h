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

        void AddVar(cSymbol *variable)
        {
            AddChild(variable);
        }

        void AddVar(cExprListNode *exprList)
        {
            AddChild(exprList);
        }

        cSymbol * GetSymbol()
        {
            return dynamic_cast<cSymbol *>(GetChild(0));
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        cDeclNode *GetType()
        {
            cSymbol * varSym = dynamic_cast<cSymbol *>(GetChild(0));
            return varSym->GetDecl();
        }
};
