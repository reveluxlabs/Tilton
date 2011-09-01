//  text.cpp: implementation of the Text class.
//  2006-10-06

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

//  Text wraps a string, and provides methods for setting and modifying the
//  string and for doing I/O with it. A Text can also have a name, which 
//  is used as a macro name. A Text can have a link which can chain Texts 
//  together. This is used to manage hash collisions.

//  The encoding of strings is UTF-8 (the 8-bit form of Unicode). A character 
//  is between 1 and 4 bytes in length. The utfLength and utfSubstr methods
//  count multibyte characters. However, if a multibyte character appears to
//  be badly formed, it will interpret the first byte as a single byte
//  character. So while expecting UTF-8 encoded strings, it will usually
//  do the right thing with Latin-1 and similar encodings.

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tilton.h"
#include "text.h"

// null constructor

Text::Text() {
    init(NULL, 0);
}


// length constructor

Text::Text(int len) {
    init(NULL, len);
}


// c-string constructor

Text::Text(const char* s) {
    init(s, (int)strlen(s));
}


// string constructor

Text::Text(const char* s, int len) {
    init(s, len);
}


// text constructor

Text::Text(Text* t) {
    if (t) {
        init(t->string, t->length);
    } else {
        init(NULL, 0);
    }
}


// deconstructor

Text::~Text() {
    delete this->string;
    delete this->name;
}


// append character (8-bit)

void Text::append(int c) {
    checkMaxLength(1);
    string[length] = (char)c;
    length += 1;
    myHash = 0;
}


// append characters

void Text::append(int c, int n) {
    checkMaxLength(n);
    while (n > 0) {
        string[length] = (char)c;
        length += 1;
        n -= 1;
    }
    myHash = 0;
}


// append c-string

void Text::append(const char* s) {
    if (s) {
        append(s, (int)strlen(s));
    }
}


// append string

void Text::append(const char* s, int len) {
    if (s && len) {
        checkMaxLength(len);
        memmove(&string[length], s, len);
        length += len;
        myHash = 0;
    }
}


// append text

void Text::append(Text* t) {
    if (t) {
        append(t->string, t->length);
    }
}


// append number

void Text::appendNumber(number n) {
    number d;
    if (n != NAN) {
        if (n < 0) {
            append('-');
            n = -n;
        }
        d = n / 10;
        if (d > 0) {
            appendNumber(d);
        }
        append((int)(n % 10) + '0');
        myHash = 0;
    }
}


//  If the requested amount does not fit within the allocated max length,
//  then increase the size of the string. The new allocation will be at least
//  twice the previous allocation.

void Text::checkMaxLength(int len) {
    int newMaxLength;
    int req = length + len;
    if (maxLength < req) {
        newMaxLength = maxLength * 2;
        if (newMaxLength < req) {
            newMaxLength = req;
        }
        char* newString = new char[newMaxLength];
        memmove(newString, string, maxLength);
        delete string;
        string = newString;
        maxLength = newMaxLength;
    }
}


void Text::dump() {
    Text* t = this;
    while (t) {
        fwrite(t->name, sizeof(char), t->nameLength, stderr);
        if (t->length) {
            fputc('~', stderr);
            fwrite(t->string, sizeof(char), t->length, stderr);
        }
        fprintf(stderr, "\n");
        t = t->link;
    }
}


// get character

int Text::getChar(int index) {
    if (index >= 0 && index < length) {
        return string[index];
    } else {
        return EOT;
    }
}


// get number, ignoring leading and trailing whitespace.

