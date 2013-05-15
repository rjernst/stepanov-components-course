
template <typename T>
// requires T is Semiregular
inline
void swap(T& a, T& b) {
    T tmp(a);
    a = b;
    b = tmp; 
}

// partial specialization (won't actually work here, needs to be friend of vector)
template <typename T>
inline
void swap(std::vector<T>& a, std::vector<T>& b) {
    // swap headers of a and b
    // fix back pointers (if they exist)
}

/*
std::vector<int> a(1000000);
std::vector<int> b(1000000);
swap(a, b);
*/

template <typename T>
// requires T is UnsignedIntegral
inline
void swap_xor(T& a, T& b) {
    if (&a != &b) {
        a = a ^ b; // a ^ b, b
        b = a ^ b; // a ^ b, a
        a = a ^ b; // b, a
    }
}
