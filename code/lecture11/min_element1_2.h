#ifndef MIN_ELEMENT1_2_H
#define MIN_ELEMENT1_2_H

#include <cstddef>
#include "algorithm.h"
#include "binary_counter.h"
#include "list_pool.h"

template <typename Compare>
class compare_dereference
{
private:
  Compare cmp;
public:
  compare_dereference(const Compare& cmp) : cmp(cmp) {}
  template <typename I>
  bool operator() (const I& x, const I& y) const {
    return cmp(*x, *y);
  }
};

/****************** optimal algorithm (stable) ****************************/

template <typename T, typename N, typename Compare>
class op_min1_2 
{
private:
  Compare cmp;
  list_pool<T, N>* p;
public:
  typedef typename list_pool<T, N>::list_type list_type;
  typedef std::pair<T, std::pair<list_type, list_type> > argument_type;

  op_min1_2(const Compare& cmp, list_pool<T, N>& pool) : cmp(cmp), p(&pool) {}

  argument_type operator()(const argument_type& x, 
                           const argument_type& y) {
    if (!cmp(y.first, x.first)) {
      p->free(y.second);
      return std::make_pair(x.first, p->push_back(x.second, y.first));
    } else {
      p->free(x.second);
      return std::make_pair(y.first, p->push_front(y.second, x.first));
    }
  }
};

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min_element1_2(I first, I last, Compare cmp) {
  if (first == last || successor(first) == last) {
    return std::make_pair(first, last);
  }

  typedef op_min1_2<I, size_t, compare_dereference<Compare> > op_type;
  typedef typename list_pool<I, std::size_t>::iterator iterator;

  list_pool<I, std::size_t> pool;
  pool.reserve(256);
  compare_dereference<Compare> cmp_deref(cmp);
  op_type op(cmp_deref, pool);
  binary_counter<op_type> counter(op, std::make_pair(last, pool.empty_queue()));
  counter.reserve(32);

  while (first != last) counter.add(std::make_pair(first++, pool.empty_queue()));
  typename op_type::argument_type min1_list = counter.reduce();
  I min1 = min1_list.first;
  I min2 = *std::min_element(iterator(pool, min1_list.second.first), iterator(pool), cmp_deref);
  return std::make_pair(min1, min2);
}

//********************** practical algorithm

template <typename T, typename Compare>
// Compare is a StrictWeakOrdering on T
inline
void insert_2(T& first, T& second, const T& candidate, Compare cmp) {
  if (cmp(candidate, second)) {
    if (cmp(candidate, first)) {
      second = first;
      first = candidate;
    } else {
      second = candidate;
    }
  }
}

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min_element1_2_practical(I first, I last, Compare cmp) {
  if (first == last || successor(first) == last) {
    return std::make_pair(first, last);
  }

  compare_dereference<Compare> cmp_deref(cmp);

  I first_place = first++;
  I second_place = first++;
  if (cmp_deref(second_place, first_place)) std::swap(first_place, second_place);

  while (first != last) insert_2(first_place, second_place, first++, cmp_deref);
  return std::make_pair(first_place, second_place);
}

#endif
