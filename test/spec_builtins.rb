require 'test/spec'
require 'test/test_helper'

describe "A command line macro processor (in general)" do

  it "should process the add builtin" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~add~1~2~3~>" | ./tilton ]
    # verify results
    result.should.include "6"
  end

  it "should process the add builtin with a macro arg" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~1~3~><~add~1~2~<~1~>~>" | ./tilton ]
    # verify results
    result.should.include "6"
  end

  it "should process the and builtin" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~and~1~2~3~>" | ./tilton ]
    # verify results
    result.should.include "3"
  end

  it "should process the append builtin" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~append~num~1~2~3~><~num~>" | ./tilton ]
    # verify results
    result.should.include "123"
  end

  it "should process the define builtin" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~define~EOF~-1~><~EOF~>" | ./tilton ]
    # verify results
    result.should.include "-1"
  end

  it "should process the defines lazily" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~define~x~1~><~define~y~x~><~y~>" | ./tilton ]
    # verify results
    result.should.include "x"
  end

  it "should process the get builtin" do
    # setup fixture
    define = "<~define~last, first~<~last name~>, <~first name~>~>"
    input = "<~get~last, first~>"
    output = "<~last name~>, <~first name~>"
    text = define + input
    # execute SUT
    result = %x[ echo "#{text}" | ./tilton ]
    # verify results
    result.should.include output
  end

  it "should process the read builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~read~test/front.snip~>" | ./tilton ]
    # verify results
    result.size.should.be 1939
  end

  it "should process the include builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~include~test/head.snip~Revelux~>" | ./tilton ]
    # verify results
    result.should.include "%title Revelux"
  end

it "should process the include builtin with null args" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~include~test/head.snip~~front~~~>" | ./tilton ]
    # verify results
    result.should.include "%body.front"
  end

# TC's needed
# extra args in input
# report correctly a missing definition
end
