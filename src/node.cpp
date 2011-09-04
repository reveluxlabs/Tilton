// node.cpp
// For interface definitions, see node.h

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

//  Node is used to make simple linked lists.

//  A node has three fields: text, value, and next.

#include "node.h"

#include <stdio.h>

Node::Node(Text* t) {
    text_ = t;
    value_ = NULL;
    next_ = NULL;
}

Node::~Node(void) {
    delete this->text_;
    delete this->value_;
    delete this->next_;
}

void Node::dump() {
    if (text_) {
        fwrite(text_->string_, sizeof(char), text_->length_, stderr);
    }
    if (next_) {
        fputc('~', stderr);
        next_->dump();
    }
}
