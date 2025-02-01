//
//  Cylinder.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#include "Cylinder.hpp"

bool cylinderIsOutsideBoundaries(glm::dvec3 position)
{
    //
    glm::dvec3 orientedPosition;
    
    if (CYLINDER_ORIENTATION == 0) {
        orientedPosition = glm::dvec3(position.z, position.y, -position.x);
    } else if (CYLINDER_ORIENTATION == 1) {
        orientedPosition = glm::dvec3(position.x, position.z, -position.y);
    } else if (CYLINDER_ORIENTATION == 2) {
        orientedPosition = position;
    }
    
    double radialDistanceSquared = orientedPosition.x * orientedPosition.x + orientedPosition.y * orientedPosition.y;

    // Check if particle is outside radial boundary
    if (radialDistanceSquared > CYLINDER_R * CYLINDER_R)
        return true;

    // Check if finite height cylinder and if particle is outside Z limits
    if (CYLINDER_Z > 0.0 && (orientedPosition.z > CYLINDER_Z || orientedPosition.z < -CYLINDER_Z))
        return true;

    return false;
}

glm::dvec3 cylinderReflectParticle(glm::dvec3 oldPosition, glm::dvec3 newPosition)
{
    
    glm::dvec3 orientedOldPosition;
    glm::dvec3 orientedNewPosition;
    
    if (CYLINDER_ORIENTATION == 0) {
        orientedOldPosition = glm::dvec3(oldPosition.z, oldPosition.y, -oldPosition.x);
        orientedNewPosition = glm::dvec3(newPosition.z, newPosition.y, -newPosition.x);
    } else if (CYLINDER_ORIENTATION == 1) {
        orientedOldPosition = glm::dvec3(oldPosition.x, oldPosition.z, -oldPosition.y);
        orientedNewPosition = glm::dvec3(newPosition.x, newPosition.z, -newPosition.y);
    } else if (CYLINDER_ORIENTATION == 2) {
        orientedOldPosition = oldPosition;
        orientedNewPosition = newPosition;
    }
    
    glm::dvec3 reflectedPosition = orientedNewPosition;
    double newPositionRadialDistanceSquared = orientedNewPosition.x * orientedNewPosition.x + orientedNewPosition.y * orientedNewPosition.y;

    // Reflect off the cylindrical wall (x, y direction)
    if (newPositionRadialDistanceSquared > CYLINDER_R * CYLINDER_R) {
        glm::dvec2 oldPosition2d (orientedOldPosition.x, orientedOldPosition.y);
        glm::dvec2 newPosition2d (orientedNewPosition.x, orientedNewPosition.y);
        
        // For now cylinder center is 0,0
        glm::dvec2 newReflectedPosition2d = reflectingOffCircle(oldPosition2d, newPosition2d, glm::dvec2(0.0, 0.0), CYLINDER_R);
        
        reflectedPosition.x = newReflectedPosition2d.x;
        reflectedPosition.y = newReflectedPosition2d.y;
    }
    
    
    // Reflect off Z-boundaries if CYLINDER_Z > 0 (finite height)
    if (CYLINDER_Z > 0.0) {
        if (orientedNewPosition.z > CYLINDER_Z) {
            reflectedPosition.z = 2 * CYLINDER_Z - orientedNewPosition.z;
        } else if (orientedNewPosition.z < -CYLINDER_Z) {
            reflectedPosition.z = -2 * CYLINDER_Z - orientedNewPosition.z;
        }
    }

    if (CYLINDER_ORIENTATION == 0) {
        reflectedPosition = glm::dvec3(-reflectedPosition.z, reflectedPosition.y, reflectedPosition.x);
    } else if (CYLINDER_ORIENTATION == 1) {
        reflectedPosition = glm::dvec3(reflectedPosition.x, -reflectedPosition.z, reflectedPosition.y);
    } else if (CYLINDER_ORIENTATION == 2) {
        reflectedPosition = reflectedPosition;
    }
    
    
    return reflectedPosition;
}

