
#include <iostream>

template <typename T>
// requires T is Regular
void check_equality1(T& a) {
    std::cout << a << " == " << a << std::endl;
    std::cout << (a == a ? "yes" : "no") << std::endl;
}

template <typename T>
// requires T is Regular
void check_equality2(T& a) {
    std::cout << a << " < " << a << " || " << a << " >= " << a << std::endl;
    std::cout << (a < a || a >= a? "yes" : "no") << std::endl;
}

int main() {
    double x(0.0/0.0); 
    check_equality1(x);
    check_equality2(x);
}
