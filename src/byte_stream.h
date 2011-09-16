// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef SRC_BYTE_STREAM_H_
#define SRC_BYTE_STREAM_H_

class Text;
#include <string>

// ByteStream is just a convenient way of processing a text.
// It keeps track of lines for error messages.

class ByteStream {
 public:
  explicit ByteStream(Text* t);
  virtual ~ByteStream();

  int     character();
  int     index();
  int     line();
  Text*   text();
  
  int     back();
  int     next();
  int     peek();

private:
  int     character_;
  int     index_;      // position in byte stream
  int     line_;
  Text*   text_;
};


#endif  // SRC_BYTE_STREAM_H_
