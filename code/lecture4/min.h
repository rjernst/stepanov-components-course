
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
