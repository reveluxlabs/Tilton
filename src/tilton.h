// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef SRC_TILTON_H_
#define SRC_TILTON_H_

#include <map>

#include "tiltonfwd.h"

#define EOT (-1)

const int kArgZero  = 0;
const int kArgOne   = 1;
const int kArgTwo   = 2;
const int kArgThree = 3;


// Builtin is the signature for built-in functions
typedef void (*Builtin)(Context* context, Text* &the_output);

// Tilton numbers are integers.

typedef long number;

// kInfinity is the largest positive integer than can be held in a number.
// This value may be system dependent.

#define INFINITY ((number)0x7FFFFFFF)
const number kInfinity = 0x7FFFFFFF;

// kNAN (not a number) is the smallest integer that can be held
// in a number. On two's complement machines, it is kInfinity + 1.
// This value may be system dependent.

#define NAN ((number)0x80000000)
const number kNAN = 0x80000000;

// Unsigned ints are used in computing hash.

typedef unsigned long int  uint32;   /* unsigned 4-byte quantities */
typedef unsigned      char uint8;    /* unsigned 1-byte quantities */

// MacroProcessor -- coordinator for macro processing

class MacroProcessor {
 public:
  MacroProcessor();
  virtual ~MacroProcessor();

  // CreateOptionProcessors
  // Store function objects in a map for use in command line processing
  void CreateOptionProcessors();
  
  // InstallTiltonMacros
  // loads the macro table with tilton macros
  void InstallTiltonMacros();
  
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
  HashTable* macro_table() { return macro_table_; }

 private:
  static MacroTable*  pInstance;
  HashTable*         macro_table_;

};

#endif  // SRC_TILTON_H_
