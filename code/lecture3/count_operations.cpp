#include "count_operations.h"
#include "functorized.h"

int main() {  
  count_operations(16, 16 * 1024 * 1024, heap_sort_functor());
}
