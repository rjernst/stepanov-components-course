#include <iostream>
#include <functional>
#include <cstddef>
#include "min_element1_2.h"

int main() {
    // plugin whatever numbers you want to test with
  int data[] = {9, 13, 7, 124, 32, 1, 81, 111, 237, 417, 41, 42, 13, 14, 15, 99, 11};
  int* end = data + sizeof(data)/sizeof(int);
  std::pair<int*, int*> min1_2 = min_element1_2(data, end, std::less<int>());
  if (min1_2.first == end) {
    std::cout << "No elements" << std::endl;
  } else { 
    std::cout << "Min is " << *(min1_2.first) << " " << *(min1_2.second) << std::endl;
  }
}
