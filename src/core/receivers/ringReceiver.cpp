//
//  ringReceiver.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 2.03.2025.
//

#include "ringReceiver.hpp"

RingReceiver::RingReceiver(glm::dvec3 position, int countingType, int orientation): Receiver(position, countingType), orientation(orientation) {}
