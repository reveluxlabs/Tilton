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
        the_macro_list_[i] = NULL;
    }
}

SearchList::~SearchList() {
}

Text* SearchList::the_macro_list(uint32 h) {
    return the_macro_list_[h];
}

void SearchList::set_the_macro_list(uint32 h, Text* t) {
    the_macro_list_[h] = t;
}

void SearchList::InsertIntoSearchList(Text* name, Text* t) {
    uint32 h = name->Hash() & MAXHASH;
    t->set_name(name);
    t->link_ = SearchList::the_macro_list(h);
    SearchList::set_the_macro_list(h, t);
}

//  addFunction is used by main to add primitive functions to tilton.
//  A function operates on a context which supplies the parameters.

void SearchList::InstallMacro(const char* namestring, void (*function)(Context *, Text * )) {
    Text* t = new Text();
    Text* name = new Text(namestring);
    SearchList::InsertIntoSearchList(name, t);
    t->function_ = function;
    delete name;
}

//  addMacro -- This is a little faster than InstallMacro() because it assumes that
//  the name is not already in the macro list.

void SearchList::InstallMacro(const char* namestring, const char* string) {
    Text* name = new Text(namestring);
    SearchList::InsertIntoSearchList(name, new Text(string));
    delete name;
}

//  lookup - search through the macro list for a text with a specific name.
//  The list is a hash table with links for collisions.

Text* SearchList::LookupMacro(Text* name) {
    Text* t = SearchList::the_macro_list(name->Hash() & MAXHASH);
    while (t) {
        if (t->isName(name)) {
            break;
        }
        t = t->link_;
    }
    return t;
}


//  install - if there is a text in the macro list with this name, set its
//  value. Otherwise, make a new text with this name and value and put
//  it in the list.

void SearchList::InstallMacro(Text* name, Text* value) {
    Text* t = SearchList::LookupMacro(name);
    if (t) {
        t->set_string(value);
    } else {
        SearchList::InsertIntoSearchList(name, new Text(value));
    }
}

void SearchList::PrintMacroTable() {
    int i;
    for (i = 0; i < (MAXHASH + 1); i += 1) {
        Text* macro = SearchList::the_macro_list(i);
        if (macro) {
            macro->PrintTextList();
        }
    }
}

Text* SearchList::GetMacroDefOrInsertNull(Text* name) {
    Text* t = SearchList::LookupMacro(name);
    if (!t) {
        t = new Text(0);
        uint32 h = name->Hash() & MAXHASH;
        t->set_name(name);
        t->link_ = SearchList::the_macro_list(h);
        SearchList::set_the_macro_list(h, t);
    }
    return t;
}

