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
// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
//
// This version of Tilton is licensed under the MIT license.
#include "tilton.h"

#include <stdio.h>
#include <string.h>
#include <map>

#include "context.h"
#include "node.h"
#include "function.h"
#include "option.h"

SearchList* g_macro_table = new SearchList();
Text* g_the_output = NULL;

MacroProcessor::MacroProcessor() {
  top_frame_  = new Context(NULL, NULL);
  in_         = new Text();
  the_output_ = new Text(1024);
}

MacroProcessor::~MacroProcessor() {
  delete top_frame_;
  delete in_;
  delete the_output_;

  while( option_processors_.begin() != option_processors_.end() )
  {
    delete option_processors_.begin()->second;
    option_processors_.erase( option_processors_.begin() );
  }

}

void MacroProcessor::CreateOptionProcessors() {
  option_processors_.insert(std::make_pair('e', new EvalProcessor()));
  option_processors_.insert(std::make_pair('g', new GoProcessor()));
  option_processors_.insert(std::make_pair('h', new HelpProcessor()));
  option_processors_.insert(std::make_pair('i', new IncludeProcessor()));
  option_processors_.insert(std::make_pair('m', new MuteProcessor()));
  option_processors_.insert(std::make_pair('n', new NoProcessor()));
  option_processors_.insert(std::make_pair('r', new ReadProcessor()));
  option_processors_.insert(std::make_pair('s', new SetProcessor()));
  option_processors_.insert(std::make_pair('w', new WriteProcessor()));
  option_processors_.insert(std::make_pair('d', new DigitProcessor()));
  option_processors_.insert(std::make_pair('p', new ParameterProcessor()));
}

bool MacroProcessor::ProcessCommandLine(int argc, const char* *argv) {
  bool go = true;
  const char* arg;
  int cmd_arg   = 0;  // index of argv
  int frame_arg = 0;  // index of context parameter
  std::map<char, OptionProcessor*>::const_iterator iter;

  // process the command line arguments
  cmd_arg = 0;
  while (cmd_arg < argc) {
    arg = argv[cmd_arg];
    cmd_arg += 1;

    if (arg[0] == '-') {  // args
      iter = option_processors_.find( arg[1] );
      if( iter != option_processors_.end() ) {  // valid arg
        go = iter->second->ProcessOption(argc, argv, arg, cmd_arg,
                                         frame_arg, top_frame_, in_);
      } else {  // digit arg or invalid
        iter = option_processors_.find( 'd' );
        go = iter->second->ProcessOption(argc, argv, arg, cmd_arg,
                                         frame_arg, top_frame_, in_);
      }
    } else {  // parameter, stuff it into the next slot in the frame
        iter = option_processors_.find( 'p' );
        go = iter->second->ProcessOption(argc, argv, arg, cmd_arg,
                                         frame_arg, top_frame_, in_);
    }
  }
  return go;
}

void MacroProcessor::Run(bool go) {
  // Process the input
  if (go) {
    in_->ReadStdInput();
    in_->setName("[standard input]");
    top_frame_->eval(in_);
  }

  // and finally
  g_the_output->output();
}

// main
// Registers the built-ins, processes the command line arguments and
// evaluates the standard input.

int main(int argc, const char * argv[]) {
  bool should_go                   = true;
  g_the_output                     = new Text(1024);
  MacroProcessor* tilton_processor = new MacroProcessor;
  FunctionContext* builtins        = new FunctionContext;

  tilton_processor->CreateOptionProcessors();
  
  builtins->RegisterTiltonFunctions(g_macro_table);

  should_go = tilton_processor->ProcessCommandLine(argc, argv);

  tilton_processor->Run(should_go);

  return 0;
}
