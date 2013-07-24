#include <iostream>
#include <functional>
#include <cstddef>
#include "algorithm.h"
#include "list_pool.h"
#include "list_algorithm.h"

int main() {
  std::vector<int> vec(100);
  random_iota(vec.begin(), vec.end());
  list_pool<int> pool;
  list_pool<int>::iterator nil(pool);
  list_pool<int>::iterator list = generate_list(vec.begin(), vec.end(), nil);
  print_range(list, nil);
  list = mergesort_linked(list, nil, std::less<int>());
  print_range(list, nil);
}
