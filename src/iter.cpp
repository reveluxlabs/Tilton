// iter.cpp: implementation of the Iter class.
// For interface definitions, see iter.h

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
// This version of Tilton is licensed under the MIT license.

// Iter is just a convenient way of processing a text. 
// It keeps track of lines for error messages.

#include <stdlib.h>
#include <stdio.h>
#include "tilton.h"
#include "text.h"
#include "iter.h"

Iter::Iter(Text* t) {
    text = t;
    line = character = index = 0;
}


Iter::~Iter() {
}


// back up one character. 

int Iter::back() {
    if (text) {
        index -= 1;
        character -= 1;
        return text->getChar(index);
    } else {
        return EOT;
    }
}


// return the next character. 

int Iter::next() {
    if (text) {
        int c = text->getChar(index);
        index += 1;
        character += 1;
        if (c == '\n' || (c == '\r' && text->getChar(index) != '\n')) {
            line += 1;
            character = 0;
        }
        return c;
    } else {
        return EOT;
    }
}


// peek ahead one character

int Iter::peek() {
    if (text) {
        return text->getChar(index);
    } else {
        return EOF;
    }
}
