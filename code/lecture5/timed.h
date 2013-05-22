/*-----------------------------------------------------------------------------
 * Utilities for timing functions
 *----------------------------------------------------------------------------*/

#ifndef TIMED_H
#define TIMED_H

#include <algorithm>
#include <ctime>

class timer {
private:
    clock_t start_time;
public:
    typedef double result_type;

    void start() {
        start_time = clock();
    }

    result_type seconds() {
        return (clock() - start_time) / double(CLOCKS_PER_SEC);
    }

    result_type nanoseconds() {
      return 1000000000.0 * seconds();
    }
};


// Function object for applying unary function to all elements of a range.

template <typename Function>
struct transform_all {
  Function fun;

  transform_all() {};
  transform_all(const Function& fun) : fun(fun) {};

  template <typename I> // I is forward iterator
  void operator()(I first, I last) { std::transform(first, last, first, fun); }
};

// Copy data from [source_first, source_last), then
// apply function fun to buffer.  Repeat iters times.
// Subtracts time to copy from total time

template <template <typename T, class Alloc = std::allocator<T> > class Container>
struct timed_function
{
  template <typename I, typename Function> // I is forward iterator
  double operator()(I source_first, I source_last, size_t iters, Function fun) {
    typedef typename std::iterator_traits<I>::value_type T;
    Container<T> buf(std::distance(source_first, source_last));
    timer t;
    t.start();
    for (size_t i = iters; i > 0; --i) {
        std::copy(source_first, source_last, buf.begin());
    }
    double copy_time = t.seconds();
    t.start();
    for (size_t i = iters; i > 0; --i) {
        std::copy(source_first, source_last, buf.begin());
        fun(buf.begin(), buf.end());
    }
    return t.seconds() - copy_time;
  }
};

// Copy data from [source_first, source_last) one segment at a time.
// apply function fun to buffer of segment_size
// Subtracts time to copy from total time

template <template <typename T, class Alloc = std::allocator<T> > class Container>
struct timed_function_segmented
{
  template <typename I, typename Function> // I is random access iterator
  double operator()(I source_first, I source_last, size_t segment_size, Function fun) {
    typedef typename std::iterator_traits<I>::value_type T;
    Container<T> buf(segment_size);
    timer t;
    t.start();
    for (I i = source_first; source_last - i >= segment_size; i += segment_size) {
        std::copy(i, i + segment_size, buf.begin());
    }
    double copy_time = t.seconds();
    t.start();
    for (I i = source_first; source_last - i >= segment_size; i += segment_size) {
        std::copy(i, i + segment_size, buf.begin());
        fun(buf.begin(), buf.end());
    }
    return t.seconds() - copy_time;
  }
  template <typename Buffer, typename Function> // Function takes two iterators
  void operator()(Buffer& buffer, size_t segment_from, size_t segment_to, Function fun, std::vector<double>& results) {
    while (segment_from <= segment_to) {
      results.push_back(operator()(buffer.begin(), buffer.end(), segment_from, fun));
      segment_from <<= 1;
    }
  }
};



#endif
