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

void Receiver::writeOutput(const std::string& dirPath, const std::string& pipeName, bool isSphericalReceiver, double radius) {

    // Create the output string with just comma-separated numbers
    std::string output;

    glm::dvec3 car = cartesianToCylindrical(position);

    // Add pipe name, r, z coordinates and radius of the receiver (I am assuming all receivers are sphericalReceiver for now)
    output = pipeName + " " + std::to_string(car.x) + " " + std::to_string(car.z);

    // Checking if the receiver is of type sphericalReceiver, if so add radius
    // this is a very shitty implementation. i am only doing this for now because i will only work with spherical receivers but in the future i might need to make every receiver type have its own write function.
    if (isSphericalReceiver) {
        output += " " + std::to_string(radius) + "\n";
    }

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
