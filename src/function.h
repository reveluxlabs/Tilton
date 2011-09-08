//  function.h: interface for the Function Context class

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

#ifndef SRC_FUNCTION_H_
#define SRC_FUNCTION_H_

#include "tilton.h"
#include "search.h"
#include "context.h"
#include "node.h"

extern Text* g_the_output;

// FunctionContext -- collection of functions available as built-ins
class FunctionContext {
 public:
  // Constructor
  FunctionContext();

  // Destructor
  virtual ~FunctionContext();

  // registerTiltonFunctions
  // Register the built-in functions for use in Tilton
  static void RegisterTiltonFunctions();

 private:
};

class ArithmeticFunction {
 public:
  ArithmeticFunction();
  virtual ~ArithmeticFunction();

  // reduce
  // Reduce a list of parameters to a single value. This is used
  // to implement the tilton arithmetic functions.
  static void reduce(Context* context, number num,
                     number (*f)(number, number)) {
     Node* n = context->first_->next_;
     if (n) {
         if (num == NAN) {
             num = context->evalNumber(n);
             n = n->next_;
         }
         if (n) {
             for (;;) {
                 if (!n) {
                     break;
                 }
                 number d = context->evalNumber(n);
                 if (num == NAN || d == NAN) {
                     num = NAN;
                     break;
                 }
                 num = f(num, d);
                 n = n->next_;
             }
         }
     }
     g_the_output->AddNumberToString(num);
  }

  static number add(number first, number second) {
     return first + second;
  }

  static number sub(number first, number second) {
     return first - second;
  }

  static number mult(number first, number second) {
     return first * second;
  }

  static number div(number first, number second) {
     return second ? first / second : NAN;
  }

  static number mod(number first, number second) {
     return second ? first % second : NAN;
  }
};

class BooleanFunction {
 public:
  BooleanFunction();
  virtual ~BooleanFunction();

  //  test - This is used to implement the tilton binary conditional functions.
  static void test(Context* context, int (*f)(Text*, Text*)) {
     Node* c;
     Node* t;
     Node* s = context->first_->next_;
     if (!s) {
         context->ReportErrorAndDie("No parameters");
         return;
     }
     Text* swich = context->EvaluateArgument(s);
     c = s->next_;
     if (!c) {
         context->ReportErrorAndDie("Too few parameters");
     }
     t = c->next_;
     if (!t) {
         context->ReportErrorAndDie("Too few parameters");
     }
     for (;;) {
         if (f(swich, context->EvaluateArgument(c))) {  // then
             g_the_output->AddToString(context->EvaluateArgument(t));
             return;
         }
         c = t->next_;
         if (!c) {
             return;  // empty else
         }
         t = c->next_;
         if (!t) {    // else
             g_the_output->AddToString(context->EvaluateArgument(c));
             return;
         }
     }
  }

  static int eq(Text* first, Text* second) {
     return first->is(second);
  }

  static int ge(Text* first, Text* second) {
     return !(first->lt(second));
  }

  static int gt(Text* first, Text* second) {
     return second->lt(first);
  }

  static int le(Text* first, Text* second) {
     return !(second->lt(first));
  }

  static int lt(Text* first, Text* second) {
     return first->lt(second);
  }

  static int ne(Text* first, Text* second) {
     return !(first->is(second));
  }
};

// AddFunction -- Function object for built-in function
class AddFunction: public ArithmeticFunction {
 public:
  AddFunction();
  virtual ~AddFunction();

  void call(Context* context) {
    AddFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    ArithmeticFunction::reduce(context, 0, ArithmeticFunction::add);
  }
};

// AndFunction -- Function object for built-in function
class AndFunction {
 public:
  AndFunction();
  virtual ~AndFunction();

  void call(Context* context) {
    AndFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    Text* t = NULL;
    while (n) {
        t = context->EvaluateArgument(n);
        if (t->length_ == 0) {
            return;
        }
        n = n->next_;
    }
    g_the_output->AddToString(t);
  }
};

// AppendFunction -- Function object for built-in function
class AppendFunction {
 public:
  AppendFunction();
  virtual ~AppendFunction();

  void call(Context* context) {
    AppendFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    if (n == NULL) {
        context->ReportErrorAndDie("Missing name");
    }
    Text* name = context->EvaluateArgument(n);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }

    Text* t = MacroTable::instance()->macro_table()->getDef(name);

