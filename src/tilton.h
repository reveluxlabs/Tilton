// tilton.h: constants for Tilton

//
//  Tilton Macro Processor
//
//  Tilton is a simple macro processor. It is small,
//  portable, and Unicode compatible.
//  Written by Douglas Crockford [ www.crockford.com/tilton ]
//  2006-10-05
//

// Updated for OS X and Debian by JR at Revelux Labs
//
// Version 0.7
// 1Sep11
//
// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
//
// This version of Tilton is licensed under the MIT license.

#ifndef SRC_TILTON_H_
#define SRC_TILTON_H_

#include <map>
#define EOT (-1)

// Tilton numbers are integers.

typedef long number;

// INFINITY is the largest positive integer than can be held in a number.
// This value may be system dependent.

#define INFINITY ((number)0x7FFFFFFF)

// NAN (not a number) is the smallest integer that can be held
// in a number. On two's compliment machines, it is INFINITY + 1.
// This value may be system dependent.

#define NAN ((number)0x80000000)

// Unsigned ints are used in computing hash.

typedef unsigned long int  uint32;   /* unsigned 4-byte quantities */
typedef unsigned      char uint8;    /* unsigned 1-byte quantities */

class Context;
class Text;
class OptionProcessor;
class SearchList;

// MacroProcessor -- coordinator for macro processing

class MacroProcessor {
 public:
  MacroProcessor();
  virtual ~MacroProcessor();

  // CreateOptionProcessors
  // Store function objects in a map for use in command line processing
  void CreateOptionProcessors();
  
  // ProcessCommandLine
  // Read the command line arguments and process
  bool ProcessCommandLine(int argc, const char * argv[]);

  // Run
  // Read the standard input and expand the macros, write to standard out
  void Run(bool go);

 private:
  Context*                          top_frame_;
  Text*                             in_;
  Text*                             the_output_;
  std::map<char, OptionProcessor*>  option_processors_;
};

// MacroTable -- singleton to hold the macro table

class MacroTable {
 public:
   MacroTable();
   virtual ~MacroTable();

   static MacroTable* instance();

  // macro_table
  // Retrieve the macro table
  SearchList* macro_table() {
    return macro_table_;
  }

  // set_macro_table
  // Store a new value for macro_table
  void set_macro_table(SearchList* mt) {
    macro_table_ = mt;
  }

 private:
  static MacroTable*  pInstance;
  SearchList*         macro_table_;

};

#endif  // SRC_TILTON_H_
