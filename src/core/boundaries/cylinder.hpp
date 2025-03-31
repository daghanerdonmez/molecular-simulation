//
//  Cylinder.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 31.01.2025.
//

#ifndef Cylinder_hpp
#define Cylinder_hpp

#include <glm/glm.hpp>
#include <src/config/config.h>
#include <src/config/oldconfig.h>
#include <src/math/geometry2d.hpp>
#include "boundary.hpp"

class Cylinder: public Boundary {
private:
    int orientation;
    double radius;
    double zLimit;

    glm::dvec3 orientPosition(const glm::dvec3& position) const;
    glm::dvec3 deorientPosition(const glm::dvec3& position) const;

public:
    Cylinder(double radius = SINGLE_CYLINDER_R, double length = SINGLE_CYLINDER_Z);
    
    bool isOutsideBoundaries(const glm::dvec3& position) const override;
    glm::dvec3 reflectParticle(const glm::dvec3& oldPosition, const glm::dvec3& newPosition) const override;
    double getRadius() const;
    double getHeight() const;
    int getOrientation() const;
    // By right and left I assume the cylinder is laid along z-axis where +z is right and -z is left.
    bool isOutsideRightZBoundary(const glm::dvec3& position) const;
    bool isOutsideLeftZBoundary(const glm::dvec3& position) const;
    double getOverflow(const glm::dvec3& position) const;
};

#endif /* Cylinder_hpp */
