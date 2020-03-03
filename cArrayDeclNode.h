#pragma once
//**************************************
// cArrayDeclNode.h
//
// Defines ArrayDecl node for a lang program
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
#include "cRangeDeclNode.h"

class cArrayDeclNode : public cDeclNode
{
    public:
        // param is the block that makes up the program
        cArrayDeclNode(cSymbol *name, cSymbol *type, cDeclsNode *ranges) : cDeclNode()
        {
            name->SetDecl(this);
            name->SetIsType(true);
            g_symbolTable.Insert(name);
            AddChild(name);
            AddChild(type->GetDecl());
            AddChild(ranges);
            m_rowSizes.clear();
            m_startIndexes.clear();
        }

        cDeclNode * GetDeclType()
        {
            return this;
        }

        cDeclNode * GetIndexType()
        {
            return dynamic_cast<cDeclNode *>(GetChild(1));
        }

        string GetName()
        {
            return dynamic_cast<cSymbol *>(GetChild(0))->GetName();
        }

        bool IsType()
        {
            return true;
        }

        bool IsArray()
        {
            return true;
        }

        int NumIndexes()
        {
            cDeclsNode * indexes = dynamic_cast<cDeclsNode *>(GetChild(2));
            return indexes->NumDecls();
        }

        cDeclsNode * GetRanges()
        {
            return dynamic_cast<cDeclsNode *>(GetChild(2));
        }

        cRangeDeclNode * GetRange(int index)
        {
            return dynamic_cast<cRangeDeclNode *>(GetRanges()->GetDecl(index));
        }

        vector<int> GetRowSizes() { return m_rowSizes; }
        void SetRowSizes(vector<int> rowSize) { m_rowSizes = rowSize; }
        string GetRowSizesAsString() 
        { 
            string RowSizes = "";
            for(auto it = m_rowSizes.begin(); it != m_rowSizes.end(); ++it)
            {
                RowSizes += std::to_string(*it) + " ";
            }
            RowSizes.pop_back();
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
            StartIndexes.pop_back();
            return StartIndexes;
        }

        virtual string NodeType() { return string("array"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString() 
        {
            return " size=\"" + std::to_string(m_size) + "\"" +
                " rowsizes=\"" + GetRowSizesAsString() + "\"" +
                " startindexes=\"" + GetStartIndexesAsString() + "\"";
        }

    private:
        vector<int> m_rowSizes;
        vector<int> m_startIndexes;
};
