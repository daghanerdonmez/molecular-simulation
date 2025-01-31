//
//  Cylinder.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#ifndef Cylinder_hpp
#define Cylinder_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "../Config/config.h"
#include "../Math/geometry2d.hpp"

bool cylinderIsOutsideBoundaries(glm::dvec3 position);
glm::dvec3 cylinderReflectParticle(glm::dvec3 oldPosition, glm::dvec3 newPosition);

#endif /* Cylinder_hpp */
