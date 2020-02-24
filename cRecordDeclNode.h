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
#include "cSymbol.h"
#include "cBlockNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"
#include "cDeclsNode.h"

class cRecordDeclNode : public cDeclNode
{
    public:
        // param is the block that makes up the program
        cRecordDeclNode(cSymbol *name, cVarDeclsNode *vardecls) : cDeclNode()
        {
            name->SetDecl(this);
            name->SetIsType(true);
            g_symbolTable.Insert(name);
            AddChild(name);
            AddChild(vardecls);
        }

        cDeclNode * GetDeclType()
        {
            cVarDeclsNode * vardecls = 
                dynamic_cast<cVarDeclsNode *>(GetChild(1));
            return vardecls->GetType();
        }

        string GetName()
        {
            return "record";
        }

        virtual string NodeType() { return string("record"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
