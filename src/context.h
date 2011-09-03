//  context.h: interface for the context class.

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

#ifndef SRC_CONTEXT_H_
#define SRC_CONTEXT_H_

#include "tilton.h"
#include "iter.h"

class Iter;
class Text;
class Node;

class Context {
 public:
    Context(Context*, Iter*);
    virtual ~Context();

    void    add(const char* s);
    void    add(Text* t);
    void    dump();
    void    error(const char* reason, Text* evidence);
    void    error(const char* reason);
    void    eval(Text* input);
    Text*   evalArg(int argNr);
    Text*   evalArg(Node* n);
    number  evalNumber(int argNr);
    number  evalNumber(Node* n);
    Node*   getNode(int argNr);
    void    nop();
    void    resetArg(int argNr);

    Node*   first;
    Context* previous;

 private:
    int     character;
    int     index;
    int     line;
    int     position;
    Iter*   source;
    Node*   last;
    void    whereError(Text* report);
    void    evalAngle(Iter* in, int &depth, Text* theOutput, int &tildesSeen,
                      Context* &newContext);
    void    evalTilde(Iter* in, int &depth, Text* theOutput, int &tildesSeen,
                      Context* &newContext);
    void    evalEOT(Iter* in, int &depth, Text* theOutput, int &tildesSeen,
                      Context* &newContext);
    void    evalMacro(Context* &newContext);
    bool    isDigit(int argNo) {
        return argNo >= 0 && argNo <= 9;
    }
    bool    stackEmpty(int depth) {
        return depth == 0;
    }
    int     checkForTilde(Iter* in, int no) {
        while (in->next() == '~') {
            no += 1;
        }
        in->back();
        return no;
    }
    void     evalTextForArg(int argNo, Context* &newContext, Text* &theOutput);
    void     setMacroVariable(int varNo, Text* t);
};

#endif  // SRC_CONTEXT_H_
