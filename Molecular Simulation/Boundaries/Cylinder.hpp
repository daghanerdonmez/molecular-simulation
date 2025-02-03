//
//  Cylinder.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#ifndef Cylinder_hpp
#define Cylinder_hpp

#include <glm/glm.hpp>
#include "../Config/config.h"
#include "../Math/geometry2d.hpp"
#include "Boundary.hpp"

class Cylinder: public Boundary {
private:
    int orientation;
    double radius;
    double zLimit;

    glm::dvec3 orientPosition(const glm::dvec3& position) const;
    glm::dvec3 deorientPosition(const glm::dvec3& position) const;

public:
    Cylinder();
    
    bool isOutsideBoundaries(const glm::dvec3& position) const override;
    glm::dvec3 reflectParticle(const glm::dvec3& oldPosition, const glm::dvec3& newPosition) const override;
};

#endif /* Cylinder_hpp */

