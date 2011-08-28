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
    int     get(int index);
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
    void    set(int index, int c);
    void    set(Text* t);
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
