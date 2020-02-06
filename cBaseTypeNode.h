#pragma once
//**************************************
// cBaseTypeNode.h
//
// Defines a class to represent a base type.
//
// Author: Noah Shorter 
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"

class cBaseTypeNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cBaseTypeNode(cSymbol *type, int size, bool isFloat = false) : cDeclNode()
        {
            m_name = type->GetName();
            m_size = size;
            m_isFloat = isFloat;
        }

        virtual string AttributesToString() 
        {
            return " name=\"" + m_name + "\"" +
                    " size=\"" + std::to_string(m_size) + "\"" +
                    " isFloat=\"" + std::to_string(m_isFloat) + "\"";
        }

        virtual string NodeType() { return string("type"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        string m_name;
        int m_size;
        int m_isFloat = 0;
};