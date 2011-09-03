// tilton.cpp
// For interface definition, see tilton.h

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

#include <stdio.h>
#include <string.h>
#include "tilton.h"
#include "text.h"
#include "iter.h"
#include "context.h"
#include "node.h"
#include "search.h"
#include "function.h"

// MAXHASH is the largest index in the hash table. It must be (2**n)-1.
#define MAXHASH 1023

SearchList* macroTable = new SearchList();
Text* theOutput = NULL;

// main processes the command line arguments and evaluates the standard
// input.

int main(int argc, const char * argv[]) 
{
    bool go = true;
    Text* name = NULL;
    Text* string = NULL;
    const char* arg;
    int i = 0; // index of argv
    int j = 0; // index of context parameter
    Context* context = new Context(NULL, NULL);
    Text* in = new Text();
    theOutput = new Text(1024);
    FunctionContext* builtins = new FunctionContext;
    
    builtins->registerTiltonFunctions(macroTable);


// process the command line arguments

    i = 0;
    while (i < argc) {
        arg = argv[i];
        i += 1;

// if an arg starts with '-', then it is special, Otherwise, stuff it into 
// the next slot in the context. We match just the first letter of commands.

        if (arg[0] == '-') {
            switch (arg[1]) {

// -eval expression
            case 'e': 
                if (i < argc) {
                    string = new Text(argv[i]);
                    i += 1;
                    string->setName("[eval]");
                    context->eval(string);
                    delete string;
                } else {
                    context->error("Missing expression on -eval");
                }
                break;

// -go (process the standard input now)
            case 'g': 
                in->input();
                in->setName("[go]");
                context->eval(in);
                go = false;
                break;

// -help
            case 'h':
                printf("  tilton command line parameters:\n"
                    "    -eval <tilton expression>\n"
                    "    -go\n"
                    "    -help\n"
                    "    -include <filespec>\n"
                    "    -mute\n"
                    "    -no\n"
                    "    -read <filespec>\n"
                    "    -set <name> <value>\n"
                    "    -write <filespec>\n"
                    "    -<digit>\n"
                    "  http://www.crockford.com/tilton");
                return 0;

// -include filespec
            case 'i': 
                if (i < argc) {
                    name = new Text(argv[i]);
                    i += 1;
                    string = new Text();
                    if (!string->read(name)) {
                        context->error("Error in -include", name);
                    }
                    context->eval(string);
                    delete name;
                    delete string;
                } else {
                    context->error("Missing filename on -include");
                }
                break;

// -mute
            case 'm': 
                theOutput->length = 0;
                break;

// -no (do not process the standard input)
            case 'n': 
                go = false;
                break;

// -read filespec
            case 'r': 
                if (i < argc) {
                    name = new Text(argv[i]);
                    i += 1;
                    string = new Text();
                    if (!string->read(name)) {
                        context->error("Error in -read", name);
                    }
                    theOutput->append(string);
                    delete name;
                    delete string;
                } else {
                    context->error("Missing filename on -read");
                }
                break;

// -set name string
            case 's': 
                if (i + 1 < argc) {
                    name = new Text(argv[i]);
                    i += 1;
                    string = new Text(argv[i]);
                    i += 1;
                    macroTable->install(name, string);
                    delete name;
                    delete string;
                } else {
                    context->error("Missing parameter on -set ");

                }
                break;

// -write filespec
            case 'w': 
                if (i < argc) {
                    name = new Text(argv[i]);
                    i += 1;
                    if (!theOutput->write(name)) {
                        context->error("Error in -write", name);
                    }
                    theOutput->length = 0;
                    delete name;
                } else {
                    context->error("Missing filename on -write");
                }
                break;

// -DIGIT
            default: 
                int k = arg[1] - '0';
                if (k >= 0 && k <= 9 && i < argc) {
                    j = k;
                } else {

// none of the above
                    context->error("Unrecognized command line parameter", 
                                   new Text(arg));
                }
            }
        } else {

// parameter text
            context->getNode(j)->value = new Text(arg);
            j += 1;
        }
    }

// Process the input

    if (go) {
        in->input();
        in->setName("[standard input]");
        context->eval(in);
    }

// and finally


    theOutput->output();
    return 0;
}