    for (;;) {
        n = n->next_;
        if (!n) {
            break;
        }
        t->AddToString(context->EvaluateArgument(n));
    }
  }
};

// DefineFunction -- Function object for built-in function
class DefineFunction {
 public:
  DefineFunction();
  virtual ~DefineFunction();

  void call(Context* context) {
    DefineFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    int position = g_the_output->length_;
    g_the_output->AddToString(context->GetArgument(2)->text_);
    Text* name = context->EvaluateArgument(1);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }
    MacroTable::instance()->macro_table()->
      install(name, g_the_output->tail(position));
  }
};

// DefinedFunction -- Function object for built-in function
class DefinedFunction {
 public:
  DefinedFunction();
  virtual ~DefinedFunction();

  void call(Context* context) {
    DefinedFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
  g_the_output->AddToString(
      context->EvaluateArgument(
        MacroTable::instance()->macro_table()->
        lookup(context->EvaluateArgument(1)) ? 2 : 3));
  }
};

// DeleteFunction -- Function object for built-in function
class DeleteFunction {
 public:
  DeleteFunction();
  virtual ~DeleteFunction();

  void call(Context* context) {
    DeleteFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
  Node* n = context->first_->next_;
  while (n) {
    Text* name = context->EvaluateArgument(n);
    Text* t = MacroTable::instance()->macro_table()->lookup(name);
    Text* u = NULL;
    while (t) {
      if (t->isName(name)) {
        if (u) {
          u->link_ = t->link_;
        } else {
          MacroTable::instance()->macro_table()->install(name, t->link_);
        }
        delete t;
        break;
      }
      u = t;
      t = t->link_;
    }
    n = n->next_;
    }
  }
};

// DivFunction -- Function object for built-in function
class DivFunction: public ArithmeticFunction  {
 public:
  DivFunction();
  virtual ~DivFunction();

  void call(Context* context) {
    DivFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    ArithmeticFunction::reduce(context, NAN, ArithmeticFunction::div);
  }
};

// DumpFunction -- Function object for built-in function
class DumpFunction {
 public:
  DumpFunction();
  virtual ~DumpFunction();

  void call(Context* context) {
    DumpFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    MacroTable::instance()->macro_table()->dump();
  }
};

// EntityifyFunction -- Function object for built-in function
class EntityifyFunction {
 public:
  EntityifyFunction();
  virtual ~EntityifyFunction();

  void call(Context* context) {
    EntityifyFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Text* t = context->EvaluateArgument(1);
    int c;
    int i;
    if (t && t->length_) {
        for (i = 0; i < t->length_; i += 1) {
            c = t->getChar(i);
            switch (c) {
                case '&':
                    g_the_output->AddToString("&amp;");
                    break;
                case '<':
                    g_the_output->AddToString("&lt;");
                    break;
                case '>':
                    g_the_output->AddToString("&gt;");
                    break;
                case '"':
                    g_the_output->AddToString("&quot;");
                    break;
                case '\'':
                    g_the_output->AddToString("&#039;");
                    break;
                case '\\':
                    g_the_output->AddToString("&#092;");
                    break;
                case '~':
                    g_the_output->AddToString("&#126;");
                    break;
                default:
                    g_the_output->AddToString(c);
            }
        }
    }
  }
};

// EqFunction -- Function object for built-in function
class EqFunction {
 public:
  EqFunction();
  virtual ~EqFunction();

  void call(Context* context) {
    EqFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    BooleanFunction::test(context, BooleanFunction::eq);
  }
};

// EvalFunction -- Function object for built-in function
class EvalFunction {
 public:
  EvalFunction();
  virtual ~EvalFunction();

  void call(Context* context) {
    EvalFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Context* new_context = new Context(context, NULL);
    new_context->AddArgument("eval");
    new_context->AddArgument("<~2~>");
    new_context->AddArgument("<~3~>");
    new_context->AddArgument("<~4~>");
    new_context->AddArgument("<~5~>");
    new_context->AddArgument("<~6~>");
    new_context->AddArgument("<~7~>");
    new_context->AddArgument("<~8~>");
    new_context->eval(context->GetArgument(1)->text_);
    delete new_context;
  }
};

// FirstFunction -- Function object for built-in function
class FirstFunction {
 public:
  FirstFunction();
  virtual ~FirstFunction();

