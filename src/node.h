// node.h: interface for the node class.

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
    Node*   next;

    
    bool hasValue()
    {
      return this->value != 0;
    }
};

#endif // __NODE_H_
