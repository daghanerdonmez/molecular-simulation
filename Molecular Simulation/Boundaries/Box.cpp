//
//  Box.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#include "Box.hpp"

float Box_Z(float x, float y)
{
    return BOX_BOUNDARY_Z;
}

float Box_Y(float x, float z)
{
    return BOX_BOUNDARY_Y;
}

float Box_X(float y, float z)
{
    return BOX_BOUNDARY_X;
}

bool boxIsOutsideBoundaries(glm::vec3 position)
{
    // simply check if the particle is inside the box boundaries
    return (position.x > BOX_BOUNDARY_X || position.x < -BOX_BOUNDARY_X ||
            position.y > BOX_BOUNDARY_Y || position.y < -BOX_BOUNDARY_Y ||
            position.z > BOX_BOUNDARY_Z || position.z < -BOX_BOUNDARY_Z);
}

glm::vec3 boxReflectParticle(glm::vec3 position)
{
    glm::vec3 reflectedPosition = position;

    if (position.x > BOX_BOUNDARY_X) {
        reflectedPosition.x = 2 * BOX_BOUNDARY_X - position.x;
    } else if (position.x < -BOX_BOUNDARY_X) {
        reflectedPosition.x = -2 * BOX_BOUNDARY_X - position.x;
    }

    if (position.y > BOX_BOUNDARY_Y) {
        reflectedPosition.y = 2 * BOX_BOUNDARY_Y - position.y;
    } else if (position.y < -BOX_BOUNDARY_Y) {
        reflectedPosition.y = -2 * BOX_BOUNDARY_Y - position.y;
    }

    if (position.z > BOX_BOUNDARY_Z) {
        reflectedPosition.z = 2 * BOX_BOUNDARY_Z - position.z;
    } else if (position.z < -BOX_BOUNDARY_Z) {
        reflectedPosition.z = -2 * BOX_BOUNDARY_Z - position.z;
    }

    return reflectedPosition;
}


