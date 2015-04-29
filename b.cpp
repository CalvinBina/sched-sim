#include <iostream>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
 
int main()
 { 
    typedef boost::mt19937                     ENG;    // Mersenne Twister
    typedef boost::normal_distribution<double> DIST;   // Normal Distribution
    typedef boost::variate_generator<ENG,DIST> GEN;    // Variate generator
 
    ENG  eng;
    DIST dist(50,18);
    GEN  gen(eng,dist);
 
    for (int i = 0; i < 20; i++)
        std::cout << gen() << std::endl;
 
    return 0;
}
