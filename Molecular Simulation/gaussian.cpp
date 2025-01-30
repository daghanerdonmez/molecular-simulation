//
//  gaussian.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#include "gaussian.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// uses box-muller transformation
// if u1 and u2 are two uniformly distributed random variables between 0 and 1
// then the expression z0 below is a standard normal variable
// weird
double generateGaussian(double mean, double stddev)
{
    double u1 = rand() / (RAND_MAX + 1.0);
    double u2 = rand() / (RAND_MAX + 1.0);
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return z0 * stddev + mean;
}

// int main()
// {
//     srand(time(NULL));
//     printf("%.15f \n", generateGaussian(0, 2 * 79.4 * pow(10, -16)));
//     return 0;
// }

