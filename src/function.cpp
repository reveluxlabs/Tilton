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

#include <stdlib.h>
#include "tilton.h"

#include "node.h"
#include "context.h"

extern Text* g_the_output;
extern SearchList* g_macro_table;


// reduce - reduce a list of parameters to a single value. This is used
// to implement the tilton arithmetic functions.

static void reduce(Context* context, number num, number (*f)(number, number)) {
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
    g_the_output->appendNumber(num);
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
            g_the_output->append(context->EvaluateArgument(t));
            return;
        }
        c = t->next_;
        if (!c) {
            return;  // empty else
        }
        t = c->next_;
        if (!t) {    // else
            g_the_output->append(context->EvaluateArgument(c));
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


//  By convention, the names of built in functions have a 'tilton_' prefix.

//  tilton add

static void tilton_add(Context* context) {
    reduce(context, 0, add);
}


//  tilton and

static void tilton_and(Context* context) {
    Node* n = context->first_->next_;
    Text* t = NULL;
    while (n) {
        t = context->EvaluateArgument(n);
        if (t->length_ == 0) {
            return;
        }
        n = n->next_;
    }
    g_the_output->append(t);
}


//  tilton append

static void tilton_append(Context* context) {
    Node* n = context->first_->next_;
    if (n == NULL) {
        context->ReportErrorAndDie("Missing name");
    }
    Text* name = context->EvaluateArgument(n);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }

    Text* t = g_macro_table->getDef(name);

    for (;;) {
        n = n->next_;
        if (!n) {
            break;
        }
        t->append(context->EvaluateArgument(n));
    }
}


//  tilton define

static void tilton_define(Context* context) {
    int position = g_the_output->length_;
    g_the_output->append(context->GetArgument(2)->text_);
    Text* name = context->EvaluateArgument(1);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }
    g_macro_table->install(name, g_the_output->tail(position));
}


//  tilton defined?

static void tilton_defined_(Context* context) {
  g_the_output->append(
      context->EvaluateArgument(g_macro_table->lookup(context->EvaluateArgument(1)) ? 2 : 3));
}


//  tilton delete

