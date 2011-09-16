// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "byte_stream.h"

#include <stdlib.h>
#include <stdio.h>

#include "tilton.h"
#include "text.h"

ByteStream::ByteStream(Text* t) {
  text_ = t; //->string_;
    line_ = character_ = index_ = 0;
}

ByteStream::~ByteStream() {
}

int ByteStream::character() {
  return character_;
}

int ByteStream::index() {
  return index_;
}

int ByteStream::line() {
  return line_;
}

Text* ByteStream::text() {
  return text_;
}

// back up one character.

int ByteStream::back() {
    if (text_) {
      index_ -= 1;
      character_ -= 1;
      return text_->GetCharacter(index_);
    } else {
        return EOT;
    }
}

// return the next character.

int ByteStream::next() {
    if (text_) {
      int c = text_->GetCharacter(index_);
      index_ += 1;
      character_ += 1;
      if (c == '\n' || (c == '\r' && text_->GetCharacter(index_) != '\n')) {
          line_ += 1;
          character_ = 0;
      }
      return c;
    } else {
      return EOT;
    }
}

// peek ahead one character

int ByteStream::peek() {
    if (text_) {
      return text_->GetCharacter(index_);
    } else {
        return EOF;
    }
}
