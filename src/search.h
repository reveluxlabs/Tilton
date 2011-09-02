//  search.h: interface for the search class

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
#include "text.h"

// MAXHASH is the largest index in the hash table. It must be (2**n)-1.
#define MAXHASH 1023    

class SearchList {
public:
    SearchList();
    virtual ~SearchList();
    

    Text* lookup(Text* name);
    void  install(const char* namestring, void (*function)(Context * ));
    void  install(Text* name, Text* value);
    void  install(const char* namestring, const char* string);
    void  dump();
    Text* getDef(Text* name);
    

private:
    Text* theMacroList[MAXHASH + 1];


    Text* getList(uint32);
    void  setList(uint32, Text*);
    void  link(Text* name, Text* t);
};
