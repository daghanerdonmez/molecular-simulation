//
//  sphericalReceiver.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 2.03.2025.
//
#include "sphericalReceiver.hpp"

SphericalReceiver::SphericalReceiver(glm::dvec3 position, double radius)
    : Receiver(position), radius(radius) {}
