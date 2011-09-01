//  tilton.cpp
//  2006-10-06

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

//  This file contains the built-in functions and command line options.
//  This file contains main.

#include <stdio.h>
#include <string.h>
#include "tilton.h"
#include "text.h"
#include "iter.h"
#include "context.h"
#include "node.h"

#include <iostream>
using namespace std ;

static void addFunction(const char*, void (* function)(Context *));
static void addMacro(const char* , const char* );
Text* lookup( Text* );

// MAXHASH is the largest index in the hash table. It must be (2**n)-1.

#define MAXHASH 1023

static Text* theMacroList[MAXHASH + 1];

Text* theOutput = NULL;

static number theSequenceNumber = 1000;


static void link(Text* name, Text* t) {
    uint32 h = name->hash() & MAXHASH;
    t->setName(name);
    t->link = theMacroList[h];
    theMacroList[h] = t;
}


//  addFunction is used by main to add primitive functions to tilton.
//  A function operates on a context which supplies the parameters. 

static void addFunction(const char* namestring, void (*function)(Context * )) {
    Text* t = new Text();
    Text* name = new Text(namestring);
    link(name, t);
    t->function = function;
    delete name;
}


//  addMacro -- This is a little faster than install() because it assumes that
//  the name is not already in the macro list.

static void addMacro(const char* namestring, const char* string) {
    Text* name = new Text(namestring);
    link(name, new Text(string));
    delete name;
}


//  lookup - search through the macro list for a text with a specific name.
//  The list is a hash table with links for collisions.

Text* lookup(Text* name) {
    Text* t = theMacroList[name->hash() & MAXHASH];
    while (t) {
        if (t->isName(name)) {
            break;
        }
        t = t->link;
    }
    return t;
}


//  install - if there is a text in the macro list with this name, set its
//  value. Otherwise, make a new text with this name and value and put
//  it in the list.

static void install(Text* name, Text* value) {
    Text* t = lookup(name);
    if (t) {
        t->set(value);
    } else {
        link(name, new Text(value));
    }
}

// reduce - reduce a list of parameters to a single value. This is used
// to implement the tilton arithmetic functions.

static void reduce(Context* context, number num, number (*f)(number, number))
{
    Node* n = context->node->next;
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
    Node* s = context->node->next;
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
    Node* n = context->node->next;
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
    Node* n = context->node->next;
    if (n == NULL) {
        context->error("Missing name");
    }
    Text* name = context->evalArg(n);
    if (name->length < 1) {
        context->error("Missing name");
    }
    Text* t = lookup(name);
    if (!t) {
        t = new Text(0);
        uint32 h = name->hash() & MAXHASH;
        t->setName(name);
        t->link = theMacroList[h];
        theMacroList[h] = t;
    }
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
    install(name, theOutput->tail(position));
}


//  tilton defined?

static void tilton_defined_(Context* context) {
    theOutput->append(context->evalArg(lookup(context->evalArg(1)) ? 2 : 3));
}


//  tilton delete

