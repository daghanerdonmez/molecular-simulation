//
//  sphericalReceiver.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 2.03.2025.
//

#ifndef spherical_receiver_hpp
#define spherical_receiver_hpp

#include "receiver.hpp"
#include "glm/glm.hpp"

class SphericalReceiver : public Receiver {
private:
    double radius;

public:
    SphericalReceiver(glm::dvec3 position, int countingType, double radius);
    bool hit(glm::dvec3 particlePosition) const override;
    double getRadius() const;
};

inline bool SphericalReceiver::hit(glm::dvec3 particlePosition) const {
    return glm::length(particlePosition - position) < radius;
}

inline double SphericalReceiver::getRadius() const {
    return radius;
}

#endif /* spherical_receiver_hpp */
