//  tilton.h: constants for Tilton
//  2006-10-05

//  The Tilton Macro Processor

//  Douglas Crockford
//  http://www.crockford.com/tilton

//  This program is Open Source and Public Domain.

#ifndef __TILTON_H_
#define __TILTON_H_

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

#endif //__TILTON_H_
