require 'test/spec'
require 'test/test_helper'

describe "A  command line macro process (in general)" do

  it "should pass through plain text unchanged" do
    # setup fixture
    name = "Carl_Hollywood"
    # execute SUT
    result = %x[ echo "Now is the time for all good me to come to the aide of their country." | ./tilton ]
    # verify results
    result.should.include "Now is the time for all good me to come to the aide of their country."
  end

  it "should null args in the input that are not provided" do
    # setup fixture
    name = "Carl_Hollywood"
    # execute SUT
    result = %x[ echo "<~1~><~2~>" | ./tilton -2 Charlie ]
    # verify results
    result.should.include "Charlie"
  end

  it "should take named args from the command line" do
    # setup fixture
    name = "Carl_Hollywood"
    # execute SUT
    # if name has spaces in a string, %x seems to just return the first
    # token followed by a newline
    result = %x[ echo "<~name~>" | ./tilton -s name #{name} ]
    # verify results
    result.should.include name
  end

  it "should process the zero digit macro" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~0~>" | ./tilton ]
    # verify results
    result.should.include "tilton"
  end

  it "should process the one digit macro" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~1~>" | ./tilton Charlie ]
    # verify results
    result.should.include "Charlie"
  end

  it "should process the add macro" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~add~1~2~3~>" | ./tilton ]
    # verify results
    result.should.include "6"
  end

  it "should process the and macro" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~and~1~2~3~>" | ./tilton ]
    # verify results
    result.should.include "3"
  end

  it "should process the define macro" do
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

  it "should process the read macro" do
    # setup fixture
    # execute SUT
    # appends a newline
    result = %x[ echo "<~read~test/front.snip~>" | ./tilton ]
    # verify results
    result.size.should.be 1939
  end

  it "should process the include macro" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~include~test/head.snip~Revelux~>" | ./tilton ]
    # verify results
    result.should.include "%title Revelux"
  end

xit "should process the include macro with null args" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~include~test/head.snip~~charlie~~~>" | ./tilton ]
    # verify results
    result.size.should.include "charlie"
  end

# TC's needed
# extra args in input
# report correctly a missing definition
end
