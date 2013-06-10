#include <cstddef>
#include <vector>

template <typename T, typename N = size_t>
// Regular<T>
// Unsigned<N>
class list_pool
{
private:
  struct node_t {
    T value;
    N next;
  };
  std::vector<node_t> pool;
  N free_list;
  node_t& node(N n) { return pool[n - 1]; }
  const node_t& node(N n) const { return pool[n - 1]; }
  // 0 index indicate end; thefore, we shift indices by one
  void push_back() { pool.push_back(node_t()); }
public:
  typedef N list_type;

  list_type size() const { return pool.size(); }
  bool empty() const { return pool.empty(); }
  void reserve(list_type n) { pool.reserve(n); }

  list_type end() const { return list_type(0); }

  list_pool() { free_list = end(); }
  
  bool is_end(list_type n) const { return n == end(); }
  bool is_valid(list_type n) const { return n <= size(); }
  bool has_value(list_type n) const { return is_valid(n) && !is_end(n); }

  // precondition for value and next: has_value(n)
  T& value(list_type n) { return node(n).value; } 
  const T& value(list_type n) const { return node(n).value; }
  list_type& next(list_type n) { return node(n).next; }
  const list_type& next(list_type n) const { return node(n).next; }  

  list_type allocate(const T& val, list_type tail) {
    // precondition: is_valid(next);
    list_type tmp = free_list;
    if (is_end(tmp)) {
      push_back();
      tmp = size();
    } else {
      free_list = next(free_list);
    }
    value(tmp) = val;
    next(tmp) = tail;
    return tmp;
  }

  list_type free(list_type n) {
    // precondition: has_value(n);
    list_type tmp(next(n));
    next(n) = free_list;
    free_list = n;
    return tmp;
  }
};

template <typename T, typename N>
void free_list(list_pool<T, N>& pool,
		    typename list_pool<T, N>::list_type list) {
  while (!pool.is_end(list)) list = pool.free(list);
}
 
template <typename T, typename N, typename F>
void for_each_list(const list_pool<T, N>& pool, 
		     typename list_pool<T, N>::list_type list,
		     F fun) {
  while (!pool.is_end(list)) {
    f(pool.value(list));
    list = pool.next(list);
  }
}

template <typename T, typename N, typename Compare>
typename list_pool<T, N>::list_type
min_element_list(const list_pool<T, N>& pool, 
		 typename list_pool<T, N>::list_type list,
		 Compare cmp) {
  if (pool.is_end(list)) return list;
  typename list_pool<T, N>::list_type current_min = list;
  list = pool.next(list);
  while (!pool.is_end(list)) {
    if (cmp(pool.value(list), pool.value(current_min)))
      current_min = list;
    list = pool.next(list);
  }
  return current_min;
}
      
template <typename T, typename N>
typename list_pool<T, N>::index_t 
reverse_append_list(list_pool<T, N>& pool, 
			 typename list_pool<T, N>::list_type first,
			 typename list_pool<T, N>::list_type second) {
  while (!pool.is_end(first)) {
    typename list_pool<T, N>::list_type next = pool.next(first);
    pool.next(first) = second;
    second = first;
    first = next;
  }
  return second;
}
  


    
