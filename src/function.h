// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef SRC_FUNCTION_H_
#define SRC_FUNCTION_H_

#include <map>
#include <string>

#include "tilton.h"
#include "hash_table.h"
#include "context.h"
#include "node.h"
#include "macro.h"


// FunctionContext -- collection of functions available as built-ins
class FunctionContext {
 public:
  // Constructor
  FunctionContext();
  virtual ~FunctionContext();
  
  static FunctionContext* instance();
  
  std::map<std::string, Builtin>& functions() {
    static std::map<std::string, Builtin>  functions_;
    return functions_;
  }
  
  // registerTiltonFunctions
  // Register the built-in functions for use in Tilton
  void RegisterTiltonFunctions();

  Builtin GetFunction(std::string name) {
    it_ = functions().find(name);
    if (it_ != functions().end()) {
      return it_->second;
    } else {
      return NULL;
    }
  }
  
 private:
  void RegisterFunction(std::string name, Builtin function) {
    functions().insert(std::make_pair(name, function));
  }
  
  static FunctionContext*  pInstance;
  
  // Container for built ins  
//  std::map<std::string, Builtin>  functions_;
  std::map<std::string, Builtin>::const_iterator it_;
};

class ArithmeticFunction {
 public:
  ArithmeticFunction();
  virtual ~ArithmeticFunction();

  // reduce
  // Reduce a list of parameters to a single value. This is used
  // to implement the tilton arithmetic functions.
  static void reduce(Context* context, number num,
                     number (*f)(number, number), Text* &the_output) {
     Node* n = context->first_->next_;
     if (n) {
         if (num == kNAN) {
             num = context->EvaluateNumber(n, the_output);
             n = n->next_;
         }
         if (n) {
             for (;;) {
                 if (!n) {
                     break;
                 }
                 number d = context->EvaluateNumber(n, the_output);
                 if (num == kNAN || d == kNAN) {
                     num = kNAN;
                     break;
                 }
                 num = f(num, d);
                 n = n->next_;
             }
         }
     }
     the_output->AddNumberToString(num);
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
     return second ? first / second : kNAN;
  }

  static number mod(number first, number second) {
     return second ? first % second : kNAN;
  }
};

class BooleanFunction {
 public:
  BooleanFunction();
  virtual ~BooleanFunction();

  //  test - This is used to implement the tilton binary conditional functions.
  static void test(Context* context, int (*f)(Text*, Text*), Text* &the_output) {
     Node* c;
     Node* t;
     Node* s = context->first_->next_;
     if (!s) {
         context->ReportErrorAndDie("No parameters");
         return;
     }
     Text* swich = context->EvaluateArgument(s, the_output);
     c = s->next_;
     if (!c) {
         context->ReportErrorAndDie("Too few parameters");
     }
     t = c->next_;
     if (!t) {
         context->ReportErrorAndDie("Too few parameters");
     }
     for (;;) {
         if (f(swich, context->EvaluateArgument(c, the_output))) {  // then
             the_output->AddToString(context->EvaluateArgument(t, the_output));
             return;
         }
         c = t->next_;
         if (!c) {
             return;  // empty else
         }
         t = c->next_;
         if (!t) {    // else
             the_output->AddToString(context->EvaluateArgument(c, the_output));
             return;
         }
     }
  }

  static int eq(Text* first, Text* second) {
     return first->IsEqual(second);
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
     return !(first->IsEqual(second));
  }
};

// AddFunction -- Function object for built-in function
class AddFunction: public ArithmeticFunction {
 public:
  AddFunction();
  virtual ~AddFunction();

  static void evaluate(Context* context, Text* &the_output) {
    ArithmeticFunction::reduce(context, 0, ArithmeticFunction::add, the_output);
  }
};

// AndFunction -- Function object for built-in function
class AndFunction {
 public:
  AndFunction();
  virtual ~AndFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* n = context->first_->next_;
    Text* t = NULL;
    while (n) {
        t = context->EvaluateArgument(n, the_output);
        if (t->length_ == 0) {
            return;
        }
        n = n->next_;
    }
    the_output->AddToString(t);
  }
};

