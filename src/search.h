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

// class Text;
class Macro;

// MaxHash is the largest index in the hash table. It must be (2**n)-1.
const int MaxHash = 1023;

// SearchList -- responsible for managing a hash table of macros

class SearchList {
 public:
  SearchList();
  virtual ~SearchList();

  // LookupMacro
  //  Search through the macro list for a text with a specific name.
  //  The list is a hash table with links for collisions.
  Macro* LookupMacro(Text* name);

  // InstallMacro
  //  if there is a text in the macro list with this name, set its
  //  value. Otherwise, make a new text with this name and value and put
  //  it in the list.
  void  InstallMacro(Text* name, Text* value);
  void  InstallMacro(Text* name, Macro* value);
  //  This is a little faster than InstallMacro() because it assumes that
  //  the name is not already in the macro list.
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

