//
//  ringReceiverWithThickness.hpp
//  molecular-simulation
//
//  Created by Dağhan Erdönmez on 5.07.2025.
//

#ifndef ringReceiverWithThickness_hpp
#define ringReceiverWithThickness_hpp

#include <stdio.h>
#include "receiver.hpp"
#include "glm/glm.hpp"

class RingReceiverWithThickness : public Receiver {
private:
    int orientation;
    double thickness;
public:
    RingReceiverWithThickness(glm::dvec3 position, int countingType, int orientation, double thickness);
    bool hit(glm::dvec3 particlePosition) const override;
    int getOrientation() const;
    double getThickness() const;
    void setOrientation(int orientation);
};

inline bool RingReceiverWithThickness::hit(glm::dvec3 particlePosition) const {
    assert(orientation == 0 || orientation == 1 || orientation == 2);
    
    switch (orientation) {
        case 0: return (particlePosition.x > position.x && particlePosition.x < (position.x + thickness));
        case 1: return (particlePosition.y > position.y && particlePosition.y < (position.y + thickness));
        case 2: return (particlePosition.z > position.z && particlePosition.z < (position.z + thickness));
    }

    return false;
}

inline int RingReceiverWithThickness::getOrientation() const {
    return orientation;
}

inline void RingReceiverWithThickness::setOrientation(int orientation) {
    this->orientation = orientation;
}

#endif /* ringReceiverWithThickness_hpp */
