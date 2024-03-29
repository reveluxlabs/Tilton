// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "context.h"

#include <stdlib.h>
#include <stdio.h>

#include "function.h"
#include "byte_stream.h"
#include "macro.h"
#include "node.h"
#include "hash_table.h"
#include "tilton.h"
#include "text.h"

Context::Context(Context* prev, ByteStream* s) {
    position_ = 0;
    first_ = NULL;
    last_ = NULL;
    previous_ = prev;
    source_ = s;
    if (s) {
        line_ = s->line();
        character_ = s->character();
        index_ = s->index();
    } else {
        line_ = 0;
        character_ = 0;
        index_ = 0;
    }
}

Context::~Context() {
    delete this->first_;
}

void Context::AddArgument(const char* s) {
    AddArgument(new Text(s));
}

void Context::AddArgument(Text* t) {
    Node* p = new Node(t);
    if (last_) {
        last_->next_ = p;
    } else {
        first_ = p;
    }
    last_ = p;
}


void Context::DumpContext() {
    if (first_) {
        first_->WriteNode();
    }
    fputc('\n', stderr);
}


// Write a message to stderr and then exit.

void Context::ReportErrorAndDie(const char* reason) {
    ReportErrorAndDie(reason, NULL);
}


void Context::ReportErrorAndDie(const char* reason, Text* evidence) {
    Text* report = new Text(80);
    FindError(report);
    report->AddToString(reason);
    if (evidence) {
        report->AddToString(": ");
        report->AddToString(evidence);
    }
    report->AddToString(".\n");
    fwrite(report->string_, sizeof(char), report->length_, stdout);
    fwrite(report->string_, sizeof(char), report->length_, stderr);
    exit(1);
}

// Eval is the heart of Tilton, see comment in context.h
// It is currently being refactored towards the command pattern.
void Context::ParseAndEvaluate(Text* input, Text* &the_output) {
  int c;                 // current character
  int depth = 0;         // depth of nested <~ ~>
  int tildes_seen= 0;     // the number of tildes in the separator ~~~
  Context* new_context = NULL;
  ByteStream* in = new ByteStream(input);

  // Loop over the characters in the input

  for (;;) {
    switch ((c = in->next())) {
      case '<':
        this->ParseLeftAngle(in, depth, the_output,
                                tildes_seen, new_context);
        break;
      case '~':
        this->ParseTilde(in, depth, the_output, tildes_seen, new_context);
        break;
      case EOT:
        this->ParseEOT(in, depth, the_output, tildes_seen, new_context);
        return;
      // literal character
      default:
        the_output->AddToString(c);
        break;
    }
  }
}

void Context::ParseLeftAngle(ByteStream* in, int &depth, Text* &the_output,
                        int &tildes_seen, Context* &new_context) {
  int run_length;  // the number of tildes currently under consideration

  run_length = checkForTilde(in, 0);

  if (!stackEmpty(depth)) {       // in middle of macro expansion
    if (haveTildes(run_length)) {  // found embedded macro bracket
        depth += 1;
    }
    the_output->AddToString('<');
    the_output->AddToString('~', run_length);
  } else if (haveTildes(run_length)) {  // found first macro bracket
    depth = 1;
    tildes_seen = run_length;
    new_context = new Context(this, in);
    new_context->position_ = the_output->length_;
  } else {  // left angle in the middle of text being passed through
    the_output->AddToString('<');
  }
}

void Context::ParseTilde(ByteStream* in, int &depth, Text* &the_output,
                        int &tildes_seen, Context* &new_context) {
  int arg_number;    // argument number
  Node* arg;         // current arg
  Text* arg_text;    // current text
  int run_length;    // the number of tildes currently under consideration

  run_length = checkForTilde(in, 1);

  // if in middle of expansion and tildes seen, create args for each
  if (depth == 1 && run_length >= tildes_seen) {
    new_context->AddArgument(
        the_output->RemoveFromString(new_context->position_));
    for (;;) {
      run_length -= tildes_seen;
      if (run_length < tildes_seen) {
        break;
      }
      new_context->AddArgument(new Text());
    }
  }

  // pass through tildes in plain text
  the_output->AddToString('~', run_length);

  // if we have seen the closing bracket
  if (in->peek() == '>') {
    //  ~>
    in->next();
    if (!stackEmpty(depth)) {
      depth -= 1;
      if (!stackEmpty(depth)) {
          the_output->AddToString('>');
      } else {
        if (run_length) {
          new_context->ReportErrorAndDie("Short ~>");
        }
        //    apply:
        arg = new_context->first_;
        //    <~NUMBER~>
        arg_text = arg->text_;
        arg_number = arg_text->GetCharacter(0) - '0';
        if (Text::isDigit(arg_number)) {
          arg_number = arg_text->ConvertAlphaToInteger();
          if (arg_number >= 0) {
            if (!arg->next_) {
              the_output->AddToString(this->EvaluateArgument(arg_number, the_output));
            } else {
              //    <~NUMBER~value~>
              arg = new_context->GetArgument(arg_number);
              if (!arg->hasValue()) {
                arg = this->EvalTextForArg(1, new_context, the_output);
              }
              this->SetMacroVariable(arg_number, arg->value_);
            }
            delete new_context;
            return;
          }
        }
        //    look up
        Context::EvaluateMacro(new_context, the_output);
      }
    } else {
      (new Context(this, in))->ReportErrorAndDie("Extra ~>");
    }
  }
}

