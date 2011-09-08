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

typedef void (*Builtin)(Context* context);

class Text {
 public:
  Text();
  explicit Text(int len);
  explicit Text(const char* s);
  Text(const char* s, int len);
  explicit Text(Text* t);
  virtual ~Text();

  // AddToString
  // appends text to the string wrapped by Text
  void    AddToString(int c);
  void    AddToString(int c, int n);
  void    AddToString(const char* s);
  void    AddToString(const char* s, int len);
  void    AddToString(Text* t);
  
  // AddNumberToString
  // appends a number to the string wrapped by Text
  void    AddNumberToString(number);
  
  void    dump();
  int     getChar(int index);
  number  getNumber();
  
  // Hash
  // calculate a hash for a string
  uint32  Hash();
  
  int     indexOf(Text* t);

  // ReadStdInput
  // Read from stdin and append to string
  void    ReadStdInput();
  bool    is(const char* s);
  bool    is(Text* t);
  bool    isName(Text* t);
  int     lastIndexOf(Text* t);
  bool    lt(Text* t);
  
  // WriteStdOutput
  // write string_ to stdout
  void    WriteStdOutput();
  
  // ReadFromFile
  // read the file in 10K chunks and add to string_
  bool    ReadFromFile(Text* t);
  
  // set_string
  // setter for string_
  void    set_string(Text* t);
  
  // set_name
  // setter for name_
  void    set_name(const char* s);
  void    set_name(const char* s, int len);
  void    set_name(Text* t);
  
  void    substr(int start, int len);
  Text*   tail(int index);
  
  // RemoveSpacesAddToString
  // trims whitespace before appending to string_
  void    RemoveSpacesAddToString(Text* t);
  
  int     utfLength();
  Text*   utfSubstr(int start, int len);
  
  // WriteToFile
  // writes string_ to a file
  bool    WriteToFile(Text* t);

  // added in initial refactoring of eval
  // replace with body
  void    addToString(char c) {
      this->AddToString(c);
  }
  void    addToString(char c, int i) {
      this->AddToString(c, i);
  }
  void    addToString(Text* s) {
      if (s) {
          this->AddToString(s->string_, s->length_);
      }
  }
  Text*    removeFromString(int i) {
      return this->tail(i);
  }
  
  // allDigits
  // Tests to see if a string is all digits
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

  // ltNum
  // less than for numbers
  // used by lt
  bool ltNum(Text* t);
  
  // ltStr
  // less than for strings
  // used by lt
  bool ltStr(Text* t);

  Builtin      function_;
  int          length_;
  Text*        link_;       // hash collisions
  char*        name_;
  int          name_length_;
  char*        string_;

 private:
  // CheckLengthAndIncrease
  // Test the length of string against the max, increase if needed
  void    CheckLengthAndIncrease(int len);
  void    init(const char* s, int len);

  uint32  my_hash_;
  int     max_length_;
};

#endif  // SRC_TEXT_H_
