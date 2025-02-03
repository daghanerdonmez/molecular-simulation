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
#include "Boundary.hpp"

class Box: public Boundary { // inheriting from boundary
private:
    double boundaryX;
    double boundaryY;
    double boundaryZ;

public:
    Box();

    double getBoundaryZ(double x, double y) const;
    double getBoundaryY(double x, double z) const;
    double getBoundaryX(double y, double z) const;

    bool isOutsideBoundaries(const glm::dvec3& position) const override;
    glm::dvec3 reflectParticle(const glm::dvec3& oldPosition, const glm::dvec3& newPosition) const override;
};

#endif /* Box_hpp */