static void tilton_delete(Context* context) {
  Node* n = context->first_->next_;
  while (n) {
    Text* name = context->EvaluateArgument(n);
    Text* t = g_macro_table->lookup(name);
    Text* u = NULL;
    while (t) {
      if (t->isName(name)) {
        if (u) {
          u->link_ = t->link_;
        } else {
          g_macro_table->install(name, t->link_);
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


//  tilton div

static void tilton_div(Context* context) {
    reduce(context, NAN, div);
}


//  tilton dump

static void tilton_dump(Context* context) {
    g_macro_table->dump();
//    context->nop();
}


//  tilton entityify - outputs a string in which selected characters have
//  been replaced with their HTML entity equivalents.

static void tilton_entityify(Context* context) {
    Text* t = context->EvaluateArgument(1);
    int c;
    int i;
    if (t && t->length_) {
        for (i = 0; i < t->length_; i += 1) {
            c = t->getChar(i);
            switch (c) {
                case '&':
                    g_the_output->append("&amp;");
                    break;
                case '<':
                    g_the_output->append("&lt;");
                    break;
                case '>':
                    g_the_output->append("&gt;");
                    break;
                case '"':
                    g_the_output->append("&quot;");
                    break;
                case '\'':
                    g_the_output->append("&#039;");
                    break;
                case '\\':
                    g_the_output->append("&#092;");
                    break;
                case '~':
                    g_the_output->append("&#126;");
                    break;
                default:
                    g_the_output->append(c);
            }
        }
    }
}



//  tilton eq?

// eq? switch-value case1 then1 (casei theni) else

static void tilton_eq_(Context* context) {
    test(context, eq);
}


//  tilton eval

static void tilton_eval(Context* context) {
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


//  tilton first

static void tilton_first(Context* context) {
    Node* n = context->first_->next_;
    Text* name = context->EvaluateArgument(n);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name: first");
    }
    Text* string = g_macro_table->lookup(name);
    if (!string) {
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
    g_the_output->append(string->string_, r);
    string->substr(r + len, string->length_ - (r + len));
    n = context->previous_->GetArgument(0);
    delete n->text_;
    n->text_ = NULL;
    delete n->value_;
    n->value_ = d ? new Text(d) : NULL;
}


//  tilton ge?

static void tilton_ge_(Context* context) {
    test(context, ge);
}


//  tilton gensym

static void tilton_gensym(Context* context) {
  static number theSequenceNumber = 1000;
  theSequenceNumber += 1;
  g_the_output->appendNumber(theSequenceNumber);
//  context->nop();
}


//  tilton get

static void tilton_get(Context* context) {
    Node* n = context->first_->next_;
    while (n) {
        Text* name = context->EvaluateArgument(n);
        Text* macro = g_macro_table->lookup(name);
        if (macro) {
            g_the_output->append(macro);
        } else {
            context->ReportErrorAndDie("Undefined variable", name);
        }
        n = n->next_;
    }
}


//  tilton gt?

static void tilton_gt_(Context* context) {
    test(context, gt);
}


//  tilton include

static void tilton_include(Context* context) {
    // Node* n = context->first_->next_;
    Text* string = new Text();
    Text* name = context->EvaluateArgument(1);
    if (!string->read(name)) {
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


//  tilton last

static void tilton_last(Context* context) {
    Node* n = context->first_->next_;
    Text* name = context->EvaluateArgument(1);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }
    Text* string = g_macro_table->lookup(name);
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
    g_the_output->append(string->string_ + r + len,
                      string->length_ - (r + len));
    string->length_ = r;
    n = context->previous_->GetArgument(0);
    delete n->text_;
    n->text_ = NULL;
    delete n->value_;
    n->value_ = d ? new Text(d) : NULL;
}


//  tilton le?

static void tilton_le_(Context* context) {
    test(context, le);
}


//  tilton length

static void tilton_length(Context* context) {
    g_the_output->appendNumber(context->EvaluateArgument(1)->utfLength());
}


//  tilton literal

static void tilton_literal(Context* context) {
    g_the_output->append(context->GetArgument(1)->text_);
}


//  tilton loop - eval the second parameter as long as the first parameter
//  is not null.

static void tilton_loop(Context* context) {
    // Node* n = context->first_->next_;
    while (context->EvaluateArgument(1)->length_ > 0) {
        context->resetArg(1);
        context->resetArg(2);
        g_the_output->append(context->EvaluateArgument(2));
    }
}


//  tilton lt?

static void tilton_lt_(Context* context) {
    test(context, lt);
}


//  tilton mod

static void tilton_mod(Context* context) {
    reduce(context, NAN, div);
}


//  tilton mult

static void tilton_mult(Context* context) {
    reduce(context, 1, mult);
}


//  tilton mute

static void tilton_mute(Context* context) {
    Node* n = context->first_->next_;
    while (n) {
        context->EvaluateArgument(n);
        n = n->next_;
    }
}


//  tilton ne?

static void tilton_ne_(Context* context) {
    test(context, ne);
}


//  tilton null - ignore the parameters. This can also be used for comments.

static void tilton_null(Context* context) {
//    context->nop();
}


//  tilton number?

static void tilton_number_(Context* context) {
    number num = context->EvaluateArgument(1)->getNumber();
    g_the_output->append(context->EvaluateArgument(num != NAN ? 2 : 3));
}


//  tilton or

static void tilton_or(Context* context) {
    Node* n = context->first_->next_;
    Text* t = NULL;
    while (n) {
        t = context->EvaluateArgument(n);
        if (t->length_) {
            break;
        }
        n = n->next_;
    }
    g_the_output->append(t);
}


//  tilton print

static void tilton_print(Context* context) {
    context->DumpContext();
}


//  tilton read

static void tilton_read(Context* context) {
    Text* string = new Text();
    Text* name = context->EvaluateArgument(1);
    if (!string->read(name)) {
        context->ReportErrorAndDie("Error in reading file", name);
    }
    g_the_output->append(string);
    delete string;
}


// tilton rep

static void tilton_rep(Context* context) {
    // Node* n = context->first_->next_;
    Text* value = context->EvaluateArgument(1);
    for (number num = context->evalNumber(2); num > 0; num -= 1) {
        g_the_output->append(value);
    }
}

//  tilton set

static void tilton_set(Context* context) {
    Text* name = context->EvaluateArgument(1);
    if (name->length_ < 1) {
        context->ReportErrorAndDie("Missing name");
    }
    g_macro_table->install(name, context->EvaluateArgument(2));
}


//  tilton slashify outputs a string in which selected
//  characters get a \ prefix.

static void tilton_slashify(Context* context) {
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
                    g_the_output->append('\\');
                    break;
            }
            g_the_output->append(c);
        }
    }
}


//  tilton stop

static void tilton_stop(Context* context) {
    context->ReportErrorAndDie("Stop", context->EvaluateArgument(1));
}


//  tilton sub

static void tilton_sub(Context* context) {
    reduce(context, NAN, sub);
}


//  tilton substr

static void tilton_substr(Context* context) {
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
            g_the_output->append(
                context->EvaluateArgument(1)->utfSubstr(static_cast<int>(num),
                                               static_cast<int>(ber)));
        }
    }
}


//  tilton trim

static void tilton_trim(Context* context) {
    Node* n = context->first_->next_;
    while (n) {
        g_the_output->trim(context->EvaluateArgument(n));
        n = n->next_;
    }
}


//  tilton unicode

static void tilton_unicode(Context* context) {
    Node* n = context->first_->next_;
    while (n) {
        number num = context->evalNumber(n);
        if (num >= 0) {
            int i = static_cast<int>(num);
            if (i <= 0x7F) {
                g_the_output->append(i);
            } else if (i <= 0x7FF) {
                g_the_output->append(0xC000 |  (i >> 6));
                g_the_output->append(0x8000 |  (i        & 0x3F));
            } else if (i <= 0xFFFF) {
                g_the_output->append(0xE000 |  (i >> 12));
                g_the_output->append(0x8000 | ((i >> 6)  & 0x3F));
                g_the_output->append(0x8000 |  (i        & 0x3F));
            } else {
                g_the_output->append(0xF000 |  (i >> 18));
                g_the_output->append(0x8000 | ((i >> 12) & 0x3F));
                g_the_output->append(0x8000 | ((i >> 6)  & 0x3F));
                g_the_output->append(0x8000 |  (i        & 0x3F));
            }
        } else {
            context->ReportErrorAndDie(
                "Bad character code", context->EvaluateArgument(n));
            return;
        }
        n = n->next_;
    }
}


//  tilton write

static void tilton_write(Context* context) {
    Text* name = context->EvaluateArgument(1);
    if (!context->EvaluateArgument(2)->write(name)) {
        context->ReportErrorAndDie("Error in writing file", name);
    }
}


// jr 1Sep11
FunctionContext::FunctionContext() {
    // No further initialization required.
}

// jr 1Sep11
FunctionContext::~FunctionContext() {
    // No further deallocation required.
}

// jr 1Sep11 (based on code by Douglas Crockford)
void FunctionContext::RegisterTiltonFunctions(SearchList* g_macro_table) {
    g_macro_table->install("add",       tilton_add);
    g_macro_table->install("and",       tilton_and);
    g_macro_table->install("append",    tilton_append);
    g_macro_table->install("define",    tilton_define);
    g_macro_table->install("defined?",  tilton_defined_);
    g_macro_table->install("delete",    tilton_delete);
    g_macro_table->install("div",       tilton_div);
    g_macro_table->install("dump",      tilton_dump);
    g_macro_table->install("entityify", tilton_entityify);
    g_macro_table->install("eq?",       tilton_eq_);
    g_macro_table->install("eval",      tilton_eval);
    g_macro_table->install("first",     tilton_first);
    g_macro_table->install("ge?",       tilton_ge_);
    g_macro_table->install("gensym",    tilton_gensym);
    g_macro_table->install("get",       tilton_get);
    g_macro_table->install("gt?",       tilton_gt_);
    g_macro_table->install("include",   tilton_include);
    g_macro_table->install("last",      tilton_last);
    g_macro_table->install("le?",       tilton_le_);
    g_macro_table->install("length",    tilton_length);
    g_macro_table->install("literal",   tilton_literal);
    g_macro_table->install("loop",      tilton_loop);
    g_macro_table->install("lt?",       tilton_lt_);
    g_macro_table->install("mod",       tilton_mod);
    g_macro_table->install("mult",      tilton_mult);
    g_macro_table->install("mute",      tilton_mute);
    g_macro_table->install("ne?",       tilton_ne_);
    g_macro_table->install("null",      tilton_null);
    g_macro_table->install("number?",   tilton_number_);
    g_macro_table->install("or",        tilton_or);
    g_macro_table->install("print",     tilton_print);
    g_macro_table->install("read",      tilton_read);
    g_macro_table->install("rep",       tilton_rep);
    g_macro_table->install("set",       tilton_set);
    g_macro_table->install("slashify",  tilton_slashify);
    g_macro_table->install("stop",      tilton_stop);
    g_macro_table->install("sub",       tilton_sub);
    g_macro_table->install("substr",    tilton_substr);
    g_macro_table->install("trim",      tilton_trim);
    g_macro_table->install("unicode",   tilton_unicode);
    g_macro_table->install("write",     tilton_write);
    g_macro_table->install("gt", ">");
    g_macro_table->install("lt", "<");
    g_macro_table->install("tilde", "~");
    g_macro_table->install("tilton", "0");
}


