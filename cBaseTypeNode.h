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

class cBaseTypeNode : public cAstNode
{
    public:
        // param is the first decl in this decls
        cBaseTypeNode(cSymbol *type) : cAstNode()
        {
            m_name = type->GetName();
            if(m_name == "integer") m_size = 4;
            else if(m_name == "char") m_size = 1;
            else if(m_name == "real") 
            {
                m_size = 8;
                m_isFloat = 1;
            }
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
