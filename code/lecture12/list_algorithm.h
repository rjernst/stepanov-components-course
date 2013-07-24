
#include "binary_counter.h"

#include "merge_linked.h"

template <typename I>
// requires I is Linked Iterator
I reverse_linked(I first, I last, I tail) {
  while (first != last) {
    I next = first;
    ++next;
    set_successor(first, tail);
    tail = first;
    first = next;
  }
  return tail;
}

template <typename I, typename Compare>
// requires I is Linked Iterator
I merge_linked_simple(I first1, I last1, I first2, I last2, Compare cmp) {
  I result = last1;
  while (first1 != last1 && first2 != last2) {
    I tmp = cmp(*first2, *first1) ? first2++ : first1++;
    set_successor(tmp, result);
    result = tmp;
  }
  return reverse_linked(result, last1, first1 == last1 ? first2 : first1);
}

template <typename I, typename Compare>
// I is Linked Iterator
struct mergesort_linked_operation
{
  typedef I argument_type;
  I nil;
  Compare cmp;
  mergesort_linked_operation(I nil, const Compare& cmp) : nil(nil), cmp(cmp) {}
  I operator()(I x, I y) { return merge_linked_simple(x, nil, y, nil, cmp); }
};

template <typename I, typename Compare>
// I is Linked Iterator
I mergesort_linked(I first, I last, Compare cmp) {
  mergesort_linked_operation<I, Compare> op(last, cmp);
  binary_counter<mergesort_linked_operation<I, Compare> > counter(op, last);
  counter.reserve(16);
  while (first != last) {
    I tmp = first++;
    set_successor(tmp, last);
    counter.add(tmp);
  }
  return counter.reduce();
}

template <typename I0, typename I1>
// requires I0 is Input Iterator
// requires I1 is Singly Linked List Iterator
I1 generate_list(I0 first, I0 last, I1 tail) {  
  if (first == last) return tail;
  push_front(tail, *first++);
  I1 front = tail;
  while (first != last) {
    push_back(tail, *first++);
    ++tail;
  }
  return front;
}





