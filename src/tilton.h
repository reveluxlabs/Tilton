// tilton.h: constants for Tilton

//
//  Tilton Macro Processor
//
//  Tilton is a simple macro processor. It is small,
//  portable, and Unicode compatible.
//  Written by Douglas Crockford [ www.crockford.com/tilton ]
//  2006-10-05
//

// Updated for OS X and Debian by JR at Revelux Labs
//
// Version 0.7
// 1Sep11
//
// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
//
// This version of Tilton is licensed under the MIT license.

#ifndef SRC_TILTON_H_
#define SRC_TILTON_H_

#define EOT (-1)

// Tilton numbers are integers.

typedef long number;

// INFINITY is the largest positive integer than can be held in a number.
// This value may be system dependent.

#define INFINITY ((number)0x7FFFFFFF)

// NAN (not a number) is the smallest integer that can be held
// in a number. On two's compliment machines, it is INFINITY + 1.
// This value may be system dependent.

#define NAN ((number)0x80000000)

// Unsigned ints are used in computing hash.

typedef unsigned long int  uint32;   /* unsigned 4-byte quantities */
typedef unsigned      char uint8;    /* unsigned 1-byte quantities */

#endif  // SRC_TILTON_H_
