#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() {}

        virtual bool IsReal()  { return false; }
        virtual bool IsInt()   { return false; }
        virtual bool IsChar()  { return false; }
        virtual bool IsType()  { return false; }
        virtual bool IsFunc()  { return false; }

        virtual cDeclNode * GetDeclType() = 0;
        virtual cDeclNode * GetIndexType() { return nullptr; }

        virtual string GetName() = 0;

        bool IsCompatable(cDeclNode * rval)
        {
            bool returnType = false;

            if (this->IsReal())
                returnType = true;
            else if (this->IsInt())
            {
                if(!rval->IsType() && !rval->IsReal())
                    returnType = true;
            }
            else if (this->IsChar())
            {
                if(!rval->IsType() && rval->IsChar())
                    returnType = true;
            }
            else if (this == rval)
                returnType = true;

            return returnType;
        }

};
