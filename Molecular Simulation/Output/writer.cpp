//
//  writer.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 1.02.2025.
//

#include "writer.hpp"

void writeToFile(const std::string& filename, const std::string& data, bool append = false) {
    std::ofstream outFile;

    if (append) {
        outFile.open(filename, std::ios::app); // Open in append mode
    } else {
        outFile.open(filename); // Default overwrite mode
    }

    if (!outFile) { // Check if the file opened successfully
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    outFile << data << std::endl; // Write the data to the file
    outFile.close(); // Close the file
}