static void tilton_delete(Context* context) {
    Node* n = context->node->next;
    while (n) {
        Text* name = context->evalArg(n);
        Text* t = theMacroList[name->hash() & MAXHASH];
        Text* u = NULL;
        while (t) {
            if (t->isName(name)) {
                if (u) {
                    u->link = t->link;
                } else {
                    theMacroList[name->hash() & MAXHASH] = t->link;
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
    int i;
    for (i = 0; i < (MAXHASH + 1); i += 1) {
        Text* macro = theMacroList[i];
        if (macro) {
            macro->dump();
        }
    }
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
    Node* n = context->node->next;
    Text* name = context->evalArg(n);
    if (name->length < 1) {
        context->error("Missing name: first");
    }
    Text* string = lookup(name);
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
    Node* n = context->node->next;
    while (n) {
        Text* name = context->evalArg(n);
        Text* macro = lookup(name);
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
    // Node* n = context->node->next;
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
    Node* n = context->node->next;
    Text* name = context->evalArg(1);
    if (name->length < 1) {
        context->error("Missing name");
    }
    Text* string = lookup(name);
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
    // Node* n = context->node->next;
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
    Node* n = context->node->next;
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
    Node* n = context->node->next;
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
    // Node* n = context->node->next;
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
    install(name, context->evalArg(2));
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
    Node* n = context->node->next;
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
    Node* n = context->node->next;
    while (n) {
        theOutput->trim(context->evalArg(n));
        n = n->next;
    }
}


//  tilton unicode

static void tilton_unicode(Context* context) {
    Node* n = context->node->next;
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


// main processes the command line arguments and evaluates the standard
// input.

int main(int argc, const char * argv[]) 
{
    bool go = true;
    Text* name = NULL;
    Text* string = NULL;
    const char* arg;
    int i = 0; // index of argv
    int j = 0; // index of context parameter
    Context* context = new Context(NULL, NULL);
    Text* in = new Text();
    theOutput = new Text(1024);
    for (i = MAXHASH; i >= 0; i -= 1) {
        theMacroList[i] = NULL;
    }
    addFunction("add",       tilton_add);
    addFunction("and",       tilton_and);
    addFunction("append",    tilton_append);
    addFunction("define",    tilton_define);
    addFunction("defined?",  tilton_defined_);
    addFunction("delete",    tilton_delete);
    addFunction("div",       tilton_div);
    addFunction("dump",      tilton_dump);
    addFunction("entityify", tilton_entityify);
    addFunction("eq?",       tilton_eq_);
    addFunction("eval",      tilton_eval);
    addFunction("first",     tilton_first);
    addFunction("ge?",       tilton_ge_);
    addFunction("gensym",    tilton_gensym);
    addFunction("get",       tilton_get);
    addFunction("gt?",       tilton_gt_);
    addFunction("include",   tilton_include);
    addFunction("last",      tilton_last);
    addFunction("le?",       tilton_le_);
    addFunction("length",    tilton_length);
    addFunction("literal",   tilton_literal);
    addFunction("loop",      tilton_loop);
    addFunction("lt?",       tilton_lt_);
    addFunction("mod",       tilton_mod);
    addFunction("mult",      tilton_mult);
    addFunction("mute",      tilton_mute);
    addFunction("ne?",       tilton_ne_);
    addFunction("null",      tilton_null);
    addFunction("number?",   tilton_number_);
    addFunction("or",        tilton_or);
    addFunction("print",     tilton_print);
    addFunction("read",      tilton_read);
    addFunction("rep",       tilton_rep);
    addFunction("set",       tilton_set);
    addFunction("slashify",  tilton_slashify);
    addFunction("stop",      tilton_stop);
    addFunction("sub",       tilton_sub);
    addFunction("substr",    tilton_substr);
    addFunction("trim",      tilton_trim);
    addFunction("unicode",   tilton_unicode);
    addFunction("write",     tilton_write);
    addMacro("gt", ">");
    addMacro("lt", "<");
    addMacro("tilde", "~");
    addMacro("tilton", "0");

// process the command line arguments

//    cout << "MAIN: Processing args" << endl ;
    
    i = 0;
    while (i < argc) {
        arg = argv[i];
        i += 1;

// if an arg starts with '-', then it is special, Otherwise, stuff it into 
// the next slot in the context. We match just the first letter of commands.

        if (arg[0] == '-') {
            switch (arg[1]) {

// -eval expression
            case 'e': 
                if (i < argc) {
                    string = new Text(argv[i]);
                    i += 1;
                    string->setName("[eval]");
                    context->eval(string);
                    delete string;
                } else {
                    context->error("Missing expression on -eval");
                }
                break;

// -go (process the standard input now)
            case 'g': 
                in->input();
                in->setName("[go]");
                context->eval(in);
                go = false;
                break;

// -help
            case 'h':
                printf("  tilton command line parameters:\n"
                    "    -eval <tilton expression>\n"
                    "    -go\n"
                    "    -help\n"
                    "    -include <filespec>\n"
                    "    -mute\n"
                    "    -no\n"
                    "    -read <filespec>\n"
                    "    -set <name> <value>\n"
                    "    -write <filespec>\n"
                    "    -<digit>\n"
                    "  http://www.crockford.com/tilton");
                return 0;

// -include filespec
            case 'i': 
                if (i < argc) {
                    name = new Text(argv[i]);
                    i += 1;
                    string = new Text();
                    if (!string->read(name)) {
                        context->error("Error in -include", name);
                    }
                    context->eval(string);
                    delete name;
                    delete string;
                } else {
                    context->error("Missing filename on -include");
                }
                break;

// -mute
            case 'm': 
                theOutput->length = 0;
                break;

// -no (do not process the standard input)
            case 'n': 
                go = false;
                break;

// -read filespec
            case 'r': 
                if (i < argc) {
                    name = new Text(argv[i]);
                    i += 1;
                    string = new Text();
                    if (!string->read(name)) {
                        context->error("Error in -read", name);
                    }
                    theOutput->append(string);
                    delete name;
                    delete string;
                } else {
                    context->error("Missing filename on -read");
                }
                break;

// -set name string
            case 's': 
                if (i + 1 < argc) {
                    name = new Text(argv[i]);
                    i += 1;
                    string = new Text(argv[i]);
                    i += 1;
                    install(name, string);
                    delete name;
                    delete string;
                } else {
                    context->error("Missing parameter on -set ");

                }
                break;

// -write filespec
            case 'w': 
                if (i < argc) {
                    name = new Text(argv[i]);
                    i += 1;
                    if (!theOutput->write(name)) {
                        context->error("Error in -write", name);
                    }
                    theOutput->length = 0;
                    delete name;
                } else {
                    context->error("Missing filename on -write");
                }
                break;

// -DIGIT
            default: 
                int k = arg[1] - '0';
                if (k >= 0 && k <= 9 && i < argc) {
                    j = k;
                } else {

// none of the above
                    context->error("Unrecognized command line parameter", 
                                   new Text(arg));
                }
            }
        } else {

// parameter text
            context->getNode(j)->value = new Text(arg);
            j += 1;
        }
    }

// Process the input

    if (go) {
//        cout << "MAIN: reading stdin" << endl ;
        in->input();
        in->setName("[standard input]");
//        cout << "MAIN: Starting eval" << endl ;
        context->eval(in);
    }

// and finally

//    cout << "MAIN: Writing output" << endl ;
    theOutput->output();
    return 0;
}
