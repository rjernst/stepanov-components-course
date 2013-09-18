#ifndef _SORT_AKRAFT
#define _SORT_AKRAFT
// Include Everything!!!
#include "algorithm.h"
#include "search.h"
#include "insertion_sort.h"

template <typename I, typename N, typename R, typename B>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
inline
I sort_adaptive_n_akraft(I first, N n, R r, B buffer, N buffer_size) {
  if (!n) return first;
  if (n < INSERTION_SORT_CUTOFF) return insertion_sort_n(first, n, r);
  //if (n < INSERTION_SORT_CUTOFF) return linear_insert_with_sentinel_n(first, n, r);
  N half = n >> 1;
  if (!half) return ++first;
  I middle = sort_adaptive_n(first, half, r, buffer, buffer_size);
  I last   = sort_adaptive_n(middle, n - half, r, buffer, buffer_size);
  merge_adaptive_n(first, half, middle, n - half, r, buffer, buffer_size);
  return last;
}

template <typename I>
// I is ForwardIterator
// R is WeakStrictOrdering on the value type of I
inline
void sort_akraft(I first, I last) {
    // Attempt 1, just use merge sort
    typedef typename std::iterator_traits<I>::value_type T;
    typedef typename std::iterator_traits<I>::difference_type N;
    N n = std::distance(first, last);
    std::vector<T> buffer(n >> 1);
    sort_adaptive_n_akraft(first, n, std::less<T>(), buffer.begin(), N(buffer.size()));
    
    //std::stable_sort(first, last, std::less<T>());
}

#endif