// AppendFunction -- Function object for built-in function
class AppendFunction {
 public:
  AppendFunction();
  virtual ~AppendFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* n = context->first_->next_;
    if (n == NULL) {
        context->ReportErrorAndDie("Missing name");
    }
    Text* name = context->EvaluateArgument(n, the_output);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }

    Macro* t = MacroTable::instance()->macro_table()->GetMacroDefOrInsertNull(name);

    for (;;) {
        n = n->next_;
        if (!n) {
            break;
        }
        t->AddToString(context->EvaluateArgument(n, the_output));
    }
  }
};

// DefineFunction -- Function object for built-in function
class DefineFunction {
 public:
  DefineFunction();
  virtual ~DefineFunction();

  static void evaluate(Context* context, Text* &the_output) {
    int position = the_output->length_;
    the_output->AddToString(context->GetArgument(kArgTwo)->text_);
    Text* name = context->EvaluateArgument(kArgOne, the_output);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }
    MacroTable::instance()->macro_table()->
      InstallMacro(name, the_output->RemoveFromString(position));
  }
};

// DefinedFunction -- Function object for built-in function
class DefinedFunction {
 public:
  DefinedFunction();
  virtual ~DefinedFunction();

  static void evaluate(Context* context, Text* &the_output) {
  the_output->AddToString(
      context->EvaluateArgument(
        MacroTable::instance()->macro_table()->
        LookupMacro(context->EvaluateArgument(kArgOne, the_output)) ? kArgTwo : kArgThree, the_output));
  }
};

// DeleteFunction -- Function object for built-in function
class DeleteFunction {
 public:
  DeleteFunction();
  virtual ~DeleteFunction();

