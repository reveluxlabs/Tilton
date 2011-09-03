// search.cpp
// For interface descriptions, see search.h

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

#include "search.h"

#include <stdlib.h>

#include "tilton.h"
#include "text.h"


SearchList::SearchList() {
    int i;

    for (i = MAXHASH; i >= 0; i -= 1) {
        theMacroList[i] = NULL;
    }
}

SearchList::~SearchList() {
}

Text* SearchList::getList(uint32 h) {
    return theMacroList[h];
}

void SearchList::setList(uint32 h, Text* t) {
    theMacroList[h] = t;
}

void SearchList::link(Text* name, Text* t) {
    uint32 h = name->hash() & MAXHASH;
    t->setName(name);
    t->link = SearchList::getList(h);
    SearchList::setList(h, t);
}

//  addFunction is used by main to add primitive functions to tilton.
//  A function operates on a context which supplies the parameters.

void SearchList::install(const char* namestring, void (*function)(Context * )) {
    Text* t = new Text();
    Text* name = new Text(namestring);
    SearchList::link(name, t);
    t->function = function;
    delete name;
}

//  addMacro -- This is a little faster than install() because it assumes that
//  the name is not already in the macro list.

void SearchList::install(const char* namestring, const char* string) {
    Text* name = new Text(namestring);
    SearchList::link(name, new Text(string));
    delete name;
}

//  lookup - search through the macro list for a text with a specific name.
//  The list is a hash table with links for collisions.

Text* SearchList::lookup(Text* name) {
    Text* t = SearchList::getList(name->hash() & MAXHASH);
    while (t) {
        if (t->isName(name)) {
            break;
        }
        t = t->link;
    }
    return t;
}


//  install - if there is a text in the macro list with this name, set its
//  value. Otherwise, make a new text with this name and value and put
//  it in the list.

void SearchList::install(Text* name, Text* value) {
    Text* t = SearchList::lookup(name);
    if (t) {
        t->setString(value);
    } else {
        SearchList::link(name, new Text(value));
    }
}

void SearchList::dump() {
    int i;
    for (i = 0; i < (MAXHASH + 1); i += 1) {
        Text* macro = SearchList::getList(i);
        if (macro) {
            macro->dump();
        }
    }
}

Text* SearchList::getDef(Text* name) {
    Text* t = SearchList::lookup(name);
    if (!t) {
        t = new Text(0);
        uint32 h = name->hash() & MAXHASH;
        t->setName(name);
        t->link = SearchList::getList(h);
        SearchList::setList(h, t);
    }
    return t;
}

