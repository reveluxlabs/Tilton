//  node.h: interface for the node class.
//  2006-10-05

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

#ifndef __NODE_H_
#define __NODE_H_

#include "text.h"

class Text;

class Node {
public:
    Node(Text*);
    virtual ~Node();

    void    dump();

    Text*   text;
    Text*   value;
    Node*   node;
};

#endif // __NODE_H_
