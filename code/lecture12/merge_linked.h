


template <typename I>
// I is a linked forward iterator
triple<I, I, I> merge_non_empty(I first1, I last1, I first2, I last2, Compare cmp) {
  I result = first1;
  if (cmp(*first2, *first1)) {
    result = first2++;
    if (first2 == last2) {
	set_successor(result, first1);
	return make_triple(result, first1, last1);
    }
  } else {
    ++first1;
    if (first1 == last1) {
	set_successor(result, first2);
	return make_triple(result, first2, last2);
    }
  }
  I tail = result;
  while (true) {
    if (cmp(*first2, *first1)) {
      set_successor(tail, first2);
      tail = first2++;
      if (first2 == last2) {
	set_successor(tail, first1);
	return make_triple(result, first1, last1);
      }
    } else {
      set_successor(tail, first1);
      tail = first1++;
      if (first1 == last1) {
	set_successor(tail, first2);
	return make_triple(result, first2, last2);
      } 
    }
  }
}
