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
#include "emit.h"

class cCodeGen : public cVisitor
{
    private:
        int m_labelCounter;
    public:
        //*********************************************************************
        cCodeGen(std::string filename) : cVisitor()
        {
            InitOutput(filename);
            m_labelCounter = 0;
        }
        //*********************************************************************
        ~cCodeGen()
        {
            FinalizeOutput();
        }
        //*********************************************************************
        virtual void VisitAllNodes(cAstNode *node)
        {
            node->VisitAllChildren(this);
        }
        //*********************************************************************
        virtual void VisitNode(cAstNode *node)
        {
            node->Visit(this);
        }
        //*********************************************************************
        virtual void Visit(cProgramNode * node)
        {
            cBlockNode * block = node->GetBlock();
            if(block->GetDecls() != nullptr)
                block->GetDecls()->Visit(this);

            EmitString(".function main\n");
            EmitString("main:\n");
            EmitString("ADJSP ");

            EmitInt(block->GetSize());
            EmitString("\n");

            block->GetStmts()->Visit(this);

            EmitString("PUSH 0\n");
            EmitString("RETURNV\n");
        }
        //*********************************************************************
        virtual void Visit(cWriteNode * node)
        {
            cExprListNode * exprList = node->GetExprList();
            for(int ii = 0; ii < exprList->NumExpr(); ++ii)
            {
                //VisitAllNodes(exprList->GetExpr(ii));
                exprList->GetExpr(ii)->Visit(this);

                EmitString("Call @print\n");
                EmitString("POP\n");
                EmitString("POP\n");
            }
        }
        //*********************************************************************
        virtual void Visit(cIntExprNode * node)
        {
            EmitString("PUSH ");
            EmitInt(node->GetValue());
            EmitString("\n");
        }
        //*********************************************************************
        virtual void Visit(cVarExprNode * node)
        {
            if(!node->HasIndex())
            {
                if(node->GetType()->IsChar())
                    EmitString("PUSHCVAR ");
                else
                    EmitString("PUSHVAR ");
                EmitInt(node->GetOffset());
                EmitString("\n");
            }
            else
            {
                bool ischar = false;
                EmitString("PUSHFP\n");
                EmitString("PUSH ");
                EmitInt(node->GetOffset());
                EmitString("\nPLUS\n");
                cExprListNode * indexes = node->GetIndexes();
                vector<int> rowSizes = node->GetRowSizes();
                if(rowSizes[0] == 1)
                    ischar = true;
                for(int ii = 0; ii < indexes->NumExpr(); ++ii)
                {
                    indexes->GetExpr(ii)->Visit(this);
                    EmitString("PUSH ");
                    EmitInt(rowSizes[ii]);
                    EmitString("\nTIMES\nPLUS\n");
                }
                if(ischar)
                    EmitString("PUSHCVARIND\n");
                else
                    EmitString("PUSHVARIND\n");
            }
        }
        //*********************************************************************
        virtual void Visit(cUnaryExprNode * node)
        {
            node->GetExpr()->Visit(this);
            EmitString("NEG\n");
        }
        //*********************************************************************
        virtual void Visit(cAssignNode * node)
        {
            node->GetExpr()->Visit(this);
            cVarExprNode * lval = node->GetLval();
            if(!lval->HasIndex())
            {
                if(lval->GetType()->IsChar())
                    EmitString("POPCVAR ");
                else
                    EmitString("POPVAR ");
                EmitInt(lval->GetOffset());
                EmitString("\n");
            }
            else
            {
                bool ischar = false;
                EmitString("PUSHFP\n");
                EmitString("PUSH ");
                EmitInt(lval->GetOffset());
                EmitString("\nPLUS\n");
                cExprListNode * indexes = lval->GetIndexes();
                vector<int> rowSizes = lval->GetRowSizes();
                if(rowSizes[0] == 1)
                    ischar = true;
                for(int ii = 0; ii < indexes->NumExpr(); ++ii)
                {
                    indexes->GetExpr(ii)->Visit(this);
                    EmitString("PUSH ");
                    EmitInt(rowSizes[ii]);
                    EmitString("\nTIMES\nPLUS\n");
                }
                if(ischar)
                    EmitString("POPCVARIND\n");
                else
                    EmitString("POPVARIND\n");
            }
        }
        //*********************************************************************
        virtual void Visit(cBinaryExprNode * node)
        {
            node->GetLeft()->Visit(this);
            node->GetRight()->Visit(this);
            switch(node->GetOp())
            {
                case '+':
                    EmitString("PLUS");
                    break;
                case '-':
                    EmitString("MINUS");
                    break;
                case '*':
                    EmitString("TIMES");
                    break;
                case '/':
                    EmitString("DIVIDE");
                    break;
                case '<':
                    EmitString("LT");
                    break;
                case '>':
                    EmitString("GT");
                    break;
                case '=':
                    EmitString("EQ");
                    break;
                case LE:
                    EmitString("LE");
                    break;
                case GE:
                    EmitString("GE");
                    break;
                case NOT_EQUAL:
                    EmitString("NE");
                    break;
                case AND:
                    EmitString("AND");
                    break;
                case OR:
                    EmitString("OR");
                    break;
            }
            EmitString("\n");
        }
        //*********************************************************************
        virtual void Visit(cFuncDeclNode * node)
        {
            if(node->HasBlock())
            {
                node->GetBlock()->GetDecls()->Visit(this);
                EmitString(".function " + node->GetFuncName() + "\n");
                EmitString(node->GetFuncName() + ":\n");
                EmitString("ADJSP ");
                EmitInt(node->GetBlock()->GetSize());
                EmitString("\n");

                node->GetBlock()->GetStmts()->Visit(this);
                
                EmitString("PUSHVAR 0\n");
                EmitString("RETURNV\n");
            }
        }
        //*********************************************************************
        virtual void Visit(cProcDeclNode * node)
        {
            if(node->HasBlock())
            {
                node->GetBlock()->GetDecls()->Visit(this);
                EmitString(".function " + node->GetFuncName() + "\n");
                EmitString(node->GetFuncName() + ":\n");
                EmitString("ADJSP ");
                EmitInt(node->GetBlock()->GetSize());
                EmitString("\n");

                node->GetBlock()->GetStmts()->Visit(this);
                
                EmitString("PUSHVAR 0\n");
                EmitString("RETURN\n");
            }
        }
        //*********************************************************************
        virtual void Visit(cFuncExprNode * node)
        {
            cExprListNode * params = node->GetParams();
            if(params != nullptr)
            {
                for(int ii = (params->NumExpr() - 1); ii >= 0; --ii)
                {
                    params->GetExpr(ii)->Visit(this);
                }
            }
            EmitString("CALL @" + node->GetName()->GetName() + "\n");
            EmitString("POPARGS ");
            EmitInt(node->GetSize());
            EmitString("\n");
        }
        //*********************************************************************
        virtual void Visit(cProcCallNode * node)
        {
            cExprListNode * params = node->GetParams();
            if(params != nullptr)
            {
                for(int ii = (params->NumExpr() - 1); ii >= 0; --ii)
                {
                    params->GetExpr(ii)->Visit(this);
                }
            }
            EmitString("CALL @" + node->GetName()->GetName() + "\n");
            EmitString("POPARGS ");
            EmitInt(node->GetSize());
            EmitString("\n");
        }
        //*********************************************************************
        virtual void Visit(cIfNode * node)
        {
            node->GetCondition()->Visit(this);
            EmitString("JUMPE @LABEL" + std::to_string(m_labelCounter) + "\n");

            node->GetIfStmt()->Visit(this);

            EmitString("JUMP @LABEL" + std::to_string(m_labelCounter + 1) + "\n");

            EmitString("LABEL" + std::to_string(m_labelCounter) + ":\n");

            if(node->GetElseStmt() != nullptr)
                node->GetElseStmt()->Visit(this);

            EmitString("LABEL" + std::to_string(m_labelCounter + 1) + ":\n");

            m_labelCounter += 2;
        }
        //*********************************************************************
        virtual void Visit(cWhileNode * node)
        {
            EmitString("LABEL" + std::to_string(m_labelCounter + 1) + ":\n");
            node->GetCondition()->Visit(this);
            EmitString("JUMPE @LABEL" + std::to_string(m_labelCounter) + "\n");

            node->GetStmt()->Visit(this);

            EmitString("JUMP @LABEL" + std::to_string(m_labelCounter + 1) + "\n");
            EmitString("LABEL" + std::to_string(m_labelCounter) + ":\n");

            m_labelCounter += 2;
        }
};
