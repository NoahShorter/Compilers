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

        cFuncDeclNode * GetFunc()
        {
            cSymbol * funcNameSym = dynamic_cast<cSymbol *>(GetChild(0));
            cFuncDeclNode * returnDecl = nullptr;
            if(funcNameSym != nullptr)
            {
                returnDecl = 
                    dynamic_cast<cFuncDeclNode *>(funcNameSym->GetDecl());
            }
            return returnDecl;
        }

        cDeclNode *GetType()
        {
            cSymbol * funcNameSym = dynamic_cast<cSymbol *>(GetChild(0));
            cDeclNode * returnDeclType = nullptr;
            if(funcNameSym != nullptr)
            {
                cFuncDeclNode * returnDecl = 
                    dynamic_cast<cFuncDeclNode *>(funcNameSym->GetDecl());
                if(returnDecl != nullptr)
                    returnDeclType = returnDecl->GetDeclType();
            }
            return returnDeclType;
        }

        virtual string AttributesToString() 
        {
            return" paramSize=\"" + std::to_string(m_size) + "\"";
        }

};
