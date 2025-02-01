//
//  writer.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 1.02.2025.
//

#include "writer.hpp"

void writeToFile(const std::string& filename, const std::string& data) {
    std::ofstream outFile(filename); // Opens the file in overwrite mode by default

    if (!outFile) { // Check if the file opened successfully
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    outFile << data; // Write the data to the file
    outFile.close(); // Close the file
}
