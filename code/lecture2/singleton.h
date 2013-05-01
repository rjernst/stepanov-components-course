#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T> 
// T is Semiregualr or Regular or TotallyOrdered
struct singleton
{
  typedef T value_type;
  T value;
  // Conversions from T and to T:
  explicit singleton(const T& x) : value(x) {}
  explicit operator T() const { return value; }
  template <typename U>
  singleton(const singleton<U>& x) : value(x.value) {}
  // Write conversions from T to singleton<T> and singleton<T> to T.

  // Semiregular:
  singleton(const singleton& x) : value(x.value) {} // could be implicitly declared
  singleton() {}                                    // could be implicitly declared sometimes
  ~singleton() {}                                   // could be implicitly declared 
  singleton& operator=(const singleton& x) {        // could be implicitly declared 
    value = x.value;
    return *this;
  }
  // Regular
  friend
  bool operator==(const singleton& x, const singleton& y) {
    return x.value == y.value;
  }
  friend
  bool operator!=(const singleton& x, const singleton& y) {
     return !(x == y);
  }
  // TotallyOrdered
  friend
  bool operator<(const singleton& x, const singleton& y) { 
    return x.value < y.value;
  }
  friend
  bool operator>(const singleton& x, const singleton& y) {
    return y < x;
  }
  friend
  bool operator<=(const singleton& x, const singleton& y) {
    return !(y < x);
  }
  friend
  bool operator>=(const singleton& x, const singleton& y) {
    return !(x < y);
  } 


};

#endif







