// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef SRC_HASH_TABLE_H_
#define SRC_HASH_TABLE_H_

#include "tilton.h"

class Macro;

// kMaxHash is the largest index in the hash table. It must be (2**n)-1.
const int kMaxHash = 1023;

// HashTable -- responsible for managing a hash table of macros

class HashTable {
 public:
  HashTable();
  virtual ~HashTable();

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
  Macro* the_macro_list_[kMaxHash + 1];

  Macro* the_macro_list(uint32 h) const;
  void  set_the_macro_list(uint32 h, Macro* m);
  void  InsertIntoHashTable(Text* name, Macro* m);
};

#endif  // SRC_HASH_TABLE_H_

