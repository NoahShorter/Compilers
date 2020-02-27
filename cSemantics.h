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
            cFuncDeclNode * nameDecl = 
                dynamic_cast<cFuncDeclNode *>(funcName->GetDecl());

            if (nameDecl == nullptr)
            {
                if (funcName->GetDecl())
                {
                    node->SemanticError(
                        funcName->GetName() +" is not a function");
                }
                else
                {
                    node->SemanticError(
                        funcName->GetName() +" is not declared");
                }
            }
            else if (!nameDecl->HasBlock())
            {
                node->SemanticError(
                    funcName->GetName() +" is not fully defined");
            }
            if (nameDecl != nullptr && node != nullptr)
            {
                cVarDeclsNode * params = nameDecl->GetParams();
                cExprListNode * callParams = node->GetParams();
                if(params != nullptr && callParams != nullptr)
                {
                    if (params->NumDecls() != callParams->NumExpr())
                        node->SemanticError(
                            funcName->GetName() +
                            " called with wrong number of arguments");
                    else
                    {
                        for(int ii = 0; ii < params->NumDecls(); ++ii)
                        {
                            cDeclNode * lval = params->GetParam(ii)->GetDeclType();
                            cDeclNode * rval = callParams->GetExpr(ii)->GetType();
                            if(!lval->IsCompatable(rval))
                            {
                                node->SemanticError(
                                    "Cannot assign " +
                                    rval->GetName() +
                                    " to " +
                                    lval->GetName());
                            }
                        }
                    }
                }
            }

        }

        void Visit(cVarExprNode *node)
        {
            VisitAllNodes(node);
            if(node != nullptr && !(node->GetType()))
            {
                node->SemanticError(
                    "Variable " + 
                    node->GetSymbol()->GetName() + 
                    " is not defined");
            }
            else if(node != nullptr && node->HasIndex())
            {
                if(!node->GetType()->IsArray())
                {
                    node->SemanticError(
                        node->GetSymbol()->GetName() + 
                        " is not an array");
                }
                else
                {
                    cArrayDeclNode * var = 
                        dynamic_cast<cArrayDeclNode *>(node->GetType());
                    
                    cExprListNode * indexes = node->GetIndexes();

                    if(node->NumIndexes() != var->NumIndexes())
                    {
                        node->SemanticError(
                            node->GetSymbol()->GetName() + 
                            " does not have the correct number of indexes");
                    }
                    else
                    {
                        for(int ii = 0; ii < node->NumIndexes(); ++ii)
                        {
                            cExprNode * index = indexes->GetExpr(ii);
                            if(!index->GetBaseType()->IsInt() &&
                                !index->GetBaseType()->IsChar())
                            {
                                node->SemanticError(
                                    "Index of " +
                                    node->GetSymbol()->GetName() + 
                                    " is not an integer");
                            }
                        }
                    }
                }
            }
        }

        void Visit(cAssignNode *node)
        {
            VisitAllNodes(node);
            if(!node->HasSemanticError())
            {
                cVarExprNode * lvalExpr = node->GetLval();
                cExprNode * rvalExpr = node->GetExpr();

                if(lvalExpr != nullptr && rvalExpr != nullptr)
                {
                    cDeclNode * lval = lvalExpr->GetBaseType();
                    /*if(lvalExpr->HasIndex())
                        lval = lval->GetIndexType();
                    else
                        lval = lval->GetDeclType();*/

                    cDeclNode * rval = rvalExpr->GetBaseType();
                    if (rval != nullptr)
                    {    
                        //rval = rval->GetType();
                        /*if (rvalExpr->HasIndex())
                            rval = rval->GetIndexType();
                        else
                            rval = rval->GetDeclType();*/
                    }

                    if(!lval->IsCompatable(rval))
                    {
                        node->SemanticError(
                            "Cannot assign " +
                            rval->GetName() +
                            " to " +
                            lval->GetName());
                    }
                }
            }
        }

};
