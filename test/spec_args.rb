require 'test/spec'
require 'test/test_helper'

describe "A command line macro processor (in general)" do

  it "should pass through plain text unchanged" do
    # setup fixture
    # execute SUT
    result = %x[ echo "Now is the time for all good me to come to the aide of their country." | ./tilton ]
    # verify results
    result.should.include "Now is the time for all good me to come to the aide of their country."
  end

  it "should null args in the input that are not provided" do
    # setup fixture
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
    result = %x[ echo "<~0~>" | ./tilton ]
    # verify results
    result.should.include "tilton"
  end

  it "should process the one digit macro" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~1~>" | ./tilton Charlie ]
    # verify results
    result.should.include "Charlie"
  end

  it "should set the one digit macro with no prior arg" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~1~3~><~1~>" | ./tilton ]
    # verify results
    result.should.include "3"
  end

  it "should set the one digit macro with an arg" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~1~3~><~1~>" | ./tilton charlie ]
    # verify results
    result.should.include "3"
  end

end
