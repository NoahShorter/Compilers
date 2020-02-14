#pragma once
//**************************************
// cFuncDeclNode.h
//
// Defines FuncDecl node for a lang program
//
// Author: Noah Shorter
// noah.shorter@oit.edu
//

#include "cAstNode.h"
#include "cSymbol.h"
#include "cBlockNode.h"
#include "cDeclNode.h"
#include "cSymbolTable.h"
#include "cDeclsNode.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        // param is the block that makes up the program
        cFuncDeclNode(cSymbol *name) : cDeclNode()
        {
            cSymbol *token = g_symbolTable.GlobalLookup(name->GetName());
            if (token == nullptr)
            {
                token = name;
                g_symbolTable.Insert(token);
                token->SetDecl(this);
                AddChild(token);
            }
            else
            {
                cSymbol * sym = g_symbolTable.LocalLookup(name->GetName());
                if (sym == nullptr)
                {
                    token = new cSymbol(name->GetName());
                    token->SetDecl(this);
                    g_symbolTable.Insert(token);
                    AddChild(token);
                }
                else if (!sym->GetDecl()->IsFunc())
                {
                    token->SetDecl(this);
                    SemanticParseError(
                            "Symbol " + sym->GetName() + 
                            " already exists in current scope");
                }
                else
                {
                    token = sym;
                    AddChild(token);
                }
                //sym = token;
                //sym->SetDecl(token->GetDecl());
                //AddChild(sym);
                
            }

        }

        void AddParamType(cSymbol *type, cDeclsNode *vardecls = nullptr)
        {
            cSymbol * name = dynamic_cast<cSymbol *>(GetChild(0));
            //cSymbol * oldOne = g_symbolTable.GlobalLookup(name->GetName());

            cFuncDeclNode * decl = 
                dynamic_cast<cFuncDeclNode *>(name->GetDecl());
            cSymbol * returnType = dynamic_cast<cSymbol *>(decl->GetChild(1));
            if(returnType != nullptr && 
                returnType->GetName() != type->GetName())
            {
                SemanticParseError( 
                    name->GetName() +
                    " previously declared with different return type");
            }

            AddChild(type->GetDecl());
            AddChild(vardecls);
        }

        void AddBlock(cBlockNode *block)
        {
            AddChild(block);
        }

        cSymbol * GetReturnType()
        {
            return dynamic_cast<cSymbol *>(GetChild(1));
        }

        bool HasBlock()
        {
            return dynamic_cast<cBlockNode *>(GetChild(3))
                         == nullptr ? true : false;
        }

        bool IsFunc()
        {
            return true;
        }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
