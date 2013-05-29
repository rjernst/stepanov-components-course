#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>

#include "concepts.h"

namespace course { 

template <InputIterator I, Integral N, OutputIterator O>
O copy_n(I first, N n, O result) {     // the function should really return pair<I, O>, but the new standard decides otherwise
  while (n--) *result++ = *first++;
  return result;
}

template <typename I, typename C>
// I is forward iterator
// C is strict weak ordering on value type of I
bool is_sorted(I first, I last, C comp) {
  if (first == last) return true;
  I next = first;
  while (++next != last) {
    if (comp(*next, *first)) return false;
    ++first;
  }
  return true;
}

template <typename I>
// I is forward iterator
// value type of I is totally ordered
inline
bool is_sorted(I first, I last) {
  std::less<typename std::iterator_traits<I>::value_type> comp;
  return is_sorted(first, last, comp);
}

template <InputIterator I>
void print_range(I first, I last, const char* msg = NULL) {
  if (msg) std::cout << msg << ": ";
  while (first != last) std::cout << *first++ << " ";
  std::cout << std::endl;
}

template <ForwardIterator I, Integral N> 
N iota(I first, I last, N start = N(0), N step = N(1)) {
  typedef typename std::iterator_traits<I>::value_type T;
  while (first != last) {
    *first = T(start);
    start += step;
    ++first;
  }
  return start;
}

template <ForwardIterator I, Integral N> 
I iota_n(I first, N n, N start = N(0), N step = N(1)) {
  while (n-- > 0) {
    *first = start;
    start += step;
    ++first;
  }
  return first;
}


template <ForwardIterator I> 
inline
void constant(I first, I last) {
  std::fill(first, last, ValueType(I)(0));
}


template <ForwardIterator I> 
inline
void iota(I first, I last) {
  iota(first, last, ValueType(I)(0), ValueType(I)(1));
}

template <typename I, typename N>
// I is forward iterator, N is integral
void segmented_iota(I first, I last, N segment_size) {
  typedef typename std::iterator_traits<I>::value_type T;
  while (last - first >= segment_size) {
    first = course::iota_n(first, segment_size, N(0));
  }
  course::iota(first, last, N(0));
}

template <BidirectionalIterator I> 
inline
void reverse_iota(I first, I last) {
  iota(first, last);
  std::reverse(first, last);
}

template <RandomAccessIterator I>
  void random_iota(I first, I last) {
  iota(first, last);
  std::random_shuffle(first, last);
}

template <BidirectionalIterator I>
void hill(I first, I last)
{
  I middle = successor(first, std::distance(first, last)/2);
  iota(first, middle);
  reverse_iota(middle, last);
}

template <BidirectionalIterator I>
void valley(I first, I last)
{
  I middle = successor(first, std::distance(first, last)/2);
  reverse_iota(first, middle);
  iota(middle, last);
}

template <ForwardIterator I, Integral N>
std::pair<N, N>
repeating_iota(I first, I last, N start = N(0), N step = N(1), N repetitions = N(1)) {
  // assert(repetitions > 0)
  N i = repetitions;
  while (first != last) {
    *first++ = start;
    if (--i == 0) {
      i = repetitions;
      start += step;
    }
  }
  return std::pair<N, N>(start, i);
}

template <BidirectionalIterator I> 
inline
const char* function_name(void (*gen)(I, I)) {
  void (*generator[])(I, I) = 
    {
      iota<I>,
      hill<I>,
      valley<I>,
      reverse_iota<I>, 
      random_iota<I>
    };
  const char* names[] =
    {
      "iota",
      "hill",
      "valley",
      "reverse_iota",
      "random_iota"
    };
  size_t number_of_generators = sizeof(generator)/sizeof(generator[0]);
  size_t index = std::find(generator, generator + number_of_generators, gen) - generator;
  if (index >= number_of_generators) return "unknown data generation ";
  return names[index];
}
} // end of namespace course

#endif
