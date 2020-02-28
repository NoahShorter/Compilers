#pragma once
//**************************************
// cExprNode.h
//
// Defines base class for all expressions
//
// This is a pure virtual class because there is no definition for
// cAstNode::NodeType()
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cStmtNode.h"

class cExprNode : public cStmtNode
{
    public:
        cExprNode() : cStmtNode() {}

        virtual cDeclNode *GetType() = 0;
        virtual cDeclNode *GetBaseType() { return GetType(); }

        virtual bool HasIndex() { return false; }

        int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }

        int GetOffset() { return m_offset; }
        void SetOffset(int offset) { m_offset = offset; }

    protected:
        int m_size;
        int m_offset;
};
