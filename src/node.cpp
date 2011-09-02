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
// This version of Tilton is licensed under the MIT license.

//  Node is used to make simple linked lists.

//  A node has three fields: text, value, and next.


#include <stdio.h>
#include "node.h"

Node::Node(Text* t)
{
    text = t;
    value = NULL;
    next = NULL;
}

Node::~Node(void)
{
    delete this->text;
    delete this->value;
    delete this->next;
}

void Node::dump() 
{
    if (text) {
        fwrite(text->string, sizeof(char), text->length, stderr);
    }
    if (next) {
        fputc('~', stderr);
        next->dump();
    }
}