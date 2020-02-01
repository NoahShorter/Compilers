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

class cVarDeclNode : public cAstNode
{
    public:
        // param is the first decl in this decls
        cVarDeclNode(cDeclNode *decl = nullptr) : cAstNode()
        {
            if (decl != nullptr) AddChild(decl);
        }

        // Copy all the decls from another node into this one
        void AddDecls(cDeclsNode *decls) { AddAllChildren(decls); }

        // Add another decl to the list
        void AddDecl(cDeclNode *decl) { AddChild(decl); }

        virtual string NodeType() { return string("var_decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
