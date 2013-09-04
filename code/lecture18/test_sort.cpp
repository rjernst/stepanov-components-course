#include <cstddef>
#include <vector>
#include "test_sort.h"
#include "algorithm.h"

const size_t min_size(8);
const size_t max_size(2 * 1024 * 1024);

int main() {
  test_sort<double>(min_size, max_size, random_iota<double*>); 
}
