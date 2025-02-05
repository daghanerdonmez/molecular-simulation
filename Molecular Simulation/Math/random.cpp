//
//  random.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 4.02.2025.
//

#include "random.hpp"

// Function to generate a random point in a circle of radius r
std::pair<double, double> generatePointInCircle(double r) {
    // Random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angleDist(0, 2 * M_PI);  // θ in [0, 2π)
    std::uniform_real_distribution<> radiusDist(0, 1);        // For uniform area distribution

    // Generate random angle and radius
    double theta = angleDist(gen);
    double radius = r * std::sqrt(radiusDist(gen));  // Adjust for uniform distribution over the area

    // Convert polar coordinates to Cartesian coordinates
    double x = radius * std::cos(theta);
    double y = radius * std::sin(theta);

    return {x, y};
}
