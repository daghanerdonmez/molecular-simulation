//
//  receiver.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#include "receiver.hpp"

Receiver::Receiver(glm::dvec3 position) : position(position) {
    particlesReceived = new int[NUMBER_OF_ITERATIONS]();
}

Receiver::~Receiver() {
    delete[] particlesReceived;
}

void Receiver::writeOutput() {
    std::string output = "";
    for (int i = 0; i < NUMBER_OF_ITERATIONS; ++i) {
        output += std::to_string(particlesReceived[i]);
        if (i != NUMBER_OF_ITERATIONS - 1) {
            output += ",";
        }
    }
    writeToFile("/Users/daghanerdonmez/Desktop/Molecular-Simulation/Molecular Simulation/Output/Outputs/r3output.txt", output);
}
