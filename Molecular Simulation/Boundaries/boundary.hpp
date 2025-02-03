//
//  Boundary.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#ifndef Boundary_hpp
#define Boundary_hpp

#include <glm/glm.hpp>

// Abstract base class (similar to an interface in Java)
class Boundary {
public:
    virtual ~Boundary() = default;  // Virtual destructor for proper cleanup

    // Pure virtual functions (must be implemented by derived classes)
    virtual bool isOutsideBoundaries(const glm::dvec3& position) const = 0;
    virtual glm::dvec3 reflectParticle(const glm::dvec3& oldPosition, const glm::dvec3& newPosition) const = 0;
};

#endif /* Boundary_hpp */