number Text::getNumber() {
    int c;
    int i = 0;
    bool sign = false;
    bool ok = false;
    number value = 0;
    
    if (length == 0) {
        return NAN;  // joh 31Aug11
    }
    
    for(;;) {
        c = string[i];
        i += 1;
        if (i > length) {
            return NAN;
        }
        if (c > ' ') {
            break;
        }
    }
    if (c == '-') {
        sign = true;
        c = string[i];
        i += 1;
        if (i > length) {
            return NAN;
        }
    }
    for (;;) {
        if (c >= '0' && c <= '9') {
            value = (value * 10) + (c - '0');
            ok = true;
            if (value < 0) {
                ok = false;
                break;
            }
        } else {
             for (;;) {
                if (c > ' ') {
                    return NAN;
                }
                if (i >= length) {
                    break;
                }
                c = string[i];
                i += 1;
            }
        }
        if (i >= length) {
            break;
        }
        c = string[i];
        i += 1;
    }
    if (ok) {
        if (sign) {
            return -value;
        } else {
            return value;
        }
    } else {
        return NAN;
    }
}


// find the first occurance of a substring

int Text::indexOf(Text *t) {
    int len = t->length;
    char* s = t->string;
    if (len) {
        bool b;
        int d = length - len;
        int i;
        int r;
        for (r = 0; r <= d; r += 1) {
            b = true;
            for (i = 0; i < len; i += 1) {
                if (string[r + i] != s[i]) {
                    b = false;
                    break;
                }
            }
            if (b) {
                return r;
            }
        };

    }
    return -1;
}


void Text::init(const char* s, int len) {
    name = NULL;
    link = NULL;
    function = NULL;
    length = nameLength = 0;
    myHash = 0;
    maxLength = len;
    if (len == 0) {
        string = NULL;
    } else {
        string = new char[len];
        if (s) {
            memmove(string, s, len);
            length = len;
        }
    }
}


// read from standard input

void Text::input() {
    char buffer[10240];
    int len;
    length = 0;
    myHash = (int) 0;
    for (;;) {
        len = (int) fread(buffer, sizeof(char), sizeof(buffer), stdin);
        if (len <= 0) {
            break;
        }
        append(buffer, len);
    }
}


// is c-string

bool Text::is(const char* s) {
    int i;
    for (i = 0; i < length; i += 1) {
        if (string[i] != s[i]) {
            return false;
        }
    }
    return (s[length] == 0);
}


// is text

bool Text::is(Text* t) {
    int i;
    if (length != t->length) {
        return false;
    }
    for (i = 0; i < length; i += 1) {
        if (string[i] != t->string[i]) {
            return false;
        }
    }
    return true;
}


// is name text

bool Text::isName(Text* t) {
    if (nameLength != t->length) {
        return false;
    }
    for (int i = 0; i < nameLength; i += 1) {
        if (name[i] != t->string[i]) {
            return false;
        }
    }
    return true;
}


// find the last occurance of a substring

int Text::lastIndexOf(Text *t) {
    int len = t->length;
    char* s = t->string;
    if (len) {
        bool b;
        int d = length - len;
        for (int r = d; r >= 0; r -= 1) {
            b = true;
            for (int i = 0; i < len; i += 1) {
                if (string[r + i] != s[i]) {
                    b = false;
                    break;
                }
            }
            if (b) {
                return r;
            }
        }
    }
    return -1;
}


// less than text

bool Text::lt(Text* t)
{
    // refactored to seperate all digits from text -- jr 28Aug11
    if (this->allDigits() && t->allDigits()) {
      return this->ltNum(t);
    } else {
      return this->ltStr(t);
    }
}

bool Text::ltNum(Text* t)
{
    int first, second;

    first = atoi(this->string);
    second = atoi(t->string);
    return first < second;
}

bool Text::ltStr(Text* t) {
    // original lt
    int len = t->length;
    if (len > length) {
        len = length;
    }
    for (int i = 0; i < len; i += 1) {
        if (string[i] != t->string[i]) {
            return (string[i] < t->string[i]);
        }
    }
    return len != t->length;
}


// write to standard output

void Text::output() {
    fwrite(string, sizeof(char), length, stdout);
}


//  read filename -- read the file in 10K chunks.

