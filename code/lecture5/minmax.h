
/*
min<int, std::greater<int> >
min<int>
*/

namespace course {

template <typename T>
// requires T is TotallyOrdered
struct less {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};

/*
std::sort(a.begin(), a.end(), less<int>());
*/


template <typename T, typename Compare>
// requires Compare defines a StrictWeakOrdering on T
inline
const T& min(const T& a, const T& b, Compare cmp) {
    if (cmp(b, a)) {
        return b;
    } else {
        return a;
    }
}

template <typename T, typename Compare>
// requires Compare defines a StrictWeakOrdering on T
inline
const T& max(const T& a, const T& b, Compare cmp) {
    if (cmp(b, a)) {
        return a;
    } else {
        return b;
    }
}

template <typename T>
inline
const T& min(const T& a, const T& b) {
    return min(a, b, std::less<T>());
}

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
I min_element(I first, I last, Compare cmp) {
    if (first == last) return last;
    I min_el = first;
    while (++first != last) {
        if (cmp(*first, *min_el)) {
            min_el = first;
        }
    }
    return min_el; 
}

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
I max_element(I first, I last, Compare cmp) {
    if (first == last) return last;
    I max_el = first;
    while (++first != last) {
        if (!cmp(*first, *max_el)) {
            max_el = first;
        }
    }
    return max_el; 
}

template <typename T, typename Compare>
// requires T is StrictWeakOrdering
inline
void sort2(T& a, T& b, Compare cmp) {
    if (cmp(b, a)) {
        std::swap(a, b);
    }
}

/*
template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> minmax_element(I first, I last, Compare cmp) {
    if (first == last) return std::make_pair(last, last);
    // TODO: intialize min_el and max_el, first and next
    
    // TODO: fill in loop condition
    while (...) {
        // invariants:
        // min_el points to current minimum
        // max_el points to current maximum
        // first and next point to next 2 elements
        I potential_min = first;
        I potential_max = next; 
        if (cmp(*potential_max, *potential_min)) {
            std::swap(potential_max, potential_min);
        }
        if (!cmp(*potential_max, *max_el)) {
            max_el = potential_max;
        }
        if (cmp(*potential_min, *min_el)) {
            min_el = potential_min;
        }
        // TODO: advance first and next
    }
    // TODO: handle endcase
    // TODO: return something
}

*/

template <typename I>
I successor(I i) {
    I j = i;
    ++j;
    return j;
}
template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> minmax_element(I first, I last, Compare cmp) {
    if (first == last) return std::make_pair(last, last);
    I min_el = first;
    ++first;
    if (first == last) return std::make_pair(min_el, min_el);
    ++first;
    I max_el = first;
    if (cmp(*max_el, *min_el)) {
        std::swap(min_el, max_el);
    }
    while (first != last && successor(first) != last) {
        // invariants:
        // min_el points to current minimum
        // max_el points to current maximum
        // first and next point to next 2 elements
        I potential_min = first;
        I potential_max = successor(first); 
        if (cmp(*potential_max, *potential_min)) {
            std::swap(potential_max, potential_min);
        }
        if (!cmp(*potential_max, *max_el)) {
            max_el = potential_max;
        }
        if (cmp(*potential_min, *min_el)) {
            min_el = potential_min;
        }
        ++first;
        ++first;
    }
    if (first != last) {
        if (!cmp(*first, *max_el)) {
            max_el = first;
        }
        if (cmp(*first, *min_el)) {
            min_el = first;
        }
    }
    return std::make_pair(min_el, max_el);
}

} // end namespace course
