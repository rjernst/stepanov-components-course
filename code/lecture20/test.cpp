#include <iostream>

template <typename I>
void print_range(I first, I last) {
  while (first != last) {
    std::cout << *first << " ";
    ++first;
  }
  std::cout << std::endl;
}

template <typename I>
// I is ForwardIterator
void rotate_right_by_one(I first, I last) {
  if (first == last) return;
  I current = first; 
  while (++current != last) std::swap(*first, *current);
}

int main () {
  int a[] = {1, 2, 3, 4};
  print_range(a, a + 4);
  rotate_right_by_one(a, a + 4);
  print_range(a, a + 4);
}
