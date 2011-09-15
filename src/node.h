// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.


#ifndef SRC_NODE_H_
#define SRC_NODE_H_

#include "text.h"

class Text;

// Node -- represents items in a simple linked lists.

class Node {
 public:
  explicit Node(Text* t);
  virtual ~Node();

  // WriteNode
  // Recursively visit each node on the list and print the text
  void    WriteNode();

  Text*   text_;
  Text*   value_;
  Node*   next_;


  bool hasValue() {
    return this->value_ != 0;
  }
};

#endif  // SRC_NODE_H_
