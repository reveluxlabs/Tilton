// context.cpp
// For interface defintions, see context.h

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

//  Context is the key datastructure in Tilton. It keeps a collection of
//  parameters in numbered slots. We keep in each slot a raw string
//  and an evaluated value (for memoization).

//  A Context can point to a previous context, which allows contexts to be
//  nested. A Context can also include source information for use in error
//  messages.

#include "context.h"

#include <stdlib.h>
#include <stdio.h>

#include "iter.h"
#include "node.h"
#include "search.h"
#include "tilton.h"
#include "text.h"

Context::Context(Context* prev, Iter* s) {
    position_ = 0;
    first_ = NULL;
    last_ = NULL;
    previous_ = prev;
    source_ = s;
    if (s) {
        line_ = s->line;
        character_ = s->character;
        index_ = s->index;
    } else {
        line_ = 0;
        character_ = 0;
        index_ = 0;
    }
}


Context::~Context() {
    delete this->first_;
}


// add a c-string to a context

void Context::AddArgument(const char* s) {
    AddArgument(new Text(s));
}

//  add an argument to a context

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
void Context::eval(Text* input, Text* the_output) {
  int c;                 // current character
  int depth = 0;         // depth of nested <~ ~>
  int tildes_seen= 0;     // the number of tildes in the separator ~~~
  Context* new_context = NULL;
  Iter* in = new Iter(input);

  // Loop over the characters in the input

  for (;;) {
    switch ((c = in->next())) {
      case '<':
        this->EvaluateLeftAngle(in, depth, the_output,
                                tildes_seen, new_context);
        break;
      case '~':
        this->EvaluateTilde(in, depth, the_output, tildes_seen, new_context);
        break;
      case EOT:
        this->EvaluateEOT(in, depth, the_output, tildes_seen, new_context);
        return;
      // literal character
      default:
        the_output->addToString(c);
        break;
    }
  }
}

void Context::EvaluateLeftAngle(Iter* in, int &depth, Text* the_output,
                        int &tildes_seen, Context* &new_context) {
  int run_length;  // the number of tildes currently under consideration

  run_length = checkForTilde(in, 0);

  if (!stackEmpty(depth)) {       // in middle of macro expansion
    if (haveTildes(run_length)) {  // found embedded macro bracket
        depth += 1;
    }
    the_output->addToString('<');
    the_output->addToString('~', run_length);
  } else if (haveTildes(run_length)) {  // found first macro bracket
    depth = 1;
    tildes_seen = run_length;
    new_context = new Context(this, in);
    new_context->position_ = the_output->length_;
  } else {  // left angle in the middle of text being passed through
    the_output->addToString('<');
  }
}

void Context::EvaluateTilde(Iter* in, int &depth, Text* the_output,
                        int &tildes_seen, Context* &new_context) {
  int arg_number;    // argument number
  Node* arg;         // current arg
  Text* arg_text;    // current text
  int run_length;    // the number of tildes currently under consideration

  run_length = checkForTilde(in, 1);

  // if in middle of expansion and tildes seen, create args for each
  if (depth == 1 && run_length >= tildes_seen) {
    new_context->AddArgument(
        the_output->removeFromString(new_context->position_));
    for (;;) {
      run_length -= tildes_seen;
      if (run_length < tildes_seen) {
        break;
      }
      new_context->AddArgument(new Text());
    }
  }

  // pass through tildes in plain text
  the_output->addToString('~', run_length);

  // if we have seen the closing bracket
  if (in->peek() == '>') {
    //  ~>
    in->next();
    if (!stackEmpty(depth)) {
      depth -= 1;
      if (!stackEmpty(depth)) {
          the_output->addToString('>');
      } else {
        if (run_length) {
          new_context->ReportErrorAndDie("Short ~>");
        }
        //    apply:
        arg = new_context->first_;
        //    <~NUMBER~>
        arg_text = arg->text_;
        arg_number = arg_text->getChar(0) - '0';
        if (Text::isDigit(arg_number)) {
          arg_number = arg_text->ConvertAlphaToInteger();
          if (arg_number >= 0) {
            if (!arg->next_) {
              the_output->addToString(this->EvaluateArgument(arg_number, the_output));
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
        Context::ExpandMacro(new_context, the_output);
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
  this->eval(n->text_, the_output);
  n->value_ = the_output->removeFromString(new_context->position_);
  return n;
}

void Context::SetMacroVariable(int varNo, Text* t) {
  Node* o;
  o = this->GetArgument(varNo);
  delete o->text_;
  o->text_ = NULL;
  delete o->value_;
  // o->value[varNo] = new Text(t);
  o->value_ = new Text(t);
}

void Context::ExpandMacro(Context* &new_context, Text* the_output) {
    Text* macro;
    Text* name;

    name = new_context->EvaluateArgument(0, the_output);
    macro = MacroTable::instance()->macro_table()->LookupMacro(name);
    if (macro) {
        if (macro->function_) {
            macro->function_(new_context, the_output);
        } else {
            new_context->eval(macro, the_output);
        }
    } else {
        //    undefined
        new_context->ReportErrorAndDie("Undefined macro");
    }
    delete new_context;
    new_context = NULL;
}

void Context::EvaluateEOT(Iter* in, int &depth, Text* the_output,
                      int &tildes_seen, Context* &new_context) {
    if (!stackEmpty(depth)) {
        new_context->ReportErrorAndDie("Missing ~>");
    }
    delete in;
}

Text* Context::EvaluateArgument(int argNr, Text* the_output) {
    return EvaluateArgument(GetArgument(argNr), the_output);
}


Text* Context::EvaluateArgument(Node* n, Text* the_output) {
  if (n == NULL) {
      return NULL;
  }
  if (n->value_ == NULL) {
    if (n->text_ == NULL) {
      return NULL;
    }
    Text* arg = n->text_;
    int position_ = the_output->length_;
    this->previous_->eval(arg, the_output);
    n->value_ = the_output->removeFromString(position_);
  }
  return n->value_;
}


number Context::evalNumber(int argNr, Text* the_output) {
  return evalNumber(GetArgument(argNr), the_output);
}


number Context::evalNumber(Node* n, Text* the_output) {
  if (!n) {
    return 0;
  }
  number num = EvaluateArgument(n, the_output)->getNumber();
  if (num == NAN) {
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

void Context::resetArg(int argNr) {
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
    report->AddToString(source_->text->name_, source_->text->name_length_);
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
