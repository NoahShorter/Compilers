#pragma once
//*********************************************************
// cSymbol.h
//
// cSymbol class declaration.
// cSymbols are objects that represent identifiers and other such strings in 
// the compiler.
//
// Author: Phil Howard
// phil.howard@oit.edu
//

#include <string>
#include <utility>
#include "cAstNode.h"
#include "pascalparse.h"

using std::string;

class cSymbol : public cAstNode
{
    public:
        cSymbol(string name, int id = -1, bool isType = false) : cAstNode()
        {
            m_isType = isType;
            if(id == -1)
            {
                m_id = ++nextId;
                m_tokenType = IDENTIFIER;
            }
            else
            {
                m_id = id;
                m_tokenType = id;
            }
            m_name = name;
        }

        virtual string AttributesToString()
        {
            string result(" id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\"";
            return result;
        }

        // return the name of the cSymbol
        string GetName() { return m_name; }
        int GetType() { return m_tokenType; }

        bool IsType() { return m_isType; }

        cDeclNode *GetDecl() { return m_decl; }
        void SetDecl(cDeclNode *decl) { m_decl =decl; }

        virtual string NodeType() { return string("sym"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this);}

    protected:
        static long long nextId;    // used to generate unique IDs
        long long m_id;             // ID for this cSymbol
        string m_name;              // name for this cSymbol
        int m_tokenType;
        cDeclNode *m_decl;
        bool m_isType;
};