Node* Context::EvalTextForArg(int arg_number, Context* &new_context,
                             Text* &the_output) {
  Node* n;
  n = new_context->GetArgument(arg_number);
  this->ParseAndEvaluate(n->text_, the_output);
  n->value_ = the_output->RemoveFromString(new_context->position_);
  return n;
}

void Context::SetMacroVariable(int varNo, Text* t) {
  Node* o;
  o = this->GetArgument(varNo);
  delete o->text_;
  o->text_ = NULL;
  delete o->value_;
  // o->value[varNo] = new Text(t);
  // previous bug fix -- jr 19Sep11
  o->value_ = new Text(t);
}

void Context::EvaluateMacro(Context* &new_context, Text* &the_output) {
  Macro* macro;
  Text* name;
  Builtin function;

  name = new_context->EvaluateArgument(kArgZero, the_output);
  // look for name as built in
  function = FunctionContext::instance()->GetFunction(name->string_);
  if (function) {
    (*function)(new_context, the_output);
  } else {
    // look for macro definition
    macro = MacroTable::instance()->macro_table()->LookupMacro(name);
    if (macro) {
      new_context->ParseAndEvaluate(new Text(macro), the_output);
    } else {
      //    undefined
      new_context->ReportErrorAndDie("Undefined macro");
    }
  }
  delete new_context;
  new_context = NULL;
}

void Context::ParseEOT(ByteStream* in, int &depth, Text* &the_output,
                      int &tildes_seen, Context* &new_context) {
    if (!stackEmpty(depth)) {
        new_context->ReportErrorAndDie("Missing ~>");
    }
    delete in;
}

Text* Context::EvaluateArgument(int argNr, Text* &the_output) {
    return EvaluateArgument(GetArgument(argNr), the_output);
}


Text* Context::EvaluateArgument(Node* n, Text* &the_output) {
  if (n == NULL) {
      return NULL;
  }
  if (n->value_ == NULL) {
    if (n->text_ == NULL) {
      return NULL;
    }
    Text* arg = n->text_;
    int position_ = the_output->length_;
    this->previous_->ParseAndEvaluate(arg, the_output);
    n->value_ = the_output->RemoveFromString(position_);
  }
  return n->value_;
}


number Context::EvaluateNumber(int argNr, Text* &the_output) {
  return EvaluateNumber(GetArgument(argNr), the_output);
}


number Context::EvaluateNumber(Node* n, Text* &the_output) {
  if (!n) {
    return 0;
  }
  number num = EvaluateArgument(n, the_output)->getNumber();
  if (num == kNAN) {
    ReportErrorAndDie("Not a number", EvaluateArgument(n, the_output));
  }
  return num;
}


Node* Context::GetArgument(int argNr) {
  Node* p;
  if (!first_) {
    first_ = last_ = new Node(NULL);
  }
  p = first_;
  for (;;) {
    if (!argNr) {
      return p;
    }
    p = p->next_;
    if (!p) {
      p = new Node(NULL);
      last_->next_ = p;  // fixed <~1~> bug -- jr 17Aug11
      last_ = p;
    }
    argNr -= 1;
  }
}

void Context::ResetArgument(int argNr) {
  Node* n;
  n = this->GetArgument(argNr);
  delete n->value_;
  n->value_ = NULL;
}


void Context::FindError(Text* report) {
  if (previous_) {
    previous_->FindError(report);
  }
  if (source_) {
    report->AddToString(source_->text()->name_, source_->text()->name_length_);
    report->AddToString('(');
    report->AddNumberToString(line_ + 1);
    report->AddToString(',');
    report->AddNumberToString(character_ + 1);
    report->AddToString('/');
    report->AddNumberToString(index_ + 1);
    report->AddToString(") ");
  }
  // add first_ jr 4Sep11
  if (first_ && first_->text_ && first_->text_->length_) {
    report->AddToString("<~");
    report->AddToString(first_->text_);
    report->AddToString("~> ");
  }
}
