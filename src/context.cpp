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
// This version of Tilton is licensed under the MIT license.

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
#include "search.h"

extern Text* theOutput;
extern SearchList* macroTable;


Context::Context(Context* prev, Iter* s) {
    position = 0;
    first = NULL;
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
    delete this->first;
}


// add a c-string to a context

void Context::add(const char* s) {
    add(new Text(s));
}

//  add an argument to a context

void Context::add(Text* t) {
    Node* p = new Node(t);
    if (last) {
        last->next = p;
    } else {
        first = p;
    }
    last = p;
}


void Context::dump() {
    if (first) {
        first->dump();
    }
    fputc('\n', stderr);
}


// Write a message to stderr and then exit.

void Context::error(const char* reason) {
    error(reason, NULL);
}


void Context::error(const char* reason, Text* evidence) {
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
  int c;                 // current character
  int depth = 0;         // depth of nested <~ ~>
  int tildesSeen= 0;     // the number of tildes in the separator ~~~
  Context* newContext = NULL;
  Iter* in = new Iter(input);

// Loop over the characters in the input

  for (;;) {
    switch ((c = in->next())) {

    case '<':
        this->evalAngle(in, depth, theOutput, tildesSeen, newContext);
        break;

    case '~':
        this->evalTilde(in, depth, theOutput, tildesSeen, newContext);
        break;

    case EOT:
        this->evalEOT(in, depth, theOutput, tildesSeen, newContext);
        return;

// literal character
    default:
        theOutput->addToString(c);
        break;
    }
  }
}

void Context::evalAngle(Iter* in, int &depth, Text* theOutput, 
                        int &tildesSeen, Context* &newContext) {
  int runLength;     // the number of tildes currently under consideration
  
  runLength = checkForTilde(in, 0);
  
  if (!stackEmpty(depth)) {
    if (runLength) {
        depth += 1;
    } 
    theOutput->addToString('<');
    theOutput->addToString('~', runLength);
  } else if (runLength) {
    depth = 1;
    tildesSeen = runLength;
    newContext = new Context(this, in);
    newContext->position = theOutput->length;
  } else {
    theOutput->addToString('<');
  }
}

void Context::evalTilde(Iter* in, int &depth, Text* theOutput, 
                        int &tildesSeen, Context* &newContext){
  int argNo;         // argument number
  int c;             // current character
  int i;             // loop counter
  Node* n;           // current node
  Text* s;           // current text
  int runLength;     // the number of tildes currently under consideration

  runLength = checkForTilde(in, 1);
  
  if (depth == 1 && runLength >= tildesSeen) {
    newContext->add(theOutput->removeFromString(newContext->position));
    for (;;) {
      runLength -= tildesSeen;
      if (runLength < tildesSeen) break;
      newContext->add(new Text());
    }
  }
  
  theOutput->addToString('~', runLength);
  
  if (in->peek() == '>') {
      //  ~>
      in->next();
      if (!stackEmpty(depth)) {
          depth -= 1;
          if (!stackEmpty(depth)) {
              theOutput->addToString('>');
          } else {
              if (runLength) {
                  newContext->error("Short ~>");
              }
              //    apply:           
              n = newContext->first;
              //    <~NUMBER~>
              s = n->text;
              argNo = s->getChar(0) - '0';
              if (isDigit(argNo)) {
                  for (i = 1; i < s->length; i += 1) {
                      c = s->getChar(0) - '0';
                      if (c < 0 || c > 9) {
                          argNo = -1;
                          break;
                      }
                      argNo = argNo * 10 + c;
                  }
                  if (argNo >= 0) {
                      if (!n->next) { 
                          theOutput->addToString(this->evalArg(argNo));
                      } else {
                          //    <~NUMBER~value~>
                          n = newContext->getNode(argNo);
                          if (!n->hasValue()) Context::evalTextForArg(1, newContext, theOutput);
                          Context::setMacroVariable(argNo, n->value);
                      }
                      delete newContext;
                      return;
                  }
              }
              //    look up
              Context::evalMacro(newContext);
          }
      } else {
          (new Context(this, in))->error("Extra ~>");
      }
  }    
}

void Context::evalTextForArg(int argNo, Context* &newContext, Text* &theOutput)
{
  Node* n;

  n = newContext->getNode(argNo);
  this->eval(n->text);
  n->value = theOutput->removeFromString(newContext->position);
}

void Context::setMacroVariable(int varNo, Text* t)
{
  Node* o;

  o = this->getNode(varNo);
  delete o->text;
  o->text = NULL;
  delete o->value;
  // o->value[varNo] = new Text(t);
  o->value = new Text(t);
}

void Context::evalMacro(Context* &newContext)
{
    Text* macro;
    Text* name;
    
    name = newContext->evalArg(0);
    macro = macroTable->lookup(name);
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

void Context::evalEOT(Iter* in, int &depth, Text* theOutput, int &tildesSeen, Context* &newContext)
{
    if (depth) {
        newContext->error("Missing ~>");
    }
    delete in;
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
        n->value = theOutput->removeFromString(position);
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
    Node* p;
    if (!first) {
        first = last = new Node(NULL);
    }
    p = first;
    for (;;) {
        if (!argNr) {
            return p;
        }
        p = p->next;
        if (!p) {
            p = new Node(NULL);
            last->next = p;  // fixed <~1~> bug -- joh 8/17/11
            last = p;
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
    if (first->text && first->text->length) {
        report->append("<~");
        report->append(first->text);
        report->append("~> ");
    }
}
