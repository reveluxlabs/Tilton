// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.


#ifndef SRC_ITER_H_
#define SRC_ITER_H_

class Text;

class ByteStream {
 public:
    explicit ByteStream(Text* t);
    virtual ~ByteStream();

    int     back();
    int     next();
    int     peek();

    int     character;
    int     index;
    int     line;
    Text*   text;
};


#endif  // SRC_ITER_H_
