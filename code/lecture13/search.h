template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
I find_if(I first, I last, P pred) {
  while (first != last && !pred(*first)) ++first;
  return first;
}

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
I find_if_not(I first, I last, P pred) {
  while (first != last && pred(*first)) ++first;
  return first;
}

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
bool all_of(I first, I last, P pred) {
  return find_if_not(first, last, pred) == last;
}

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
bool none_of(I first, I last, P pred) {
  return find_if(first, last, pred) == last;
}

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
bool any_of(I first, I last, P pred) {
  return find_if(first, last, pred) != last;
}
 
template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
bool is_partitioned(I first, I last, P pred) {
  I first_true = find_if(first, last, pred);
  I next_false = find_if_not(first_true, last, pred);
  return next_false == last; 
}

template <typename I, typename N, typename P>
// I is InputIterator, N is Integral, P is UnaryPredicate
// value type of I is the same as argument type of P
std::pair<I, N> find_if_n(I first, N n, P pred) {
  while (n && !pred(*first)) {--n; ++first;}
  return std::make_pair<I, N> (first, n);
}

template <typename I, typename N, typename P>
// I is InputIterator, N is Integral, P is UnaryPredicate
// value type of I is the same as argument type of P
std::pair<I, N> find_if_not_n(I first, N n, P pred) {
  while (n && pred(*first)) {--n; ++first;}
  return std::make_pair<I, N> (first, n);
}

template <typename I, typename P>
// I is InputIterator, N is Integral, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
bool is_partitioned_n(I first, N n, P pred) {
  std::pair<I, N> first_true = find_if_n(first, n, pred);
  std::pair<I, N> next_false = find_if_not(first_true.first, first_true.second, pred);
  return !next_false.second; 
}

template <typename I, typename P>
// I is InputIterator, N is Integral, P is UnaryPredicate
// value type of I is the same as argument type of P
I partition_point_n(I first, N n, P pred) {
  // precondition: is_partitioned_n(first, n, pred)
  while (n) {
    N half = n >> 1;
    I middle = first;
    std::advance(middle, half);
    if (pred(*middle)) { 
      n = half; 
    }
    else { 
      n -= (half + 1); 
      first = ++middle; 
    }
  } 
  return first;
}

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
I partition_point(I first, I last, P pred) {
  // precondition: is_partitioned(first, last, pred)
  return partition_point_n(first, std::distance(first, last), pred);
}

template <typename I, typename N>
// I is InputIterator
// N is Integral
inline
void advance(I& first, N n) {
  // [first, n) is a valid range
  advance(first, n, std::iterator_traits<I>::iterator_category());
}

template <typename I, typename N>
// I is InputIterator
// N is Integral
inline
void advance(I& first, N n, std::input_iterator_tag) {
  // [first, n) is a valid range
  while (n--) ++first; 
}

template <typename I, typename N>
// I is RandomAccessIterator
// N is Integral
inline
void advance(I& first, N n, std::random_access_iterator_tag) {
  // [first, n) is a valid range
  first += n;
}

template <typename I>
// I is InputIterator
inline
typename std::iterator_traits<I>::difference_type 
distance(I first, I last) {
  // [first, n) is a valid range
  return distance(first, last, 
		  std::iterator_traits<I>::iterator_category());
}

template <typename I>
// I is InputIterator
inline
typename std::iterator_traits<I>::difference_type 
distance(I first, I last, std::input_iterator_tag) {
  // [first, n) is a valid range
  typename std::iterator_traits<I>::difference_type n(0);
  while (first != last) { 
    ++n; 
    ++first; 
  }
  return n; 
}

template <typename I>
// I is RandomAccessIterator
inline
typename std::iterator_traits<I>::difference_type 
distance(I first, I last, std::random_access_iterator_tag) {
  // [first, n) is a valid range
  return last - first; 
}

template <typename I, typename P>
I partition_point(I first, I last, P pred) {
  return partition_point_n(first, distance(first, last), pred);
}