  static void evaluate(Context* context, Text* &the_output) {
  Node* n = context->first_->next_;
  while (n) {
    Text* name = context->EvaluateArgument(n, the_output);
    Macro* t = MacroTable::instance()->macro_table()->LookupMacro(name);
    Macro* u = NULL;
    while (t) {
      if (t->IsNameEqual(name)) {
        if (u) {
          u->link_ = t->link_;
        } else {
          MacroTable::instance()->macro_table()->InstallMacro(name, t->link_);
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

  static void evaluate(Context* context, Text* &the_output) {
    ArithmeticFunction::reduce(context, kNAN, ArithmeticFunction::div, the_output);
  }
};

// DumpFunction -- Function object for built-in function
class DumpFunction {
 public:
  DumpFunction();
  virtual ~DumpFunction();

  static void evaluate(Context* context, Text* &the_output) {
    MacroTable::instance()->macro_table()->PrintMacroTable();
  }
};

// EntityifyFunction -- Function object for built-in function
class EntityifyFunction {
 public:
  EntityifyFunction();
  virtual ~EntityifyFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Text* t = context->EvaluateArgument(kArgOne, the_output);
    int c;
    int i;
    if (t && t->length_) {
        for (i = 0; i < t->length_; i += 1) {
            c = t->GetCharacter(i);
            switch (c) {
                case '&':
                    the_output->AddToString("&amp;");
                    break;
                case '<':
                    the_output->AddToString("&lt;");
                    break;
                case '>':
                    the_output->AddToString("&gt;");
                    break;
                case '"':
                    the_output->AddToString("&quot;");
                    break;
                case '\'':
                    the_output->AddToString("&#039;");
                    break;
                case '\\':
                    the_output->AddToString("&#092;");
                    break;
                case '~':
                    the_output->AddToString("&#126;");
                    break;
                default:
                    the_output->AddToString(c);
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

  static void evaluate(Context* context, Text* &the_output) {
    BooleanFunction::test(context, BooleanFunction::eq, the_output);
  }
};

// EvalFunction -- Function object for built-in function
class EvalFunction {
 public:
  EvalFunction();
  virtual ~EvalFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Context* new_context = new Context(context, NULL);
    new_context->AddArgument("eval");
    new_context->AddArgument("<~2~>");
    new_context->AddArgument("<~3~>");
    new_context->AddArgument("<~4~>");
    new_context->AddArgument("<~5~>");
    new_context->AddArgument("<~6~>");
    new_context->AddArgument("<~7~>");
    new_context->AddArgument("<~8~>");
    new_context->ParseAndEvaluate(context->GetArgument(kArgOne)->text_, the_output);
    delete new_context;
  }
};

// FirstFunction -- Function object for built-in function
class FirstFunction {
 public:
  FirstFunction();
  virtual ~FirstFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* arg = context->first_->next_;
    Text* name = context->EvaluateArgument(arg, the_output);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name: first");
    }
    Macro* macro = MacroTable::instance()->macro_table()->LookupMacro(name);
    if (!macro) {;
        context->ReportErrorAndDie("Undefined variable", name);
        return;
    }
    Text* d = NULL;
    int   len = 0;
    int   r = macro->length_;
    for (;;) {
        arg= arg->next_;
        if (!arg) {
            break;
        }
        int index = macro->FindFirstSubstring(context->EvaluateArgument(arg, the_output));
        if (index >= 0 && index < r) {
            r = index;
            d = context->EvaluateArgument(arg, the_output);
            len = d->length_;
        }
    }
    the_output->AddToString(macro->definition_, r);
    macro->ReplaceDefWithSubstring(r + len, macro->length_ - (r + len));
    arg = context->previous_->GetArgument(kArgZero);
    delete arg->text_;
    arg->text_ = NULL;
    delete arg->value_;
    arg->value_ = d ? new Text(d) : NULL;
  }
};

// GeFunction -- Function object for built-in function
class GeFunction {
 public:
  GeFunction();
  virtual ~GeFunction();

  static void evaluate(Context* context, Text* &the_output) {
    BooleanFunction::test(context, BooleanFunction::ge, the_output);
  }
};

// GensymFunction -- Function object for built-in function
class GensymFunction {
 public:
  GensymFunction();
  virtual ~GensymFunction();

  static void evaluate(Context* context, Text* &the_output) {
    static number theSequenceNumber = 1000;
    theSequenceNumber += 1;
    the_output->AddNumberToString(theSequenceNumber);
  }
};

// GetFunction -- Function object for built-in function
class GetFunction {
 public:
  GetFunction();
  virtual ~GetFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* n = context->first_->next_;
    while (n) {
        Text* name = context->EvaluateArgument(n, the_output);
        Macro* macro = MacroTable::instance()->macro_table()->LookupMacro(name);
        if (macro) {
            the_output->AddToString(new Text(macro));
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

  static void evaluate(Context* context, Text* &the_output) {
    BooleanFunction::test(context, BooleanFunction::gt, the_output);
  }
};

// IncludeFunction -- Function object for built-in function
class IncludeFunction {
 public:
  IncludeFunction();
  virtual ~IncludeFunction();

  static void evaluate(Context* context, Text* &the_output) {
    // Node* n = context->first_->next_;
    Text* string = new Text();
    Text* name = context->EvaluateArgument(kArgOne, the_output);
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
    new_context->ParseAndEvaluate(string, the_output);
    delete new_context;
    delete string;
  }
};

// LastFunction -- Function object for built-in function
class LastFunction {
 public:
  LastFunction();
  virtual ~LastFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* n = context->first_->next_;
    Text* name = context->EvaluateArgument(kArgOne, the_output);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }
    Macro* macro = MacroTable::instance()->macro_table()->LookupMacro(name);
    if (!macro) {
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
        int index = macro->FindLastSubstring(context->EvaluateArgument(n, the_output));
        if (index > r) {
            r = index;
            d = context->EvaluateArgument(n, the_output);
            len = d->length_;
        }
    }
    the_output->AddToString(macro->definition_ + r + len,
                      macro->length_ - (r + len));
    macro->length_ = r;
    n = context->previous_->GetArgument(kArgZero);
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

  static void evaluate(Context* context, Text* &the_output) {
    BooleanFunction::test(context, BooleanFunction::le, the_output);
  }
};

// LengthFunction -- Function object for built-in function
class LengthFunction {
 public:
  LengthFunction();
  virtual ~LengthFunction();

  static void evaluate(Context* context, Text* &the_output) {
    the_output->AddNumberToString(context->EvaluateArgument(kArgOne, the_output)->utfLength());
  }
};

// LiteralFunction -- Function object for built-in function
class LiteralFunction {
 public:
  LiteralFunction();
  virtual ~LiteralFunction();

  static void evaluate(Context* context, Text* &the_output) {
    the_output->AddToString(context->GetArgument(kArgOne)->text_);
  }
};

// LoopFunction -- Function object for built-in function
class LoopFunction {
 public:
  LoopFunction();
  virtual ~LoopFunction();

  static void evaluate(Context* context, Text* &the_output) {
    // Node* n = context->first_->next_;
    while (context->EvaluateArgument(kArgOne, the_output)->length_ > 0) {
        context->ResetArgument(kArgOne);
        context->ResetArgument(kArgTwo);
        the_output->AddToString(context->EvaluateArgument(kArgTwo, the_output));
    }
  }
};

// LtFunction -- Function object for built-in function
class LtFunction {
 public:
  LtFunction();
  virtual ~LtFunction();

  static void evaluate(Context* context, Text* &the_output) {
    BooleanFunction::test(context, BooleanFunction::lt, the_output);
  }
};

// ModFunction -- Function object for built-in function
class ModFunction: public ArithmeticFunction  {
 public:
  ModFunction();
  virtual ~ModFunction();

  static void evaluate(Context* context, Text* &the_output) {
    ArithmeticFunction::reduce(context, kNAN, ArithmeticFunction::div, the_output);
  }
};

// MultFunction -- Function object for built-in function
class MultFunction: public ArithmeticFunction  {
 public:
  MultFunction();
  virtual ~MultFunction();

  static void evaluate(Context* context, Text* &the_output) {
    ArithmeticFunction::reduce(context, 1, ArithmeticFunction::mult, the_output);
  }
};

// MuteFunction -- Function object for built-in function
class MuteFunction {
 public:
  MuteFunction();
  virtual ~MuteFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* n = context->first_->next_;
    while (n) {
        context->EvaluateArgument(n, the_output);
        n = n->next_;
    }
  }
};

// NeFunction -- Function object for built-in function
class NeFunction {
 public:
  NeFunction();
  virtual ~NeFunction();

  static void evaluate(Context* context, Text* &the_output) {
    BooleanFunction::test(context, BooleanFunction::ne, the_output);
  }
};

// NullFunction -- Function object for built-in function
class NullFunction {
 public:
  NullFunction();
  virtual ~NullFunction();

  static void evaluate(Context* context, Text* &the_output) {
  }
};

// NumberFunction -- Function object for built-in function
class NumberFunction {
 public:
  NumberFunction();
  virtual ~NumberFunction();

  static void evaluate(Context* context, Text* &the_output) {
    number num = context->EvaluateArgument(kArgOne, the_output)->getNumber();
    the_output->AddToString(context->EvaluateArgument(num != kNAN ? 2 : 3, the_output));
  }
};

// OrFunction -- Function object for built-in function
class OrFunction {
 public:
  OrFunction();
  virtual ~OrFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* n = context->first_->next_;
    Text* t = NULL;
    while (n) {
        t = context->EvaluateArgument(n, the_output);
        if (t->length_) {
            break;
        }
        n = n->next_;
    }
    the_output->AddToString(t);
  }
};

// PrintFunction -- Function object for built-in function
class PrintFunction {
 public:
  PrintFunction();
  virtual ~PrintFunction();

  static void evaluate(Context* context, Text* &the_output) {
    context->DumpContext();
  }
};

// ReadFunction -- Function object for built-in function
class ReadFunction {
 public:
  ReadFunction();
  virtual ~ReadFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Text* string = new Text();
    Text* name = context->EvaluateArgument(kArgOne, the_output);
    if (!string->ReadFromFile(name)) {
        context->ReportErrorAndDie("Error in reading file", name);
    }
    the_output->AddToString(string);
    delete string;
  }
};

// RepFunction -- Function object for built-in function
class RepFunction {
 public:
  RepFunction();
  virtual ~RepFunction();

  static void evaluate(Context* context, Text* &the_output) {
    // Node* n = context->first_->next_;
    Text* value = context->EvaluateArgument(kArgOne, the_output);
    for (number num = context->EvaluateNumber(kArgTwo, the_output); num > 0; num -= 1) {
        the_output->AddToString(value);
    }
  }
};

// SetFunction -- Function object for built-in function
class SetFunction {
 public:
  SetFunction();
  virtual ~SetFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Text* name = context->EvaluateArgument(kArgOne, the_output);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }
    MacroTable::instance()->macro_table()->
      InstallMacro(name, context->EvaluateArgument(kArgTwo, the_output));
  }
};

// SlashifyFunction -- Function object for built-in function
class SlashifyFunction {
 public:
  SlashifyFunction();
  virtual ~SlashifyFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Text* t = context->EvaluateArgument(kArgOne, the_output);
    int c;
    int i;
    if (t && t->length_) {
        for (i = 0; i < t->length_; i += 1) {
            c = t->GetCharacter(i);
            switch (c) {
                case '\\':  // backslash
                case '\'':  // single quote
                case  '"':  // double quote
                    the_output->AddToString('\\');
                    break;
            }
            the_output->AddToString(c);
        }
    }
  }
};

