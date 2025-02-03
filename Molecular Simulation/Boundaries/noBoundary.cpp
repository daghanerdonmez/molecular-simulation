//
//  NoBoundary.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#include "noBoundary.hpp"

NoBoundary::NoBoundary(){}

bool NoBoundary::isOutsideBoundaries(const glm::dvec3 &position) const {
    return false;
}

glm::dvec3 NoBoundary::reflectParticle(const glm::dvec3 &oldPosition, const glm::dvec3 &newPosition) const {
    return newPosition;
}
