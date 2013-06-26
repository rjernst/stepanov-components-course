#ifndef MIN_ELEMENT1_2_H
#define MIN_ELEMENT1_2_H

#include <cstddef>
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
std::pair<I, I> min_element1_2_practical_lecture(I first, I last, Compare cmp) {
  if (first == last) return std::make_pair(last, last);
  I min = first;
  ++first;
  if (first == last) return std::make_pair(min, min);
  I min2 = first;
  if (cmp(*min2, *min)) std::swap(min, min2);
  ++first;
  while (first != last) {
    if (cmp(*first, *min2)) {
      if (cmp(*first, *min)) {
        min2 = min;
        min = first;
      } else {
        min2 = first;
      }
    }
    ++first;
  }
  return std::make_pair(min, min2); 
}

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min_element1_2(I first, I last, Compare cmp) {
  typedef typename list_pool<I, std::size_t>::list_type list_type;
  typedef compare_dereference<Compare> comp_deref_type;
  typedef op_min1_2<I, size_t, comp_deref_type> counter_operation_type;
  typedef binary_counter<counter_operation_type> counter_type;

  list_pool<I, std::size_t> pool;
  pool.reserve(256);
  comp_deref_type cmp_deref(cmp);
  counter_operation_type op(cmp_deref, pool);
  counter_type counter(op, std::make_pair(last, pool.end()));
  counter.reserve(32);

  while (first != last) counter.add(std::make_pair(first++, pool.end()));
  std::pair<I, list_type> tmp = counter.reduce();
  list_type list = min_element_list(pool, tmp.second, cmp_deref);
  return std::make_pair(tmp.first, pool.is_end(list) ? tmp.first : pool.value(list));
}

/****************** stable case algorithm 0 ****************************/


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
std::pair<I, I> min_element1_2_stable0(I first, I last, Compare cmp) {
  typedef typename list_pool<I, std::size_t>::list_type list_type;
  typedef compare_dereference<Compare> comp_deref_type;
  typedef compare_dereference_random_access<Compare> comp_deref_stable_type;
  typedef op_min1_2<I, size_t, comp_deref_type> counter_operation_type;
  typedef binary_counter<counter_operation_type> counter_type;

  list_pool<I, std::size_t> pool;
  comp_deref_type cmp_deref(cmp);
  comp_deref_stable_type cmp_deref_stable(cmp);
  counter_operation_type op(cmp_deref, pool);
  counter_type counter(op, std::make_pair(last, pool.end()));

  while (first != last) counter.add(std::make_pair(first++, pool.end()));
  std::pair<I, list_type> tmp = counter.reduce();
  list_type list = min_element_list(pool, tmp.second, cmp_deref_stable);
  return std::make_pair(tmp.first, pool.is_end(list) ? tmp.first : pool.value(list));
}


/****************** stable case algorithm 1 ****************************/

template <typename T, typename N>
inline
std::pair<T, std::pair<N, N> >
combine(const std::pair<T, std::pair<N, N> >& x,
        const std::pair<T, std::pair<N, N> >& y,
        list_pool<T, N >& pool,
        bool x_won) {
  if (x_won) {
    free_list(pool, y.second.first);
    free_list(pool, y.second.second);
    return std::make_pair(x.first, 
                          std::make_pair(x.second.first,
                                         pool.allocate(y.first, x.second.second)));
  } else  {
    free_list(pool, x.second.first);
    free_list(pool, x.second.second);
    return std::make_pair(y.first, 
                          std::make_pair(pool.allocate(x.first, y.second.first),
					                               y.second.second));
  }
} 

template <typename T, typename N, typename Compare>
class op_min1_2_stable 
{
private:
  Compare cmp;
  list_pool<T, N>* p;
public:
  typedef typename list_pool<T, N>::list_type list_type;
  typedef std::pair<T, std::pair<list_type, list_type> > argument_type;
  op_min1_2_stable(const Compare& cmp, list_pool<T, N>& pool) 
    : cmp(cmp), p(&pool) {}
  argument_type operator()(const argument_type& x, 
                           const argument_type& y) {
    return combine(x, y, *p, !cmp(y.first, x.first));
  }
};


template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min_element1_2_stable(I first, I last, Compare cmp) {
  typedef typename list_pool<I, std::size_t>::list_type list_type;
  typedef compare_dereference<Compare> comp_deref_type;
  typedef op_min1_2_stable<I, size_t, comp_deref_type> counter_operation_type;
  typedef binary_counter<counter_operation_type> counter_type;

  list_pool<I, std::size_t> pool;
  comp_deref_type cmp_deref(cmp);
  counter_operation_type op(cmp_deref, pool);
  
  std::pair<list_type, list_type> pair_of_ends(pool.end(), pool.end());
  counter_type counter(op, std::make_pair(last, pair_of_ends));

  while (first != last) counter.add(std::make_pair(first++, pair_of_ends));
  std::pair<I, std::pair<list_type, list_type> > min_and_candidates = counter.reduce();

  I min_element = min_and_candidates.first;
  list_type left_list  = min_and_candidates.second.first;
  list_type right_list = min_and_candidates.second.second;

  list_type left_candidate = min_element_list(pool, left_list, cmp_deref);
  list_type right_candidate = min_element_last_list(pool, right_list, cmp_deref);

  if (pool.is_end(left_candidate) && pool.is_end(right_candidate)) {
    return std::make_pair(min_element, min_element);
  }
  if (pool.is_end(left_candidate)) {
     return std::make_pair(min_element, pool.value(right_candidate));
  }
  if (pool.is_end(right_candidate)) {
     return std::make_pair(min_element, pool.value(left_candidate));
  }
  if (cmp_deref(pool.value(right_candidate), pool.value(left_candidate))) {
      return std::make_pair(min_element, pool.value(right_candidate));
  }
  return std::make_pair(min_element, pool.value(left_candidate));
}

/****************** stable case algorithm 2 ****************************/


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
std::pair<I, I> min_element1_2_stable2(I first, I last, Compare cmp) {
  typedef typename std::iterator_traits<I>::difference_type diff_t;
  typedef std::pair<I, diff_t> pair_type;
  typedef list_pool<pair_type, std::size_t> pool_type;
  typedef typename pool_type::list_type list_type;
  typedef compare_dereference_first<Compare> comp_deref_type;
  typedef compare_dereference_stable<Compare> comp_deref_stable_type;
  typedef op_min1_2<pair_type, size_t, comp_deref_type> counter_operation_type;
  typedef binary_counter<counter_operation_type> counter_type;

  pool_type pool;
  comp_deref_type cmp_deref(cmp);
  counter_operation_type op(cmp_deref, pool);
  comp_deref_stable_type op1(cmp);
  diff_t n(0);
  counter_type counter(op, std::make_pair(pair_type(last, n), pool.end()));

  while (first != last) counter.add(std::make_pair(pair_type(first++, n++), pool.end()));
  std::pair<pair_type, list_type> tmp = counter.reduce();
  list_type list = min_element_list(pool, tmp.second, op1);
  return std::make_pair(tmp.first.first, pool.is_end(list) ? tmp.first.first : pool.value(list).first);
}


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
  compare_dereference<Compare> cmp_deref(cmp);
  if (first == last) return std::make_pair(first, first);
  I first_place = first++;
  if (first == last) return std::make_pair(first, first);
  I second_place = first++;

  if (cmp_deref(second_place, first_place)) std::swap(first_place, second_place);
  while (first != last) insert_2(first_place, second_place, first++, cmp_deref);
  return std::make_pair(first_place, second_place);
}

#endif
