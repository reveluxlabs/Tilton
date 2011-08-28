//  context.cpp
//  2006-10-06

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

//  Context is the key datastructure in Tilton. It keeps a collection of 
//  parameters in numbered slots. We keep in each slot a raw string
//  and an evaluated value (for memoization).

//  A Context can point to a previous context, which allows contexts to be 
//  nested. A Context can also include source information for use in error 
//  messages.

#include <stdlib.h>
#include <stdio.h>
#include "tilton.h"
#include "text.h"
#include "iter.h"
#include "context.h"
#include "node.h"

extern Text* theOutput;
extern Text* find(Text* name);


Context::Context(Context* prev, Iter* s) {
    position = 0;
    node = NULL;
    last = NULL;
    previous = prev;
    source = s;
    if (s) {
        line = s->line;
        character = s->character;
        index = s->index;
    } else {
        line = 0;
        character = 0;
        index = 0;
    }
}


Context::~Context() {
    delete this->node;
}


// add a c-string to a context

void Context::add(char* s) {
    add(new Text(s));
}

//  add an argument to a context

void Context::add(Text* t) {
    Node* n = new Node(t);
    if (last) {
        last->node = n;
    } else {
        node = n;
    }
    last = n;
}


void Context::dump() {
    if (node) {
        node->dump();
    }
    fputc('\n', stderr);
}


// Write a message to stderr and then exit.

void Context::error(char* reason) {
    error(reason, NULL);
}


void Context::error(char* reason, Text* evidence) {
    Text* report = new Text(80);
    whereError(report);
    report->append(reason);
    if (evidence) {
        report->append(": ");
        report->append(evidence);
    }
    report->append(".\n");
    fwrite(report->string, sizeof(char), report->length, stdout);
    fwrite(report->string, sizeof(char), report->length, stderr);
    exit(1);
}


//  eval is the heart of Tilton. It is called on a context which is the 
//  container of the source of <~NUMBER~> parameters. It scans its input text 
//  for <~ ~> patterns which it builds into new contexts and recursively 
//  evaluates. Characters outside of the <~ ~> are treated as literal. 
//  Within <~ ~>, substrings are produced, separated by ~ . These are stored 
//  in the new context as argument strings. Any nested <~ ~> sequences within 
//  the arguments are treated for now as literal (lazy evaluation). The [0] 
//  argument is the name of the macro to be invoked. 

//  Tilton is so simple that parsing and evaluation take place at the same 
//  time.

