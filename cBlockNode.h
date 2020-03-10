#pragma once
//**************************************
// cBlockNode.h
//
// Defines AST node for a block of code (stuff inside {})
//
// Since blocks can take the place of statements, this class inherits from 
// cStmtNode
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cBlockNode : public cStmtNode
{
    public:
        // params are the decls and statements contained in the block
        cBlockNode(cDeclsNode *decls, cStmtsNode *statements)
            : cStmtNode()
        {
            m_size = 0; //decls->GetSize() + statements->GetSize();
            AddChild(decls);
            AddChild(statements);
        }

        cDeclsNode * GetDecls()
        {
            return dynamic_cast<cDeclsNode *>(GetChild(0));
        }

        cStmtsNode * GetStmts()
        {
            return dynamic_cast<cStmtsNode *>(GetChild(1));
        }
        virtual string NodeType() { return string("block"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        int GetSize() { return m_size; }
        void SetSize(int size) { m_size = size; }

        virtual string AttributesToString() 
        {
            return " size=\"" + std::to_string(m_size) + "\"";
        }

    private:
        int m_size;  
};