// StopFunction -- Function object for built-in function
class StopFunction {
 public:
  StopFunction();
  virtual ~StopFunction();

  static void evaluate(Context* context, Text* &the_output) {
    context->ReportErrorAndDie("Stop", context->EvaluateArgument(kArgOne, the_output));
  }
};

// SubFunction -- Function object for built-in function
class SubFunction: public ArithmeticFunction {
 public:
  SubFunction();
  virtual ~SubFunction();

  static void evaluate(Context* context, Text* &the_output) {
    ArithmeticFunction::reduce(context, kNAN, ArithmeticFunction::sub, the_output);
  }
};

// SubstrFunction -- Function object for built-in function
class SubstrFunction {
 public:
  SubstrFunction();
  virtual ~SubstrFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* arg = context->first_->next_;
    Text* string = context->EvaluateArgument(arg, the_output);
    arg = arg->next_;
    if (arg) {
      number start = context->EvaluateNumber(arg, the_output);
      if (start < 0) {
        start += string->length_;
      }
      number len = kInfinity;
      arg = arg->next_;
      if (arg) {
        len = context->EvaluateNumber(arg, the_output);
      }
      if (start >= 0 && len > 0) {
        the_output->AddToString(
            context->EvaluateArgument(kArgOne, the_output)->utfSubstr(
                static_cast<int>(start),
                static_cast<int>(len)));
      }
    }
  }
};

