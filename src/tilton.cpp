// Copyright (c) 2011 Revelux Labs, LLC. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "tilton.h"

#include <stdio.h>
#include <string.h>
#include <map>

#include "context.h"
#include "node.h"
#include "function.h"
#include "option.h"

MacroProcessor::MacroProcessor() {
  top_frame_  = new Context(NULL, NULL);
  in_         = new Text();
  the_output_ = new Text(1024);
}

MacroProcessor::~MacroProcessor() {
  while ( option_processors_.begin() != option_processors_.end() ) {
    delete option_processors_.begin()->second;
    option_processors_.erase(option_processors_.begin());
  }

  delete the_output_;
  delete top_frame_;
  delete in_;
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

void MacroProcessor::InstallTiltonMacros() {
  HashTable* macro_table = MacroTable::instance()->macro_table();

  macro_table->InstallMacro("gt", ">");
  macro_table->InstallMacro("lt", "<");
  macro_table->InstallMacro("tilde", "~");
  macro_table->InstallMacro("tilton", "0.7");
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
      iter = option_processors_.find(arg[1]);
      if ( iter != option_processors_.end() ) {  // valid arg
        go = iter->second->ProcessOption(argc, argv, arg, cmd_arg,
                                         frame_arg, top_frame_, in_, the_output_);
      } else {  // digit arg or invalid
        iter = option_processors_.find('d');
        go = iter->second->ProcessOption(argc, argv, arg, cmd_arg,
                                         frame_arg, top_frame_, in_, the_output_);
      }
    } else {  // parameter, stuff it into the next slot in the frame
        iter = option_processors_.find('p');
        go = iter->second->ProcessOption(argc, argv, arg, cmd_arg,
                                         frame_arg, top_frame_, in_, the_output_);
    }
  }
  return go;
}

void MacroProcessor::Run(bool go) {
  // Process the input
  if (go) {
    in_->ReadStdInput();
    in_->set_name("[standard input]");
    top_frame_->ParseAndEvaluate(in_, the_output_);
  }

  // and finally
  the_output_->WriteStdOutput();
}

// Singleton implementation for macro list

MacroTable::MacroTable() {
  macro_table_ = new HashTable();
}

MacroTable::~MacroTable() {
  delete macro_table_;
}

MacroTable* MacroTable::pInstance = 0;

MacroTable* MacroTable::instance() {
  if ( pInstance == 0 ) {
    pInstance = new MacroTable;
  }
  return pInstance;
}

// main
// Registers the built-ins, processes the command line arguments and
// evaluates the standard input.

int main(int argc, const char * argv[]) {
  bool should_go                   = true;

  MacroProcessor* tilton_processor = new MacroProcessor;
  
  tilton_processor->CreateOptionProcessors();
  tilton_processor->InstallTiltonMacros();

  FunctionContext::instance()->RegisterTiltonFunctions();

  should_go = tilton_processor->ProcessCommandLine(argc, argv);

  tilton_processor->Run(should_go);

  return 0;
}
