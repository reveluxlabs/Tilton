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

class Text;
class Macro;

// MaxHash is the largest index in the hash table. It must be (2**n)-1.
const int MaxHash = 1023;

class SearchList {
 public:
  SearchList();
  virtual ~SearchList();

  Macro* LookupMacro(Text* name);
  void  InstallMacro(Text* name, Text* value);
  void  InstallMacro(Text* name, Macro* value);
  void  InstallMacro(const char* namestring, const char* string);
  void  PrintMacroTable();
  Macro* GetMacroDefOrInsertNull(Text* name);


 private:
  Macro* the_macro_list_[MaxHash + 1];

  Macro* the_macro_list(uint32 h) const;
  void  set_the_macro_list(uint32 h, Macro* m);
  void  InsertIntoSearchList(Text* name, Macro* m);
};

#endif  // SRC_SEARCH_H_

