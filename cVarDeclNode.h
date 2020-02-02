#pragma once
//**************************************
// cVarDeclNode.h
//
// Defines a class to represent a variable declaration.
//
// Author: Noah Shorter 
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cVarDeclNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cVarDeclNode(cBaseTypeNode * type, cSymbol * id) : cDeclNode()
        {
            AddChild(type);
            AddChild(id);
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
