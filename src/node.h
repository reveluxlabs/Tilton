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
// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
//
// This version of Tilton is licensed under the MIT license.

#ifndef SRC_NODE_H_
#define SRC_NODE_H_

#include "text.h"

class Text;

class Node {
 public:
    explicit Node(Text* t);
    virtual ~Node();

    void    dump();

    Text*   text;
    Text*   value;
    Node*   next;


    bool hasValue() {
      return this->value != 0;
    }
};

#endif  // SRC_NODE_H_
