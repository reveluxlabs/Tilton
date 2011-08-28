The Tilton Macro Processor
=========================

<~success~n~macros~>

Douglas Crockford
July-2002

JR
August-2011

Tilton
------

Tilton is a simple macro processor. It is small, portable, and Unicode compatible. 
It is an even uglier language than Lisp because it replaces the parens with angle brackets and tildes 
while lacking any useful data structures.

Macro expressions are completely contained within a set of macro brackets.

    <~ ~>

Macro arguments are separated by tildes. Adjacency is concatenation.

Tilton only uses those three characters. All other characters are treated literally, even whitespace. 
This makes Tilton uncommonly good at dealing with textual content.

The character set used by Tilton is the UTF-8 form of Unicode. Tilton's character functions 
(length and substr) deal in whole Unicode characters, not in bytes.

The [original](http://www.crockford.com/tilton/) for Tilton was written by 
[Douglas Crockford](http://www.crockford.com/) of Javascript fame.
This version has been:

*  Ported to OS X
*  Supplemented with automated tests
*  Refactored for better understanding.

Easy
----

In its easiest form, Tilton requires no programming. For example, the shell command

    tilton deluxe bogus <base >result

will replace all occurrences of <~1~> in the file base with deluxe and will replace all 
occurrences of <~2~> with bogus and store the result in the file result.

The shell command

    tilton -s name "Carl Hollywood"

will replace all occurances of <~name~> with Carl Hollywood in the standard input.

Tilton has two functions that can be used to include additional files. These can be 
embedded in the input text.

    <~read~filename~>

The read function reads a file and inserts its contents.

    <~include~filename~parameters~>

The include function reads a file, but evaluates it before inserting it. It can take a 
set of parameters and do substitutions with them.

These functions can be used together, so that the statement

    <~include~macros<~3~>.tilton~>

will use the third command line argument to select the name of a file to include. If the 
third argument is "mac", then the file included will be macrosmac.tilton.

Macros
------

A macro expression is

    <~macroname~firstArgument~secondArgument...~>

If macroname matches the name of a known macro, then the macro is expanded with the arguments, 
and the macro expression is replaced with its result. Macro expressions can be nested to any depth.

Extra arguments are usually ignored. Missing arguments are usually treated as empty text. 
Some of the built-in macros report errors if the correct number of parameters is not provided.

Tilton does lazy evaluation. Each parameter of a macro is evaluated zero or one times. 
A parameter is not evaluated until its value is needed. That value is reused if the parameter 
needs to be evaluated again. This makes it possible to write functions like min without worrying 
about side-effects due to multiple evaluations of the arguments. It is possible to force 
evaluation of arguments with the mute function.

There are no rules on macro names. You can call them anything you like, with any combination 
of characters. Names are case sensitive.

The terms macros and variables are used interchangeably. The values stored in them are strings.

Strings of digits can be treated as though they are integers.

The flexibility of naming can be used to simulate arrays.

    <~set~subscript~18~><~set~myArray[<~subscript~>]~42~>

Because subscript has a value of 18, we will assign 42 to a variable called myArray[18]. 
Note that the brackets are not special in Tilton.

The macro expressions <~0~> thru <~9~> are used to access parameters. They can also be 
used as local variables. <~1~> gets the value of the first argument. <~1~value~> sets it to a new value.

Built-Ins
---------

These are the built-in macros:

    a single   -  <~0~> <~1~> <~2~> <~3~> <~4~> <~5~> <~6~> <~7~> <~8~> <~9~>
    digit 
                  Digit macros are used to access arguments. They can also be used as local variables, 
                  for example: <~8~value~>


    add        -  <~add~number1~number2...~>

                  This function can take up to nine arguments. It produces the sum. If there are no 
                  arguments, it returns 0.


    and        -  <~and~value1~value2...~>

                  Each of the values is evaluated in turn, until a null value (or 
                  empty string) is found. If all of the arguments have values, then 
                  the result is the last argument.


    append     -  <~append~name~value1~value2...~>

                  The values are evaluated and appended to the named variable.


    define     -  <~define~name~macrobody~>

                  Same as set, except that the macrobody is not evaluated. It is used to make user 
                  defined macros. It is equivalent to <~set~name~<~literal~macrobody~>~>

                  There are 9 macro expressions that can be placed inside of macrobodies that 
                  are replaced with parameters when the macro is evaluated:

                  <~1~> <~2~> <~3~> <~4~> <~5~> <~6~> <~7~> <~8~> <~9~>

                  <~1~> is replaced by the value of the first parameter, and so on. Also 
                  available is <~0~>, which returns the macro's name.


    defined?   -  <~defined?~name~trueValue~falseValue~>

                  If the named variable exists, it produces the trueValue. Otherwise, 
                  it produces the optional falseValue.


    delete     -  <~delete~name1~name2...~>

                  The named variables and macros are deleted. If a named variable does not exist, no error. 
                  This is different from setting a variable to an empty value because it causes the variable 
                  to not exist.

                  <~delete~foo~bar~>

                  is shorthand for

                  <~delete~foo~><~delete~bar~>


    div        -  <~div~value1~value2~>

                  Both of the arguments must be numbers. The first value is divided by the second value. 
                  The result is the integer quotient. If the second value is zero, then nothing is produced.


    dump       -  <~dump~>

                  This is used for debugging. It prints everything that has been defined or set.


    entityify  -  <~entityify~arg~>

                  Replace certain special characters with their HTML entity equivalents. This 
                  escapes characters for HTML.

                    '  -  &#039;

                    "  -  &quot;

                    \  -  &#092;

                    &  -  &amp;

                    <  -  &lt;

                    >  -  &gt;

                    ~  -  &#126;


    eq?       -  <~eq?~firstArg~secondArg~trueValue~falseValue~>

                 The two arg strings are compared. If they are exactly the same, then the 
                 true value is evaluated. Otherwise, the false value is evaluated. The false value is optional.

                 It can also be used as a case statement.

                 <~eq?~caseValue~case1~value1~case2~value2~...~defaultValue~>

                 The result is the first value whose case matches the case value.


    eval      -  <~eval~string~value...~>

                 Evaluate the string, using the values to replace numbered variables in the string.


    first     -  <~first~name~delim...~>

                 Search the variable for the delimiters. The result is the text before the delimiter. 
                 That text and the delimiter are deleted from the variable. <~0~> is set to the 
                 matched delimiter. The function is handy for parsing.


    ge?       -  <~ge?~firstArg~secondArg~trueValue~falseValue~>

                 The two arg strings are compared. If the first is greater than or equal to the second, 
                 then the true value is evaluated. Otherwise, the false value is evaluated. The false 
                 value is optional. If both arguments are numbers, then they are compared as numbers. 
                 Otherwise, they are compared as strings.


    gensym    -  <~gensym~>

                 This produces a four-digit sequence number.


    get       -  <~get~name1~name2...~>

                 Get the value of a variable without evaluating it. Any number of variables may be named, so

                 <~get~foo~bar~>

                 is shorthand for

                 <~get~foo~><~get~bar~>

                 If the variable or macro cannot be found, then there is an error.


    gt        -  <~gt~>

                 This produces the > character.


    gt?       -  <~gt?~firstArg~secondArg~trueValue~falseValue~>

                 The two arg strings are compared. If the first is greater than the second, then 
                 the true value is evaluated. Otherwise, the false value is evaluated. The false value 
                 is optional. If both arguments are numbers, then they are compared as numbers. Otherwise, 
                 they are compared as strings.


    include   -  <~include~filespec~arg1~arg2...~>

                 Same as read, except that the contents of the file are evaluated. The include expression 
                 can contain parameters that can replace parameter expressions.


    last      -  <~last~name~delim...~>

                 Search the variable for the last delimiter. The result is the text after the delimiter. 
                 That text and the delimiter are deleted from the variable. <~0~> is set to the matched 
                 delimiter. The function is handy for parsing.


    length    -  <~length~string~>

                 The number of Unicode characters encoded in the UTF-8 string is returned.


    le?       -  <~le?~firstArg~secondArg~trueValue~falseValue~>

                 The two arg strings are compared. If the first is less than or equal the second, 
                 then the true value is evaluated. Otherwise, the false value is evaluated. The 
                 false value is optional. If both arguments are numbers, then they are compared as 
                 numbers. Otherwise, they are compared as strings.


    literal   -  <~literal~string~>

                 This produces the string without evaluating it.


    loop      -  <~loop~condition~body~>

                 Evaluate the condition. If it is not null (empty string) evaluate the body. Repeat.


    lt        -  <~lt~>

                 This produces the < character.


    lt?       -  <~lt?~firstArg~secondArg~trueValue~falseValue~>

                 The two arg strings are compared. If the first is less than the second, then the 
                 true value is evaluated. Otherwise, the false value is evaluated. The false value 
                 is optional. If both arguments are numbers, then they are compared as numbers. Otherwise, 
                 they are compared as strings.


    mod       -  <~mod~value1~value2~>

                 Both of the arguments must be numbers. The first value is divided by the second value. 
                 The result is the integer remainder. If the second value is zero, then nothing is produced.


    mult      -  <~mult~value1~value2...~>

                 All of the arguments must be numbers. Any number of arguments can supplied. The result 
                 is the product. If there are no arguments, it returns 1.


    mute      -  <~mute~arguments...~>

                 All of the arguments are evaluated, but their output is suppressed. This macro can be 
                 used when a macro is to be used only for its side-effects, or to provide annotation and 
                 whitespace to definitions without cluttering the output.


    ne?       -  <~ne?~firstArg~secondArg~trueValue~falseValue~>

                 The two arg strings are compared. If they are exactly the same, then the false value 
                 is evaluated. Otherwise, the true value is evaluated. The false value is optional.


    null      -  <~null~This is a swinging comment!~>

                 The null macro does not evaluate its arguments, and it does not produce a value. It 
                 can be used to insert comments into Tilton files. If the arguments contain macro brackets, 
                 they must balance.


    number?   -  <~number?~string~trueValue~falseValue~>

                 If the string argument is a number (containing one or more decimal digits 
                 with an optional leading minus sign), then the true value is evaluated. 
                 Otherwise, the optional false value is evaluated.


    or        -  <~or~value1~value2...~>

                 Each of the values is evaluated in turn, until a non-null value is found. 
                 That value is the result. If none of the arguments have values, then there 
                 is no result.


    print     -  <~print~value~>

                 The value is evaluated and output to the alternate output channel.


    read      -  <~read~filespec~>

                 The named file is opened and read. It is similar to get, except the text comes 
                 from a file instead of a variable. If the file does not exist or is not 
                 accessible, it is an error.


    rep       -  <~rep~value~number~>

                 Produce as many copies of the value as determined by the number.


    set       -  <~set~name~value~>

                 A variable is created with the name of the first parameter and the 
                 value of the second parameter. If the value parameter is omitted, then 
                 the variable will be emptied. (See delete.)


    slashify  -  <~slashify~arg~>

                 Insert \ characters before \ and ' and " . This escapes characters for JavaScript.


    stop      -  <~stop~reason~>

                 This stops the program without writing the output. It is used to halt when an error is detected.


    sub       -  <~sub~value1~value2~>

                 Both of the arguments must be numbers or it is an error. The second value is subtracted 
                 from the first. The result is the difference.


    substr    -  <~substr~string~position~length~>

                 Both position and length must be numbers or it is an error. Length is optional. 
                 A substring is produced. position identifies the first character of the substring. 
                 If it is zero, then the substring includes the first character of the string. The 
                 result may be shorter than the requested length. UTF-8 encoding.


    tilde     -  <~tilde~>

                 This produces the ~ character.


    tilton   -  <~tilton~>

                 This produces the current version of the Tilton Macro Processor. Currently it returns 0. 
                 It will be incremented as the language evolves through the evolutionary process of evolution.


    trim      -  <~trim~string~>

                 The result is a string in which the leading and trailing whitespace is removed, and 
                 all remaining runs of whitespace are replaced with single spaces.


    unicode   -  <~unicode~number1~number2~...~>$    

                 Convert the numbers to Unicode characters. <~unicode~67~97~116~> is equivalent to Cat.


    write     -  <~write~filespec~value~>

                 The value is written to the named file, replacing its previous contents (if any).


Using these functions, you can create your own macros. There are examples below.

User-defined macros can replace built-in macros.

Running Tilton
--------------

Tilton interprets its command line argument list as a list of parameters. It 
assigns them to <~0~> thru <~9~>. <~0~> is usually the name of the program. 
Missing parameters are treated as empty strings. Extra parameters are ignored. 
It then reads the standard input and evaluates it. When it is finished, if there 
were no errors, it writes the result to the standard output. <~print~>, 
<~dump~>, <~stop~>, and errors write to stderr.

Tilton also provides some command line options.

    -e expression  - Evaluate the Tilton expression. It is usually necessary to put the 
                     expression in quotes. Equivalent to the *eval* macro.

    -g             - Go ahead and process the standard input now. The default is to process it 
                     after all of the command line parameters have been processed. 
                     Equivalent to the *go* macro.

    -h             - Display a summary of the command line options. Equivalent to the *help* macro.

    -i filespec    - Include the named file, evaluate it using the current command line arguments. 
                     Equivalent to the *include* macro.

    -m             - Discard the output generated by -r, -i, -g, or -e so far. Equivalent to the *mute* macro.

    -n             - Do not process the standard input. Equivalent to the *eval* macro.

    -r filespec    - Read the named file and copy it to the output stream. Equivalent to the *read* macro.

    -s name value  - Set a variable with the supplied name and value. Equivalent to the *set* macro.

    -w filespec    - Capture the output stream up to this point and write it to the named file. 
                     Equivalent to the *write* macro.

    -digit value   - The next non-option item in the command line will go to <~digit~value~>. Additional 
                     values will be assigned to the succeeding digits.

Examples
--------

### Last Name, First

    <~define~last, first~<~last name~>, <~first name~>~>

So, if <~set~first name~Carl~><~set~last name~Hollywood~> then <~last, first~> 
produces Hollywood, Carl.

<~get~last, first~> produces <~last name~>, <~first name~>.

### Minimum

    <~define~min~<~lt?~<~1~>~<~2~>~<~1~>~<~2~>~>~>

### Factorial

    <~define~!~<~lt?~<~1~>~3~<~1~>~<~mult~<~!~<~sub~<~1~>~1~>~>~<~1~>~>~>~>

So <~!~5~> produces 120.

### Absolute value

The numerical approach to absolute value is to take the larger of the number 
and the number subtracted from zero:

    <~define~abs~<~9~<~sub~0~<~1~>~>~><~lt?~<~1~>~<~9~>~<~9~>~<~1~>~>~>

The textual approach is to notice if it starts with a minus sign, and if 
it does, remove it:

    <~define~abs~<~eq?~<~substr~<~1~>~0~1~>~-~<~substr~<~1~>~1~>~<~1~>~>~>

Errors
------

If Tilton has a problem, it will print an error message and stop. 
The message will contain a header

    sourceFile(lineNumber,columnNumber/characterNumber):

The header will contain an entry for each level in Tilton's stack. I hope 
that this will provide enough information to identify the cause of the error.


The Tilton Macro Processor is named for the famous Christian Recreationalist, 
TV's Robert Tilton. He was selected for this honor on account of his name 
miraculously containing many of the same letters as the word "tilde", the 
most important character in this language. Robert Tilton is himself something 
of a linguist, being fluent in the speaking of tongues.

Boring Legal Stuff
------------------

Copyright (c) 2011 Revelux Labs, LLC.
 
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
