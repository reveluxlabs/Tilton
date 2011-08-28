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

    void    add(const char* s);
    void    add(Text*);
    void    dump();
    void    error(const char* reason, Text* evidence);
    void    error(const char* reason);
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
    void    evalAngle(Iter* in, int &depth, Text* theOutput, int &tildesSeen, Context* &newContext);
    void    evalTilde(Iter* in, int &depth, Text* theOutput, int &tildesSeen, Context* &newContext);    
    void    evalEOT(Iter* in, int &depth, Text* theOutput, int &tildesSeen, Context* &newContext);
    void evalMacro(Context* &newContext);
    bool isDigit(int argNo)
    {
        return argNo >= 0 && argNo <= 9;
    }
    bool stackEmpty(int depth)
    {
        return depth == 0;
    }
    int checkForTilde(Iter* in, int no)
    {
        while (in->next() == '~') {
            no += 1;
        }
        in->back();
        return no;
    }
};

#endif // __CONTEXT_H_
