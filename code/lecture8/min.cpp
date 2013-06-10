#include <iostream>
#include <vector>
#include <functional>
#include "binary_counter.h"
#include "list_pool.h"

template <typename I, typename Compare>
class op_min1_2 
{
private:
  Compare cmp;
  list_pool<I>* pool_p;
public:
  typedef typename list_pool<I>::list_type list_type;
  typedef std::pair<I, list_type> argument_type;
  op_min1_2(const Compare& cmp, list_pool<I>& pool) 
    : cmp(cmp), pool_p(&pool) {}
  argument_type combine(const argument_type& x, 
			const argument_type& y) {
    free_list(*pool_p, y.second);
    return std::make_pair(x.first, (*pool_p).allocate(y.first, x.second));
  }
  argument_type operator()(const argument_type& x, 
			   const argument_type& y) {
    return cmp(*(y.first), *(x.first)) ? combine(y, x) : combine(x, y);
  }
};

template <typename Compare>
class compare_dereference
{
private:
  Compare cmp;
public:
  compare_dereference(const Compare& cmp) : cmp(cmp) {}
  template <typename I>
  bool operator() (I x, I y) {
    return cmp(*x, *y);
  }
};

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min1_2_element(I first, I last, Compare cmp) {
  typedef op_min1_2<I, Compare> op_t;
  typedef typename list_pool<I>::list_type list_type;
  list_pool<I> pool;
  binary_counter<op_t> min_counter(op_t(cmp, pool),
				   std::make_pair(last, pool.end()));
  while (first != last) min_counter.add(std::make_pair(first++, pool.end()));
  std::pair<I, list_type> tmp = min_counter.reduce();
  list_type list = min_element_list(pool, tmp.second, 
				    compare_dereference<Compare>(cmp));
  I min2 = pool.is_end(list) ? tmp.first : pool.value(list);
  return std::make_pair(tmp.first, min2);
}

int main() {
    // plugin whatever numbers you want to test with
  int data[] = {9, 13, 7, 124, 32, 1, 81, 111, 237, 417, 41, 42, 13, 14, 15, 99, 11, 4};
  int* end = data + sizeof(data)/sizeof(int);
  std::pair<int*, int*> min1_2 = min1_2_element(data, end, std::less<int>());
  if (min1_2.first == end) {
    std::cout << "No elements" << std::endl;
  } else { 
    std::cout << "Min is " << *(min1_2.first) << " " << *(min1_2.second) << std::endl;
  }
}
