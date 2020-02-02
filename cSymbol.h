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
        cSymbol(string name, int id = -1) : cAstNode()
        {
            if(id == -1)
            {
                m_id = ++nextId;
                if(m_id == 1 || m_id == 2 || m_id == 3) 
                    m_tokenType = TYPE_ID;
                else m_tokenType = IDENTIFIER;
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

        virtual string NodeType() { return string("sym"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this);}

    protected:
        static long long nextId;    // used to generate unique IDs
        long long m_id;             // ID for this cSymbol
        string m_name;              // name for this cSymbol
        int m_tokenType;
};
