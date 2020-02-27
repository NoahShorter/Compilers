#pragma once
//**************************************
// cRecordDeclNode.h
//
// Defines RecordDecl node for a lang program
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cVisitor.h"

class cMemory : public cVisitor
{
    private:
        int m_totalOffset;
    public:
        // param is the block that makes up the program
        cMemory() : cVisitor()
        {
            m_totalOffset = 0;
        }
        virtual void VisitAllNodes(cAstNode *node)
        {
            node->VisitAllChildren(this);
        }

        void Visit(cBlockNode * node)
        {
            VisitAllNodes(node);
            int BlockOffset = m_totalOffset;
            if(m_totalOffset % 4 != 0)
                BlockOffset += (4 - (m_totalOffset % 4));
            node->SetSize(BlockOffset);
        }

        void Visit(cVarDeclNode * node)
        {
            cDeclNode * decl = node->GetDeclType();
            int TypeSize = decl->GetSize();
            node->SetSize(TypeSize);

            if(TypeSize != 1 && m_totalOffset % 4 != 0)
                m_totalOffset += (4 - (m_totalOffset % 4));

            node->SetOffset(m_totalOffset);
            m_totalOffset += decl->GetSize();
        }

        void Visit(cVarExprNode * node)
        {
            cDeclNode * decl = node->GetVarDecl();
            node->SetSize(decl->GetSize());
            node->SetOffset(decl->GetOffset());
        }

        void Visit(cArrayDeclNode * node)
        {
            VisitAllNodes(node);
            int ArraySize = node->GetIndexType()->GetSize();
            string RowSizes = std::to_string(ArraySize);
            string StartIndexes = 
                std::to_string(node->GetRange(0)->GetStart());
            cDeclsNode * ranges = node->GetRanges();

            for (int ii = 0; ii < ranges->NumDecls(); ++ii)
            {
                if(ii != 0)
                {
                    RowSizes += " " + std::to_string(ArraySize);
                    StartIndexes += " " + std::to_string(node->GetRange(ii)->GetStart());
                }
                int rangeSize = ranges->GetDecl(ii)->GetSize();
                ArraySize *= rangeSize;
            }

            node->SetSize(ArraySize);
            node->SetRowSizes(RowSizes);
            node->SetStartIndexes(StartIndexes);
        }

        void Visit(cRangeDeclNode * node)
        {
            int startValue = node->GetStart();
            int endValue = node->GetEnd();
            node->SetSize((endValue - startValue) + 1);
        }

};
