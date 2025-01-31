//
//  Box.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#ifndef Box_hpp
#define Box_hpp

#include <stdio.h>
#include "../Config/config.h"
#include <glm/glm.hpp>

bool boxIsOutsideBoundaries(glm::dvec3 position);
glm::dvec3 boxReflectParticle(glm::dvec3 position);

#endif /* Box_hpp */
