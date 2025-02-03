//
//  Cylinder.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#include "cylinder.hpp"

Cylinder::Cylinder() {
    if (MODE == 0) {
        orientation = SINGLE_CYLINDER_ORIENTATION;
        radius = SINGLE_CYLINDER_R;
        zLimit = SINGLE_CYLINDER_Z;
    }
}

glm::dvec3 Cylinder::orientPosition(const glm::dvec3& position) const {
    if (orientation == 0) return glm::dvec3(position.z, position.y, -position.x);
    if (orientation == 1) return glm::dvec3(position.x, position.z, -position.y);
    return position;  // orientation == 2
}

glm::dvec3 Cylinder::deorientPosition(const glm::dvec3& position) const {
    if (orientation == 0) return glm::dvec3(-position.z, position.y, position.x);
    if (orientation == 1) return glm::dvec3(position.x, -position.z, position.y);
    return position;  // orientation == 2
}

bool Cylinder::isOutsideBoundaries(const glm::dvec3& position) const {
    glm::dvec3 orientedPos = orientPosition(position);
    double radialDistanceSq = orientedPos.x * orientedPos.x + orientedPos.y * orientedPos.y;

    // Check if particle is outside radial boundary
    if (radialDistanceSq > radius * radius) return true;

    // Check if finite height cylinder and if particle is outside Z limits
    if (zLimit > 0.0 && (orientedPos.z > zLimit || orientedPos.z < -zLimit)) return true;

    return false;
}

glm::dvec3 Cylinder::reflectParticle(const glm::dvec3& oldPos, const glm::dvec3& newPos) const {
    glm::dvec3 orientedOld = orientPosition(oldPos);
    glm::dvec3 orientedNew = orientPosition(newPos);
    glm::dvec3 reflectedPos = orientedNew;

    double radialDistSq = orientedNew.x * orientedNew.x + orientedNew.y * orientedNew.y;

    // Reflect off the cylindrical wall (x, y direction)
    if (radialDistSq > radius * radius) {
        glm::dvec2 old2d(orientedOld.x, orientedOld.y);
        glm::dvec2 new2d(orientedNew.x, orientedNew.y);

        // For now cylinder center is 0,0
        glm::dvec2 reflected2d = reflectingOffCircle(old2d, new2d, glm::dvec2(0.0, 0.0), radius);
        
        reflectedPos.x = reflected2d.x;
        reflectedPos.y = reflected2d.y;
    }

    // Reflect off Z-boundaries if zLimit > 0 (finite height)
    if (zLimit > 0.0) {
        if (orientedNew.z > zLimit) {
            reflectedPos.z = 2 * zLimit - orientedNew.z;
        } else if (orientedNew.z < -zLimit) {
            reflectedPos.z = -2 * zLimit - orientedNew.z;
        }
    }

    return deorientPosition(reflectedPos);
}

double Cylinder::getRadius() const {
    return radius;
}

