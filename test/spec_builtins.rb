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
    result = %x[ echo "<~add~1~Tilton~3~>" | ./tilton  2> /dev/null ]
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
    result = %x[ echo "<~add~1~2~<~1~>~>" | ./tilton 2> /dev/null ]
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
    result = %x[ echo "<~define~EOF~-1~><~delete~EOF~><~EOF~>" | ./tilton 2> /dev/null ]
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
    result = %x[ echo "<~div~6~Tilton~>" | ./tilton 2> /dev/null ]
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
    result = %x[ echo "<~div~6~<~1~>~>" | ./tilton 2> /dev/null ]
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

  it "should process the dump builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~dump~>" | ./tilton 2> dump.txt]
    result = %x[ wc dump.txt ]
    # verify results
    result.should.include "45"
    # tear down fixture
    %x[ rm dump.txt ]
  end

  it "should process the entityify builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~entityify~&~>" | ./tilton ]
    # verify results
    result.should.include "&amp;"
  end

  it "should process the eval builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~eval~<~1~> token~my~>" | ./tilton ]
    # verify results
    result.should.include "my token"
  end

  it "should process the first builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~define~token~hi!~><~first~token~!~>" | ./tilton ]
    # verify results
    result.should.include "hi"
  end

  it "should process the gensym builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~gensym~>" | ./tilton ]
    # verify results
    result.should.include "1001"
  end

  it "should process the get builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~define~token~hi!~><~get~token~>" | ./tilton ]
    # verify results
    result.should.include "hi!"
  end

  it "should process the last builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~define~token~hi!there~><~last~token~!~>" | ./tilton ]
    # verify results
    result.should.include "there"
  end

  it "should process the length builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~length~666~>" | ./tilton ]
    # verify results
    result.should.include "3"
  end

  it "should process the literal builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~literal~42~>" | ./tilton ]
    # verify results
    result.should.include "42"
  end

  it "should process the loop builtin" do
    # TODO: fix
    # setup fixture
    # execute SUT
    result = %x[ echo "<~div~6~3~>" | ./tilton ]
    # verify results
    result.should.include "2"
  end

  it "should process the lt builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~lt~>" | ./tilton ]
    # verify results
    result.should.include "<"
  end

  it "should process the mod builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~mod~5~2~>" | ./tilton ]
    # verify results
    result.should.include "2"
  end

  it "should process the mult builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~mult~2~3~>" | ./tilton ]
    # verify results
    result.should.include "6"
  end

  it "should process the mute builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~mute~token~>" | ./tilton ]
    # verify results
    result.should.equal "\n"
  end

  it "should process the null builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~null~This is a swinging comment~>" | ./tilton ]
    # verify results
    result.should.equal "\n"
  end

  # TODO: check this for a bug
  xit "should process the number? builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~number?~1.618~yes~no~>" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process the or builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~or~~2~token~>" | ./tilton ]
    # verify results
    result.should.include "2"
  end

  it "should process the print builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~print~Hello World!~>" | ./tilton 2>print.txt ]
    result = %x[ wc print.txt ]
    # verify results
    result.should.include "19"
    # tear down fixture
    #%x[ rm print.txt ]
  end

  it "should process the rep builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~rep~666~2~>" | ./tilton ]
    # verify results
    result.should.include "666666"
  end

  it "should process the slashify builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~slashify~'Hello'~>" | ./tilton ]
    # verify results
    result.should.include "\\'Hello\\'"
  end

  it "should process the stop builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~stop~hair on fire~>" | ./tilton 2> /dev/null ]
    # verify results
    result.should.include "Stop: hair on fire."
  end

  it "should process the sub builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~sub~5~3~>" | ./tilton ]
    # verify results
    result.should.include "2"
  end

  # TODO: check this for a bug
  xit "should process the substr builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~substr~abcde~3~>" | ./tilton ]
    # verify results
    result.should.include "cde"
  end

  it "should process the tilde builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~tilde~>" | ./tilton ]
    # verify results
    result.should.equal "~\n"
  end

  it "should process the tilton builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~tilton~>" | ./tilton ]
    # verify results
    result.should.equal "0\n"
  end

  it "should process the trim builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~trim~     Hello   World   !  ~>" | ./tilton ]
    # verify results
    result.should.include "Hello World !"
  end

  it "should process the unicode builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~unicode~67~97~116~>" | ./tilton ]
    # verify results
    result.should.include "Cat"
  end

  it "should process the write builtin" do
    # setup fixture
    # execute SUT
    result = %x[ echo "<~write~write.txt~Hello World!~>" | ./tilton ]
    result = %x[ wc write.txt ]
    # verify results
    result.should.include "12"
    # tear down fixture
    %x[ rm write.txt ]
  end
end
