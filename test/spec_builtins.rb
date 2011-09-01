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

  it "should produce an error msg on the add builtin with a bad arg" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~add~1~Tilton~3~>" | ./tilton ]
    # verify results
    result.should.include "<~add~> Not a number: Tilton"
  end

  it "should process the add builtin with a macro arg" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~1~3~><~add~1~2~<~1~>~>" | ./tilton ]
    # verify results
    result.should.include "6"
  end

  it "should process the add builtin with an undefined macro arg" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~add~1~2~<~1~>~>" | ./tilton ]
    # verify results
    result.should.include "<~add~> Not a number: "
  end

  it "should process the and builtin" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~and~1~2~3~>" | ./tilton ]
    # verify results
    result.should.include "3"
  end

  it "should process the and builtin with a macro arg" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~1~3~><~and~1~2~<~1~>~>" | ./tilton ]
    # verify results
    result.should.include "3"
  end

  it "should process the and builtin with an undefined macro arg" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~and~1~2~<~1~>~>" | ./tilton ]
    # verify results
    result.should.equal "\n"
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

  it "should return the true value for the defined? builtin when the macro exists" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~define~EOF~-1~><~defined?~EOF~yes~no~>" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should return the false value for the defined? builtin when the macro does not exist" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~defined?~EOF~yes~no~>" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process the delete builtin when the macro exists" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~define~EOF~-1~><~delete~EOF~><~EOF~>" | ./tilton ]
    # verify results
    result.should.include "Undefined macro"
  end

  it "should process the delete builtin when the variable exists" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~define~1~-1~><~delete~1~><~1~>" | ./tilton ]
    # verify results
    result.should.equal "\n"
  end

  it "should process the delete builtin when the variable does not exist" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~delete~1~><~1~>" | ./tilton ]
    # verify results
    result.should.equal "\n"
  end

  it "should process the div builtin" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~div~6~3~>" | ./tilton ]
    # verify results
    result.should.include "2"
  end

  it "should produce an error msg on the div builtin with a bad arg" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~div~6~Tilton~>" | ./tilton ]
    # verify results
    result.should.include "<~div~> Not a number: Tilton"
  end

  it "should produce an error msg on the div builtin dividing by zero" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~div~6~0~>" | ./tilton ]
    # verify results
    result.should.equal "\n"
  end

  it "should process the div builtin with a macro arg" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~1~3~><~div~6~<~1~>~>" | ./tilton ]
    # verify results
    result.should.include "2"
  end

  it "should process the div builtin with an undefined macro arg" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~div~6~<~1~>~>" | ./tilton ]
    # verify results
    result.should.include "<~div~> Not a number: "
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
