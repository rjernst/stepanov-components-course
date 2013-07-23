#include <iostream>
#include <functional>
#include <cstddef>
#include "algorithm.h"
#include "list_pool.h"
#include "list_algorithm.h"

template <typename Compare>
class counted_compare
{
private:
  Compare cmp;
  size_t* counter_p;
public:
  counted_compare(const Compare& cmp, size_t& counter) : cmp(cmp), counter_p(&counter) {}
  counted_compare(size_t& counter) : cmp(), counter_p(&counter) {}
  template <typename T>
  bool operator()(const T& x, const T& y) const {
    ++*counter_p;
    return cmp(x & 63, y & 63);
  }
};

template <typename I>
void count_comparisons(I first, I last, 
		       std::pair<I, I> (*algorithm)(I, I, counted_compare<std::less<typename std::iterator_traits<I>::value_type> >), 
		       const char* algorithm_name) {
  typedef typename std::iterator_traits<I>::value_type T;
  size_t counter(0);
  counted_compare<std::less<T> > cmp(counter);
  std::pair<I, I> result = algorithm(first, last, cmp);
  std::cout << algorithm_name << " ";  
  std::cout << "results " << *result.first << " " << *result.second << " ";
  std::cout << "number of comparisons " << counter << std::endl;
} 


int main() {
  std::vector<int> vec(1000);
  typedef std::vector<int>::iterator I;
  random_iota(vec.begin(), vec.end());
  list_pool<int> pool;
  list_pool<int>::iterator nil(pool);
  list_pool<int>::iterator list1 = generate_list(vec.begin(), vec.end(), nil);
  //  list_pool<int>::iterator list2 = generate_list(vec.begin(), vec.end(), nil);
  print_range(list1, nil);
  //  print_range(list2, nil);
  list_pool<int>::iterator list = mergesort_linked(list1, nil, std::less<int>());
  print_range(list, nil);
  
}
