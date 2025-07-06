//
//  ringReceiverWithThickness.cpp
//  molecular-simulation
//
//  Created by Dağhan Erdönmez on 5.07.2025.
//

#include "ringReceiverWithThickness.hpp"

RingReceiverWithThickness::RingReceiverWithThickness(glm::dvec3 position, int countingType, int orientation, double thickness): Receiver(position, countingType), orientation(orientation), thickness(thickness) {}

