//  node.cpp
//  2006-10-05

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

//  Node is used to make simple linked lists.

//  A node has three fields: text, value, and next.


#include <stdio.h>
#include "node.h"

Node::Node(Text* t)
{
    text = t;
    value = NULL;
    node = NULL;
}

Node::~Node(void)
{
    delete this->text;
    delete this->value;
    delete this->node;
}

void Node::dump() 
{
    if (text) {
        fwrite(text->string, sizeof(char), text->length, stderr);
    }
    if (node) {
        fputc('~', stderr);
        node->dump();
    }
}