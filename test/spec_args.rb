require 'test/spec'
require 'test/test_helper'

describe "A command line macro processor (in general)" do

  it "should null args in the input that are not provided" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~1~><~2~>" | ./tilton -2 Charlie ]
    # verify results
    result.should.include "Charlie"
  end

  it "should process the eval option from the command line" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~name~>" | ./tilton -e "<~define~name~charlie~>" ]
    # verify results
    result.should.include "charlie" 
  end

  it "should process the include option from the command line" do
    # setup fixture
    # execute SUT
    result = %x[ ./tilton Revelux -i "test/head.snip"  -n]
    # verify results
    result.should.include "%title Revelux"
  end

  it "should process the help option from the command line" do
    # setup fixture
    # execute SUT
    result = %x[ ./tilton -h ]
    # verify results
    result.size.should.be 243
  end

  it "should process the mute option from the command line" do
    # setup fixture
    # execute SUT
    result = %x[ ./tilton -r "test/front.snip" -m -n ]
    # verify results
    result.size.should.be 0
  end

  it "should process the read option from the command line" do
    # setup fixture
    # execute SUT
    result = %x[ ./tilton -r "test/front.snip" -n ]
    # verify results
    result.size.should.be 1938
  end

  it "should process the set option from the command line" do
    # setup fixture
    name = "Carl_Hollywood"
    # execute SUT
    result = %x[ echo "<~name~>" | ./tilton -s name #{name} ]
    # verify results
    result.should.include name
  end

  it "should process the write option from the command line" do
    # setup fixture
    # execute SUT
    result = %x[ ./tilton -r "test/front.snip" -w write.txt -n ]
    result = %x[ wc write.txt ]
    # verify results
    result.should.include "1938"
    # tear down fixture
    %x[ rm write.txt ]
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

  it "should set the ten digit macro with no prior arg" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~10~3~><~10~>" | ./tilton ]
    # verify results
    result.should.include "3"
  end

  it "should set the ten digit macro with an arg" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~10~3~><~10~>" | ./tilton -10 charlie ]
    # verify results
    result.should.include "3"
  end

  it "should produce an error for unrecognized command line args" do
    # setup fixture
    # execute SUT
    result = %x[ ./tilton -j charlie  2> /dev/null ]
    # verify results
    result.should.include "Unrecognized command line parameter"
  end

  it "should produce an error for uppercase command line args" do
    # setup fixture
    # execute SUT
    result = %x[ ./tilton -E charlie  2> /dev/null ]
    # verify results
    result.should.include "Unrecognized command line parameter"
  end

end