void Context::eval(Text* input) {
    int a;     // argument number
    int c;     // current character
    int d = 0; // depth of nested <~ ~>
    int i;     // loop counter
    Node* n;   // current node
    Node* o;   // another node
    Text* s;   // current text
    int t = 0; // the number of tildes in the separator ~~~
    int u;     // the number of tildes currently under consideration
    Text* macro;
    Text* name;
    Context* newContext = NULL;
    Iter* in = new Iter(input);

// Loop over the characters in the input

    for (;;) {
        switch ((c = in->next())) {

//  <~
        case '<':
            u = 0;
            while (in->next() == '~') {
                u += 1;
            }
            in->back();
            if (d) {
                if (u) {
                    d += 1;
                } 
                theOutput->append('<');
                theOutput->append('~', u);
            } else if (u) {
                d = 1;
                t = u;
                newContext = new Context(this, in);
                newContext->position = theOutput->length;
            } else {
                theOutput->append('<');
            }
            break;

//  ~
        case '~':
            for (u = 1; in->next() == '~'; u += 1) {
            }
            in->back();
            if (d == 1 && u >= t) {
                newContext->add(theOutput->tail(newContext->position));
                for (;;) {
                    u -= t;
                    if (u < t) {
                        break;
                    }
                    newContext->add(new Text());
                }
            }
            theOutput->append('~', u);
            if (in->peek() == '>') {
//  ~>
                in->next();
                if (d) {
                    d -= 1;
                    if (d) {
                        theOutput->append('>');
                    } else {
                        if (u) {
                            newContext->error("Short ~>");
                        }
//    apply:           
                        n = newContext->node;
//    <~NUMBER~>
                        s = n->text;
                        a = s->get(0) - '0';
                        if (a >= 0 && a <= 9) {
                            for (i = 1; i < s->length; i += 1) {
                                c = s->get(0) - '0';
                                if (c < 0 || c > 9) {
                                    a = -1;
                                    break;
                                }
                                a = a * 10 + c;
                            }
                            if (a >= 0) {
                                if (!n->node) { 
                                    theOutput->append(this->evalArg(a));
                                } else {
//    <~NUMBER~value~>
                                    n = newContext->getNode(a);
                                    if (!n->value) {
                                        n = newContext->getNode(1);
                                        this->eval(n->text);
                                        n->value = theOutput->tail(newContext->position);
                                    }
                                    o = this->getNode(a);
                                    delete o->text;
                                    o->text = NULL;
                                    delete o->value;
                                    o->value[a] = new Text(n->value);
                                }
                                delete newContext;
                                break;
                            }
                        }
//    look up
                        name = newContext->evalArg(0);
                        macro = find(name);
                        if (macro) {
                            if (macro->function) {
                                macro->function(newContext);
                            } else {
                                newContext->eval(macro);
                            }
                        } else {
//    undefined
                            newContext->error("Undefined macro");
                        }
                        delete newContext;
                        newContext = NULL;
                    }
                } else {
                    (new Context(this, in))->error("Extra ~>");
                }
            }
            break;

// end of text
        case EOT:
            if (d) {
                newContext->error("Missing ~>");
            }
            delete in;
            return;

// literal character
        default:
            theOutput->append(c);
            break;
        }
    }
}


//  evalArg - Get an argument of a macro. If we have already determined its 
//  value, then simply return it. Otherwise, evaluate the argument to obtain
//  its value (memoization).

Text* Context::evalArg(int argNr) 
{
    return evalArg(getNode(argNr));
}


Text* Context::evalArg(Node* n) 
{
    if (n == NULL) {
        return NULL;
    }
    if (n->value == NULL) {
        if (n->text == NULL) {
            return NULL;
        }
        Text* arg = n->text;
        int position = theOutput->length;
        this->previous->eval(arg);
        n->value = theOutput->tail(position);
    }
    return n->value;
}


number Context::evalNumber(int argNr) 
{
    return evalNumber(getNode(argNr));
}


number Context::evalNumber(Node* n) 
{
    if (!n) {
        return 0;
    }
    number num = evalArg(n)->getNumber();
    if (num == NAN) {
        error("Not a number", evalArg(n));
    }
    return num;
}


Node* Context::getNode(int argNr)
{
    Node* n;
    if (!node) {
        node = last = new Node(NULL);
    }
    n = node;
    for (;;) {
        if (!argNr) {
            return n;
        }
        n = n->node;
        if (!n) {
            n = new Node(NULL);
            last->node = n;  // fixed <~1~> bug -- joh 8/17/11
            last = n;
        }
        argNr -= 1;
    }
}


void Context::nop() {
}


//  resetArg - Delete the value of an argument of a macro. This allows for
//  evaluating an arg more than once. This is used by <~loop~>

void Context::resetArg(int argNr) {
    Node* n;
    n = this->getNode(argNr);
    delete n->value;
    n->value = NULL;
}


// Recurse through the context chain to identify the point of the error.

void Context::whereError(Text* report) {
    if (previous) {
        previous->whereError(report);
    }
    if (source) {
        report->append(source->text->name, source->text->nameLength);
        report->append('(');
        report->appendNumber(line + 1); 
        report->append(',');
        report->appendNumber(character + 1); 
        report->append('/');
        report->appendNumber(index + 1);
        report->append(") ");
    }
    if (node->text && node->text->length) {
        report->append("<~");
        report->append(node->text);
        report->append("~> ");
    }
}
