/*--------------------------------------------------------------------------------------
 * minmax.cpp
 *
 * Alexander Stepanov (stepanov@a9.com)
 * Daniel Rose (danrose@a9.com)
 * February 2013 
 * 
 *--------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstdlib>  
#include <ctime>

#include <functional>
#include <algorithm>
#include <vector>
#include <deque>
#include <string>

#include <iostream>
#include <iomanip>

#include "algorithm.h"
#include "concepts.h"
#include "minmax.h"


namespace course {
  
template <typename I, typename Compare> // I is ForwardIterator
inline
std::pair<I, I> minmax_element_simple(I first, I last, Compare comp) {
  return std::make_pair(course::min_element(first, last, comp), course::max_element(first, last, comp));
}

/****************************************************************************************
                                 COMPARISON COUNTING
****************************************************************************************/

double comparisons = 0;

struct counting_compare
{
  template <typename T>
  bool operator()(const T& x,
		  const T& y) const {
      ++comparisons;
      return x < y;
  }   
};

counting_compare counting_less;

template <typename I> 
std::pair<double, double> minmax_comparisons(I first, I last) {
  typedef typename std::iterator_traits<I>::value_type T;
  std::pair<double, double> result;
  std::pair<I, I> m0, m1;
  {
    std::vector<T> seq(first, last);
    comparisons = 0;
    m0 = minmax_element_simple(seq.begin(), seq.end(), counting_less);
    result.first = comparisons;
  }
  {
    std::vector<T> seq(first, last);
    comparisons = 0;
    m1 = minmax_element(seq.begin(), seq.end(), counting_less);
    result.second = comparisons;
  }
  if (m0 != m1) std::cout << "Failed: different mins or maxs\n";
  return result;
}

template <typename I> 
std::pair<double, double> minmax_times(I first, I last, size_t iterations) {
  typedef typename std::iterator_traits<I>::value_type T;
  typedef typename std::iterator_traits<I>::difference_type N;
  N n = std::distance(first, last);
  std::pair<double, double> result;
  std::pair<I, I> m0, m1; 
  double time;
  double nanoseconds = 1000000000.0 /  double(CLOCKS_PER_SEC);
  std::vector<T> seq(iterations * n);

  for (N i(0); i < iterations; ++i) std::copy(first, last, seq.begin() + i * n);
  time = clock();
  for (N i(0); i < iterations; ++i) {
    m0 = minmax_element_simple((&*seq.begin()) + i * n, (&*seq.begin()) + (i + 1) * n, std::less<T>());
  }
  time = clock() - time; 

  result.first = (time * nanoseconds) / double(iterations); 
  for (N i(0); i < iterations; ++i) std::copy(first, last, seq.begin() + i * n);
  time = clock();
  for (size_t i(0); i < iterations; ++i) {
    m1 = minmax_element((&*seq.begin()) + i * n, (&*seq.begin()) + (i + 1) * n, std::less<T>());
  }
  time = clock() - time; 
  result.second = (time * nanoseconds) / double(iterations); 

  if (m0 != m1) std::cout << "Failed: different mins or maxs\n";
  return result;
}

double log2(double x) { return log(x) / log(2.0); }

void print_comparisons(size_t n) {
  std::cout << "Comparisons\n" << "\tn\t      minmax_simple\tminmax\t  gain (%)\t\n";
  for (size_t i(64); i < n; i <<= 1) {
    std::vector<uint64_t> buffer(i);
    course::iota(buffer.begin(), buffer.end(), uint64_t(0)); // newer versions of std include iota
    std::srand ( unsigned (std::time( 0 ) ));
    std::random_shuffle(buffer.begin(), buffer.end());
    std::pair<double, double> result = minmax_comparisons(buffer.begin(), buffer.end());
    std::cout << std::setw(9) << i  << std::fixed  
	      << std::setprecision(2)  << "\t\t" << result.first / i << "\t\t" << result.second / i 
	      << "\t\t" << std::setprecision(0) << (1 - result.second / result.first) * 100  << std::endl;
  }
}

template <typename T>
void print_times(size_t n) {
  std::cout << "\nTimes\n" << "\tn\t      minmax_simple\tminmax\t  gain (%)\t\n";
  for (size_t i(64); i < n; i <<= 1) {
    std::vector<T> buffer(i);
    course::iota(buffer.begin(), buffer.end(), T(0)); // newer versions of std include iota
    std::srand ( unsigned (std::time( 0 ) ));
    std::random_shuffle(buffer.begin(), buffer.end());
    std::pair<double, double> result = minmax_times(buffer.begin(), buffer.end(), n / i);
    std::cout << std::setw(9) << i  << std::fixed  
	      << std::setprecision(2)  << "\t\t" << result.first / i << "\t\t" << result.second / i 
	      << "\t\t" << std::setprecision(0) << (1 - result.second / result.first) * 100  << std::endl;
  }
}
} // end of namespace course

int main() {
  course::print_comparisons(128 * 1024 * 1024);
  course::print_times<uint64_t>(128 * 1024 * 1024);
}




