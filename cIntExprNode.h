#pragma once
//**************************************
// cIntExprNode.h
//
// Defines an AST node for an integer constant (literals).
//
// Inherits from cExprNode so that integer constants can be used anywhere 
// expressions are used.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"

class cIntExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cIntExprNode(int value) : cExprNode()
        {
            m_value = value;
        }

        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }
        virtual string NodeType() { return string("INTEGER"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        cDeclNode *GetType()
        {
            if (m_value > -128 && m_value < 127)
            {
                cSymbol * intSym = g_symbolTable.GlobalLookup("char");
                return intSym->GetDecl();
            }
            else
            {
                cSymbol * intSym = g_symbolTable.GlobalLookup("integer");
                return intSym->GetDecl();
            }
        }

        int GetValue()
        {
            return m_value;
        }

    protected:
        int m_value;        // value of integer constant (literal)
};
