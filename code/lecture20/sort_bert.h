#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

#include "merge_inplace.h"
#include "insertion_sort.h"

const size_t BERT_INSERTION_SORT_CUTOFF = 64;

template <typename I, typename R, typename B>
// requires I is ForwardIterator
// requires R is StrictWeakOrdering
// requires B is ForwardIterator
void bert_merge_with_buffer(I first, I middle, I last, R r, B buffer) {
  B buffer_last = std::copy(first, middle, buffer);
  std::merge(buffer, buffer_last, middle, last, first, r);
}

template <typename I, typename N, typename R, typename B>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I
I bert_sort_inplace_n_with_buffer(I first, N n, R r, B buffer) {
  if (!n) return first;
  N half = n >> 1;
  if (!half) return ++first;
  if (n < BERT_INSERTION_SORT_CUTOFF) return insertion_sort_n(first, n, r);
  // if (n < BERT_INSERTION_SORT_CUTOFF) return linear_insertion_sort_n(first, n, r);
  // if (n < BERT_INSERTION_SORT_CUTOFF) return binary_insertion_sort_n(first, n, r);
  I middle = bert_sort_inplace_n_with_buffer(first, half, r, buffer);
  I last   = bert_sort_inplace_n_with_buffer(middle, n - half, r, buffer);
  bert_merge_with_buffer(first, middle, last, r, buffer);
  return last;
}

template <typename I, typename R>
// I is ForwardIterator
// R is WeakStrictOrdering on the value type of I
inline
  void sort_bert(I first, I last, R r) {
  typedef typename std::iterator_traits<I>::value_type T;
  typedef typename std::iterator_traits<I>::difference_type N;
  N n = std::distance(first, last);
  std::vector<T> buffer(n >> 1);
  bert_sort_inplace_n_with_buffer(first, n, r, buffer.begin());
}

template <typename I>
// I is ForwardIterator
inline
void sort_bert(I first, I last) {
  typedef typename std::iterator_traits<I>::value_type T;
  // sort_bert(first, last, std::less<T>());
  typedef typename std::iterator_traits<I>::difference_type N;
  N n = std::distance(first, last);
  std::vector<T> buffer(n >> 1);
  bert_sort_inplace_n_with_buffer(first, n, std::less<T>(), buffer.begin());
}
