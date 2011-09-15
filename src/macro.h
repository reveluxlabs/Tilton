// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef SRC_MACRO_H_
#define SRC_MACRO_H_

#include "tilton.h"
#include "string.h"
#include "text.h"

class Context;

typedef void (*Builtin)(Context* context, Text* &the_output);

// Macro -- represents the name and expansion text of a macro.
//  A Macro can have a link which can chain Macros
//  together. This is used to manage hash collisions.

//  The encoding of strings is UTF-8 (the 8-bit form of Unicode). A character
//  is between 1 and 4 bytes in length. The utfLength and utfSubstr methods
//  count multibyte characters. However, if a multibyte character appears to
//  be badly formed, it will interpret the first byte as a single byte
//  character. So while expecting UTF-8 encoded strings, it will usually
//  do the right thing with Latin-1 and similar encodings.

class Macro {
 public:
  Macro();
  explicit Macro(int len);
  explicit Macro(const char* s);
  explicit Macro(Text* t);
  virtual ~Macro();

  // AddToString
  // appends text to the string wrapped by Macro
  void    AddToString(const char* s, int len);
  void    AddToString(Text* t);
  
  // PrintMacroList
  // write the macro list of stdout
  void    PrintMacroList();
  
  // Hash
  // calculate a hash for a string
  // used to hash macro names
  uint32  Hash();
  
  // FindFirstSubstring
  // returns the first match in the macro definition
  int     FindFirstSubstring(Text* t);

  // IsNameEqual
  bool    IsNameEqual(Text* t);

  // find the last occurance of a substring  
  int     FindLastSubstring(Text* t);
  
  // set_string
  // setter for definition_
  void    set_string(Text* t);
  
  // set_name
  // setter for name_
  void    set_name(const char* s);
  void    set_name(const char* s, int len);
  void    set_name(Text* t);
  
  // ReplaceDefWithSubstring
  // replace the definition with a substring of the definition
  void    ReplaceDefWithSubstring(int start, int len);
  
  // RemoveSpacesAddToString
  // trims whitespace before appending to definition_
  void    RemoveSpacesAddToString(Text* t);
  
  char*        definition_;
  int          length_;
  Macro*       link_;       // hash collisions
  char*        name_;
  int          name_length_;

 private:
  // CheckLengthAndIncrease
  // Test the length of string against the max, increase if needed
  //  If the requested amount does not fit within the allocated max length,
  //  then increase the size of the string. The new allocation will be at least
  //  twice the previous allocation.
  void    CheckLengthAndIncrease(int len);
  
  // InitializeMacro
  // initialize the macro object
  void    InitializeMacro(const char* s, int len);

  uint32  my_hash_;
  int     max_length_;
};

#endif  // SRC_MACRO_H_
