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
        int m_funcOffset;
    public:
        // param is the block that makes up the program
        cMemory() : cVisitor()
        {
            m_totalOffset = 0;
            m_funcOffset = 0;
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
            VisitAllNodes(node);
            cDeclNode * decl = node->GetVarDecl();
            if(decl != nullptr)
            {
                node->SetSize(decl->GetSize());
                node->SetOffset(decl->GetOffset());
            }
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

        void Visit(cVarDeclsNode * node)
        {
            VisitAllNodes(node);
            int VarSize = 0;
            for (int ii = 0; ii < node->NumVarDecls(); ++ii)
                VarSize += (node->GetParam(ii)->GetSize()) > 4 ? 
                    node->GetParam(ii)->GetSize() : 4 ;

            node->SetSize(VarSize);
            m_totalOffset -= VarSize;
        }

        void Visit(cFuncDeclNode * node)
        {
            int tempTotal = m_totalOffset;
            Visit(node->GetParams());
            m_funcOffset = 0;
            node->SetSize(4);
            node->SetOffset(m_funcOffset);
            m_funcOffset -= 12;

            cVarDeclsNode * params = node->GetParams();

            for (int ii = 0; ii < params->NumVarDecls(); ++ii)
            {
                int TypeSize = params->GetParam(ii)->GetSize();

                if(m_funcOffset % 4 != 0)
                    m_funcOffset -= (4 + (m_funcOffset % 4));

                params->GetParam(ii)->SetOffset(m_funcOffset);
                m_funcOffset -= TypeSize;
            }
            
            m_totalOffset = tempTotal;
            Visit(node->GetBlock());

            m_totalOffset = tempTotal;
        }

        void Visit(cFuncExprNode * node)
        {
            int size = 
                node->GetFunc()->GetParams()->GetSize();

            node->SetSize(size);
        }

};
