//
//  Cylinder.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#include "Cylinder.hpp"

bool cylinderIsOutsideBoundaries(glm::dvec3 position)
{
    double radialDistanceSquared = position.x * position.x + position.y * position.y;

    // Check if particle is outside radial boundary
    if (radialDistanceSquared > CYLINDER_R * CYLINDER_R)
        return true;

    // Check if finite height cylinder and if particle is outside Z limits
    if (CYLINDER_Z > 0.0 && (position.z > CYLINDER_Z || position.z < -CYLINDER_Z))
        return true;

    return false;
}

glm::dvec3 cylinderReflectParticle(glm::dvec3 oldPosition, glm::dvec3 newPosition)
{
    glm::dvec3 reflectedPosition = newPosition;
    double newPositionRadialDistanceSquared = newPosition.x * newPosition.x + newPosition.y * newPosition.y;

    // Reflect off the cylindrical wall (x, y direction)
    if (newPositionRadialDistanceSquared > CYLINDER_R * CYLINDER_R) {
        glm::dvec2 oldPosition2d (oldPosition.x, oldPosition.y);
        glm::dvec2 newPosition2d (newPosition.x, newPosition.y);
        
        // For now cylinder center is 0,0
        glm::dvec2 newReflectedPosition2d = reflectingOffCircle(oldPosition2d, newPosition2d, glm::dvec2(0.0, 0.0), CYLINDER_R);
        
        reflectedPosition.x = newReflectedPosition2d.x;
        reflectedPosition.y = newReflectedPosition2d.y;
    }
    
    
    // Reflect off Z-boundaries if CYLINDER_Z > 0 (finite height)
    if (CYLINDER_Z > 0.0) {
        if (newPosition.z > CYLINDER_Z) {
            reflectedPosition.z = 2 * CYLINDER_Z - newPosition.z;
        } else if (newPosition.z < -CYLINDER_Z) {
            reflectedPosition.z = -2 * CYLINDER_Z - newPosition.z;
        }
    }

    return reflectedPosition;
}

