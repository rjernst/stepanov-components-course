#include <iostream>
#include <functional>
#include <cstddef>
#include <vector>
#include "algorithm.h"
#include "merge_inplace.h"

 
int main() {
  std::vector<int> vec(111);
  random_iota(vec.begin(), vec.end());
  print_range(vec.begin(), vec.end());
  sort_inplace_n(vec.begin(), vec.size(), std::less<int>());
  print_range(vec.begin(), vec.end());
}
