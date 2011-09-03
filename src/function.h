//  function.h: interface for the Function Context class

//
//  Tilton Macro Processor
//
//  Tilton is a simple macro processor. It is small, 
//  portable, and Unicode compatible.
//  Written by Douglas Crockford [ www.crockford.com/tilton ]
//  2006-10-06
//

// Updated for OS X and Debian by JR at Revelux Labs
//
// Version 0.7
// 1Sep11
//
// This version of Tilton is licensed under the MIT license.

#include "tilton.h"
#include "search.h"


// FunctionContext -- collection of functions available as built-ins
class FunctionContext
{
public:
    // Constructor
    FunctionContext();

    // Destructor
    virtual ~FunctionContext();

    // registerTiltonFunctions
    // Register the built-in functions for use in Tilton
    void registerTiltonFunctions(SearchList* macroTable);    

private:

};
