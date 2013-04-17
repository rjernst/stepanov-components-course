#ifndef IOTA_H
#define IOTA_H

#define ForwardIterator typename
#define Integral typename

namespace course {

template <ForwardIterator I, Integral N> 
N iota(I first, I last, N start = N(0), N step = N(1)) {
  while (first != last) {
    *first = start;
    start += step;
    ++first;
  }
  return start;
}

}

#endif
