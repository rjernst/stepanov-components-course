
template <typename I, typename Compare>
// I is a linked forward iterator
std::pair<I, std::pair<I, I> >
merge_linked_non_empty(I first1, I last1, I first2, I last2, Compare cmp) {
  I head, tail;
  if (cmp(*first2, *first1)) {
    head = first2;
    tail = first2;
    ++first2;
    goto winner2;
  } else {
    head = first1;
    tail = first1;
    ++first1;
    // goto winner1;
  }
 winner1:
  if (first1 == last1) goto empty1;
  if (!cmp(*first2, *first1)) {
    tail = first1;
    ++first1;
    goto winner1;
  } else {
    set_successor(tail, first2); 
    tail = first2;
    ++first2;
    // goto winner2;
  }
 winner2:
  if (first2 == last2) goto empty2;
  if (cmp(*first2, *first1)) {
    tail = first2;
    ++first2;
    goto winner2;
  } else {
    set_successor(tail, first1);
    tail = first1;
    ++first1;
    goto winner1;
  }
 empty1:
  set_successor(tail, first2);
  return std::make_pair(head, std::make_pair(first2, last2));
 empty2:
  set_successor(tail, first1);
  return std::make_pair(head, std::make_pair(first1, last1));
}
