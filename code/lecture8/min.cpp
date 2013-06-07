#include <iostream>
#include <vector>
#include <functional>
#include "binary_counter.h"
#include "list_pool.h"

template <typename I, typename Compare>
class op_min1_min2 
{
private:
  Compare cmp;
  list_pool<I>* pool;
public:
  typedef typename list_pool<I>::list_type list_type;
  typedef std::pair<I, list_type> argument_type;
  op_min1_min2(const Compare& cmp, list_pool<I>& pool) 
    : cmp(cmp), pool(&pool) {}
  argument_type operator()(const argument_type& x, 
			   const argument_type& y) {
    if (cmp(*(y.first), *(x.first))) {
      free_pool_list(*pool, x.second);
      return std::make_pair(y.first, (*pool).allocate(x.first, y.second));
    } else {
      free_pool_list(*pool, y.second);
      return std::make_pair(x.first, (*pool).allocate(y.first, x.second));
    }
  }
};

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
I min_element_binary(I first, I last, Compare cmp) {
  typedef op_min1_min2<I, Compare> op_t;
  list_pool<I> pool;
  binary_counter<op_t> min_counter(op_t(cmp, pool),
				   std::make_pair(last, pool.end()));
  while (first != last) min_counter.add(std::make_pair(first++, pool.end()));
  std::pair<I, size_t> foo =  min_counter.reduce();
  print_pool_list(pool, foo.second);
  return foo.first;
}

int main() {
    // plugin whatever numbers you want to test with
  int data[] = {9, 13, 7, 124, 32, 17, 8, 32, 237, 417, 41, 42, 13, 14, 15, 1, 99};
    int* end = data + sizeof(data)/sizeof(int);
    int* min = min_element_binary(data, end, std::less<int>());
    if (min == end) {
        std::cout << "No elements" << std::endl;
    } else { 
        std::cout << "Min is " << *min << std::endl;
    }
}
