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

using std::string;

class cSymbol
{
    public:
        cSymbol(string name, int id = -1)
        {
            if(id == -1)
                m_id = ++nextId;
            else
                m_id = id;
            m_name = name;
        }

        // Used to print cSymbols
        string ToString()
        {
            string result("<sym id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\" />";

            return result;
        }

        // return the name of the cSymbol
        string GetName() { return m_name; }
        int GetType() { return m_type; }
        
    protected:
        static long long nextId;    // used to generate unique IDs
        long long m_id;             // ID for this cSymbol
        string m_name;              // name for this cSymbol
        int m_tokenType = 1031;
};
