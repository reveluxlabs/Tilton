//  iter.cpp: implementation of the Iter class.
//  2006-10-05

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

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
