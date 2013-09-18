#ifndef SORT_RJERNST_H
#define SORT_RJERNST_H

#include <algorithm>
#include <iterator>
#include "algorithm.h"
#include "search.h"
#include "merge.h"
#include "merge_inplace.h"
#include "insertion_sort.h"

const size_t RJERNST_INSERTION_SORT_CUTOFF = 64;

template <typename I, typename N, typename R, typename B>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
I rjernst_sort_adaptive_n(I first, N n, R r, B buffer, N buffer_size) {
  if (!n) return first;
  if (n < RJERNST_INSERTION_SORT_CUTOFF) return insertion_sort_n(first, n, r);
  N half = n >> 1;
  if (!half) return ++first;
  I middle = rjernst_sort_adaptive_n(first, half, r, buffer, buffer_size);
  I last   = rjernst_sort_adaptive_n(middle, n - half, r, buffer, buffer_size);
  merge_adaptive_n(first, half, middle, n - half, r, buffer, buffer_size);
  return last;
}

template <typename I>
// I is ForwardIterator
inline
void sort_rjernst(I first, I last) {
  typedef typename std::iterator_traits<I>::value_type T;
  typedef typename std::iterator_traits<I>::difference_type N;
  N n = std::distance(first, last);

  if (n < RJERNST_INSERTION_SORT_CUTOFF) {
    insertion_sort(first, last, std::less<T>());
  } else {
    std::vector<T> buffer(n >> 2);
    rjernst_sort_adaptive_n(first, n, std::less<T>(), buffer.begin(), N(buffer.size()));
  }
}

#endif
