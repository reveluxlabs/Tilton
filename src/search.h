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
// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
//
// This version of Tilton is licensed under the MIT license.

#ifndef SRC_SEARCH_H_
#define SRC_SEARCH_H_

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
   Text* the_macro_list_[MAXHASH + 1];

   Text* the_macro_list(uint32 h);
   void  set_the_macro_list(uint32 h, Text* t);
   void  link(Text* name, Text* t);
};

#endif  // SRC_SEARCH_H_
