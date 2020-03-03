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

        bool m_paramDecls;
        int m_varDeclSize;
    public:
        // param is the block that makes up the program
        cMemory() : cVisitor()
        {
            m_totalOffset = 0;
            m_funcOffset = 0;

            m_paramDecls = false;
            m_varDeclSize = 0;
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

            if(m_paramDecls)
            {
                if(m_funcOffset % 4 != 0)
                    m_funcOffset -= (4 + (m_funcOffset % 4));

                node->SetOffset(m_funcOffset);
                m_funcOffset -= TypeSize;
                m_varDeclSize += TypeSize > 4 ? TypeSize : 4;
            }
            else
            {
                if(TypeSize != 1 && m_totalOffset % 4 != 0)
                    m_totalOffset += (4 - (m_totalOffset % 4));

                node->SetOffset(m_totalOffset);
                m_totalOffset += decl->GetSize();
            }
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
            vector<int> RowSizes;
            RowSizes.push_back(ArraySize);
            vector<int> StartIndexes;
            StartIndexes.push_back(node->GetRange(0)->GetStart());
            cDeclsNode * ranges = node->GetRanges();

            for (int ii = 0; ii < ranges->NumDecls(); ++ii)
            {
                if(ii != 0)
                {
                    RowSizes.push_back(ArraySize);
                    StartIndexes.push_back(node->GetRange(ii)->GetStart());
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
            m_varDeclSize = 0;

            m_paramDecls = true;
            VisitAllNodes(node);
            m_paramDecls = false;
            
            node->SetSize(m_varDeclSize);
        }

        void Visit(cFuncDeclNode * node)
        {
            int offset = m_totalOffset;

            node->SetSize(node->GetDeclType()->GetSize());
            node->SetOffset(0);

            m_funcOffset = -12;

            Visit(node->GetParams());

            m_totalOffset = node->GetDeclType()->GetSize();

            Visit(node->GetBlock());

            m_totalOffset = offset;
        }

        void Visit(cFuncExprNode * node)
        {
            int size = 
                node->GetFunc()->GetParams()->GetSize();

            node->SetSize(size);
        }

        void Visit(cProcDeclNode * node)
        {
            int offset = m_totalOffset;

            m_funcOffset = -12;

            Visit(node->GetParams());

            m_totalOffset = 0;

            Visit(node->GetBlock());

            m_totalOffset = offset;
        }

        void Visit(cProcCallNode * node)
        {
            int size =
                node->GetProc()->GetParams()->GetSize();

            node->SetSize(size);
        }

};
