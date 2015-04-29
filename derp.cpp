#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <boost/math/distributions.hpp>
#include <boost/math/distributions/normal.hpp>

using namespace std;
int main(){
    boost::math::normal_distribution<>test(10.0, 1.0);
    std::cout <<  "PDF: " << pdf(test, 10.0) << "\n";
    std::cout << "Range: " << range(test).first << "\n";
  return 0;
}
