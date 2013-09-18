#ifndef SORT_PH_H
#define SORT_PH_H

#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include "timer.h"
#include "type_description.h"
#include "algorithm.h"

#include "merge_inplace.h"
#include "merge.h"

template <typename I, typename N, typename R, typename B>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
inline
void ph_merge_adaptive_n(I f0, N n0,
                         I f1, N n1,   
                         R r, B buffer, N buffer_size) {
  // precondition std::distance(f0, f1) == n0
  // precondition is_sorted_n(f0, n0, r) && is_sorted(f1, n1, r)
  if (!n0 || !n1) return;
  if (n0 <= buffer_size) {
    I last = f1;
    std::advance(last, n1);
    merge_with_buffer(f0, f1, last, r, buffer);
    return;
  }
  I f0_0, f0_1, f1_0, f1_1;
  N n0_0, n0_1, n1_0, n1_1;
  if (n0 < n1)  merge_inplace_left_subproblem(f0,   n0,
                                              f1,   n1,
                                              f0_0, n0_0,
                                              f0_1, n0_1,
                                              f1_0, n1_0,
                                              f1_1, n1_1,
                                              r);
  else         merge_inplace_right_subproblem(f0,   n0,
                                              f1,   n1,
                                              f0_0, n0_0,
                                              f0_1, n0_1,
                                              f1_0, n1_0,
                                              f1_1, n1_1,
                                              r);

  ph_merge_adaptive_n(f0_0, n0_0, f0_1, n0_1, r, buffer, buffer_size);
  ph_merge_adaptive_n(f1_0, n1_0, f1_1, n1_1, r, buffer, buffer_size);
}

template <typename I, typename N, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
inline
I ph_insertion_sort_n(I first, N n, R r) {
  I last = first + n;
  if (first == last) return last; 
  I current = first;
  ++current;
  if (current == last) return last; 
  // create a sentinel 
  I min = std::min_element(first, last, r);
  rotate_right_by_one(first, ++min);
  insertion_sort_suffix(current, last, r);
  return last; 
}

const size_t PH_INSERTION_SORT_CUTOFF = 64;

template <typename I, typename N, typename R, typename B>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
inline
I ph_sort_adaptive_n(I first, N n, R r, B buffer, N buffer_size) {
  if (!n) return first;
  if (n < PH_INSERTION_SORT_CUTOFF) return ph_insertion_sort_n(first, n, r);
  N half = n >> 1;
  if (!half) return ++first;
  I middle = ph_sort_adaptive_n(first, half, r, buffer, buffer_size);
  I last   = ph_sort_adaptive_n(middle, n - half, r, buffer, buffer_size);
  ph_merge_adaptive_n(first, half, middle, n - half, r, buffer, buffer_size);
  return last;
}

template <typename I>
inline
void sort_ph(I first, I last) {
  typedef typename std::iterator_traits<I>::value_type T;
  typedef typename std::iterator_traits<I>::difference_type N;
  N n = std::distance(first, last);
  // n     : 117
  // n >> 1: 115
  // n >> 2: 122
  // n >> 3: 130
  std::vector<T> buffer(n >> 1);
  ph_sort_adaptive_n(first, n, std::less<T>(), buffer.begin(), N(buffer.size()));
}

#endif


