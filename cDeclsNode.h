#pragma once
//**************************************
// cDeclsNode.h
//
// Defines a class to represent a list of declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cDeclsNode : public cAstNode
{
    public:
        // param is the first decl in this decls
        cDeclsNode(cDeclsNode *decl = nullptr) : cAstNode()
        {
            if (decl != nullptr) AddAllChildren(decl);
        }

        // Copy all the decls from another node into this one
        void AddDecls(cDeclsNode *decls) 
        { 
            if(decls != nullptr) AddAllChildren(decls);
        }

        // Add another decl to the list
        void AddDecl(cDeclNode *decl) 
        { 
            if(decl != nullptr) AddChild(decl); 
        }

        virtual string NodeType() { return string("decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};