bool Text::read(Text* filename) {
    FILE *fp;
    char buffer[10240];
    int len;

    delete name;
    nameLength = filename->length;
    name = new char[nameLength];
    memmove(name, filename->string, nameLength);
    memmove(buffer, name, nameLength);
    buffer[filename->length] = 0;

    myHash = 0;
    length = 0;
    fp = fopen(buffer, "rb");
    if (fp) {
        for (;;) {
            len = (int) fread(buffer, sizeof(char), sizeof(buffer), fp);
            if (len <= 0) {
                break;
            }
            append(buffer, len);
        }
        fclose(fp);
        return true;
    } else {
        return false;
    }
}


// set character

void Text::set(int index, int c) {
    myHash = 0;
    if (index >= length) {
        append(c);
    } else if (index >= 0) {
        string[index] = (char)c;
    }
}


// set text

void Text::set(Text* t) {
    myHash = 0;
    if (t && t->length) {
        length = t->length;
        if (length > maxLength) {
            delete string;
            string = new char[length];
            maxLength = length;
        }
        memmove(string, t->string, length);
    } else {
        length = 0;
    }
}


// set name with c-string

void Text::setName(const char* s) {
    setName(s, (int) strlen(s));
}


// set name with string

void Text::setName(const char* s, int len) {
    delete name;
    nameLength = len;
    name = new char[nameLength];
    memmove(name, s, nameLength);
}


// set name with text
    
void Text::setName(Text* t) {
    setName(t->string, t->length);
}

        
//  substring

void Text::substr(int start, int len) {
    memmove(string, &string[start], len);
    length = len;
}


// remove tail and return it
    
Text* Text::tail(int index) {
    if (index >= 0 && index < length) {
        int len = length - index;
        length = index;
        myHash = 0;
        return new Text(&string[index], len);
    } else {
        return new Text();
    }
}


// trim is like append, except that it trims leading, trailing spaces, and
// reduces runs of whitespace to single space

void Text::trim(Text* t) { 
    char* s = t->string;
    int l = t->length;
    int i = 0;
    bool b = false;
    for (;;) {
        while (s[i] > ' ') {
            append(s[i]);
            b = true;
            i += 1;
            if (i >= l) {
                return;
            }
        }
        do {
            i += 1;
            if (i >= l) {
                return;
            }
        } while (s[i] <= ' ');
        if (b) {
            append(' ');
        }
    }
}


// determine the utf-8 length

int Text::utfLength() {
    int c;
    int i = 0;
    int num = 0;
    while (i < length) {
        c = string[i] & 0xFF;
        i += 1;
        if (c >= 0xC0) { 
            if (c < 0xE0) { // 2-byte form
                if ((i + 1) < length && ((string[i] & 0xC0) == 0x80)) {
                    i += 1;
                }
            } else if (c < 0xF0) { // 3-byte form
                if ((i + 2) < length && 
                        ((string[i]     & 0xC0) == 0x80) && 
                        ((string[i + 1] & 0xC0) == 0x80)) {
                    i += 2;
                }
            } else { // 4-byte form
                if ((i + 3) < length && 
                        ((string[i]     & 0xC0) == 0x80) && 
                        ((string[i + 1] & 0xC0) == 0x80) && 
                        ((string[i + 2] & 0xC0) == 0x80)) {
                    i += 3;
                }
            }
        }
        num += 1;
    }
    return num;
}


// utf-8 substring

