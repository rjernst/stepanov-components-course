#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include <algorithm>
#include <iterator>
#include "algorithm.h"
#include "search.h"

template <typename I>
// I is ForwardIterator
void rotate_right_by_one(I first, I butlast, I last, std::forward_iterator_tag) {
  //     1 2 3 4
  //     4 1 2 3
  /*
  typedef typename std::iterator_traits<I>::value_type T;
  I current = first; 
  if (current == last) return;
  I prev = current;
  ++current;
  while (current != last) {
    *current = prev_value;
    ++current;
    prev_value = *current;
  }
  */
  std::rotate(first, butlast, last);
}

template <typename I>
// I is BidirectionalIterator
void rotate_right_by_one(I first, I butlast, I last, std::bidirectional_iterator_tag) {
  typedef typename std::iterator_traits<I>::value_type T;
  T x = *butlast;
  std::copy_backward(first, butlast, last);
  *first = x;
}

template <typename I>
inline
void rotate_right_by_one(I first, I butlast, I last) {
  rotate_right_by_one(first, butlast, last, typename std::iterator_traits<I>::iterator_category());
}


template <typename I, typename N, typename R>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
I binary_insertion_sort_n(I first, N n, R r) {
  I last = first;
  N i(0);
  while (i < n) {
    I insertion_point = upper_bound_n(first, i, *last, r);
    rotate_right_by_one(insertion_point, last, successor(last));
    ++last;
    ++i;
  }
  return last;
}

#endif
