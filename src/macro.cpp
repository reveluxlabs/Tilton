// macro.cpp: implementation of the Macro class.
// For interface definitions, see macro.h

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

//  Macro wraps a string, and provides methods for setting and modifying the
//  string and for doing I/O with it. A Macro can also have a name, which
//  is used as a macro name. A Macro can have a link which can chain Macros
//  together. This is used to manage hash collisions.

//  The encoding of strings is UTF-8 (the 8-bit form of Unicode). A character
//  is between 1 and 4 bytes in length. The utfLength and utfSubstr methods
//  count multibyte characters. However, if a multibyte character appears to
//  be badly formed, it will interpret the first byte as a single byte
//  character. So while expecting UTF-8 encoded strings, it will usually
//  do the right thing with Latin-1 and similar encodings.

#include "macro.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "tilton.h"

// constructors

Macro::Macro() {
    InitializeMacro(NULL, 0);
}

Macro::Macro(int len) {
    InitializeMacro(NULL, len);
}

Macro::Macro(const char* s) {
    InitializeMacro(s, static_cast<int>(strlen(s)));
}

Macro::Macro(Text* t) {
    if (t) {
        InitializeMacro(t->string_, t->length_);
    } else {
        InitializeMacro(NULL, 0);
    }
}

Macro::~Macro() {
    delete this->definition_;
    delete this->name_;
}


// append string

void Macro::AddToString(const char* s, int len) {
    if (s && len) {
        CheckLengthAndIncrease(len);
        memmove(&definition_[length_], s, len);
        length_ += len;
        my_hash_ = 0;
    }
}


// append text

void Macro::AddToString(Text* t) {
    if (t) {
        AddToString(t->string_, t->length_);
    }
}


//  If the requested amount does not fit within the allocated max length,
//  then increase the size of the string. The new allocation will be at least
//  twice the previous allocation.

void Macro::CheckLengthAndIncrease(int len) {
    int newMaxLength;
    int req = length_ + len;
    if (max_length_ < req) {
        newMaxLength = max_length_ * 2;
        if (newMaxLength < req) {
            newMaxLength = req;
        }
        char* newString = new char[newMaxLength];
        memmove(newString, definition_, max_length_);
        delete definition_;
        definition_ = newString;
        max_length_ = newMaxLength;
    }
}


void Macro::PrintMacroList() {
    Macro* t = this;
    while (t) {
        fwrite(t->name_, sizeof(char), t->name_length_, stderr);
        if (t->length_) {
            fputc('~', stderr);
            fwrite(t->definition_, sizeof(char), t->length_, stderr);
        }
        fprintf(stderr, "\n");
        t = t->link_;
    }
}


// find the first occurance of a substring

int Macro::FindFirstSubstring(Text *t) {
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
        if (definition_[r + i] != s[i]) {
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


void Macro::InitializeMacro(const char* s, int len) {
    name_ = NULL;
    link_ = NULL;
    length_ = name_length_ = 0;
    my_hash_ = 0;
    max_length_ = len;
    if (len == 0) {
        definition_ = NULL;
    } else {
        definition_ = new char[len];
        if (s) {
            memmove(definition_, s, len);
            length_ = len;
        }
    }
}


// is name text

bool Macro::IsNameEqual(Text* t) {
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

int Macro::FindLastSubstring(Text *t) {
  int len = t->length_;
  char* s = t->string_;
  if (len) {
    bool b;
    int d = length_ - len;
    for (int r = d; r >= 0; r -= 1) {
      b = true;
      for (int i = 0; i < len; i += 1) {
        if (definition_[r + i] != s[i]) {
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


// set text

void Macro::set_string(Text* t) {
    my_hash_ = 0;
    if (t && t->length_) {
        length_ = t->length_;
        if (length_ > max_length_) {
            delete definition_;
            definition_ = new char[length_];
            max_length_ = length_;
        }
        memmove(definition_, t->string_, length_);
    } else {
        length_ = 0;
    }
}


// set name with c-string

void Macro::set_name(const char* s) {
    set_name(s, static_cast<int>(strlen(s)));
}


// set name with string

void Macro::set_name(const char* s, int len) {
    delete name_;
    name_length_ = len;
    name_ = new char[name_length_];
    memmove(name_, s, name_length_);
}


// set name with text

void Macro::set_name(Text* t) {
    set_name(t->string_, t->length_);
}


//  substring

void Macro::ReplaceDefWithSubstring(int start, int len) {
    memmove(definition_, &definition_[start], len);
    length_ = len;
}
