#include "singleton.h"
#include <iostream>

typedef singleton<double> sd;
typedef singleton<double> si;

int main() {
  sd x(1.5);
  sd y(2.5);
  sd z(1);
  std::cout << x + x << std::endl;
  std::cout << y + y << std::endl;
  std::cout << x + z << std::endl;

}
  
  
