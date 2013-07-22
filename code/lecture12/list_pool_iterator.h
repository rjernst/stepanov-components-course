
struct iterator
// iterator is a lined forward iterator
{
  typedef typename std::forward_iterator_tag iterator_category;
  typedef T value_type;
  typedef list_type difference_type;

  typedef T& reference;
  typedef T* pointer;

  list_pool* p;
  list_type node;
  
  iterator() : p(NULL) {}

  iterator(list_pool& pool, list_type node) : p(&pool), node(node) {} 
  iterator(list_pool& pool) : p(&pool), node(pool.end()) {} 

  reference operator*() const { 
    return p->value(node);
  }

  pointer operator->() const { return &(operator*()); }

  iterator& operator++() { 
    node = p->next(node);
    return *this;
  }

  iterator operator++(int) {
    iterator tmp = *this;
    ++*this;
    return tmp;
  }

  friend
  bool operator==(const iterator& x, const iterator& y) {
    // assert(x.p == y.p)
    return x.node == y.node;
  }

  friend
  bool operator!=(const iterator& x, const iterator& y) {
    return !(x == y);
  }

  friend
  void set_successor(iterator x, iterator y) {
    // assert(x.p == y.p)
    x.p->next(x.node) = y.node;
  }
};

