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

        virtual int GetValue() { return 0; }

        int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }

        int GetOffset() { return m_offset; }
        void SetOffset(int offset) { m_offset = offset; }

        vector<int> GetRowSizes() { return m_rowSizes; }
        void SetRowSizes(vector<int> rowSize) { m_rowSizes = rowSize; }
        string GetRowSizesAsString() 
        { 
            string RowSizes = "";
            for(auto it = m_rowSizes.begin(); it != m_rowSizes.end(); ++it)
            {
                RowSizes += std::to_string(*it) + " ";
            }
            if(RowSizes != "") RowSizes.pop_back();
            return RowSizes;
        }

        vector<int> GetStartIndexes() { return m_startIndexes; }
        void SetStartIndexes(vector<int> startIndexes) { m_startIndexes = startIndexes; }
        string GetStartIndexesAsString() 
        { 
            string StartIndexes = "";
            for(auto it = m_startIndexes.begin(); it != m_startIndexes.end(); ++it)
            {
                StartIndexes += std::to_string(*it) + " ";
            }
            if(StartIndexes != "") StartIndexes.pop_back();
            return StartIndexes;
        }

    protected:
        int m_size;
        int m_offset;
        vector<int> m_rowSizes;
        vector<int> m_startIndexes;
};
