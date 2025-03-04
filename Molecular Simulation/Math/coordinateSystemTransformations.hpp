//
//  coordinateSystemTransformations.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 2.03.2025.
//

#ifndef coordinateSystemTransformations_hpp
#define coordinateSystemTransformations_hpp

#include <stdio.h>
#include "glm/glm.hpp"

glm::dvec3 cartesianToCylindrical(glm::dvec3 vector);
glm::dvec3 cylindricalToCartesian(glm::dvec3 vector);

#endif /* coordinateSystemTransformations_hpp */
