#include <boost/math/distributions.hpp>
#include <iostream>
using std::cout;
using std::endl;
using namespace boost::math;
int main() {
    // x is set to test the pdf function of the dists
    double x = 5.0;

    // NORMAL
    normal_distribution<> myNormal(2.8, 1.0);
    cout << "NORMAL DIST" << endl;
    cout << "Mean: " << myNormal.mean()
        << ", standard deviation: " << myNormal.standard_deviation() << endl;
    cout << "pdf: " << pdf(myNormal, x) << endl;

    // CHI SQUARED
    chi_squared_distribution<> myChi(2);
    cout << "CHI SQUARED DIST" << endl;
    cout << "pdf: " << pdf(myChi, x) << endl;
    return 0;
}
