#ifndef CONCEPTS_H
#define CONCEPTS_H

#define TotallyOrdered typename
#define Pointer typename
#define Number typename
#define Unsigned typename
#define Integral typename
#define InputIterator typename
#define OutputIterator typename
#define ForwardIterator typename
#define BidirectionalIterator typename
#define RandomAccessIterator typename
#define Sorter typename
#define Incrementable typename
#define StrictWeakOrdering typename
#define Generator typename
#define BinaryOperation typename
#define Sequence typename

#define ValueType(I) typename std::iterator_traits<I>::value_type
#define DifferenceType(I) typename std::iterator_traits<I>::difference_type
#define IteratorCategory(I) typename std::iterator_traits<I>::iterator_category

struct less {
  template <TotallyOrdered T>
  bool operator()(const T& x, const T& y) { 
    return x < y;
  }
};

template <InputIterator I>
inline
I successor(I x) { return ++x; }

template <BidirectionalIterator I>
inline
I predecessor(I x) { return ++x; }

template <RandomAccessIterator I, Integral N>
inline
I successor(I x, N n, std::random_access_iterator_tag) { 
  return x + n;
}

template <InputIterator I, Integral N>
inline
I successor(I x, N n, std::input_iterator_tag) { 
  while (n != N(0)) {
    ++x;
    --n;
  }
  return x;
}

template <InputIterator I, Integral N>
inline
I successor(I x, N n) {
  return successor(x, n, IteratorCategory(I)());
}

template <RandomAccessIterator I, Integral N>
inline
I successor_guarded(I first, I last, N n, std::random_access_iterator_tag) { 
  return first + std::min(n, last - first);
}

template <InputIterator I, Integral N>
inline
I successor_guarded(I first, I last, N n, std::input_iterator_tag) { 
  while (n != N(0) && first != last) {
    ++first;
    --n;
  }
  return first;
}

template <InputIterator I, Integral N>
inline
I successor_guarded(I first, I last, N n) {
  return successor_guarded(first, last, n, IteratorCategory(I)());
}

#endif
