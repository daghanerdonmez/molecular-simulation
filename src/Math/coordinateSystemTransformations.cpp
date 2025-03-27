//
//  coordinateSystemTransformations.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 2.03.2025.
//

#include "coordinateSystemTransformations.hpp"

glm::dvec3 cartesianToCylindrical(glm::dvec3 vector) {
    double x = vector.x;
    double y = vector.y;
    double z = vector.z;

    double r = std::sqrt(x * x + y * y);
    double theta = std::atan2(y, x);

    // Returns cylindrical coordinates (r, theta, z)
    return glm::dvec3(r, theta, z);
}

glm::dvec3 cylindricalToCartesian(glm::dvec3 vector) {
    double r = vector.x;
    double theta = vector.y;
    double z = vector.z;
    
    double x = r * std::cos(theta);
    double y = r * std::sin(theta);
    
    return glm::dvec3(x, y, z);
}
