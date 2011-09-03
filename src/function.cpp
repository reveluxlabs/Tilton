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
// This version of Tilton is licensed under the MIT license.

#include <stdlib.h>
#include "tilton.h"
#include "function.h"
#include "node.h"
#include "context.h"

extern Text* theOutput;
extern SearchList* macroTable;
static number theSequenceNumber = 1000;

// reduce - reduce a list of parameters to a single value. This is used
// to implement the tilton arithmetic functions.

static void reduce(Context* context, number num, number (*f)(number, number))
{
    Node* n = context->first->next;
    if (n) {
        if (num == NAN) {
            num = context->evalNumber(n);
            n = n->next;
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
                n = n->next;
            }
        }
    }
    theOutput->appendNumber(num);
}

static number add(number first, number second)
{
    return first + second;
}

static number sub(number first, number second)
{
    return first - second;
}

static number mult(number first, number second)
{
    return first * second;
}

static number div(number first, number second)
{
    return second ? first / second : NAN;
}

static number mod(number first, number second)
{
    return second ? first % second : NAN;
}

//  test - This is used to implement the tilton binary conditional functions.

static void test(Context* context, int (*f)(Text*, Text*)) 
{
    Node* c;
    Node* t;
    Node* s = context->first->next;
    if (!s) {
        context->error("No parameters");
        return;
    }
    Text* swich = context->evalArg(s);
    c = s->next;
    if (!c) {
        context->error("Too few parameters");
    }
    t = c->next;
    if (!t) {
        context->error("Too few parameters");
    }
    for (;;) {
        if (f(swich, context->evalArg(c))) { // then
            theOutput->append(context->evalArg(t));
            return;
        }
        c = t->next;
        if (!c) {
            return;  // empty else
        }
        t = c->next;
        if (!t) {    // else
            theOutput->append(context->evalArg(c));
            return;
        }
    } 
}    

static int eq(Text* first, Text* second)
{
    return first->is(second);
}


static int ge(Text* first, Text* second)
{
    return !(first->lt(second));
}

static int gt(Text* first, Text* second)
{
    return second->lt(first);
}

static int le(Text* first, Text* second)
{
    return !(second->lt(first));
}

static int lt(Text* first, Text* second)
{
    return first->lt(second);
}

static int ne(Text* first, Text* second)
{
    return !(first->is(second));
}


//  By convention, the names of built in functions have a 'tilton_' prefix.

//  tilton add

static void tilton_add(Context* context) {
    reduce(context, 0, add);
}


//  tilton and

static void tilton_and(Context* context) {
    Node* n = context->first->next;
    Text* t = NULL;
    while (n) {
        t = context->evalArg(n);
        if (t->length == 0) {
            return;
        } 
        n = n->next;
    }
    theOutput->append(t);
}


//  tilton append

static void tilton_append(Context* context) {
    Node* n = context->first->next;
    if (n == NULL) {
        context->error("Missing name");
    }
    Text* name = context->evalArg(n);
    if (name->length < 1) {
        context->error("Missing name");
    }
    
    Text* t = macroTable->getDef(name);
    
    for (;;) {
        n = n->next;
        if (!n) {
            break;
        }
        t->append(context->evalArg(n));
    }
}


//  tilton define

static void tilton_define(Context* context) {
    int position = theOutput->length;
    theOutput->append(context->getNode(2)->text);
    Text* name = context->evalArg(1);
    if (name->length < 1) {
        context->error("Missing name");
    }
    macroTable->install(name, theOutput->tail(position));
}


//  tilton defined?

static void tilton_defined_(Context* context) {
    theOutput->append(context->evalArg(macroTable->lookup(context->evalArg(1)) ? 2 : 3));
}


//  tilton delete

static void tilton_delete(Context* context) {
    Node* n = context->first->next;
    while (n) {
        Text* name = context->evalArg(n);
        // Text* t = macroTable->getList(name->hash() & MAXHASH);
        Text* t = macroTable->lookup(name);
        Text* u = NULL;
        while (t) {
            if (t->isName(name)) {
                if (u) {
                    u->link = t->link;
                } else {
                    macroTable->install(name, t->link);
                    // macroTable->setList(name->hash() & MAXHASH, t->link);
                }
                delete t;
                break;
            }
            u = t;
            t = t->link;
        }
        n = n->next;
    }
}


