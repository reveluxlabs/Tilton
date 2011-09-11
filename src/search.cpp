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
#include "macro.h"


SearchList::SearchList() {
    int i;

    for (i = MaxHash; i >= 0; i -= 1) {
        the_macro_list_[i] = NULL;
    }
}

SearchList::~SearchList() {}

Macro* SearchList::the_macro_list(uint32 h) const {
    return the_macro_list_[h];
}

void SearchList::set_the_macro_list(uint32 h, Macro* m) {
    the_macro_list_[h] = m;
}

void SearchList::InsertIntoSearchList(Text* name, Macro* m) {
    uint32 h = name->Hash() & MaxHash;
    m->set_name(name);
    m->link_ = SearchList::the_macro_list(h);
    SearchList::set_the_macro_list(h, m);
}

//  addMacro -- This is a little faster than InstallMacro() because it assumes that
//  the name is not already in the macro list.

void SearchList::InstallMacro(const char* namestring, const char* string) {
    Text* name = new Text(namestring);
    SearchList::InsertIntoSearchList(name, new Macro(string));
    delete name;
}

//  lookup - search through the macro list for a text with a specific name.
//  The list is a hash table with links for collisions.

Macro* SearchList::LookupMacro(Text* name) {
    Macro* m = SearchList::the_macro_list(name->Hash() & MaxHash);
    while (m) {
        if (m->IsNameEqual(name)) {
            break;
        }
        m = m->link_;
    }
    return m;
}


//  install - if there is a text in the macro list with this name, set its
//  value. Otherwise, make a new text with this name and value and put
//  it in the list.

void SearchList::InstallMacro(Text* name, Text* value) {
    Macro* m = SearchList::LookupMacro(name);
    if (m) {
        m->set_string(value);
    } else {
        SearchList::InsertIntoSearchList(name, new Macro(value));
    }
}

void SearchList::InstallMacro(Text* name, Macro* value) {
  Macro* m = SearchList::LookupMacro(name);
  if (m) {
    m->set_string(new Text(value));
  } else {
    SearchList::InsertIntoSearchList(name, value);
  }
}

void SearchList::PrintMacroTable() {
    int i;
    for (i = 0; i < (MaxHash + 1); i += 1) {
        Macro* macro = SearchList::the_macro_list(i);
        if (macro) {
            macro->PrintMacroList();
        }
    }
}

Macro* SearchList::GetMacroDefOrInsertNull(Text* name) {
    Macro* t = SearchList::LookupMacro(name);
    if (!t) {
        t = new Macro(0);
        uint32 h = name->Hash() & MaxHash;
        t->set_name(name);
        t->link_ = SearchList::the_macro_list(h);
        SearchList::set_the_macro_list(h, t);
    }
    return t;
}

