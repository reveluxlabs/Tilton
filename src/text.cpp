// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "text.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tilton.h"
#include "macro.h"

Text::Text() {
    InitializeText(NULL, 0);
}


Text::Text(int len) {
    InitializeText(NULL, len);
}


Text::Text(const char* s) {
    InitializeText(s, static_cast<int>(strlen(s)));
}


Text::Text(const char* s, int len) {
    InitializeText(s, len);
}


Text::Text(Text* t) {
    if (t) {
        InitializeText(t->string_, t->length_);
    } else {
        InitializeText(NULL, 0);
    }
}

Text::Text(Macro* m) {
  if (m) {
    // copy string
    InitializeText(m->definition_, m->length_);
    // copy name
    if (m->name_length_ == 0) {
      name_ = NULL;
    } else {
      name_ = new char[m->name_length_];
      if (m->name_) {
        memmove(name_, m->name_, m->name_length_);
        name_length_ = m->name_length_;
      }
    }  
  } else {
    InitializeText(NULL, 0);
  }
}

Text::~Text() {
    delete this->string_;
    delete this->name_;
}


void Text::AddToString(int c) {
    CheckLengthAndIncrease(1);
    string_[length_] = static_cast<char>(c);
    length_ += 1;
    my_hash_ = 0;
}


void Text::AddToString(int c, int n) {
    CheckLengthAndIncrease(n);
    while (n > 0) {
        string_[length_] = static_cast<char>(c);
        length_ += 1;
        n -= 1;
    }
    my_hash_ = 0;
}


void Text::AddToString(const char* s) {
    if (s) {
        AddToString(s, static_cast<int>(strlen(s)));
    }
}


void Text::AddToString(const char* s, int len) {
    if (s && len) {
        CheckLengthAndIncrease(len);
        memmove(&string_[length_], s, len);
        length_ += len;
        my_hash_ = 0;
    }
}


void Text::AddToString(Text* t) {
    if (t) {
        AddToString(t->string_, t->length_);
    }
}


void Text::AddNumberToString(number n) {
    number d;
    if (n != kNAN) {
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

int Text::GetCharacter(int index) {
    if (index >= 0 && index < length_) {
        return string_[index];
    } else {
        return EOT;
    }
}

number Text::getNumber() {
  int c;
  int i = 0;
  bool sign = false;
  bool ok = false;
  number value = 0;

  if (length_ == 0) {
    return kNAN;  // jr 31Aug11
  }

  for (;;) {
    c = string_[i];
    i += 1;
    if (i > length_) {
        return kNAN;
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
        return kNAN;
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
            return kNAN;
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
    return kNAN;
  }
}

void Text::InitializeText(const char* s, int len) {
    name_ = NULL;
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


bool Text::IsEqual(Text* t) {
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


void Text::WriteStdOutput() {
    fwrite(string_, sizeof(char), length_, stdout);
}


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


void Text::set_name(const char* s) {
    set_name(s, static_cast<int>(strlen(s)));
}


void Text::set_name(const char* s, int len) {
    delete name_;
    name_length_ = len;
    name_ = new char[name_length_];
    memmove(name_, s, name_length_);
}


void Text::set_name(Text* t) {
    set_name(t->string_, t->length_);
}


void Text::substr(int start, int len) {
    memmove(string_, &string_[start], len);
    length_ = len;
}


Text* Text::RemoveFromString(int index) {
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


int Text::utfLength() {
  int index = 0;
  int num = 0;
  while (index < length_) {
    index = AdvanceToNextChar(index);
    num += 1;
  }
  return num;
}

int Text::AdvanceToNextChar(int i) {
  int c;
  c = string_[i] & 0xFF;
  i += 1;
  if (c >= 0xC0) {
    if (c < 0xE0) {  // 2-byte form
      // <= jr 18Sep11
      if ((i + 1) <= length_ && ((string_[i] & 0xC0) == 0x80)) {
        i += 1;
      }
    } else if (c < 0xF0) {  // 3-byte form
      // <= jr 18Sep11
      if ((i + 2) <= length_ &&
          ((string_[i]     & 0xC0) == 0x80) &&
          ((string_[i + 1] & 0xC0) == 0x80)) {
        i += 2;
      }
    } else {  // 4-byte form
      // <= jr 18Sep11
      if ((i + 3) <= length_ &&
          ((string_[i]     & 0xC0) == 0x80) &&
          ((string_[i + 1] & 0xC0) == 0x80) &&
          ((string_[i + 2] & 0xC0) == 0x80)) {
        i += 3;
      }
    }
  }
  return i;
}


Text* Text::utfSubstr(const int start, int len) {
  int i = 0;
  int j;
  Text* t;
  
  // skip start UTF-8 chars in string
  // start -1 fixes off by one bug with position -- jr 18Sep11
  for (j=0; j < start - 1 ; j++) {
    if (i >= length_) {
      return NULL;
    }
    i = AdvanceToNextChar(i);
  }
  
  // make a new string
  t = new Text(length_ - i);
  
  // and copy UTF-8 chars
  while (len && i < length_) {
    j = AdvanceToNextChar(i);
    t->AddToString(&string_[i], j-i);
    i = j;
    len -= 1;
  }
  return t;
}


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
