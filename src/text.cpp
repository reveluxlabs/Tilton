// text.cpp: implementation of the Text class.
// For interface definitions, see text.h

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
    init(s, static_cast<int>(strlen(s)));
}


// string constructor

Text::Text(const char* s, int len) {
    init(s, len);
}


// text constructor

Text::Text(Text* t) {
    if (t) {
        init(t->string_, t->length_);
    } else {
        init(NULL, 0);
    }
}


// deconstructor

Text::~Text() {
    delete this->string_;
    delete this->name_;
}


// append character (8-bit)

void Text::AddToString(int c) {
    CheckLengthAndIncrease(1);
    string_[length_] = static_cast<char>(c);
    length_ += 1;
    my_hash_ = 0;
}


// append characters

void Text::AddToString(int c, int n) {
    CheckLengthAndIncrease(n);
    while (n > 0) {
        string_[length_] = static_cast<char>(c);
        length_ += 1;
        n -= 1;
    }
    my_hash_ = 0;
}


// append c-string

void Text::AddToString(const char* s) {
    if (s) {
        AddToString(s, static_cast<int>(strlen(s)));
    }
}


// append string

void Text::AddToString(const char* s, int len) {
    if (s && len) {
        CheckLengthAndIncrease(len);
        memmove(&string_[length_], s, len);
        length_ += len;
        my_hash_ = 0;
    }
}


// append text

void Text::AddToString(Text* t) {
    if (t) {
        AddToString(t->string_, t->length_);
    }
}


// append number

void Text::AddNumberToString(number n) {
    number d;
    if (n != NAN) {
        if (n < 0) {
            AddToString('-');
            n = -n;
        }
        d = n / 10;
        if (d > 0) {
            AddNumberToString(d);
        }
        AddToString(static_cast<int>((n % 10) + '0'));
        my_hash_ = 0;
    }
}


//  If the requested amount does not fit within the allocated max length,
//  then increase the size of the string. The new allocation will be at least
//  twice the previous allocation.

void Text::CheckLengthAndIncrease(int len) {
    int newMaxLength;
    int req = length_ + len;
    if (max_length_ < req) {
        newMaxLength = max_length_ * 2;
        if (newMaxLength < req) {
            newMaxLength = req;
        }
        char* newString = new char[newMaxLength];
        memmove(newString, string_, max_length_);
        delete string_;
        string_ = newString;
        max_length_ = newMaxLength;
    }
}


void Text::PrintTextList() {
    Text* t = this;
    while (t) {
        fwrite(t->name_, sizeof(char), t->name_length_, stderr);
        if (t->length_) {
            fputc('~', stderr);
            fwrite(t->string_, sizeof(char), t->length_, stderr);
        }
        fprintf(stderr, "\n");
        t = t->link_;
    }
}


// get character

