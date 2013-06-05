#include "count_operations.h"
#include "functorized.h"

int main() {  
  count_operations(16, 16 * 1028 * 1028, heap_sort_functor(),
		   normalized_by_nlogn1);
}
                      
