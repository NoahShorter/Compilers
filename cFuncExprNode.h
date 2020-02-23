#pragma once
//**************************************
// cFuncCallNode.h
//
// Defines FuncCall node for a lang program
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"
#include "cExprNode.h"
#include "cSymbolTable.h"

class cFuncExprNode : public cExprNode
{
    public:
        // param is the block that makes up the program
        cFuncExprNode(cSymbol *name, cExprListNode *args = nullptr) : cExprNode()
        {
            AddChild(name);
            AddChild(args);
        }
        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        cSymbol * GetName()
        {
            return dynamic_cast<cSymbol *>(GetChild(0));
        }

        cExprListNode * GetParams()
        {
            return dynamic_cast<cExprListNode *>(GetChild(1));
        }

        cDeclNode *GetType()
        {
            cSymbol * funcNameSym = dynamic_cast<cSymbol *>(GetChild(0));
            cSymbol * returnSym = nullptr;
            if(funcNameSym != nullptr)
            {
                cFuncDeclNode * returnDecl = 
                    dynamic_cast<cFuncDeclNode *>(funcNameSym->GetDecl());
                if(returnDecl != nullptr)
                    returnSym = returnDecl->GetReturnType();
            }
            return returnSym != nullptr ? returnSym->GetDecl() : nullptr;
        }
};
