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

};
