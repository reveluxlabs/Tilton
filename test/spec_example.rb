require 'test/spec'
require 'test/test_helper'

describe "Tilton" do

  it "should process the Last, First example" do
    # setup fixture
    define = "<~define~last, first~<~last name~>, <~first name~>~>"
    vars = "<~set~first name~Carl~><~set~last name~Hollywood~>"
    input = "<~last, first~>"
    text = define + vars + input
    # execute SUT
    result = %x[ echo "#{text}" | ./tilton ]
    # verify results
    result.should.include "Hollywood, Carl"
  end

  it "should process the Minimum example" do
    # setup fixture
    define = "<~define~min~<~lt?~<~1~>~<~2~>~<~1~>~<~2~>~>~>"
    input = "<~min~5~10~>"
    text = define + input
    # execute SUT
    result = %x[ echo "#{text}" | ./tilton ]
    # verify results
    result.should.include "5"
  end

end
