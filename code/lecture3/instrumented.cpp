#include "instrumented.h"
#include <algorithm>

double instrumented_base::counts[];
const char* instrumented_base::counter_names[number_ops] = {"n", "copy", "assign", "destruct", "default", "equal", "less", "construct"};
void instrumented_base::initialize(size_t m) {
    std::fill(counts, counts + number_ops, 0.0);
    counts[n] = double(m);
}

