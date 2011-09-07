//  context.h: interface for the context class.

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

#ifndef SRC_CONTEXT_H_
#define SRC_CONTEXT_H_

#include "tilton.h"
#include "iter.h"

class Iter;
class Text;
class Node;

// Context -- a stack frame for evaluation.

class Context {
 public:
  Context(Context*, Iter*);
  virtual ~Context();

  // AddArgument
  // Add an argument to a frame
  void    AddArgument(const char* s);
  void    AddArgument(Text* t);

  // DumpContext
  // Print info about the args in a frame
  void    DumpContext();

  void    ReportErrorAndDie(const char* reason, Text* evidence);
  void    ReportErrorAndDie(const char* reason);

  //  eval
  //  eval is the heart of Tilton. It is called on a context which is the
  //  container of the source of <~NUMBER~> parameters. It scans its input text
  //  for <~ ~> patterns which it builds into new contexts and recursively
  //  evaluates. Characters outside of the <~ ~> are treated as literal.
  //  Within <~ ~>, substrings are produced, separated by ~ . These are stored
  //  in the new context as argument strings. Any nested <~ ~> sequences within
  //  the arguments are treated for now as literal (lazy evaluation). The [0]
  //  argument is the name of the macro to be invoked.
  void    eval(Text* input);

  // EvaluateArgument
  // Evaluate an argument of a macro. If we have already determined its
  // value, then simply return it. Otherwise, evaluate the argument to obtain
  // its value (memoization).
  Text*   EvaluateArgument(int argNr);
  Text*   EvaluateArgument(Node* n);

  number  evalNumber(int argNr);
  number  evalNumber(Node* n);

  // GetArgument
  // Retrieves an argument
  // Arguments are stored as nodes on the frame.
  Node*   GetArgument(int argNr);

  void    nop();

  //  resetArg
  //  Delete the value of an argument of a macro.
  //  This allows for evaluating an arg more than once.
  //  This is used by <~loop~>
  void    resetArg(int argNr);

  Node*   first_;
  Context* previous_;

 private:
  // FindError
  // Recurse through the stack frames to find the location of the error
  void    FindError(Text* report);

  // EvaluateLeftAngle
  // Parse and eval the text following a left angle bracket
  void EvaluateLeftAngle(Iter* in, int &depth, Text* g_the_output,
                         int &tildes_seen, Context* &new_context);

  // EvaluateTilde
  void EvaluateTilde(Iter* in, int &depth, Text* g_the_output,
                     int &tildes_seen, Context* &new_context);

  // EvaluateEOT
  void EvaluateEOT(Iter* in, int &depth, Text* g_the_output,
                   int &tildes_seen, Context* &new_context);

  // ExpandMacro
  void ExpandMacro(Context* &new_context);

  // stackEmpty
  // Tests to determine if the eval stack is empty
  bool stackEmpty(int depth) {
    return depth == 0;
  }

  // checkForTilde
  // Examines the input stream for a run of tildes
  // Returns the number of consecutive tildes at the beginning
  // of the stream and removes them
  int checkForTilde(Iter* in, int no) {
    while (in->next() == '~') {
      no += 1;
    }
    in->back();
    return no;
  }

  // haveTildes
  // Tests to determine if we have a run of tildes
  bool haveTildes(int run_length) {
    return run_length > 0;
  }

  // EvalTextForArg
  // Evaluate a text and produce a value, store in an argument
  Node* EvalTextForArg(int arg_number, Context* &new_context,
                       Text* &g_the_output);

  // setMacroVariable
  // Sets a digit macro to a value
  void SetMacroVariable(int varNo, Text* t);

  int     character_;
  int     index_;
  int     line_;
  int     position_;
  Iter*   source_;
  Node*   last_;
};

#endif  // SRC_CONTEXT_H_
