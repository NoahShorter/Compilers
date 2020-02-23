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
#include "cVisitor.h"

class cSemantics : public cVisitor
{
    public:
        // param is the block that makes up the program
        cSemantics() : cVisitor()
        {
        }
        virtual void VisitAllNodes(cAstNode *node)
        {
            node->VisitAllChildren(this);
        }

        void Visit(cFuncExprNode *node)
        { 
            cSymbol * funcName = node->GetName();
            cFuncDeclNode * nameDecl = dynamic_cast<cFuncDeclNode *>(funcName->GetDecl());

            if (nameDecl == nullptr)
            {
                if (funcName->GetDecl())
                {
                    node->SemanticError(funcName->GetName() +" is not a function");
                }
                else
                {
                    node->SemanticError(funcName->GetName() +" is not declared");
                }
            }
            else if (!nameDecl->HasBlock())
            {
                node->SemanticError(funcName->GetName() +" is not fully defined");
            }
            if (nameDecl != nullptr && node != nullptr)
            {
                if (nameDecl->GetParams()->NumDecls() != node->GetParams()->NumExpr())
                    node->SemanticError(funcName->GetName() +" called with wrong number of arguments");
            }

        }

        void Visit(cVarExprNode *node)
        {
            if(node != nullptr && !(node->GetType()))
            {
                node->SemanticError(
                    "Variable " + node->GetSymbol()->GetName() + " is not defined");
            }
        }

        void Visit(cAssignNode *node)
        {
            VisitAllNodes(node);
            cVarExprNode * lvalExpr = node->GetLval();
            cExprNode * rvalExpr = node->GetExpr();

            if(lvalExpr != nullptr && rvalExpr != nullptr)
            {
                cDeclNode * lval = lvalExpr->GetType();
                cDeclNode * rval = rvalExpr->GetType();
                /*if(lval != rval)
                {
                    node->SemanticError(
                        "Variable  is not defined");
                }*/
            }
        }

};
