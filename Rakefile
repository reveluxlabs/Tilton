require 'rake/clean' 

CLEAN.include('src/*.o')
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
end

# Rule 
rule '.o' => ['.cpp'] do |t|
  sh "g++ #{t.source} -c -o #{t.name}"
end

file "tilton" => OBJ do
  sh "g++ -o tilton #{OBJ}"
end

# File Dependencies
file "tilton.o"      => ['tilton.cpp', 'tilton.h']
file "context.o"     => ['context.cpp', 'context.h']
file "node.o"        => ['node.cpp', 'node.h']
file "text.o"        => ['text.cpp', 'text.h']
file "iter.o"        => ['iter.cpp', 'iter.h']
file "search.o"      => ['search.cpp', 'search.h']
file "function.o"    => ['function.cpp', 'function.h']
