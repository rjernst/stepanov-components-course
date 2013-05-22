
/*
min<int, std::greater<int> >
min<int>
*/

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
I min_element(I first, I last, Compare cmp) {
    if (first == last) return last;
    I min_el = first;
    ++first;
    while (first != last) {
        if (cmp(*first, *min_el)) {
            min_el = first;
        }
        ++first;
    }
    return min_el; 
}

template <typename T, typename Compare>
// requires T is StrictWeakOrdering
inline
void sort2(T& a, T& b, Compare cmp) {
    if (cmp(b, a)) {
        std::swap(a, b);
    }
}
