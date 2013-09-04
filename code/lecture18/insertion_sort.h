#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include <algorithm>
#include <iterator>
#include "algorithm.h"
#include "search.h"

template <typename I>
// I is ForwardIterator
void rotate_right_by_one(I first, I last, std::forward_iterator_tag) {
  if (first == last) return;
  I current = first; 
  while (++current != last) std::swap(first, current);
}

template <typename I>
// I is BidirectionalIterator
void rotate_right_by_one(I first, I last, std::bidirectional_iterator_tag) {
  typedef typename std::iterator_traits<I>::value_type T;
  I butlast = last;
  --butlast;
  T x = *butlast;
  std::copy_backward(first, butlast, last);
  *first = x;
}

template <typename I>
inline
void rotate_right_by_one(I first, I last) {
  rotate_right_by_one(first, last, typename std::iterator_traits<I>::iterator_category());
}

template <typename I, typename N, typename R>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
I binary_insert_n(I first, N n, I current, R r) {
  // precondition: is_sorted(first, current, r) && current is a valid iterator
  //            && std::distance(first, current) == n
  I insertion_point = upper_bound_n(first, n, *current, r);
  rotate_right_by_one(insertion_point, ++current);
  return insertion_point; 
}

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
I linear_insert(I first, I current, R r) {
  // precondition: is_sorted(first, current, r) && current is a valid iterator
  typedef typename std::iterator_traits<I>::value_type T;
  if (first == current) return first;
  T value = *current;
  --current;
  while (r(value, *current)) {
    // invariant: 
    *successor(current) = *current;
    if (first == current) {
      *current = value;
      return current;
    }
    --current;
  }
  ++current;
  *current = value;
  return current; 
}

template <typename I, typename N, typename R>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
I binary_insertion_sort_n(I first, N n, R r) {
  if (n == N(0)) return first;
  I current = first;
  ++current;
  N i(1);
  while (i < n) {
    // invariant: is_sorted_n(first, i, r) && std::distance(first, current) == i
    binary_insert_n(first, i++, current++, r);
  }
  return current;
}

template <typename I, typename N, typename R>
// I is BidirectionalIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
I linear_insertion_sort_n(I first, N n, R r) {
  if (n == N(0)) return first;
  I current = first;
  ++current;
  N i(1);
  while (i < n) {
    // invariant: is_sorted_n(first, i, r) && std::distance(first, current) == i
    linear_insert(first, current++, r);
    ++i;
  }
  return current;
}

#endif
