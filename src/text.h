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
// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
//
// This version of Tilton is licensed under the MIT license.

#ifndef SRC_TEXT_H_
#define SRC_TEXT_H_

#include "tilton.h"
#include "string.h"

class Context;

typedef void (*Op)(Context* context);

class Text {
 public:
  Text();
  explicit Text(int len);
  explicit Text(const char* s);
  Text(const char* s, int len);
  explicit Text(Text* t);
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
  
  // ReadStdInput
  // Read from stdin and append to string
  void    ReadStdInput();
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

  void    addToString(char c) {
      this->append(c);
  }
  void    addToString(char c, int i) {
      this->append(c, i);
  }
  void    addToString(Text* s) {
      if (s) {
          this->append(s->string_, s->length_);
      }
  }
  Text*    removeFromString(int i) {
      return this->tail(i);
  }
  bool    allDigits() {
    const char*   cset = "1234567890";
    return this->length_ == strspn(this->string_, cset);
  }

  // isDigit
  // Tests to see if the arg is a digit
  static bool isDigit(int arg_number) {
    return arg_number >= 0 && arg_number <= 9;
  }

  // ConvertAlphaToIntegrer
  // unlike atoi, this function knows about Text
  int ConvertAlphaToInteger() {
    int c;             // current character
    int i;             // loop counter

    int num = this->getChar(0) - '0';
    if (isDigit(num)) {
      for (i = 1; i < this->length_; i += 1) {
        c = this->getChar(i) - '0';             // i for 0, jr 4Sep11
        if (c < 0 || c > 9) {
          num = -1;
          break;
        }
        num =num * 10 + c;
      }
    }
    return num;
  }

  bool ltNum(Text* t);
  bool ltStr(Text* t);

  Op      function_;
  int     length_;
  Text*   link_;       // hash collisions
  char*   name_;
  int     name_length_;
  char*   string_;

 private:
  void    checkMaxLength(int len);
  void    init(const char* s, int len);

  uint32  my_hash_;
  int     max_length_;
};

#endif  // SRC_TEXT_H_