int Text::getChar(int index) {
    if (index >= 0 && index < length_) {
        return string_[index];
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

    if (length_ == 0) {
        return NAN;  // joh 31Aug11
    }

    for (;;) {
        c = string_[i];
        i += 1;
        if (i > length_) {
            return NAN;
        }
        if (c > ' ') {
            break;
        }
    }
    if (c == '-') {
        sign = true;
        c = string_[i];
        i += 1;
        if (i > length_) {
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
                if (i >= length_) {
                    break;
                }
                c = string_[i];
                i += 1;
            }
        }
        if (i >= length_) {
            break;
        }
        c = string_[i];
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
    int len = t->length_;
    char* s = t->string_;
    if (len) {
        bool b;
        int d = length_ - len;
        int i;
        int r;
        for (r = 0; r <= d; r += 1) {
            b = true;
            for (i = 0; i < len; i += 1) {
                if (string_[r + i] != s[i]) {
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
    name_ = NULL;
    link_ = NULL;
    function_ = NULL;
    length_ = name_length_ = 0;
    my_hash_ = 0;
    max_length_ = len;
    if (len == 0) {
        string_ = NULL;
    } else {
        string_ = new char[len];
        if (s) {
            memmove(string_, s, len);
            length_ = len;
        }
    }
}


// read from standard input

void Text::ReadStdInput() {
    char buffer[10240];
    int len;
    length_ = 0;
    my_hash_ = static_cast<int>(0);
    for (;;) {
        len = static_cast<int>(fread(buffer, sizeof(char),
                               sizeof(buffer), stdin));
        if (len <= 0) {
            break;
        }
        AddToString(buffer, len);
    }
}


// is c-string

bool Text::is(const char* s) {
    int i;
    for (i = 0; i < length_; i += 1) {
        if (string_[i] != s[i]) {
            return false;
        }
    }
    return (s[length_] == 0);
}


// is text

bool Text::is(Text* t) {
    int i;
    if (length_ != t->length_) {
        return false;
    }
    for (i = 0; i < length_; i += 1) {
        if (string_[i] != t->string_[i]) {
            return false;
        }
    }
    return true;
}


// is name text

bool Text::isName(Text* t) {
    if (name_length_ != t->length_) {
        return false;
    }
    for (int i = 0; i < name_length_; i += 1) {
        if (name_[i] != t->string_[i]) {
            return false;
        }
    }
    return true;
}


// find the last occurance of a substring

int Text::lastIndexOf(Text *t) {
    int len = t->length_;
    char* s = t->string_;
    if (len) {
        bool b;
        int d = length_ - len;
        for (int r = d; r >= 0; r -= 1) {
            b = true;
            for (int i = 0; i < len; i += 1) {
                if (string_[r + i] != s[i]) {
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

bool Text::lt(Text* t) {
    // refactored to seperate all digits from text -- jr 28Aug11
    if (this->allDigits() && t->allDigits()) {
      return this->ltNum(t);
    } else {
      return this->ltStr(t);
    }
}

bool Text::ltNum(Text* t) {
    int first, second;

    first = atoi(this->string_);
    second = atoi(t->string_);
    return first < second;
}

bool Text::ltStr(Text* t) {
    // original lt
    int len = t->length_;
    if (len > length_) {
        len = length_;
    }
    for (int i = 0; i < len; i += 1) {
        if (string_[i] != t->string_[i]) {
            return (string_[i] < t->string_[i]);
        }
    }
    return len != t->length_;
}


// write to standard output

void Text::WriteStdOutput() {
    fwrite(string_, sizeof(char), length_, stdout);
}


//  read filename -- read the file in 10K chunks.

bool Text::ReadFromFile(Text* filename) {
  FILE *fp;
  char buffer[10240];
  int len;

  delete name_;
  name_length_ = filename->length_;
  name_ = new char[name_length_];
  memmove(name_, filename->string_, name_length_);
  memmove(buffer, name_, name_length_);
  buffer[filename->length_] = 0;

  my_hash_ = 0;
  length_ = 0;
  fp = fopen(buffer, "rb");
  if (fp) {
    for (;;) {
      len = static_cast<int>(fread(buffer, sizeof(char),
                             sizeof(buffer), fp));
      if (len <= 0) {
          break;
      }
      AddToString(buffer, len);
    }
    fclose(fp);
    return true;
  } else {
    return false;
  }
}


// set text

void Text::set_string(Text* t) {
    my_hash_ = 0;
    if (t && t->length_) {
        length_ = t->length_;
        if (length_ > max_length_) {
            delete string_;
            string_ = new char[length_];
            max_length_ = length_;
        }
        memmove(string_, t->string_, length_);
    } else {
        length_ = 0;
    }
}


// set name with c-string

void Text::set_name(const char* s) {
    set_name(s, static_cast<int>(strlen(s)));
}


// set name with string

void Text::set_name(const char* s, int len) {
    delete name_;
    name_length_ = len;
    name_ = new char[name_length_];
    memmove(name_, s, name_length_);
}


// set name with text

void Text::set_name(Text* t) {
    set_name(t->string_, t->length_);
}


//  substring

void Text::substr(int start, int len) {
    memmove(string_, &string_[start], len);
    length_ = len;
}


// remove tail and return it

Text* Text::tail(int index) {
    if (index >= 0 && index < length_) {
        int len = length_ - index;
        length_ = index;
        my_hash_ = 0;
        return new Text(&string_[index], len);
    } else {
        return new Text();
    }
}


// trim is like append, except that it trims leading, trailing spaces, and
// reduces runs of whitespace to single space

void Text::RemoveSpacesAddToString(Text* t) {
    char* s = t->string_;
    int l = t->length_;
    int i = 0;
    bool b = false;
    for (;;) {
        while (s[i] > ' ') {
            AddToString(s[i]);
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
            AddToString(' ');
        }
    }
}


// determine the utf-8 length

int Text::utfLength() {
    int c;
    int i = 0;
    int num = 0;
    while (i < length_) {
        c = string_[i] & 0xFF;
        i += 1;
        if (c >= 0xC0) {
            if (c < 0xE0) {  // 2-byte form
                if ((i + 1) < length_ && ((string_[i] & 0xC0) == 0x80)) {
                    i += 1;
                }
            } else if (c < 0xF0) {  // 3-byte form
                if ((i + 2) < length_ &&
                        ((string_[i]     & 0xC0) == 0x80) &&
                        ((string_[i + 1] & 0xC0) == 0x80)) {
                    i += 2;
                }
            } else {  // 4-byte form
                if ((i + 3) < length_ &&
                        ((string_[i]     & 0xC0) == 0x80) &&
                        ((string_[i + 1] & 0xC0) == 0x80) &&
                        ((string_[i + 2] & 0xC0) == 0x80)) {
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
        if (i >= length_) {
            return NULL;
        }
        c = string_[i] & 0xFF;
        i += 1;
        if (c >= 0xC0) {
            if (c < 0xE0) {  // 2-byte form
                if ((i + 1) < length_ && ((string_[i] & 0xC0) == 0x80)) {
                    i += 1;
                }
            } else if (c < 0xF0) {  // 3-byte form
                if ((i + 2) < length_ &&
                        ((string_[i]     & 0xC0) == 0x80) &&
                        ((string_[i + 1] & 0xC0) == 0x80)) {
                    i += 2;
                }
            } else {  // 4-byte form
                if ((i + 3) < length_ &&
                        ((string_[i]     & 0xC0) == 0x80) &&
                        ((string_[i + 1] & 0xC0) == 0x80) &&
                        ((string_[i + 2] & 0xC0) == 0x80)) {
                    i += 3;
                }
            }
        }
        start -= 1;
    }
    t = new Text(length_ - i);
    while (len && i < length_) {
        c = string_[i] & 0xFF;
        i += 1;
        t->AddToString(c);
        if (c >= 0xC0) {
            if (c < 0xE0) {  // 2-byte form
                if ((i + 1) < length_ && ((string_[i] & 0xC0) == 0x80)) {
                    t->AddToString(string_[i]);
                    i += 1;
                }
            } else if (c < 0xF0) {  // 3-byte form
                if ((i + 2) < length_ &&
                        ((string_[i]     & 0xC0) == 0x80) &&
                        ((string_[i + 1] & 0xC0) == 0x80)) {
                    t->AddToString(&string_[i], 2);
                    i += 2;
                }
            } else {  // 4-byte form
                if ((i + 3) < length_ &&
                        ((string_[i]     & 0xC0) == 0x80) &&
                        ((string_[i + 1] & 0xC0) == 0x80) &&
                        ((string_[i + 2] & 0xC0) == 0x80)) {
                    t->AddToString(&string_[i], 3);
                    i += 3;
                }
            }
        }
        len -= 1;
    }
    return t;
}


// write filename

bool Text::WriteToFile(Text* filename) {
    FILE *fp;
    char fname[256];
    memmove(fname, filename->string_, filename->length_);
    fname[filename->length_] = 0;
    fp = fopen(fname, "wb");
    if (fp) {
        fwrite(string_, sizeof(char), length_, fp);
        fclose(fp);
        return true;
    } else {
        return false;
    }
}


// hash is based on work by Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.
// See http://burtleburtle.net/bob/hash/evahash.html


#define mix(a, b, c) { \
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

uint32 Text::Hash() {
    if (my_hash_) {
        return my_hash_;  // If we have already memoized the hash, use it.
    }
    register uint32  a = 0;
    register uint32  b = 0xDEADBEAD;
    register uint32  c = 0xCAFEB00B;
    register uint8* k = reinterpret_cast<uint8*>(string_);
    register int  len = length_;

    while (len >= 12) {
        a += static_cast<uint32>(k[0]) +
             (static_cast<uint32>(k[1] << 8)) +
             (static_cast<uint32>(k[2] << 16)) +
             (static_cast<uint32>(k[3] <<24));
        b += static_cast<uint32>(k[4]) +
             (static_cast<uint32>(k[5] << 8)) +
             (static_cast<uint32>(k[6] << 16)) +
             (static_cast<uint32>(k[7] <<24));
        c += static_cast<uint32>(k[8]) +
             (static_cast<uint32>(k[9] << 8)) +
             (static_cast<uint32>(k[10]<< 16)) +
             (static_cast<uint32>(k[11]<<24));
        mix(a, b, c);
        k += 12;
        len -= 12;
    }
    c += length_;
    switch (len) {
    case 11: c += static_cast<uint32>(k[10] << 24);
    case 10: c += static_cast<uint32>(k[9]  << 16);
    case 9:  c += static_cast<uint32>(k[8]  <<  8);
    case 8:  b += static_cast<uint32>(k[7]  << 24);
    case 7:  b += static_cast<uint32>(k[6]  << 16);
    case 6:  b += static_cast<uint32>(k[5]  <<  8);
    case 5:  b += static_cast<uint32>(k[4]);
    case 4:  a += static_cast<uint32>(k[3]  << 24);
    case 3:  a += static_cast<uint32>(k[2]  << 16);
    case 2:  a += static_cast<uint32>(k[1]  <<  8);
    case 1:  a += static_cast<uint32>(k[0]);
    }
    mix(a, b, c);
    my_hash_ = c;
    return c;
}
