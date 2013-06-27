#ifndef MIN_ELEMENT1_2_H
#define MIN_ELEMENT1_2_H

#include <cstddef>
#include "algorithm.h"
#include "binary_counter.h"
#include "list_pool.h"

template <typename T, typename N>
inline
std::pair<T, N>
combine(const std::pair<T, N>& x,
	const std::pair<T, N>& y,
	list_pool<T, N>& pool) {
  free_list(pool, y.second);
  return std::make_pair(x.first, pool.allocate(y.first, x.second));
} 

template <typename T, typename N, typename Compare>
class op_min1_2 
{
private:
  Compare cmp;
  list_pool<T, N>* p;
public:
  typedef typename list_pool<T, N>::list_type list_type;
  typedef std::pair<T, list_type> argument_type;
  op_min1_2(const Compare& cmp, list_pool<T, N>& pool) 
    : cmp(cmp), p(&pool) {}
  argument_type operator()(const argument_type& x, 
			   const argument_type& y) {
    return cmp(y.first, x.first) ? combine(y, x, *p) : combine(x, y, *p);
  }
};

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

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min_element1_2(I first, I last, Compare cmp) {
  if (first == last || successor(first) == last) {
    return std::make_pair(first, last);
  }

  typedef typename list_pool<I, std::size_t>::list_type list_type;
  typedef op_min1_2<I, size_t, compare_dereference<Compare> > op_type;

  list_pool<I, std::size_t> pool;
  pool.reserve(256);
  compare_dereference<Compare> cmp_deref(cmp);
  op_type op(cmp_deref, pool);
  binary_counter<op_type> counter(op, std::make_pair(last, pool.end()));
  counter.reserve(32);

  while (first != last) counter.add(std::make_pair(first++, pool.end()));
  typename op_type::argument_type min1_list = counter.reduce();
  I min1 = min1_list.first;
  I min2 = pool.value(min_element_list(pool, min1_list.second, cmp_deref));
  return std::make_pair(min1, min2);
}

/****************** stable algorithm for random access ****************************/


template <typename Compare>
class compare_dereference_random_access
{
private:
  Compare cmp;
public:
  compare_dereference_random_access(const Compare& cmp) : cmp(cmp) {}
  template <typename I>
  bool operator() (const I& x, const I& y) const {
    if (x < y) {
      return !cmp(*y, *x);
    } else {
      return cmp(*x, *y);
    }
  }
};

template <typename I, typename Compare>
// requires I is a RandomAccessIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min_element1_2_stable_random_access(I first, I last, Compare cmp) {
  if (first == last || successor(first) == last) {
    return std::make_pair(first, last);
  }

  typedef typename list_pool<I, std::size_t>::list_type list_type;
  typedef op_min1_2<I, size_t, compare_dereference<Compare> > op_type;

  list_pool<I, std::size_t> pool;
  pool.reserve(256);
  compare_dereference<Compare> cmp_deref(cmp);
  compare_dereference_random_access<Compare> cmp_deref_stable(cmp);
  op_type op(cmp_deref, pool);
  binary_counter<op_type> counter(op, std::make_pair(last, pool.end()));
  counter.reserve(32);

  while (first != last) counter.add(std::make_pair(first++, pool.end()));
  typename op_type::argument_type min1_list = counter.reduce();
  I min1 = min1_list.first;
  I min2 = pool.value(min_element_list(pool, min1_list.second, cmp_deref_stable));
  return std::make_pair(min1, min2);
}


/****************** stable case best algorithm ****************************/

template <typename T, typename N, typename Compare>
class op_min1_2_stable 
{
private:
  Compare cmp;
  list_pool<T, N>* p;
public:
  typedef typename list_pool<T, N>::list_type list_type;
  typedef std::pair<T, std::pair<list_type, list_type> > argument_type;

  op_min1_2_stable(const Compare& cmp, list_pool<T, N>& pool) : cmp(cmp), p(&pool) {}

  argument_type operator()(const argument_type& x, 
                           const argument_type& y) {
    if (!cmp(y.first, x.first)) {
      p->free(y.second.first, y.second.second);
      return std::make_pair(x.first, push_back(*p, x.second.first, x.second.second, y.first));
    } else {
      p->free(x.second.first, x.second.second);
      return std::make_pair(y.first, push_front(*p, y.second.first, y.second.second, x.first));
    }
  }
};

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min_element1_2_stable(I first, I last, Compare cmp) {
  if (first == last || successor(first) == last) {
    return std::make_pair(first, last);
  }

  typedef typename list_pool<I, std::size_t>::list_type list_type;
  typedef op_min1_2_stable<I, size_t, compare_dereference<Compare> > op_type;

  list_pool<I, std::size_t> pool;
  pool.reserve(256);
  compare_dereference<Compare> cmp_deref(cmp);
  op_type op(cmp_deref, pool);
  std::pair<list_type, list_type> ends(pool.end(), pool.end());
  binary_counter<op_type> counter(op, std::make_pair(last, ends));
  counter.reserve(32);

  while (first != last) counter.add(std::make_pair(first++, ends));
  typename op_type::argument_type min1_list = counter.reduce();
  I min1 = min1_list.first;
  I min2 = pool.value(min_element_list(pool, min1_list.second.first, cmp_deref));
  return std::make_pair(min1, min2);
}

/****************** stable case algorithm indexed ****************************/


template <typename Compare>
class compare_dereference_first
{
private:
  Compare cmp;
public:
  compare_dereference_first(const Compare& cmp) : cmp(cmp) {}
  template <typename I>
  bool operator() (const I& x, const I& y) {
    return cmp(*(x.first), *(y.first));
  }
};

template <typename Compare>
class compare_dereference_stable
{
private:
  Compare cmp;
public:
  compare_dereference_stable(const Compare& cmp) : cmp(cmp) {}

  template <typename Pair>
  bool operator() (const Pair& x, const Pair& y) {
    return (x.second <= y.second) ?
      !cmp(*(y.first), *(x.first)) :
      cmp(*(x.first), *(y.first));
  }
};


template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min_element1_2_stable_indexed(I first, I last, Compare cmp) {
  if (first == last || successor(first) == last) {
    return std::make_pair(first, last);
  }

  typedef typename std::iterator_traits<I>::difference_type diff_t;
  typedef std::pair<I, diff_t> pair_type;
  typedef typename list_pool<pair_type, std::size_t>::list_type list_type;
  typedef op_min1_2<pair_type, std::size_t, compare_dereference_first<Compare> > op_type;
 
  list_pool<pair_type, std::size_t> pool;
  pool.reserve(256);
  compare_dereference_first<Compare> cmp_deref(cmp);
  compare_dereference_stable<Compare> cmp_deref_stable(cmp);
  op_type op(cmp_deref, pool);
  diff_t n(0);
  binary_counter<op_type> counter(op, std::make_pair(pair_type(last, n), pool.end()));
  counter.reserve(32);

  while (first != last) counter.add(std::make_pair(pair_type(first++, n++), pool.end()));
  typename op_type::argument_type min1_list = counter.reduce();
  I min1 = min1_list.first.first;
  I min2 = pool.value(min_element_list(pool, min1_list.second, cmp_deref_stable)).first;
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
