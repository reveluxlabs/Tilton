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
// This version of Tilton is licensed under the MIT license.

#ifndef __ITER_H_
#define __ITER_H_

class Text;

class Iter 
{
public:
    Iter(Text*);
    virtual ~Iter();

    int     back();
    int     next();
    int     peek();

    int     character;
    int     index;
    int     line;
    Text*   text;
};


#endif //__ITER_H_