Text* Text::utfSubstr(int start, int len) {
    int c;
    int i = 0;
    Text* t;
    while (start) {
        if (i >= length) {
            return NULL;
        }
        c = string[i] & 0xFF;
        i += 1;
        if (c >= 0xC0) { 
            if (c < 0xE0) { // 2-byte form
                if ((i + 1) < length && ((string[i] & 0xC0) == 0x80)) {
                    i += 1;
                }
            } else if (c < 0xF0) { // 3-byte form
                if ((i + 2) < length && 
                        ((string[i]     & 0xC0) == 0x80) && 
                        ((string[i + 1] & 0xC0) == 0x80)) {
                    i += 2;
                }
            } else { // 4-byte form
                if ((i + 3) < length && 
                        ((string[i]     & 0xC0) == 0x80) && 
                        ((string[i + 1] & 0xC0) == 0x80) && 
                        ((string[i + 2] & 0xC0) == 0x80)) {
                    i += 3;
                }
            }
        }
        start -= 1;
    }
    t = new Text(length - i);
    while (len && i < length) {
        c = string[i] & 0xFF;
        i += 1;
        t->append(c);
        if (c >= 0xC0) { 
            if (c < 0xE0) { // 2-byte form
                if ((i + 1) < length && ((string[i] & 0xC0) == 0x80)) {
                    t->append(string[i]);
                    i += 1;
                }
            } else if (c < 0xF0) { // 3-byte form
                if ((i + 2) < length && 
                        ((string[i]     & 0xC0) == 0x80) && 
                        ((string[i + 1] & 0xC0) == 0x80)) {
                    t->append(&string[i], 2);
                    i += 2;
                }
            } else { // 4-byte form
                if ((i + 3) < length && 
                        ((string[i]     & 0xC0) == 0x80) && 
                        ((string[i + 1] & 0xC0) == 0x80) && 
                        ((string[i + 2] & 0xC0) == 0x80)) {
                    t->append(&string[i], 3);
                    i += 3;
                }
            }
        }
        len -= 1;
    }
    return t;
}


// write filename

bool Text::write(Text* filename) {
    FILE *fp;
    char fname[256];
    memmove(fname, filename->string, filename->length);
    fname[filename->length] = 0;
    fp = fopen(fname, "wb");
    if (fp) {
        fwrite(string, sizeof(char), length, fp);
        fclose(fp);
        return true;
    } else {
        return false;
    }
}


// hash is based on work by Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  
// See http://burtleburtle.net/bob/hash/evahash.html


#define mix(a,b,c) { \
  a -= b; a -= c; a ^= (c >> 13); \
  b -= c; b -= a; b ^= (a << 8);  \
  c -= a; c -= b; c ^= (b >> 13); \
  a -= b; b -= a; a ^= (c >> 12); \
  b -= c; b -= a; b ^= (a << 16); \
  c -= a; c -= b; c ^= (b >> 5);  \
  a -= b; a -= c; a ^= (c >> 3);  \
  b -= c; b -= a; b ^= (a << 10); \
  c -= a; c -= b; c ^= (b >> 15); \
}

uint32 Text::hash() {
    if (myHash) {
        return myHash; // If we have already memoized the hash, use it.
    }
    register uint32  a = 0;
    register uint32  b = 0xDEADBEAD;
    register uint32  c = 0xCAFEB00B;
    register uint8* k = (uint8*)string; 
    register int  len = length; 

    while (len >= 12) {
        a += (uint32)k[0] + ((uint32)k[1] << 8) + ((uint32)k[2] << 16) + ((uint32)k[3] <<24);
        b += (uint32)k[4] + ((uint32)k[5] << 8) + ((uint32)k[6] << 16) + ((uint32)k[7] <<24);
        c += (uint32)k[8] + ((uint32)k[9] << 8) + ((uint32)k[10]<< 16) + ((uint32)k[11]<<24);
        mix(a,b,c);
        k += 12; 
        len -= 12;
    }
    c += length;
    switch (len) {
    case 11: c += (uint32)k[10] << 24;
    case 10: c += (uint32)k[9]  << 16;
    case 9:  c += (uint32)k[8]  <<  8;
    case 8:  b += (uint32)k[7]  << 24;
    case 7:  b += (uint32)k[6]  << 16;
    case 6:  b += (uint32)k[5]  <<  8;
    case 5:  b += (uint32)k[4];
    case 4:  a += (uint32)k[3]  << 24;
    case 3:  a += (uint32)k[2]  << 16;
    case 2:  a += (uint32)k[1]  <<  8;
    case 1:  a += (uint32)k[0];
    }
    mix(a,b,c);
    myHash = c;
    return c;
}
