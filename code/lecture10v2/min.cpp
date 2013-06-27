#include <iostream>
#include <functional>
#include <cstddef>
#include "min_element1_2.h"
#include "algorithm.h"

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
  std::vector<int> vec(1000 * 1000);
  typedef std::vector<int>::iterator I;
  random_iota(vec.begin(), vec.end());
  count_comparisons(vec.begin(), vec.end(), min_element1_2<I>, "min_element1_2");
  count_comparisons(vec.begin(), vec.end(), min_element1_2_stable_random_access<I>, "min_element1_2_stable_random_access");
  count_comparisons(vec.begin(), vec.end(), min_element1_2_stable<I>, "min_element1_2_stable");
  count_comparisons(vec.begin(), vec.end(), min_element1_2_stable_indexed<I>, "min_element1_2_stable_indexed");
  count_comparisons(vec.begin(), vec.end(), min_element1_2_practical<I>, "min_element1_2_practical");
}
