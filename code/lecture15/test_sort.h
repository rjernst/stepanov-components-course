#ifndef TEST_SORT_H
#define TEST_SORT_H

#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include "timer.h"
#include "type_description.h"
#include "algorithm.h"

#include "merge_inplace.h"
#include "merge.h"

template <typename T>
// requires T is TotallyOrdered
double time_sort(T* first, T* last,  void (*sort)(T*, T*), T* buffer, size_t count) {
  timer t;
  t.start();
  size_t i = count;
  while (i--) {
    std::copy(buffer, buffer + std::distance(first, last), first);
    sort(first, last);
    if (!std::is_sorted(first, last)) {
      std::cerr << "*** SORT FAILED! ***" << std::endl;
      return 0;
    }
  }
  return t.stop();
} 

template <typename T, typename G>
// requires T is TotallyOrdered
// requires G is a Generator
void test_sort(size_t min_size, size_t max_size, G gen) {

  time_t now = time(0);

  std::cout << "Sorting " << type_description(T(0)) 
	    << " from " << min_size << " up to " << max_size 
	    << " elements" << " generated with " << function_name(gen) 
	    <<" at: " << asctime(localtime(&now));

  
  void (*f_pointers[])(T*, T*) = 
  {
    std::stable_sort<T*>
    ,sort_inplace<T*>
    ,sort_inplace_with_buffer<T*>
    ,sort_bert<T*>
  };

  size_t number_of_sorts = sizeof(f_pointers) / sizeof(f_pointers[0]);

  std::cout << "        size\t" 
	    << "stable\t" 
	    << "inplace\t" 
    	<< "sort_merge\t" 
    	<< "sort_bert\t" 
	    << std::endl;

  for (size_t array_size(min_size); array_size <= max_size; array_size *= 2) {    
    const size_t n = max_size / array_size;
    std::vector<T> vec(array_size);
    gen(&*vec.begin(), (&*vec.begin()) + vec.size());
    std::vector<T> tmp(vec.begin(), vec.end());
    std::cout << std::setw(12) << array_size << "\t";    
    for (size_t i = 0; i < number_of_sorts; ++i) {
      size_t bound = max_size;
      double time = time_sort(&*tmp.begin(), (&*tmp.begin()) + tmp.size(), f_pointers[i], &*vec.begin(), n);
	time /= double(array_size * n);
	std::cout << std::setw(6) << std::fixed << std::setprecision(0) << time << "\t";
    }
    std::cout << std::endl;
  }
}


#endif
