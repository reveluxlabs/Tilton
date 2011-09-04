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
file "tilton.o"      => ['tilton.cpp', 'tilton.h']
file "context.o"     => ['context.cpp', 'context.h']
file "node.o"        => ['node.cpp', 'node.h']
file "text.o"        => ['text.cpp', 'text.h']
file "iter.o"        => ['iter.cpp', 'iter.h']
file "search.o"      => ['search.cpp', 'search.h']
file "function.o"    => ['function.cpp', 'function.h']
