//
//  receiver.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 30.01.2025.
//

#include "receiver.hpp"

Receiver::Receiver(glm::dvec3 position) : position(position), name(""), totalReceived(0) {
    particlesReceived = new int[NUMBER_OF_ITERATIONS]();
}

Receiver::~Receiver() {
    delete[] particlesReceived;
}

void Receiver::writeOutput(const std::string& dirPath) {
    // Create the output string with just comma-separated numbers
    std::string output;
    for (int i = 0; i < NUMBER_OF_ITERATIONS; ++i) {
        output += std::to_string(particlesReceived[i]);
        if (i != NUMBER_OF_ITERATIONS - 1) {
            output += ",";
        }
    }
    
    // Use the receiver's name as the filename
    std::string filename = dirPath + "/" + (name.empty() ? "unnamed_receiver" : name) + ".txt";
    
    // Write to file (overwrite mode)
    writeToFile(filename, output, false);
}
