#pragma once
//**************************************
// cIdListNode.h
//
// Defines a class to represent a list of base type.
//
// Author: Noah Shorter 
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"

class cIdListNode : public cAstNode
{
    public:
        // param is the first decl in this decls
        cIdListNode(cSymbol *identifier) : cAstNode()
        {
            AddChild(identifier);
        }

        void AddId(cSymbol *identifier)
        {
            AddChild(identifier);
        }

        int NumKids() { return NumChildren(); }
        cSymbol* GetKid(int i) { return static_cast<cSymbol*>(GetChild(i)); }

        virtual string NodeType() { return string("IdList"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
};
