#ifndef SETSORT_H
#define SETSORT_H

#include <algorithm>
#include <set>
#include "concepts.h"

template <typename I> 
// I is forward iterator
void setsort(I first, I last) {
  std::multiset<ValueType(I)> tmp(first, last);
  std::copy(tmp.begin(), tmp.end(), first);
}

template <typename I>
// I is forward iterator
I setsort_unique(I first, I last) {
  std::set<ValueType(I)> tmp(first, last);
  return std::copy(tmp.begin(), tmp.end(), 
		   first);
}

#endif
