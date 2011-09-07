// option.h: processing command-line options for Tilton

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

#ifndef SRC_OPTION_H_
#define SRC_OPTION_H_

class Context;
class Text;

// OptionProcessor -- coordinator for command-line option processing

class OptionProcessor {
 public:
  OptionProcessor();
  virtual ~OptionProcessor();

	virtual bool ProcessOption(int argc, const char * argv[], const char * arg,
	                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// EvalProcessor -- processor for the eval option

class EvalProcessor: public OptionProcessor {
 public:
  // -eval expression
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// GoProcessor -- processor for the go option

class GoProcessor: public OptionProcessor {
 public:
  // -go (process the standard input now)
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// HelpProcessor -- processor for the help option

class HelpProcessor: public OptionProcessor {
 public:
  // -help
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// IncludeProcessor -- processor for the include option

class IncludeProcessor: public OptionProcessor {
 public:
  // -include filespec
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// MuteProcessor -- processor for the mute option

class MuteProcessor: public OptionProcessor {
 public:
  // -mute
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// NoProcessor -- processor for the no option

class NoProcessor: public OptionProcessor {
 public:
  // -no (do not process the standard input)
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// ReadProcessor -- processor for the read option

class ReadProcessor: public OptionProcessor {
 public:
  // -read filespec
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// SetProcessor -- processor for the set option

class SetProcessor: public OptionProcessor {
 public:
  // -set name string
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// WriteProcessor -- processor for the write option

class WriteProcessor: public OptionProcessor {
 public:
  // -write filespec
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// DigitProcessor -- processor for the DIGIT option

class DigitProcessor: public OptionProcessor {
 public:
  // -DIGIT
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

// ParameterProcessor -- processor for parameters on the command line

class ParameterProcessor : public OptionProcessor {
 public:
  // parameter
  bool ProcessOption(int argc, const char * argv[], const char * arg,
                   int &cmd_arg, int &frame_arg, Context* top_frame, Text* in);
};

#endif  // SRC_OPTION_H_
