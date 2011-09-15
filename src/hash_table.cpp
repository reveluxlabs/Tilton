// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "hash_table.h"

#include <stdlib.h>

#include "tilton.h"
#include "text.h"
#include "macro.h"

HashTable::HashTable() {
    int i;

    for (i = kMaxHash; i >= 0; i -= 1) {
        the_macro_list_[i] = NULL;
    }
}

HashTable::~HashTable() {}

Macro* HashTable::the_macro_list(uint32 h) const {
    return the_macro_list_[h];
}

void HashTable::set_the_macro_list(uint32 h, Macro* m) {
    the_macro_list_[h] = m;
}

void HashTable::InsertIntoHashTable(Text* name, Macro* m) {
    uint32 h = name->Hash() & kMaxHash;
    m->set_name(name);
    m->link_ = HashTable::the_macro_list(h);
    HashTable::set_the_macro_list(h, m);
}

void HashTable::InstallMacro(const char* namestring, const char* string) {
    Text* name = new Text(namestring);
    HashTable::InsertIntoHashTable(name, new Macro(string));
    delete name;
}

Macro* HashTable::LookupMacro(Text* name) {
    Macro* m = HashTable::the_macro_list(name->Hash() & kMaxHash);
    while (m) {
        if (m->IsNameEqual(name)) {
            break;
        }
        m = m->link_;
    }
    return m;
}

void HashTable::InstallMacro(Text* name, Text* value) {
    Macro* m = HashTable::LookupMacro(name);
    if (m) {
        m->set_string(value);
    } else {
        HashTable::InsertIntoHashTable(name, new Macro(value));
    }
}

void HashTable::InstallMacro(Text* name, Macro* value) {
  Macro* m = HashTable::LookupMacro(name);
  if (m) {
    m->set_string(new Text(value));
  } else {
    HashTable::InsertIntoHashTable(name, value);
  }
}

void HashTable::PrintMacroTable() {
    int i;
    for (i = 0; i < (kMaxHash + 1); i += 1) {
        Macro* macro = HashTable::the_macro_list(i);
        if (macro) {
            macro->PrintMacroList();
        }
    }
}

Macro* HashTable::GetMacroDefOrInsertNull(Text* name) {
    Macro* t = HashTable::LookupMacro(name);
    if (!t) {
        t = new Macro(0);
        uint32 h = name->Hash() & kMaxHash;
        t->set_name(name);
        t->link_ = HashTable::the_macro_list(h);
        HashTable::set_the_macro_list(h, t);
    }
    return t;
}

