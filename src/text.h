// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef SRC_TEXT_H_
#define SRC_TEXT_H_

#include "tilton.h"
#include "string.h"

class Context;
class Macro;

// Text
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

class Text {
 public:
  Text();
  explicit Text(int len);
  explicit Text(const char* s);
  Text(const char* s, int len);
  explicit Text(Text* t);
  explicit Text(Macro* t);
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
  
  // GetCharacter
  // retrieve a character from string
  int     GetCharacter(int index);
  
  // getNumber
  // retrieve a number from string
  number  getNumber();
  
  // Hash
  // calculate a hash for a string
  uint32  Hash();
  
  // ReadStdInput
  // Read from stdin and append to string
  void    ReadStdInput();

  bool    IsEqual(Text* t);
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
  Text*   RemoveFromString(int index);
  
  // RemoveSpacesAddToString
  // trims whitespace before appending to string_
  void    RemoveSpacesAddToString(Text* t);
  
  int     utfLength();
  Text*   utfSubstr(int start, int len);
  
  // WriteToFile
  // writes string_ to a file
  bool    WriteToFile(Text* t);

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

    int num = this->GetCharacter(0) - '0';
    if (isDigit(num)) {
      for (i = 1; i < this->length_; i += 1) {
        c = this->GetCharacter(i) - '0';             // i for 0, jr 4Sep11
        if (c < 0 || c > 9) {
          num = -1;
          break;
        }
        num =num * 10 + c;
      }
    }
    return num;
  }

  int          length_;
  char*        name_;
  int          name_length_;
  char*        string_;

 private:
  // CheckLengthAndIncrease
  // Test the length of string against the max, increase if needed
  void    CheckLengthAndIncrease(int len);
  void    InitializeText(const char* s, int len);

  // ltNum
  // less than for numbers
  // used by lt
  bool ltNum(Text* t);
  
  // ltStr
  // less than for strings
  // used by lt
  bool ltStr(Text* t);
  
  uint32  my_hash_;
  int     max_length_;
};

#endif  // SRC_TEXT_H_
