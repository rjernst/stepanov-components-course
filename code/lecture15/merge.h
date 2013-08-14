
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

#include "merge_inplace.h"

template <typename I, typename R, typename B>
// requires I is ForwardIterator
// requires R is StrictWeakOrdering
// requires B is ForwardIterator
void merge_with_buffer(I first, I middle, I last, R r, B buffer) {
  B buffer_last = std::copy(first, middle, buffer);
  std::merge(buffer, buffer_last, middle, last, first, r);
}

template <typename I, typename N, typename R, typename B>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
void merge_adaptive_n(I  f0,   N  n0,
                      I  f1,   N  n1,   R r, B buffer, N buffer_size) {
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

  merge_adaptive_n(f0_0, n0_0, f0_1, n0_1, r, buffer, buffer_size);
  merge_adaptive_n(f1_0, n1_0, f1_1, n1_1, r, buffer, buffer_size);
}

template <typename I, typename N, typename R, typename B>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
I sort_inplace_n_with_buffer(I first, N n, R r, B buffer) {
  if (!n) return first;
  N half = n >> 1;
  if (!half) return ++first;
  I middle = sort_inplace_n_with_buffer(first, half, r, buffer);
  I last   = sort_inplace_n_with_buffer(middle, n - half, r, buffer);
  merge_with_buffer(first, middle, last, r, buffer);
  return last;
}

template <typename I>
// I is ForwardIterator
inline
void sort_inplace_with_buffer(I first, I last) {
  typedef typename std::iterator_traits<I>::value_type T;
  typedef typename std::iterator_traits<I>::difference_type N;
  N n = std::distance(first, last);
  std::vector<T> buffer(n >> 1);
  sort_inplace_n_with_buffer(first, n, std::less<T>(), buffer.begin());
}

template <typename I, typename N, typename R, typename B>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
I sort_adaptive_n(I first, N n, R r, B buffer, N buffer_size) {
  if (!n) return first;
  N half = n >> 1;
  if (!half) return ++first;
  I middle = sort_adaptive_n(first, half, r, buffer, buffer_size);
  I last   = sort_adaptive_n(middle, n - half, r, buffer, buffer_size);
  merge_adaptive_n(first, half, middle, n - half, r, buffer, buffer_size);
  return last;
}

template <typename I>
// I is ForwardIterator
inline
void sort_bert(I first, I last) {
  typedef typename std::iterator_traits<I>::value_type T;
  typedef typename std::iterator_traits<I>::difference_type N;
  N n = std::distance(first, last);
  std::vector<T> buffer(n >> 3);
  sort_adaptive_n(first, n, std::less<T>(), buffer.begin(), N(buffer.size()));
}