// TrimFunction -- Function object for built-in function
class TrimFunction {
 public:
  TrimFunction();
  virtual ~TrimFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* n = context->first_->next_;
    while (n) {
        the_output->RemoveSpacesAddToString(context->EvaluateArgument(n, the_output));
        n = n->next_;
    }
  }
};

// UnicodeFunction -- Function object for built-in function
class UnicodeFunction {
 public:
  UnicodeFunction();
  virtual ~UnicodeFunction();

  static void evaluate(Context* context, Text* &the_output) {
    Node* n = context->first_->next_;
    while (n) {
        number num = context->EvaluateNumber(n, the_output);
        if (num >= 0) {
            int i = static_cast<int>(num);
            if (i <= 0x7F) {
                the_output->AddToString(i);
            } else if (i <= 0x7FF) {
                the_output->AddToString(0xC000 |  (i >> 6));
                the_output->AddToString(0x8000 |  (i        & 0x3F));
            } else if (i <= 0xFFFF) {
                the_output->AddToString(0xE000 |  (i >> 12));
                the_output->AddToString(0x8000 | ((i >> 6)  & 0x3F));
                the_output->AddToString(0x8000 |  (i        & 0x3F));
            } else {
                the_output->AddToString(0xF000 |  (i >> 18));
                the_output->AddToString(0x8000 | ((i >> 12) & 0x3F));
                the_output->AddToString(0x8000 | ((i >> 6)  & 0x3F));
                the_output->AddToString(0x8000 |  (i        & 0x3F));
            }
        } else {
            context->ReportErrorAndDie(
                "Bad character code", context->EvaluateArgument(n, the_output));
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

  static void evaluate(Context* context, Text* &the_output) {
    Text* name = context->EvaluateArgument(kArgOne, the_output);
    if (!context->EvaluateArgument(kArgTwo, the_output)->WriteToFile(name)) {
      context->ReportErrorAndDie("Error in writing file", name);
    }
  }
};
#endif  // SRC_FUNCTION_H_