//  tilton div

static void tilton_div(Context* context) {
    reduce(context, NAN, div);
}


//  tilton dump

static void tilton_dump(Context* context) {
    macroTable->dump();
    context->nop();
}


//  tilton entityify - outputs a string in which selected characters have
//  been replaced with their HTML entity equivalents.

static void tilton_entityify(Context* context) {
    Text* t = context->evalArg(1);
    int c;
    int i;
    if (t && t->length) {
        for (i = 0; i < t->length; i += 1) {
            c = t->getChar(i);
            switch (c) {
                case '&':
                    theOutput->append("&amp;");
                    break;
                case '<':
                    theOutput->append("&lt;");
                    break;
                case '>':
                    theOutput->append("&gt;");
                    break;
                case '"':
                    theOutput->append("&quot;");
                    break;
                case '\'':
                    theOutput->append("&#039;");
                    break;
                case '\\':
                    theOutput->append("&#092;");
                    break;
                case '~':
                    theOutput->append("&#126;");
                    break;
                default:
                    theOutput->append(c);
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
    Context* newContext = new Context(context, NULL);
    newContext->add("eval");
    newContext->add("<~2~>");
    newContext->add("<~3~>");
    newContext->add("<~4~>");
    newContext->add("<~5~>");
    newContext->add("<~6~>");
    newContext->add("<~7~>");
    newContext->add("<~8~>");
    newContext->eval(context->getNode(1)->text);
    delete newContext;
}


//  tilton first

static void tilton_first(Context* context) {
    Node* n = context->first->next;
    Text* name = context->evalArg(n);
    if (name->length < 1) {
        context->error("Missing name: first");
    }
    Text* string = macroTable->lookup(name);
    if (!string) {
        context->error("Undefined variable", name);
        return;
    }
    Text* d = NULL;
    int   len = 0;
    int   r = string->length;
    for (;;) {
        n = n->next;
        if (!n) {
            break;
        }
        int index = string->indexOf(context->evalArg(n));
        if (index >= 0 && index < r) {
            r = index;
            d = context->evalArg(n);
            len = d->length;
        }
    }
    theOutput->append(string->string, r);
    string->substr(r + len, string->length - (r + len));
    n = context->previous->getNode(0);
    delete n->text;
    n->text = NULL;
    delete n->value;
    n->value = d ? new Text(d) : NULL;
}


//  tilton ge?

static void tilton_ge_(Context* context) {
    test(context, ge);
}


//  tilton gensym

static void tilton_gensym(Context* context) {
    theSequenceNumber += 1;
    theOutput->appendNumber(theSequenceNumber);
    context->nop();
}


//  tilton get

static void tilton_get(Context* context) {
    Node* n = context->first->next;
    while (n) {
        Text* name = context->evalArg(n);
        Text* macro = macroTable->lookup(name);
        if (macro) {
            theOutput->append(macro);
        } else {
            context->error("Undefined variable", name);
        }
        n = n->next;
    }
}


//  tilton gt?

static void tilton_gt_(Context* context) {
    test(context, gt);
}


//  tilton include

static void tilton_include(Context* context) {
    // Node* n = context->first->next;
    Text* string = new Text();
    Text* name = context->evalArg(1);
    if (!string->read(name)) {
        context->error("Error in reading file", name);
    }
    Context* newContext = new Context(context, NULL);
    newContext->add("include");
    newContext->add("<~2~>");
    newContext->add("<~3~>");
    newContext->add("<~4~>");
    newContext->add("<~5~>");
    newContext->add("<~6~>");
    newContext->add("<~7~>");
    newContext->add("<~8~>");
    newContext->eval(string);
    delete newContext;
    delete string;
}


//  tilton last

static void tilton_last(Context* context) {
    Node* n = context->first->next;
    Text* name = context->evalArg(1);
    if (name->length < 1) {
        context->error("Missing name");
    }
    Text* string = macroTable->lookup(name);
    if (!string) {
        context->error("Undefined variable", name);
        return;
    }
    Text* d = NULL;
    int   len = 0;
    int   r = 0;
    for (;;) {
        n = n->next;
        if (!n) {
            break;
        }
        int index = string->lastIndexOf(context->evalArg(n));
        if (index > r) {
            r = index;
            d = context->evalArg(n);
            len = d->length;
        }
    }
    theOutput->append(string->string + r + len, 
                      string->length - (r + len));
    string->length = r;
    n = context->previous->getNode(0);
    delete n->text;
    n->text = NULL;
    delete n->value;
    n->value = d ? new Text(d) : NULL;
}


//  tilton le?

static void tilton_le_(Context* context) {
    test(context, le);
}


//  tilton length

static void tilton_length(Context* context) {
    theOutput->appendNumber(context->evalArg(1)->utfLength());
}


//  tilton literal

static void tilton_literal(Context* context) {
    theOutput->append(context->getNode(1)->text);
}


//  tilton loop - eval the second parameter as long as the first parameter 
//  is not null.

static void tilton_loop(Context* context) {
    // Node* n = context->first->next;
    while (context->evalArg(1)->length > 0) {
        context->resetArg(1); 
        context->resetArg(2);
        theOutput->append(context->evalArg(2));
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
    Node* n = context->first->next;
    while (n) {
        context->evalArg(n);
        n = n->next;
    }
}


//  tilton ne?

static void tilton_ne_(Context* context) {
    test(context, ne);
}


//  tilton null - ignore the parameters. This can also be used for comments.

static void tilton_null(Context* context) {
    context->nop();
}


//  tilton number?

static void tilton_number_(Context* context) {
    number num = context->evalArg(1)->getNumber();
    theOutput->append(context->evalArg(num != NAN ? 2 : 3));
}


//  tilton or

static void tilton_or(Context* context) {
    Node* n = context->first->next;
    Text* t = NULL;
    while (n) {
        t = context->evalArg(n);
        if (t->length) {
            break;
        } 
        n = n->next;
    }
    theOutput->append(t);
}


//  tilton print

static void tilton_print(Context* context) {
    context->dump();
}


//  tilton read

static void tilton_read(Context* context) {
    Text* string = new Text();
    Text* name = context->evalArg(1);
    if (!string->read(name)) {
        context->error("Error in reading file", name);
    }
    theOutput->append(string);
    delete string;
}


// tilton rep

static void tilton_rep(Context* context) {
    // Node* n = context->first->next;
    Text* value = context->evalArg(1);
    for (number num = context->evalNumber(2); num > 0; num -= 1) {
        theOutput->append(value);
    }
}

//  tilton set

static void tilton_set(Context* context) {
    Text* name = context->evalArg(1);
    if (name->length < 1) {
        context->error("Missing name");
    }
    macroTable->install(name, context->evalArg(2));
}


//  tilton slashify outputs a string in which selected 
//  characters get a \ prefix.

static void tilton_slashify(Context* context) {
    Text* t = context->evalArg(1);
    int c;
    int i;
    if (t && t->length) {
        for (i = 0; i < t->length; i += 1) {
            c = t->getChar(i);
            switch (c) {
                case '\\': // backslash
                case '\'': // single quote
                case '"':  // double quote
                    theOutput->append('\\');
                    break;
            }
            theOutput->append(c);
        }
    }
}


//  tilton stop

static void tilton_stop(Context* context) {
    context->error("Stop", context->evalArg(1));
}


//  tilton sub

static void tilton_sub(Context* context) {
    reduce(context, NAN, sub);
}


//  tilton substr

static void tilton_substr(Context* context) {
    Node* n = context->first->next;
    Text* string = context->evalArg(n);
    n = n->next;
    if (n) {
        number num = context->evalNumber(n);
        if (num < 0) {
            num += string->length;
        }
        number ber = INFINITY;
        n = n->next;
        if (n) {
            ber = context->evalNumber(n);
        }
        if (num >= 0 && ber > 0) {
            theOutput->append(context->evalArg(1)->utfSubstr((int)num, 
                                                             (int)ber));
        }
    }
}


//  tilton trim

static void tilton_trim(Context* context) {
    Node* n = context->first->next;
    while (n) {
        theOutput->trim(context->evalArg(n));
        n = n->next;
    }
}


//  tilton unicode

static void tilton_unicode(Context* context) {
    Node* n = context->first->next;
    while (n) {
        number num = context->evalNumber(n);
        if (num >= 0) {
            int i = (int)num;
            if (i <= 0x7F) {
                theOutput->append(i);
            } else if (i <= 0x7FF) {
                theOutput->append(0xC000 |  (i >> 6));
                theOutput->append(0x8000 |  (i        & 0x3F));
            } else if (i <= 0xFFFF) {
                theOutput->append(0xE000 |  (i >> 12));
                theOutput->append(0x8000 | ((i >> 6)  & 0x3F));
                theOutput->append(0x8000 |  (i        & 0x3F));
            } else {
                theOutput->append(0xF000 |  (i >> 18));
                theOutput->append(0x8000 | ((i >> 12) & 0x3F));
                theOutput->append(0x8000 | ((i >> 6)  & 0x3F));
                theOutput->append(0x8000 |  (i        & 0x3F));
            }
        } else {
            context->error("Bad character code", context->evalArg(n));
            return;
        }
        n = n->next;
    }
}


//  tilton write

static void tilton_write(Context* context) {
    Text* name = context->evalArg(1);
    if (!context->evalArg(2)->write(name)) {
        context->error("Error in writing file", name);
    }
}


// jr 1Sep11 (based on code by Douglas Crockford)
FunctionContext::FunctionContext() {
    // No further initialization required.
}

// jr 1Sep11 (based on code by Douglas Crockford)
FunctionContext::~FunctionContext() {
    // No further deallocation required.
}

// jr 1Sep11 (based on code by Douglas Crockford)
void FunctionContext::registerTiltonFunctions(SearchList* macroTable)
{

    macroTable->install("add",       tilton_add);
    macroTable->install("and",       tilton_and);
    macroTable->install("append",    tilton_append);
    macroTable->install("define",    tilton_define);
    macroTable->install("defined?",  tilton_defined_);
    macroTable->install("delete",    tilton_delete);
    macroTable->install("div",       tilton_div);
    macroTable->install("dump",      tilton_dump);
    macroTable->install("entityify", tilton_entityify);
    macroTable->install("eq?",       tilton_eq_);
    macroTable->install("eval",      tilton_eval);
    macroTable->install("first",     tilton_first);
    macroTable->install("ge?",       tilton_ge_);
    macroTable->install("gensym",    tilton_gensym);
    macroTable->install("get",       tilton_get);
    macroTable->install("gt?",       tilton_gt_);;
    macroTable->install("include",   tilton_include);
    macroTable->install("last",      tilton_last);
    macroTable->install("le?",       tilton_le_);
    macroTable->install("length",    tilton_length);
    macroTable->install("literal",   tilton_literal);
    macroTable->install("loop",      tilton_loop);
    macroTable->install("lt?",       tilton_lt_);
    macroTable->install("mod",       tilton_mod);
    macroTable->install("mult",      tilton_mult);
    macroTable->install("mute",      tilton_mute);
    macroTable->install("ne?",       tilton_ne_);
    macroTable->install("null",      tilton_null);
    macroTable->install("number?",   tilton_number_);
    macroTable->install("or",        tilton_or);
    macroTable->install("print",     tilton_print);
    macroTable->install("read",      tilton_read);
    macroTable->install("rep",       tilton_rep);
    macroTable->install("set",       tilton_set);
    macroTable->install("slashify",  tilton_slashify);
    macroTable->install("stop",      tilton_stop);
    macroTable->install("sub",       tilton_sub);
    macroTable->install("substr",    tilton_substr);
    macroTable->install("trim",      tilton_trim);
    macroTable->install("unicode",   tilton_unicode);
    macroTable->install("write",     tilton_write);
    macroTable->install("gt", ">");
    macroTable->install("lt", "<");
    macroTable->install("tilde", "~");
    macroTable->install("tilton", "0");
    
}


