//
//  NoBoundary.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#ifndef NoBoundary_hpp
#define NoBoundary_hpp

#include <stdio.h>
#include "boundary.hpp"

class NoBoundary: public Boundary
{
public:
    NoBoundary();
    
    bool isOutsideBoundaries(const glm::dvec3& position) const override;
    glm::dvec3 reflectParticle(const glm::dvec3& oldPosition, const glm::dvec3& newPosition) const override;
};


#endif /* NoBoundary_hpp */
