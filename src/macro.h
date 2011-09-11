// macro.h: interface for the Macro class.

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

#ifndef SRC_MACRO_H_
#define SRC_MACRO_H_

#include "tilton.h"
#include "string.h"
#include "text.h"

class Context;

typedef void (*Builtin)(Context* context, Text* &the_output);

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
  void    CheckLengthAndIncrease(int len);
  
  // InitializeMacro
  // initialize the macro object
  void    InitializeMacro(const char* s, int len);

  uint32  my_hash_;
  int     max_length_;
};

#endif  // SRC_MACRO_H_