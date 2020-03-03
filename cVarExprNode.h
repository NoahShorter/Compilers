#pragma once
//**************************************
// cVarExprNode.h
//
// Defines an AST node for an variables.
//
// Inherits from cExprNode so that variables can be used anywhere 
// expressions are used.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbolTable.h"
#include "cArrayDeclNode.h"

class cVarExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cVarExprNode(cSymbol *variable) : cExprNode()
        {
            AddChild(variable);
        }

        void AddVar(cSymbol *variable)
        {
            AddChild(variable);
        }

        void AddVar(cExprListNode *exprList)
        {
            AddChild(exprList);
        }

        cSymbol * GetSymbol()
        {
            return dynamic_cast<cSymbol *>(GetChild(0));
        }

        cVarDeclNode * GetVarDecl()
        {
            return dynamic_cast<cVarDeclNode *>(GetSymbol()->GetDecl());
        }

        bool HasIndex()
        {
            return NumChildren() > 1;
        }

        int NumIndexes()
        {
            cExprListNode * exprList = dynamic_cast<cExprListNode *>(GetChild(1));
            if(exprList != nullptr)
            {
                return exprList->NumExpr();
            }
            return 0;
        }

        cExprListNode * GetIndexes()
        {
            return dynamic_cast<cExprListNode *>(GetChild(1));
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        cDeclNode *GetType()
        {
            cSymbol * varSym = dynamic_cast<cSymbol *>(GetChild(0));
            cDeclNode * varDecl = varSym->GetDecl();
            return varDecl == nullptr ? nullptr : varDecl->GetDeclType();
        }

        cDeclNode *GetBaseType()
        {
            cSymbol * varSym = dynamic_cast<cSymbol *>(GetChild(0));
            cDeclNode * varDecl = varSym->GetDecl();
            if(HasIndex())
                return varDecl == nullptr ? nullptr : varDecl->GetDeclType()->GetIndexType();
            else
                return varDecl == nullptr ? nullptr : varDecl->GetDeclType();
        }

        virtual string AttributesToString() 
        {
            string returnString = " size=\"" + std::to_string(GetBaseType()->GetSize()) + "\"" +
                " offset=\"" + std::to_string(m_offset) + "\"";

            cDeclNode * ty = GetType();

            if(ty->IsArray())
            {
                cArrayDeclNode * ar = dynamic_cast<cArrayDeclNode *>(ty);
                returnString += " rowsizes=\"" + ar->GetRowSizesAsString() + "\"" +
                    " startindexes=\"" + ar->GetStartIndexesAsString() + "\"";
            }
            return returnString;
        }
};
