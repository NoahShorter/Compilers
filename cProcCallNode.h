#pragma once
//**************************************
// cProcCallNode.h
//
// Defines ProcCall node for a lang program
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"
#include "cStmtNode.h"
#include "cSymbolTable.h"

class cProcCallNode : public cStmtNode
{
    public:
        // param is the block that makes up the program
        cProcCallNode(cSymbol *name, cExprListNode *args = nullptr) 
            : cStmtNode()
        {
            AddChild(name);
            AddChild(args);
        }

        cProcDeclNode * GetProc()
        {
            cSymbol * name = dynamic_cast<cSymbol *>(GetChild(0));
            cProcDeclNode * returnDecl = nullptr;
            if(name != nullptr)
            {
                returnDecl = dynamic_cast<cProcDeclNode *>(name->GetDecl());
            }
            return returnDecl;
        }

        int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }

        virtual string NodeType() { return string("procCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString() 
        {
            return" paramSize=\"" + std::to_string(m_size) + "\"";
        }

    private:
        int m_size;
};