  void call(Context* context) {
    FirstFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    Text* name = context->EvaluateArgument(n);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name: first");
    }
    Text* string = MacroTable::instance()->macro_table()->lookup(name);
    if (!string) {;
        context->ReportErrorAndDie("Undefined variable", name);
        return;
    }
    Text* d = NULL;
    int   len = 0;
    int   r = string->length_;
    for (;;) {
        n = n->next_;
        if (!n) {
            break;
        }
        int index = string->indexOf(context->EvaluateArgument(n));
        if (index >= 0 && index < r) {
            r = index;
            d = context->EvaluateArgument(n);
            len = d->length_;
        }
    }
    g_the_output->AddToString(string->string_, r);
    string->substr(r + len, string->length_ - (r + len));
    n = context->previous_->GetArgument(0);
    delete n->text_;
    n->text_ = NULL;
    delete n->value_;
    n->value_ = d ? new Text(d) : NULL;
  }
};

// GeFunction -- Function object for built-in function
class GeFunction {
 public:
  GeFunction();
  virtual ~GeFunction();

  void call(Context* context) {
    GeFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    BooleanFunction::test(context, BooleanFunction::ge);
  }
};

// GensymFunction -- Function object for built-in function
class GensymFunction {
 public:
  GensymFunction();
  virtual ~GensymFunction();

  void call(Context* context) {
    GensymFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    static number theSequenceNumber = 1000;
    theSequenceNumber += 1;
    g_the_output->AddNumberToString(theSequenceNumber);
  }
};

// GetFunction -- Function object for built-in function
class GetFunction {
 public:
  GetFunction();
  virtual ~GetFunction();

  void call(Context* context) {
    GetFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    while (n) {
        Text* name = context->EvaluateArgument(n);
        Text* macro = MacroTable::instance()->macro_table()->lookup(name);
        if (macro) {
            g_the_output->AddToString(macro);
        } else {
            context->ReportErrorAndDie("Undefined variable", name);
        }
        n = n->next_;
    }
  }
};

// GtFunction -- Function object for built-in function
class GtFunction {
 public:
  GtFunction();
  virtual ~GtFunction();

  void call(Context* context) {
    GtFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    BooleanFunction::test(context, BooleanFunction::gt);
  }
};

// IncludeFunction -- Function object for built-in function
class IncludeFunction {
 public:
  IncludeFunction();
  virtual ~IncludeFunction();

  void call(Context* context) {
    IncludeFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    // Node* n = context->first_->next_;
    Text* string = new Text();
    Text* name = context->EvaluateArgument(1);
    if (!string->ReadFromFile(name)) {
        context->ReportErrorAndDie("Error in reading file", name);
    }
    Context* new_context = new Context(context, NULL);

    new_context->AddArgument("include");
    new_context->AddArgument("<~2~>");
    new_context->AddArgument("<~3~>");
    new_context->AddArgument("<~4~>");
    new_context->AddArgument("<~5~>");
    new_context->AddArgument("<~6~>");
    new_context->AddArgument("<~7~>");
    new_context->AddArgument("<~8~>");
    new_context->eval(string);
    delete new_context;
    delete string;
  }
};

// LastFunction -- Function object for built-in function
class LastFunction {
 public:
  LastFunction();
  virtual ~LastFunction();

  void call(Context* context) {
    LastFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    Text* name = context->EvaluateArgument(1);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }
    Text* string = MacroTable::instance()->macro_table()->lookup(name);
    if (!string) {
        context->ReportErrorAndDie("Undefined variable", name);
        return;
    }
    Text* d = NULL;
    int   len = 0;
    int   r = 0;
    for (;;) {
        n = n->next_;
        if (!n) {
            break;
        }
        int index = string->lastIndexOf(context->EvaluateArgument(n));
        if (index > r) {
            r = index;
            d = context->EvaluateArgument(n);
            len = d->length_;
        }
    }
    g_the_output->AddToString(string->string_ + r + len,
                      string->length_ - (r + len));
    string->length_ = r;
    n = context->previous_->GetArgument(0);
    delete n->text_;
    n->text_ = NULL;
    delete n->value_;
    n->value_ = d ? new Text(d) : NULL;
  }
};

// LeFunction -- Function object for built-in function
class LeFunction {
 public:
  LeFunction();
  virtual ~LeFunction();

  void call(Context* context) {
    LeFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    BooleanFunction::test(context, BooleanFunction::le);
  }
};

