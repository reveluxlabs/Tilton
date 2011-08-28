//  text.h: interface for the Text class.
//  2006-10-05

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

#ifndef __TEXT_H_
#define __TEXT_H_

#include "tilton.h"

class Context;

typedef void (*Op)(Context* context);

class Text {
public:
    Text();
    Text(int len);
    Text(char* s);
    Text(char* s, int len);
    Text(Text* t);
    virtual ~Text();

    void    append(int c);
    void    append(int c, int n);
    void    append(char* s);
    void    append(char* s, int len);
    void    append(Text* t);
    void    appendNumber(number);
    void    dump();
    int     get(int index);
    number  getNumber();
    uint32  hash();
    int     indexOf(Text* t);
    void    input();
    bool    is(char* s);
    bool    is(Text* t);
    bool    isName(Text* t);
    int     lastIndexOf(Text* t);
    bool    lt(Text* t);
    void    output();
    bool    read(Text* t);
    void    set(int index, int c);
    void    set(Text* t);
    void    setName(char* s);
    void    setName(char* s, int len);
    void    setName(Text* t);
    void    substr(int start, int len);
    Text*   tail(int index);
    void    trim(Text* t);
    int     utfLength();
    Text*   utfSubstr(int start, int len);
    bool    write(Text* t);

    Op      function;
    int     length;
    Text*   link;       // hash collisions
    int     maxLength;
    char*   name;
    int     nameLength;
    char*   string;

private:
    void    checkMaxLength(int len);
    void    init(char* s, int len);

    uint32     myHash;
};

#endif //__TEXT_H_
