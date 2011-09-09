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

  macro_table->InstallMacro("add",       AddFunction::evaluate);
  macro_table->InstallMacro("and",       AndFunction::evaluate);
  macro_table->InstallMacro("append",    AppendFunction::evaluate);
  macro_table->InstallMacro("define",    DefineFunction::evaluate);
  macro_table->InstallMacro("defined?",  DefinedFunction::evaluate);
  macro_table->InstallMacro("delete",    DeleteFunction::evaluate);
  macro_table->InstallMacro("div",       DivFunction::evaluate);
  macro_table->InstallMacro("dump",      DumpFunction::evaluate);
  macro_table->InstallMacro("entityify", EntityifyFunction::evaluate);
  macro_table->InstallMacro("eq?",       EqFunction::evaluate);
  macro_table->InstallMacro("eval",      EvalFunction::evaluate);
  macro_table->InstallMacro("first",     FirstFunction::evaluate);
  macro_table->InstallMacro("ge?",       GeFunction::evaluate);
  macro_table->InstallMacro("gensym",    GensymFunction::evaluate);
  macro_table->InstallMacro("get",       GetFunction::evaluate);
  macro_table->InstallMacro("gt?",       GtFunction::evaluate);
  macro_table->InstallMacro("include",   IncludeFunction::evaluate);
  macro_table->InstallMacro("last",      LastFunction::evaluate);
  macro_table->InstallMacro("le?",       LeFunction::evaluate);
  macro_table->InstallMacro("length",    LengthFunction::evaluate);
  macro_table->InstallMacro("literal",   LiteralFunction::evaluate);
  macro_table->InstallMacro("loop",      LoopFunction::evaluate);
  macro_table->InstallMacro("lt?",       LtFunction::evaluate);
  macro_table->InstallMacro("mod",       ModFunction::evaluate);
  macro_table->InstallMacro("mult",      MultFunction::evaluate);
  macro_table->InstallMacro("mute",      NullFunction::evaluate);
  macro_table->InstallMacro("ne?",       NeFunction::evaluate);
  macro_table->InstallMacro("null",      NullFunction::evaluate);
  macro_table->InstallMacro("number?",   NumberFunction::evaluate);
  macro_table->InstallMacro("or",        OrFunction::evaluate);
  macro_table->InstallMacro("print",     PrintFunction::evaluate);
  macro_table->InstallMacro("read",      ReadFunction::evaluate);
  macro_table->InstallMacro("rep",       RepFunction::evaluate);
  macro_table->InstallMacro("set",       SetFunction::evaluate);
  macro_table->InstallMacro("slashify",  SlashifyFunction::evaluate);
  macro_table->InstallMacro("stop",      StopFunction::evaluate);
  macro_table->InstallMacro("sub",       SubFunction::evaluate);
  macro_table->InstallMacro("substr",    SubstrFunction::evaluate);
  macro_table->InstallMacro("trim",      TrimFunction::evaluate);
  macro_table->InstallMacro("unicode",   UnicodeFunction::evaluate);
  macro_table->InstallMacro("write",     WriteFunction::evaluate);
  macro_table->InstallMacro("gt", ">");
  macro_table->InstallMacro("lt", "<");
  macro_table->InstallMacro("tilde", "~");
  macro_table->InstallMacro("tilton", "0.7");
}


