
#include <iostream>
#include "binary_counter.h"

template <typename Compare>
struct min_combiner {
    Compare cmp;
    min_combiner(const Compare& c) : cmp(c) {}

    template <typename I>
    I operator()(I x, I y) {
        if (cmp(*y, *x)) return y;
        return x;
    }
};

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
I min_element_binary(I first, I last, Compare cmp) {
    I counter[32];
    std::fill(counter, counter + 32, last);

    min_combiner<Compare> combine(cmp); 
    while (first != last) {
        add_to_counter(counter, counter + 32, combine, last, first);
        ++first;
    }
    return reduce_counter(counter, counter + 32, combine, last); 
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
