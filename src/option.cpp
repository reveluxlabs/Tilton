// option.cpp
// For interface definition, see option.h

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
#include "option.h"

#include <stdio.h>
#include <string.h>

#include "text.h"
#include "context.h"
#include "search.h"
#include "node.h"

extern Text* g_the_output;

OptionProcessor::OptionProcessor() {}

OptionProcessor::~OptionProcessor() {}

bool OptionProcessor::ProcessOption(int argc, const char **argv,
                                    const char *arg, int &cmd_arg,
                                    int &frame_arg, Context *top_frame,
                                    Text *in) {
  return true;
}

bool EvalProcessor::ProcessOption(int argc, const char * argv[],
                                  const char * arg, int &cmd_arg,
                                  int &frame_arg, Context* top_frame,
                                  Text* in) {
  Text* string = NULL;
  if (cmd_arg < argc) {
    string = new Text(argv[cmd_arg]);
    cmd_arg += 1;
    string->setName("[eval]");
    top_frame->eval(string);
    delete string;
  } else {
    top_frame->ReportErrorAndDie("Missing expression on -eval");
  }
  return true;
}

bool GoProcessor::ProcessOption(int argc, const char * argv[],
                                const char * arg, int &cmd_arg,
                                int &frame_arg, Context* top_frame,
                                Text* in) {
  in->ReadStdInput();
  in->setName("[go]");
  top_frame->eval(in);
  return false;
}

bool HelpProcessor::ProcessOption(int argc, const char * argv[],
                                  const char * arg, int &cmd_arg,
                                  int &frame_arg, Context* top_frame,
                                  Text* in) {
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
         "  http://github.com/reveluxlabs/Tilton\n");
  return false;
};


bool IncludeProcessor::ProcessOption(int argc, const char * argv[],
                                     const char * arg, int &cmd_arg,
                                     int &frame_arg, Context* top_frame,
                                     Text* in) {
  Text* name = NULL;
  Text* string = NULL;
  if (cmd_arg < argc) {
    name = new Text(argv[cmd_arg]);
    cmd_arg += 1;
    string = new Text();
    if (!string->read(name)) {
      top_frame->ReportErrorAndDie("Error in -include", name);
    }
    top_frame->eval(string);
    delete name;
    delete string;
  } else {
    top_frame->ReportErrorAndDie("Missing filename on -include");
  }

  return true;
};

bool MuteProcessor::ProcessOption(int argc, const char * argv[],
                                  const char * arg, int &cmd_arg,
                                  int &frame_arg, Context* top_frame,
                                  Text* in) {
  g_the_output->length_ = 0;
  return true;
};

bool NoProcessor::ProcessOption(int argc, const char * argv[],
                                const char * arg, int &cmd_arg,
                                int &frame_arg, Context* top_frame,
                                Text* in) {
  return false;
};


bool ReadProcessor::ProcessOption(int argc, const char * argv[],
                                  const char * arg, int &cmd_arg,
                                  int &frame_arg, Context* top_frame,
                                  Text* in) {
  Text* name = NULL;
  Text* string = NULL;
  if (cmd_arg < argc) {
  name = new Text(argv[cmd_arg]);
  cmd_arg += 1;
  string = new Text();
  if (!string->read(name)) {
    top_frame->ReportErrorAndDie("Error in -read", name);
  }
  g_the_output->append(string);
  delete name;
  delete string;
  } else {
    top_frame->ReportErrorAndDie("Missing filename on -read");
  }

  return true;
};

bool SetProcessor::ProcessOption(int argc, const char * argv[],
                                 const char * arg, int &cmd_arg,
                                 int &frame_arg, Context* top_frame,
                                 Text* in) {
  Text* name = NULL;
  Text* string = NULL;
  if (cmd_arg + 1 < argc) {
    name = new Text(argv[cmd_arg]);
    cmd_arg += 1;
    string = new Text(argv[cmd_arg]);
    cmd_arg += 1;
    MacroTable::instance()->macro_table()->install(name, string);
    delete name;
    delete string;
  } else {
    top_frame->ReportErrorAndDie("Missing parameter on -set ");
  }

  return true;
};

bool WriteProcessor::ProcessOption(int argc, const char * argv[],
                                   const char * arg, int &cmd_arg,
                                   int &frame_arg, Context* top_frame,
                                   Text* in) {
  Text* name = NULL;
  if (cmd_arg < argc) {
    name = new Text(argv[cmd_arg]);
    cmd_arg += 1;
    if (!g_the_output->write(name)) {
      top_frame->ReportErrorAndDie("Error in -write", name);
    }
    g_the_output->length_ = 0;
    delete name;
  } else {
    top_frame->ReportErrorAndDie("Missing filename on -write");
  }

  return true;
};


bool DigitProcessor::ProcessOption(int argc, const char * argv[],
                                   const char * arg, int &cmd_arg,
                                   int &frame_arg, Context* top_frame,
                                   Text* in) {
  int k = arg[1] - '0';
  if (k >= 0 && k <= 9 && cmd_arg < argc) {
    frame_arg = k;
  } else {
    // none of the above
    top_frame->ReportErrorAndDie("Unrecognized command line parameter",
                                  new Text(arg));
  }

  return true;
};


bool ParameterProcessor::ProcessOption(int argc, const char * argv[],
                                       const char * arg, int &cmd_arg,
                                       int &frame_arg, Context* top_frame,
                                       Text* in) {
  // parameter text
  top_frame->GetArgument(frame_arg)->value_ = new Text(arg);
  frame_arg += 1;

  return true;
};
