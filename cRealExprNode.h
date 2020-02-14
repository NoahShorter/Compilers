#pragma once
//*****************************************
// cRealExprNode.h
//
// Defines an AST node for an real constant (literals).
//
// Inherits from cExprNode so that real constants can be used anywhere 
// expressions are used.
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//
//

#include "cAstNode.h"
#include "cExprNode.h"

class cRealExprNode : public cExprNode
{
    public:
        // param is the value of the double constant
        cRealExprNode(float value) : cExprNode()
        {
            m_value = value;
        }

        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }
        virtual string NodeType() { return string("REAL"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        cDeclNode *GetType()
        {
            cSymbol * realSym = g_symbolTable.GlobalLookup("real");
            return realSym->GetDecl();
        }

    protected:
        float m_value;        // value of float constant (literal)
};