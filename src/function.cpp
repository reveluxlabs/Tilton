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

// Singleton implementation for function context

FunctionContext* FunctionContext::pInstance = 0;

FunctionContext* FunctionContext::instance() {
  if ( pInstance == 0 ) {
    pInstance = new FunctionContext;
  }
  return pInstance;
}

// jr 11Sep11
void FunctionContext::RegisterTiltonFunctions() {

  RegisterFunction("add",       AddFunction::evaluate);
  RegisterFunction("add",       AddFunction::evaluate);
  RegisterFunction("and",       AndFunction::evaluate);
  RegisterFunction("append",    AppendFunction::evaluate);
  RegisterFunction("define",    DefineFunction::evaluate);
  RegisterFunction("defined?",  DefinedFunction::evaluate);
  RegisterFunction("delete",    DeleteFunction::evaluate);
  RegisterFunction("div",       DivFunction::evaluate);
  RegisterFunction("dump",      DumpFunction::evaluate);
  RegisterFunction("entityify", EntityifyFunction::evaluate);
  RegisterFunction("eq?",       EqFunction::evaluate);
  RegisterFunction("eval",      EvalFunction::evaluate);
  RegisterFunction("first",     FirstFunction::evaluate);
  RegisterFunction("ge?",       GeFunction::evaluate);
  RegisterFunction("gensym",    GensymFunction::evaluate);
  RegisterFunction("get",       GetFunction::evaluate);
  RegisterFunction("gt?",       GtFunction::evaluate);
  RegisterFunction("include",   IncludeFunction::evaluate);
  RegisterFunction("last",      LastFunction::evaluate);
  RegisterFunction("le?",       LeFunction::evaluate);
  RegisterFunction("length",    LengthFunction::evaluate);
  RegisterFunction("literal",   LiteralFunction::evaluate);
  RegisterFunction("loop",      LoopFunction::evaluate);
  RegisterFunction("lt?",       LtFunction::evaluate);
  RegisterFunction("mod",       ModFunction::evaluate);
  RegisterFunction("mult",      MultFunction::evaluate);
  RegisterFunction("mute",      NullFunction::evaluate);
  RegisterFunction("ne?",       NeFunction::evaluate);
  RegisterFunction("null",      NullFunction::evaluate);
  RegisterFunction("number?",   NumberFunction::evaluate);
  RegisterFunction("or",        OrFunction::evaluate);
  RegisterFunction("print",     PrintFunction::evaluate);
  RegisterFunction("read",      ReadFunction::evaluate);
  RegisterFunction("rep",       RepFunction::evaluate);
  RegisterFunction("set",       SetFunction::evaluate);
  RegisterFunction("slashify",  SlashifyFunction::evaluate);
  RegisterFunction("stop",      StopFunction::evaluate);
  RegisterFunction("sub",       SubFunction::evaluate);
  RegisterFunction("substr",    SubstrFunction::evaluate);
  RegisterFunction("trim",      TrimFunction::evaluate);
  RegisterFunction("unicode",   UnicodeFunction::evaluate);
  RegisterFunction("write",     WriteFunction::evaluate);
}