// LengthFunction -- Function object for built-in function
class LengthFunction {
 public:
  LengthFunction();
  virtual ~LengthFunction();

  void call(Context* context) {
    LengthFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    g_the_output->AddNumberToString(context->EvaluateArgument(1)->utfLength());
  }
};

// LiteralFunction -- Function object for built-in function
class LiteralFunction {
 public:
  LiteralFunction();
  virtual ~LiteralFunction();

  void call(Context* context) {
    LiteralFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    g_the_output->AddToString(context->GetArgument(1)->text_);
  }
};

// LoopFunction -- Function object for built-in function
class LoopFunction {
 public:
  LoopFunction();
  virtual ~LoopFunction();

  void call(Context* context) {
    LoopFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    // Node* n = context->first_->next_;
    while (context->EvaluateArgument(1)->length_ > 0) {
        context->resetArg(1);
        context->resetArg(2);
        g_the_output->AddToString(context->EvaluateArgument(2));
    }
  }
};

// LtFunction -- Function object for built-in function
class LtFunction {
 public:
  LtFunction();
  virtual ~LtFunction();

  void call(Context* context) {
    LtFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    BooleanFunction::test(context, BooleanFunction::lt);
  }
};

// ModFunction -- Function object for built-in function
class ModFunction: public ArithmeticFunction  {
 public:
  ModFunction();
  virtual ~ModFunction();

  void call(Context* context) {
    ModFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    ArithmeticFunction::reduce(context, NAN, ArithmeticFunction::div);
  }
};

// MultFunction -- Function object for built-in function
class MultFunction: public ArithmeticFunction  {
 public:
  MultFunction();
  virtual ~MultFunction();

  void call(Context* context) {
    MultFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    ArithmeticFunction::reduce(context, 1, ArithmeticFunction::mult);
  }
};

// MuteFunction -- Function object for built-in function
class MuteFunction {
 public:
  MuteFunction();
  virtual ~MuteFunction();

  void call(Context* context) {
    MuteFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    while (n) {
        context->EvaluateArgument(n);
        n = n->next_;
    }
  }
};

// NeFunction -- Function object for built-in function
class NeFunction {
 public:
  NeFunction();
  virtual ~NeFunction();

  void call(Context* context) {
    NeFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    BooleanFunction::test(context, BooleanFunction::ne);
  }
};

// NullFunction -- Function object for built-in function
class NullFunction {
 public:
  NullFunction();
  virtual ~NullFunction();

  void call(Context* context) {
    NullFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
  }
};

// NumberFunction -- Function object for built-in function
class NumberFunction {
 public:
  NumberFunction();
  virtual ~NumberFunction();

  void call(Context* context) {
    NumberFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    number num = context->EvaluateArgument(1)->getNumber();
    g_the_output->AddToString(context->EvaluateArgument(num != NAN ? 2 : 3));
  }
};

// OrFunction -- Function object for built-in function
class OrFunction {
 public:
  OrFunction();
  virtual ~OrFunction();

  void call(Context* context) {
    OrFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    Text* t = NULL;
    while (n) {
        t = context->EvaluateArgument(n);
        if (t->length_) {
            break;
        }
        n = n->next_;
    }
    g_the_output->AddToString(t);
  }
};

// PrintFunction -- Function object for built-in function
class PrintFunction {
 public:
  PrintFunction();
  virtual ~PrintFunction();

  void call(Context* context) {
    PrintFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    context->DumpContext();
  }
};

// ReadFunction -- Function object for built-in function
class ReadFunction {
 public:
  ReadFunction();
  virtual ~ReadFunction();

  void call(Context* context) {
    ReadFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Text* string = new Text();
    Text* name = context->EvaluateArgument(1);
    if (!string->ReadFromFile(name)) {
        context->ReportErrorAndDie("Error in reading file", name);
    }
    g_the_output->AddToString(string);
    delete string;
  }
};

// RepFunction -- Function object for built-in function
class RepFunction {
 public:
  RepFunction();
  virtual ~RepFunction();

  void call(Context* context) {
    RepFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    // Node* n = context->first_->next_;
    Text* value = context->EvaluateArgument(1);
    for (number num = context->evalNumber(2); num > 0; num -= 1) {
        g_the_output->AddToString(value);
    }
  }
};

// SetFunction -- Function object for built-in function
class SetFunction {
 public:
  SetFunction();
  virtual ~SetFunction();

