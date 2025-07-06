//
//  ringReceiver.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 2.03.2025.
//

#ifndef ringReceiver_hpp
#define ringReceiver_hpp

#include <stdio.h>
#include "receiver.hpp"
#include "glm/glm.hpp"

class RingReceiver : public Receiver {
private:
    int orientation;
public:
    RingReceiver(glm::dvec3 position, int countingType, int orientation);
    bool hit(glm::dvec3 particlePosition) const override;
    int getOrientation() const;
    void setOrientation(int orientation);
};

inline bool RingReceiver::hit(glm::dvec3 particlePosition) const {
    assert(orientation == 0 || orientation == 1 || orientation == 2);
    
    switch (orientation) {
        case 0: return particlePosition.x > position.x;
        case 1: return particlePosition.y > position.y;
        case 2: return particlePosition.z > position.z;
    }

    return false;
}

inline int RingReceiver::getOrientation() const {
    return orientation;
}

inline void RingReceiver::setOrientation(int orientation) {
    this->orientation = orientation;
}


#endif /* ringReceiver_hpp */
