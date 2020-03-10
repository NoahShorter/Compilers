#pragma once
//**************************************
// cProcDeclNode.h
//
// Defines ProcDecl node for a lang program
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

class cProcDeclNode : public cDeclNode
{
    public:
        // param is the block that makes up the program
        cProcDeclNode(cSymbol *name) : cDeclNode()
        {
            cSymbol *token = g_symbolTable.GlobalLookup(name->GetName());
            if (token == nullptr)
            {
                token = name;
                g_symbolTable.Insert(name);
            }
            else
            {
                token = new cSymbol(name->GetName());
                g_symbolTable.Insert(token);
            }

            token->SetDecl(this);
            AddChild(token);
        }

        void AddParamBlock(cBlockNode *block, cDeclsNode *vardecls = nullptr)
        {
            AddChild(vardecls);
            AddChild(block);
        }

        void AddParam(cDeclsNode *vardecls)
        {
            AddChild(vardecls);
        }

        cVarDeclsNode * GetParams()
        {
            return dynamic_cast<cVarDeclsNode *>(GetChild(1));
        }
        
        cBlockNode * GetBlock()
        {
            return HasBlock() ? dynamic_cast<cBlockNode *>(GetChild(2))
                : nullptr;
        }

        bool HasBlock()
        {
            return !(dynamic_cast<cBlockNode *>(GetChild(2))
                         == nullptr) ? true : false;
        }

        cDeclNode * GetDeclType()
        {
            return nullptr;
        }

        string GetName()
        {
            return "Procedure";
        }

        string GetFuncName()
        {
            return dynamic_cast<cSymbol *>(GetChild(0))->GetName();
        }

        bool IsProc()  { return true; }

        int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }

        virtual string NodeType() { return string("proc"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString() 
        {
            return "";
        }

    private:
        int m_size;
};
