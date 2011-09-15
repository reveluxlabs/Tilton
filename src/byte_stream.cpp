// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.


// ByteStream is just a convenient way of processing a text.
// It keeps track of lines for error messages.

#include "byte_stream.h"

#include <stdlib.h>
#include <stdio.h>

#include "tilton.h"
#include "text.h"

ByteStream::ByteStream(Text* t) {
    text = t;
    line = character = index = 0;
}


ByteStream::~ByteStream() {
}


// back up one character.

int ByteStream::back() {
    if (text) {
        index -= 1;
        character -= 1;
        return text->GetCharacter(index);
    } else {
        return EOT;
    }
}


// return the next character.

int ByteStream::next() {
    if (text) {
        int c = text->GetCharacter(index);
        index += 1;
        character += 1;
        if (c == '\n' || (c == '\r' && text->GetCharacter(index) != '\n')) {
            line += 1;
            character = 0;
        }
        return c;
    } else {
        return EOT;
    }
}


// peek ahead one character

int ByteStream::peek() {
    if (text) {
        return text->GetCharacter(index);
    } else {
        return EOF;
    }
}
