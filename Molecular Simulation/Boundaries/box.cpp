//
//  Box.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#include "box.hpp"

Box::Box() {
    if (MODE == 0) {
        boundaryX = SINGLE_BOX_BOUNDARY_X;
        boundaryY = SINGLE_BOX_BOUNDARY_Y;
        boundaryZ = SINGLE_BOX_BOUNDARY_Z;
    }
}

double Box::getBoundaryZ(double x, double y) const {
    return boundaryZ;
}

double Box::getBoundaryY(double x, double z) const {
    return boundaryY;
}

double Box::getBoundaryX(double y, double z) const {
    return boundaryX;
}

bool Box::isOutsideBoundaries(const glm::dvec3& position) const {
    // Simply check if the particle is inside the box boundaries
    return (position.x > boundaryX || position.x < -boundaryX ||
            position.y > boundaryY || position.y < -boundaryY ||
            position.z > boundaryZ || position.z < -boundaryZ);
}

glm::dvec3 Box::reflectParticle(const glm::dvec3& oldPosition, const glm::dvec3& position) const {
    // I am getting old position as a paremeter but not using it since box boundary does not need that
    // It is there to match the Boundary interface (for example the cylinder boundary needs it)
    
    glm::dvec3 reflectedPosition = position;

    // Reflect off X boundaries
    if (position.x > boundaryX) {
        reflectedPosition.x = 2 * boundaryX - position.x;
    } else if (position.x < -boundaryX) {
        reflectedPosition.x = -2 * boundaryX - position.x;
    }

    // Reflect off Y boundaries
    if (position.y > boundaryY) {
        reflectedPosition.y = 2 * boundaryY - position.y;
    } else if (position.y < -boundaryY) {
        reflectedPosition.y = -2 * boundaryY - position.y;
    }

    // Reflect off Z boundaries
    if (position.z > boundaryZ) {
        reflectedPosition.z = 2 * boundaryZ - position.z;
    } else if (position.z < -boundaryZ) {
        reflectedPosition.z = -2 * boundaryZ - position.z;
    }

    return reflectedPosition;
}
