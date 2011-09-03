//  iter.h: interface for the Iter class.

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

#ifndef SRC_ITER_H_
#define SRC_ITER_H_

class Text;

class Iter {
 public:
    explicit Iter(Text* t);
    virtual ~Iter();

    int     back();
    int     next();
    int     peek();

    int     character;
    int     index;
    int     line;
    Text*   text;
};


#endif  // SRC_ITER_H_
