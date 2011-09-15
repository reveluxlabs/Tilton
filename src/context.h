// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef SRC_CONTEXT_H_
#define SRC_CONTEXT_H_

#include "tilton.h"
#include "byte_stream.h"

class ByteStream;
class Text;
class Node;

// Context -- a stack frame for evaluation.

class Context {
 public:
  Context(Context*, ByteStream*);
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
  void    ParseAndEvaluate(Text* input, Text* &the_output);

  // EvaluateArgument
  // Evaluate an argument of a macro. If we have already determined its
  // value, then simply return it. Otherwise, evaluate the argument to obtain
  // its value (memoization).
  Text*   EvaluateArgument(const int argNr, Text* &the_output);
  Text*   EvaluateArgument(Node* n, Text* &the_output);

  number  EvaluateNumber(const int argNr, Text* &the_output);
  number  EvaluateNumber(Node* n, Text* &the_output);

  // GetArgument
  // Retrieves an argument
  // Arguments are stored as nodes on the frame.
  Node*   GetArgument(const int argNr);

  void    nop();

  //  ResetArgument
  //  Delete the value of an argument of a macro.
  //  This allows for evaluating an arg more than once.
  //  This is used by <~loop~>
  void    ResetArgument(const int argNr);

  Node*   first_;
  Context* previous_;

 private:
  // FindError
  // Recurse through the stack frames to find the location of the error
  void FindError(Text* report);

  // ParseLeftAngle
  // Parse and eval the text following a left angle bracket
  void ParseLeftAngle(ByteStream* in, int &depth, Text* &the_output,
                         int &tildes_seen, Context* &new_context);

  // ParseTilde
  void ParseTilde(ByteStream* in, int &depth, Text* &the_output,
                     int &tildes_seen, Context* &new_context);

  // ParseEOT
  void ParseEOT(ByteStream* in, int &depth, Text* &the_output,
                   int &tildes_seen, Context* &new_context);

  // EvaluateMacro
  void EvaluateMacro(Context* &new_context, Text* &the_output);

  // stackEmpty
  // Tests to determine if the eval stack is empty
  bool stackEmpty(const int depth) const { return depth == 0; }

  // checkForTilde
  // Examines the input stream for a run of tildes
  // Returns the number of consecutive tildes at the beginning
  // of the stream and removes them
  int checkForTilde(ByteStream* in, int no) {
    while (in->next() == '~') {
      no += 1;
    }
    in->back();
    return no;
  }

  // haveTildes
  // Tests to determine if we have a run of tildes
  bool haveTildes(const int run_length) const { return run_length > 0; }

  // EvalTextForArg
  // Evaluate a text and produce a value, store in an argument
  Node* EvalTextForArg(const int arg_number, Context* &new_context,
                       Text* &the_output);

  // setMacroVariable
  // Sets a digit macro to a value
  void SetMacroVariable(const int varNo, Text* t);

  int     character_;
  int     index_;
  int     line_;
  int     position_;
  ByteStream*   source_;
  Node*   last_;
};

#endif  // SRC_CONTEXT_H_
