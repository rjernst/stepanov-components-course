
#include <iostream>
#include "binary_counter.h"

// TODO: implement min_element_binary, using add_to_counter and reduce_counter

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
