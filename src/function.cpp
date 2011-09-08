// function.cpp
// For interface descriptions, see function.h

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
#include "function.h"

#include "tilton.h"

// jr 1Sep11
FunctionContext::FunctionContext() {
}

// jr 1Sep11
FunctionContext::~FunctionContext() {
}

// jr 1Sep11 (based on code by Douglas Crockford)
void FunctionContext::RegisterTiltonFunctions() {
  SearchList* macro_table = MacroTable::instance()->macro_table();

  macro_table->install("add",       AddFunction::evaluate);
  macro_table->install("and",       AndFunction::evaluate);
  macro_table->install("append",    AppendFunction::evaluate);
  macro_table->install("define",    DefineFunction::evaluate);
  macro_table->install("defined?",  DefinedFunction::evaluate);
  macro_table->install("delete",    DeleteFunction::evaluate);
  macro_table->install("div",       DivFunction::evaluate);
  macro_table->install("dump",      DumpFunction::evaluate);
  macro_table->install("entityify", EntityifyFunction::evaluate);
  macro_table->install("eq?",       EqFunction::evaluate);
  macro_table->install("eval",      EvalFunction::evaluate);
  macro_table->install("first",     FirstFunction::evaluate);
  macro_table->install("ge?",       GeFunction::evaluate);
  macro_table->install("gensym",    GensymFunction::evaluate);
  macro_table->install("get",       GetFunction::evaluate);
  macro_table->install("gt?",       GtFunction::evaluate);
  macro_table->install("include",   IncludeFunction::evaluate);
  macro_table->install("last",      LastFunction::evaluate);
  macro_table->install("le?",       LeFunction::evaluate);
  macro_table->install("length",    LengthFunction::evaluate);
  macro_table->install("literal",   LiteralFunction::evaluate);
  macro_table->install("loop",      LoopFunction::evaluate);
  macro_table->install("lt?",       LtFunction::evaluate);
  macro_table->install("mod",       ModFunction::evaluate);
  macro_table->install("mult",      MultFunction::evaluate);
  macro_table->install("mute",      NullFunction::evaluate);
  macro_table->install("ne?",       NeFunction::evaluate);
  macro_table->install("null",      NullFunction::evaluate);
  macro_table->install("number?",   NumberFunction::evaluate);
  macro_table->install("or",        OrFunction::evaluate);
  macro_table->install("print",     PrintFunction::evaluate);
  macro_table->install("read",      ReadFunction::evaluate);
  macro_table->install("rep",       RepFunction::evaluate);
  macro_table->install("set",       SetFunction::evaluate);
  macro_table->install("slashify",  SlashifyFunction::evaluate);
  macro_table->install("stop",      StopFunction::evaluate);
  macro_table->install("sub",       SubFunction::evaluate);
  macro_table->install("substr",    SubstrFunction::evaluate);
  macro_table->install("trim",      TrimFunction::evaluate);
  macro_table->install("unicode",   UnicodeFunction::evaluate);
  macro_table->install("write",     WriteFunction::evaluate);
  macro_table->install("gt", ">");
  macro_table->install("lt", "<");
  macro_table->install("tilde", "~");
  macro_table->install("tilton", "0.7");
}


