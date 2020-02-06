#pragma once
//**************************************
// cVarDeclsNode.h
//
// Defines a class to represent a list of variable declaration.
//
// Author: Noah Shorter 
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cIdListNode.h"

class cVarDeclsNode : public cDeclsNode
{
    public:
        // param is the first decl in this decls
        cVarDeclsNode(cSymbol * type, cIdListNode * idList) : cDeclsNode()
        {
            int count = idList->NumKids();
            for(int ii = 0; ii < count ; ++ii)
            {
                AddChild(new cVarDeclNode(type->GetDecl(), idList->GetKid(ii)));
            }
        }

        void AddVarDecls(cSymbol * type, cIdListNode * idList)
        {
            int count = idList->NumKids();
            for(int ii = 0; ii < count ; ++ii)
            {
                AddChild(new cVarDeclNode(type->GetDecl(), idList->GetKid(ii)));
            }
        }

        virtual string NodeType() { return string("var_decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
