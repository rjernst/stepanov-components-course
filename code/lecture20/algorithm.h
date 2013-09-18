#ifndef ALGORITHM_H
#define ALGORITHM_H

#define InputIterator typename
#define ForwardIterator typename
#define RandomAccessIterator typename
#define BidirectionalIterator typename
#define Integral typename

#include <iostream>

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

template <ForwardIterator I> 
inline
void iota(I first, I last) {
  typedef typename std::iterator_traits<I>::value_type T;
  iota(first, last, T(0), T(1));
}

template <RandomAccessIterator I>
void random_iota(I first, I last) {
  iota(first, last);
  std::random_shuffle(first, last);
}

template <BidirectionalIterator I>
void reverse_iota(I first, I last) {
  iota(first, last);
  std::reverse(first, last);
}

template <typename I>
void print_range(I first, I last) {
  while (first != last) {
    std::cout << *first << " ";
    ++first;
  }
  std::cout << std::endl;
}

template <typename I>
// requires I is InputIterator
inline
I successor(I x) { return ++x; }

template <typename I>
// requires I is BidirectionalIterator
inline
I predecessor(I x) { return --x; }

template <typename I, typename N>
// requires I is RandomAccessIterator
// requires N is Integral
inline
I successor(I x, N n, std::random_access_iterator_tag) { 
  return x + n;
}

template <typename I, typename N>
// requires I is InputIterator
// requires N is Integral
inline
I successor(I x, N n, std::input_iterator_tag) { 
  while (n != N(0)) {
    ++x;
    --n;
  }
  return x;
}

template <typename I, typename N>
// requires I is InputIterator
// requires N is Integral
inline
I successor(I x, N n) {
  typedef typename std::iterator_traits<I>::iterator_category C;
  return successor(x, n, C());
}

template <typename I>
// requires I is BidirectionalIterator
void hill(I first, I last)
{
  I middle = successor(first, std::distance(first, last)/2);
  iota(first, middle);
  reverse_iota(middle, last);
}

template <typename I>
// requires I is BidirectionalIterator
void valley(I first, I last)
{
  I middle = successor(first, std::distance(first, last)/2);
  reverse_iota(first, middle);
  iota(middle, last);
}

template <typename I>
// requires I is BidirectionalIterator
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

#endif
