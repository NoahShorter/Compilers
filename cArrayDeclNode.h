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
            m_rowSizes = "";
            m_startIndexes = "";
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

        string GetRowSizes() { return m_rowSizes; }
        void SetRowSizes(string rowSize) { m_rowSizes = rowSize; }

        string GetStartIndexes() { return m_startIndexes; }
        void SetStartIndexes(string startIndexes) { m_startIndexes = startIndexes; }

        virtual string NodeType() { return string("array"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString() 
        {
            return " size=\"" + std::to_string(m_size) + "\"" +
                " rowsizes=\"" + m_rowSizes + "\"" +
                " startindexes=\"" + m_startIndexes + "\"";
        }

    private:
        string m_rowSizes;
        string m_startIndexes;
};
