// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

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

void Node::WriteNode() {
    if (text_) {
        fwrite(text_->string_, sizeof(char), text_->length_, stderr);
    }
    if (next_) {
        fputc('~', stderr);
        next_->WriteNode();
    }
}
