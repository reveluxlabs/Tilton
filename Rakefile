require 'rake/clean' 

CLEAN.include('src/*.o')
CLEAN.include('src/lint/')
CLEAN.include('src/*.sav')
CLOBBER.include('tilton')

task :default => :build 

SRC = FileList['src/*.cpp']
OBJ = SRC.ext('o')

desc "Build Tilton" 
task :build => ["tilton", :test]

desc "Run tests"
task :test do
  sh "specrb -a"
  sh "specrb test/spec_builtins.rb"
  sh "specrb test/spec_text.rb"
  sh "specrb test/spec_args.rb"
end

desc "Run cpplint"
task :lint => ["src/lint"] do
  SRC.each do |f|
    name = File.basename(f)
    %x[ cpplint.py "src/#{name}" 2> "src/lint/#{name}.lint" ]
    name = name.ext('h')
    %x[ cpplint.py "src/#{name}" 2> "src/lint/#{name}.lint" ]
  end
end

directory "src/lint"

# Rule 
rule '.o' => ['.cpp'] do |t|
  sh "g++ #{t.source} -c -o #{t.name}"
end

file "tilton" => OBJ do
  sh "g++ -o tilton #{OBJ}"
end

# File Dependencies
file "tilton.o"      => ['tilton.cpp', 'tilton.h', 'context.o', 'node.o', 'function.o', 'option.o']
file "context.o"     => ['context.cpp', 'context.h', 'tilton.h', 'byte_stream.o', 'node.o', 'hash_table.o', 'text.o', 'macro.o']
file "node.o"        => ['node.cpp', 'node.h', 'tilton.h']
file "text.o"        => ['text.cpp', 'text.h', 'tilton.h', 'macro.o']
file "byte_stream.o" => ['byte_stream.cpp', 'byte_stream.h', 'tilton.h']
file "hash_table.o"  => ['hash_table.cpp', 'hash_table.h', 'tilton.h']
file "function.o"    => ['function.cpp', 'function.h', 'tilton.h', 'hash_table.o', 'node.o', 'macro.o', 'context.o']
file "macro.o"       => ['macro.cpp', 'macro.h', 'tilton.h']
file "option.o"      => ['option.cpp', 'option.h', 'tilton.h']