  void call(Context* context) {
    SetFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Text* name = context->EvaluateArgument(1);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }
    MacroTable::instance()->macro_table()->
      install(name, context->EvaluateArgument(2));
  }
};

// SlashifyFunction -- Function object for built-in function
class SlashifyFunction {
 public:
  SlashifyFunction();
  virtual ~SlashifyFunction();

  void call(Context* context) {
    SlashifyFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Text* t = context->EvaluateArgument(1);
    int c;
    int i;
    if (t && t->length_) {
        for (i = 0; i < t->length_; i += 1) {
            c = t->getChar(i);
            switch (c) {
                case '\\':  // backslash
                case '\'':  // single quote
                case  '"':  // double quote
                    g_the_output->AddToString('\\');
                    break;
            }
            g_the_output->AddToString(c);
        }
    }
  }
};

// StopFunction -- Function object for built-in function
class StopFunction {
 public:
  StopFunction();
  virtual ~StopFunction();

  void call(Context* context) {
    StopFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    context->ReportErrorAndDie("Stop", context->EvaluateArgument(1));
  }
};

// SubFunction -- Function object for built-in function
class SubFunction: public ArithmeticFunction {
 public:
  SubFunction();
  virtual ~SubFunction();

  void call(Context* context) {
    SubFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    ArithmeticFunction::reduce(context, NAN, ArithmeticFunction::sub);
  }
};

// SubstrFunction -- Function object for built-in function
class SubstrFunction {
 public:
  SubstrFunction();
  virtual ~SubstrFunction();

  void call(Context* context) {
    SubstrFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    Text* string = context->EvaluateArgument(n);
    n = n->next_;
    if (n) {
        number num = context->evalNumber(n);
        if (num < 0) {
            num += string->length_;
        }
        number ber = INFINITY;
        n = n->next_;
        if (n) {
            ber = context->evalNumber(n);
        }
        if (num >= 0 && ber > 0) {
            g_the_output->AddToString(
                context->EvaluateArgument(1)->utfSubstr(static_cast<int>(num),
                                               static_cast<int>(ber)));
        }
    }
  }
};

// TrimFunction -- Function object for built-in function
class TrimFunction {
 public:
  TrimFunction();
  virtual ~TrimFunction();

  void call(Context* context) {
    TrimFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    while (n) {
        g_the_output->RemoveSpacesAddToString(context->EvaluateArgument(n));
        n = n->next_;
    }
  }
};

// UnicodeFunction -- Function object for built-in function
class UnicodeFunction {
 public:
  UnicodeFunction();
  virtual ~UnicodeFunction();

  void call(Context* context) {
    UnicodeFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Node* n = context->first_->next_;
    while (n) {
        number num = context->evalNumber(n);
        if (num >= 0) {
            int i = static_cast<int>(num);
            if (i <= 0x7F) {
                g_the_output->AddToString(i);
            } else if (i <= 0x7FF) {
                g_the_output->AddToString(0xC000 |  (i >> 6));
                g_the_output->AddToString(0x8000 |  (i        & 0x3F));
            } else if (i <= 0xFFFF) {
                g_the_output->AddToString(0xE000 |  (i >> 12));
                g_the_output->AddToString(0x8000 | ((i >> 6)  & 0x3F));
                g_the_output->AddToString(0x8000 |  (i        & 0x3F));
            } else {
                g_the_output->AddToString(0xF000 |  (i >> 18));
                g_the_output->AddToString(0x8000 | ((i >> 12) & 0x3F));
                g_the_output->AddToString(0x8000 | ((i >> 6)  & 0x3F));
                g_the_output->AddToString(0x8000 |  (i        & 0x3F));
            }
        } else {
            context->ReportErrorAndDie(
                "Bad character code", context->EvaluateArgument(n));
            return;
        }
        n = n->next_;
    }
  }
};

// WriteFunction -- Function object for built-in function
class WriteFunction {
 public:
  WriteFunction();
  virtual ~WriteFunction();

  void call(Context* context) {
    WriteFunction::evaluate(context);
  }

  static void evaluate(Context* context) {
    Text* name = context->EvaluateArgument(1);
    if (!context->EvaluateArgument(2)->WriteToFile(name)) {
      context->ReportErrorAndDie("Error in writing file", name);
    }
  }
};
#endif  // SRC_FUNCTION_H_
