require 'rake/clean' 

CLEAN.include('src/*.o')
CLOBBER.include('tilton')

task :default => :build 

SRC = FileList['src/*.cpp']
OBJ = SRC.ext('o')

desc "Build Tilton" 
task :build => ["tilton"]

desc "Run tests"
task :test do
  sh "specrb -a"
end

# Rule 
rule '.o' => ['.cpp'] do |t|
  sh "g++ #{t.source} -c -o #{t.name}"
end

file "tilton" => OBJ do
  sh "g++ -o tilton #{OBJ}"
end

# File Dependencies
file "tilton.o"  => ['tilton.cpp', 'tilton.h']
file "context.o" => ['context.cpp', 'context.h']
file "node.o"    => ['node.cpp', 'node.h']
file "text.o"    => ['text.cpp', 'text.h']
file "iter.o"    => ['iter.cpp', 'iter.h']
