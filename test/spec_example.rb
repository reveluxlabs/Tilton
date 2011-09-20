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

  it "should process the Factorial example" do
    # setup fixture
    define = "<~define~!~<~lt?~<~1~>~3~<~1~>~<~mult~<~!~<~sub~<~1~>~1~>~>~<~1~>~>~>~>"
    input = "<~!~5~>"
    text = define + input
    # execute SUT
    result = %x[ echo "#{text}" | ./tilton ]
    # verify results
    result.should.include "120"
  end

  it "should process a negative absolute value example using numeric values" do
    # setup fixture
    define = "<~define~abs~<~9~<~sub~0~<~1~>~>~><~lt?~<~1~>~<~9~>~<~9~>~<~1~>~>~>"
    input = "<~abs~-5~>"
    text = define + input
    # execute SUT
    result = %x[ echo "#{text}" | ./tilton ]
    # verify results
    result.should.include "5"
  end

  it "should process a positive absolute value example using numeric values" do
    # setup fixture
    define = "<~define~abs~<~9~<~sub~0~<~1~>~>~><~lt?~<~1~>~<~9~>~<~9~>~<~1~>~>~>"
    input = "<~abs~5~>"
    text = define + input
    # execute SUT
    result = %x[ echo "#{text}" | ./tilton ]
    # verify results
    result.should.include "5"
  end

  it "should process a negative absolute value example using text values" do
    # setup fixture
    define = "<~define~abs~<~eq?~<~substr~<~1~>~0~1~>~-~<~substr~<~1~>~1~>~<~1~>~>~>"
    input = "<~abs~-5~>"
    text = define + input
    # execute SUT
    result = %x[ echo "#{text}" | ./tilton ]
    # verify results
    result.should.include "5"
  end

  it "should process a positive absolute value example using text values" do
    # setup fixture
    define = "<~define~abs~<~eq?~<~substr~<~1~>~0~1~>~-~<~substr~<~1~>~1~>~<~1~>~>~>"
    input = "<~abs~5~>"
    text = define + input
    # execute SUT
    result = %x[ echo "#{text}" | ./tilton ]
    # verify results
    result.should.include "5"
  end

end
