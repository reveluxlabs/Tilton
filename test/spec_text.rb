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

  it "should pass through plain text with angle brackets unchanged" do
    # setup fixture
    # execute SUT
    result = %x[ echo "Now is <the> time for all good men" | ./tilton ]
    # verify results
    result.should.include "Now is <the> time for all good men"
  end

  it "should pass through plain text with tildes unchanged" do
    # setup fixture
    # execute SUT
    result = %x[ echo "Now is ~the~ time for all good men" | ./tilton ]
    # verify results
    result.should.include "Now is ~the~ time for all good men"
  end

  it "should not pass through plain text with a closing bracket" do
    # setup fixture
    # execute SUT
    result = %x[ echo "Now is ~the~> time for all good men" | ./tilton 2> /dev/null ]
    # verify results
    result.should.include "Extra ~>"
  end

  it "should raise an error for unmatched brackets" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~1~<~2~>" | ./tilton 2> /dev/null ]
    # verify results
    result.should.include "Missing ~>"
  end

  it "should process UTF-8 demo text" do
    # setup fixture
    # execute SUT
    result = %x[ ./tilton < test/UTF-8-demo.txt > UTF.txt]
    result = %x[ diff test/UTF-8-demo.txt UTF.txt | wc ]
    # verify results
    result.should.include " 0 "
    # tear down fixture
    %x[ rm UTF.txt ]
  end

  it "should process UTF-8 macro name" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~define~→~->~><~→~><~→~>" |  ./tilton ]
    # verify results
    result.size.should.equal 5  # 4 chars + newline
  end

end
