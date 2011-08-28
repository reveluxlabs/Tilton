//  iter.h: interface for the Iter class.

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

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
