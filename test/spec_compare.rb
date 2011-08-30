require 'test/spec'
require 'test/test_helper'

describe "The compare method" do

  #
  #  eq?
  #
  #  numbers
  #
  it "should process eq? low to high with same size numbers" do
    # setup fixture
    input = "<~eq?~10~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process eq? high to low with same size numbers" do
    # setup fixture
    input = "<~eq?~11~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process eq? equal with same size numbers" do
    # setup fixture
    input = "<~eq?~11~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process eq? low to high with different size numbers" do
    # setup fixture
    input = "<~eq?~5~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process eq? high to low with different size numbers" do
    # setup fixture
    input = "<~eq?~10~5~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process eq? with different size numbers and letters" do
    # setup fixture
    input = "<~eq?~10~aaaa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  #
  #  letters
  #  
  it "should process eq? low to high with same size letters" do
    # setup fixture
    input = "<~eq?~aa~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process eq? high to low with same size letters" do
    # setup fixture
    input = "<~eq?~bb~aa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process eq? equal with same size letters" do
    # setup fixture
    input = "<~eq?~bb~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process eq? low to high with different size letters" do
    # setup fixture
    input = "<~eq?~a~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process eq? high to low with different size letters" do
    # setup fixture
    input = "<~eq?~bb~a~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  #
  #  ge?
  #
  #  numbers
  #
  it "should process ge? low to high with same size numbers" do
    # setup fixture
    input = "<~ge?~10~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process ge? high to low with same size numbers" do
    # setup fixture
    input = "<~ge?~11~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ge? equal with same size numbers" do
    # setup fixture
    input = "<~ge?~11~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ge? low to high with different size numbers" do
    # setup fixture
    input = "<~ge?~5~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process ge? high to low with different size numbers" do
    # setup fixture
    input = "<~ge?~10~5~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process ge? with different size numbers and letters" do
    # setup fixture
    input = "<~ge?~10~aaaa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  #
  #  letters
  #  
  it "should process ge? low to high with same size letters" do
    # setup fixture
    input = "<~ge?~aa~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process ge? high to low with same size letters" do
    # setup fixture
    input = "<~ge?~bb~aa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ge? equal with same size letters" do
    # setup fixture
    input = "<~ge?~bb~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ge? low to high with different size letters" do
    # setup fixture
    input = "<~ge?~a~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process ge? high to low with different size letters" do
    # setup fixture
    input = "<~ge?~bb~a~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  #
  #  gt?
  #
  #  numbers
  #
  it "should process gt? low to high with same size numbers" do
    # setup fixture
    input = "<~gt?~10~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process gt? high to low with same size numbers" do
    # setup fixture
    input = "<~gt?~11~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process gt? equal with same size numbers" do
    # setup fixture
    input = "<~gt?~11~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process gt? low to high with different size numbers" do
    # setup fixture
    input = "<~gt?~5~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process gt? high to low with different size numbers" do
    # setup fixture
    input = "<~gt?~10~5~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process gt? with different size numbers and letters" do
    # setup fixture
    input = "<~gt?~10~aaaa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  #
  #  letters
  #  
  it "should process gt? low to high with same size letters" do
    # setup fixture
    input = "<~gt?~aa~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process gt? high to low with same size letters" do
    # setup fixture
    input = "<~gt?~bb~aa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process gt? equal with same size letters" do
    # setup fixture
    input = "<~gt?~bb~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process gt? low to high with different size letters" do
    # setup fixture
    input = "<~gt?~a~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process gt? high to low with different size letters" do
    # setup fixture
    input = "<~gt?~bb~a~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  #
  #  le?
  #
  #  numbers
  #
  it "should process le? low to high with same size numbers" do
    # setup fixture
    input = "<~le?~10~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process le? high to low with same size numbers" do
    # setup fixture
    input = "<~le?~11~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process le? equal with same size numbers" do
    # setup fixture
    input = "<~le?~11~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process le? low to high with different size numbers" do
    # setup fixture
    input = "<~le?~5~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process le? high to low with different size numbers" do
    # setup fixture
    input = "<~le?~10~5~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process le? with different size numbers and letters" do
    # setup fixture
    input = "<~le?~10~aaaa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  #
  #  letters
  #  
  it "should process le? low to high with same size letters" do
    # setup fixture
    input = "<~le?~aa~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process le? high to low with same size letters" do
    # setup fixture
    input = "<~le?~bb~aa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process le? equal with same size letters" do
    # setup fixture
    input = "<~le?~bb~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process le? low to high with different size letters" do
    # setup fixture
    input = "<~le?~a~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process le? high to low with different size letters" do
    # setup fixture
    input = "<~le?~bb~a~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  #
  #  lt?
  #
  #  numbers
  #
  it "should process lt? low to high with same size numbers" do
    # setup fixture
    input = "<~lt?~10~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process lt? high to low with same size numbers" do
    # setup fixture
    input = "<~lt?~11~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process lt? equal with same size numbers" do
    # setup fixture
    input = "<~lt?~11~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process lt? low to high with different size numbers" do
    # setup fixture
    input = "<~lt?~5~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process lt? high to low with different size numbers" do
    # setup fixture
    input = "<~lt?~10~5~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process lt? with different size numbers and letters" do
    # setup fixture
    input = "<~lt?~10~aaaa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  #
  #  letters
  #  
  it "should process lt? low to high with same size letters" do
    # setup fixture
    input = "<~lt?~aa~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process lt? high to low with same size letters" do
    # setup fixture
    input = "<~lt?~bb~aa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process lt? equal with same size letters" do
    # setup fixture
    input = "<~lt?~bb~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process lt? low to high with different size letters" do
    # setup fixture
    input = "<~lt?~a~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process lt? high to low with different size letters" do
    # setup fixture
    input = "<~lt?~bb~a~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  #
  #  ne?
  #
  #  numbers
  #
  it "should process ne? low to high with same size numbers" do
    # setup fixture
    input = "<~ne?~10~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ne? high to low with same size numbers" do
    # setup fixture
    input = "<~ne?~11~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ne? equal with same size numbers" do
    # setup fixture
    input = "<~ne?~11~11~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process ne? low to high with different size numbers" do
    # setup fixture
    input = "<~ne?~5~10~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ne? high to low with different size numbers" do
    # setup fixture
    input = "<~ne?~10~5~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ne? with different size numbers and letters" do
    # setup fixture
    input = "<~ne?~10~aaaa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  #
  #  letters
  #  
  it "should process ne? low to high with same size letters" do
    # setup fixture
    input = "<~ne?~aa~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ne? high to low with same size letters" do
    # setup fixture
    input = "<~ne?~bb~aa~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ne? equal with same size letters" do
    # setup fixture
    input = "<~ne?~bb~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "no"
  end

  it "should process ne? low to high with different size letters" do
    # setup fixture
    input = "<~ne?~a~bb~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

  it "should process ne? high to low with different size letters" do
    # setup fixture
    input = "<~ne?~bb~a~yes~no~>"
    # execute SUT
    result = %x[ echo "#{input}" | ./tilton ]
    # verify results
    result.should.include "yes"
  end

end
