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
  T value = *current;
  while (first != current && r(value, *predecessor(current))) {
    *current = *predecessor(current);
    --current;
  }
  *current = value; 
  return current;
}


template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
void insertion_sort_classic(I first, I last, R r) {
  if (first == last) return; 
  I current = first;
  ++current;
  while (current != last) {
    linear_insert(first, current, r);     
    ++current;
  }
}

template <typename I, typename R>
// I is ForwardIterator
// R is WeakStrictOrdering on the value type of I 
void selection_sort(I first, I last, R r) {
  while (first != last) {
    std::swap(*first, *std::min_element(first, last, r));
    ++first;
  }
}

template <typename I, typename R>
// I is ForwardIterator
// R is WeakStrictOrdering on the value type of I 
void stable_selection_sort(I first, I last, R r) {
  while (first != last) {
    rotate_right_by_one(first, ++std::min_element(first, last, r));
    ++first;
  }
}

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
void insertion_sort_suffix(I first, I last, R r) {
  // precondition: one of -
  // (1) a valid range before first is sorted with respect to r and contains a sentinel
  // (2) *first is a sentinel
  if (first == last) return; 
  I current = first;
  ++current;
  while (current != last) {
    linear_insert_with_sentinel(first, current, r);     
    ++current;
  }
}

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
I linear_insert_with_sentinel(I first, I current, R r) {
  // precondition: is_sorted(first, current, r) &&
  //               current is a valid iterator &&
  //               first != current &&
  //               !r(*current, *first)
  typedef typename std::iterator_traits<I>::value_type T;
  T value = *current;
  
  while (r(value, *predecessor(current))) {
    *current = *predecessor(current);
    --current;
  }
  *current = value; 
  return current;
}

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
void insertion_sort(I first, I last, R r) {
  if (first == last) return; 
  I current = first;
  ++current;
  if (current == last) return;
  // create a sentinel 
  rotate_right_by_one(first, ++std::min_element(first, last, r));
  insertion_sort_suffix(current, last, r);
}

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
void insertion_sort_unstable(I first, I last, R r) {
  if (first == last) return; 
  I current = first;
  ++current;
  if (current == last) return;
  // create a sentinel 
  std::swap(*first, *std::min_element(first, last, r));
  insertion_sort_suffix(current, last, r);
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
