#include <iostream>
#include <vector>
#include "binary_counter.h"

// TODO: implement min_element_binary, using binary_counter

// - compares two iterators and returns the one
//   pointing to the smallest element
template <typename Compare>
class min_op 
{
private:
  Compare cmp;
public:
  min_op(const Compare& cmp) : cmp(cmp) {}
  template <typename I>
  I operator()(const I& x, const I& y) {
    return cmp(*y, *x) ? y : x;
  }
};

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
I min_element_binary(I first, I last, Compare cmp) {
  binary_counter<I, min_op<Compare> > min_counter(min_op<Compare>(cmp), last);
  while (first != last) min_counter.add(first++); 
  return min_counter.reduce();
}

int main() {

    // plugin whatever numbers you want to test with
    int data[] = {9, 13, 7, 124, 32, 17, 8, 32, 237, 417, 41, 42, 13, 14, 15};
    int* end = data + sizeof(data)/sizeof(int);
    int* min = min_element_binary(data, end, std::less<int>());
    if (min == end) {
        std::cout << "No elements" << std::endl;
    } else { 
        std::cout << "Min is " << *min << std::endl;
    }
}
