#ifndef FUNCTORIZED_H
#define FUNCTORIZED_H

#include <algorithm>

#include "setsort.h"

/*
strict functorization:

template <typename T0, ..., typename Tn>
R function_name(args) { code }

struct function_name_functor 
{
   template <typename T0, ..., typename Tn>
   R operator()(args) const { code }
};

partial functorization:

template <typename T0, ...,typename Ti, ..., typename Tn>
R function_name(args) { code }

template <typename Ti> // could be done for more than 1 type argument
struct function_name_functor 
{
   template <typename T0, ..., typename Tn> // does not include Ti
   R operator()(args) const { code }
};

*/  

template <typename T> 
// multiplicative semigroup
inline
T square(const T& x) { return x * x; }

struct square_functor {
  template <typename T>
  T operator()(const T& x) const { 
    return square(x); 
  }
}; 

struct sort_functor
{
  template <typename I> 
  // I is random-access iterator
  void operator()(I first, I last) const { 
    std::sort(first, last); 
  }
};

struct sort_unique_functor
{
  template <typename I> 
  // I is random-access iterator
  I operator()(I first, I last) const { 
    std::sort(first, last); 
    return std::unique(first, last); 
  }
};

struct setsort_functor
{
  template <typename I> 
  // I is forward iterator
  void operator()(I first, I last) const { 
    setsort(first, last); 
  }
};

struct setsort_unique_functor
{
  template <typename I> 
  // I is forward iterator
  I operator()(I first, I last) const { 
    return setsort_unique(first, last); 
  }
};


struct stable_sort_functor
{
  template <typename I> 
  // I is random-access iterator
  void operator()(I first, I last) const { std::stable_sort(first, last); }
};

struct heap_sort_functor
{
  template <typename I> 
  // I is random-access iterator
  void operator()(I first, I last) const { std::partial_sort(first, last, last); }
};

#endif
