// text.h: interface for the Text class.

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

#ifndef __TEXT_H_
#define __TEXT_H_

#include "tilton.h"
#include "string.h"

class Context;

typedef void (*Op)(Context* context);

class Text {
public:
    Text();
    Text(int len);
    Text(const char* s);
    Text(const char* s, int len);
    Text(Text* t);
    virtual ~Text();

    void    append(int c);
    void    append(int c, int n);
    void    append(const char* s);
    void    append(const char* s, int len);
    void    append(Text* t);
    void    appendNumber(number);
    void    dump();
    int     getChar(int index);
    number  getNumber();
    uint32  hash();
    int     indexOf(Text* t);
    void    input();
    bool    is(const char* s);
    bool    is(Text* t);
    bool    isName(Text* t);
    int     lastIndexOf(Text* t);
    bool    lt(Text* t);
    void    output();
    bool    read(Text* t);
    void    setString(Text* t);
    void    setName(const char* s);
    void    setName(const char* s, int len);
    void    setName(Text* t);
    void    substr(int start, int len);
    Text*   tail(int index);
    void    trim(Text* t);
    int     utfLength();
    Text*   utfSubstr(int start, int len);
    bool    write(Text* t);

    void    addToString(char c)
    {
        this->append(c);
    }
    void    addToString(char c, int i)
    {
        this->append(c, i);
    }
    void    addToString(Text* s)
    {
        if (s) {
            this->append(s->string, s->length);
        }
    }
    Text*    removeFromString(int i)
    {
        return this->tail(i);
    }
    bool    allDigits()
    {
      const char*   cset = "1234567890";
      return this->length == strspn(this->string, cset);
    }
    
    bool ltNum(Text* t);
    bool ltStr(Text* t);

    Op      function;
    int     length;
    Text*   link;       // hash collisions
    int     maxLength;
    char*   name;
    int     nameLength;
    char*   string;

private:
    void    checkMaxLength(int len);
    void    init(const char* s, int len);

    uint32     myHash;
};

#endif //__TEXT_H_
