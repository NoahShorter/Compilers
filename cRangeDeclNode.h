#pragma once
//**************************************
// cRangeDeclNode.h
//
// Defines RangeDecl node for a lang program
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"
#include "cBlockNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"
#include "cDeclsNode.h"

class cRangeDeclNode : public cDeclNode
{
    public:
        // param is the block that makes up the program
        cRangeDeclNode(int start, int end) : cDeclNode()
        {
            m_start = start;
            m_end = end;
        }

        virtual string AttributesToString()
        {
            string result(" start=\"");
            result += std::to_string(m_start);
            result += "\" end=\"" + std::to_string(m_end) + "\"";
            return result;
        }

        cDeclNode * GetDeclType()
        {
            return nullptr;
        }

        string GetName()
        {
            return "range";
        }

        virtual string NodeType() { return string("range"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        protected:
        int m_start;
        int m_end;
};
