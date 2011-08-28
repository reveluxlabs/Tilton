//  context.h: interface for the context class.
//  2006-10-05

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

#ifndef __CONTEXT_H_
#define __CONTEXT_H_

#include "tilton.h"

class Iter;
class Text;
class Node;

class Context {
public:
    Context(Context*, Iter*);
    virtual ~Context();

    void    add(char* s);
    void    add(Text*);
    void    dump();
    void    error(char* reason, Text* evidence);
    void    error(char* reason);
    void    eval(Text* input);
    Text*   evalArg(int argNr);
    Text*   evalArg(Node*); 
    number  evalNumber(int argNr);
    number  evalNumber(Node*);
    Node*   getNode(int argNr);
    void    nop();
    void    resetArg(int argNr);

    Node*   node;
    Context* previous;

private:
    int     character;
    int     index;
    int     line;
    int     position;
    Iter*   source;
    Node*   last;
    void    whereError(Text* report);
};

#endif // __CONTEXT_H_
