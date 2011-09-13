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

#include "macro.h"

#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "tilton.h"

// jr 11Sep11
Macro::Macro() {
    InitializeMacro(NULL, 0);
}

// jr 11Sep11
Macro::Macro(int len) {
    InitializeMacro(NULL, len);
}

// jr 11Sep11
Macro::Macro(const char* s) {
    InitializeMacro(s, static_cast<int>(strlen(s)));
}

// jr 11Sep11
Macro::Macro(Text* t) {
    if (t) {
        InitializeMacro(t->string_, t->length_);
    } else {
        InitializeMacro(NULL, 0);
    }
}

// jr 11Sep11
Macro::~Macro() {
    delete this->definition_;
    delete this->name_;
}

// jr 11Sep11
void Macro::AddToString(const char* s, int len) {
    if (s && len) {
        CheckLengthAndIncrease(len);
        memmove(&definition_[length_], s, len);
        length_ += len;
        my_hash_ = 0;
    }
}

// jr 11Sep11
void Macro::AddToString(Text* t) {
    if (t) {
        AddToString(t->string_, t->length_);
    }
}

// jr 11Sep11
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

// jr 11Sep11
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

// jr 11Sep11
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

// jr 11Sep11
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

// jr 11Sep11
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

// jr 11Sep11
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

// jr 11Sep11
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

// jr 11Sep11
void Macro::set_name(const char* s) {
    set_name(s, static_cast<int>(strlen(s)));
}

// jr 11Sep11
void Macro::set_name(const char* s, int len) {
    delete name_;
    name_length_ = len;
    name_ = new char[name_length_];
    memmove(name_, s, name_length_);
}

// jr 11Sep11
void Macro::set_name(Text* t) {
    set_name(t->string_, t->length_);
}

// jr 11Sep11
void Macro::ReplaceDefWithSubstring(int start, int len) {
    memmove(definition_, &definition_[start], len);
    length_ = len;
}
