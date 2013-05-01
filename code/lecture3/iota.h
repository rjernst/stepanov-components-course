#ifndef IOTA_H
#define IOTA_H

#include <iterator>

namespace course {

template <typename I, typename N> 
N iota(I first, I last, N start = N(0), N step = N(1)) {
  typedef typename std::iterator_traits<I>::value_type T;
  while (first != last) {
    *first = T(start);
    start += step;
    ++first;
  }
  return start;
}

}

#endif
