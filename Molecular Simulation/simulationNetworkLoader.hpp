//
//  simulationNetworkLoader.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 8.02.2025.
//

#ifndef networkLoader_hpp
#define networkLoader_hpp

#include <memory>
#include <string>

// Forward declare classes to avoid including everything here.
class SimulationNetwork;

class SimulationNetworkLoader
{
public:
    // Load from a YAML file and build a SimulationNetwork object.
    // On success, returns a unique_ptr to the newly constructed SimulationNetwork.
    // Throws on failure (file not found, malformed YAML, etc.).
    static std::unique_ptr<SimulationNetwork> loadFromYAML(const std::string& filename);
};

#endif /* networkLoader_hpp */
