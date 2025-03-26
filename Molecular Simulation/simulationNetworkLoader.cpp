//
//  simulationNetworkLoader.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 8.02.2025.
//

#include "simulationNetworkLoader.hpp"

#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <optional>

// Your own headers
#include "simulation.hpp"
#include "hub.hpp"
#include "simulationNetwork.hpp"

// Include your coordinate transform functions
#include "Math/coordinateSystemTransformations.hpp"

// Include your receiver headers
#include "Receivers/sphericalReceiver.hpp"   // e.g. if you have "SphericalReceiver" there
#include "Receivers/ringReceiver.hpp" // e.g. if you have "ringReceiver" there
// Adjust the above includes as needed for your actual file structure

namespace {

// A small helper to see if a YAML node is a sequence containing a target string
bool hasStringInNode(const YAML::Node& node, const std::string& target) {
    if (!node || !node.IsSequence()) return false;
    for (auto& val : node) {
        if (val.as<std::string>() == target) {
            return true;
        }
    }
    return false;
}

enum class Side { LEFT, RIGHT };

// Return which side of `pipeNode` references `otherPipeName`
std::optional<Side> findSideReferencing(const YAML::Node& pipeNode,
                                        const std::string& otherPipeName)
{
    if (hasStringInNode(pipeNode["left_connections"], otherPipeName)) {
        return Side::LEFT;
    }
    if (hasStringInNode(pipeNode["right_connections"], otherPipeName)) {
        return Side::RIGHT;
    }
    return std::nullopt;
}

// Simple struct to represent a side of a pipe
struct PipeSide
{
    Simulation* simulation;
    Side        side;
    std::string pipeName;
};

} // end anonymous namespace

std::unique_ptr<SimulationNetwork>
SimulationNetworkLoader::loadFromYAML(const std::string& filename)
{
    YAML::Node config = YAML::LoadFile(filename);
    auto network = std::make_unique<SimulationNetwork>();

    // ------------------------------------------------------------------------
    // 1) Read all "pipes" and build Simulation objects.
    // ------------------------------------------------------------------------
    std::unordered_map<std::string, std::unique_ptr<Simulation>> simulations;
    simulations.reserve(config["pipes"].size());

    // Store the YAML node for each pipe for later lookups
    std::unordered_map<std::string, YAML::Node> pipeNodes;
    pipeNodes.reserve(config["pipes"].size());

    for (auto it = config["pipes"].begin(); it != config["pipes"].end(); ++it) {
        std::string pipeName = it->first.as<std::string>();
        const auto& pipeCfg  = it->second;

        double length = pipeCfg["length"].as<double>();
        double radius = pipeCfg["radius"].as<double>();
        int particleCount = pipeCfg["particle_count"] ? pipeCfg["particle_count"].as<int>() : 100;
        double flow = pipeCfg["flow"] ? pipeCfg["flow"].as<double>() : 0.0;

        // Create the simulation
        glm::dvec3 flowVector(0.0, 0.0, flow);
        auto sim = std::make_unique<Simulation>(particleCount, radius, length, flowVector);
        
        // Set the name of the simulation to the pipe name from YAML
        sim->setName(pipeName);

        // Keep track of the simulation and its node
        simulations[pipeName] = std::move(sim);
        pipeNodes[pipeName]   = pipeCfg;
    }

    // ------------------------------------------------------------------------
    // 2) Create a graph of "pipe sides": each pipe has a left side & right side.
    // ------------------------------------------------------------------------
    std::vector<PipeSide> sideMapping;   // index => PipeSide
    sideMapping.reserve(simulations.size() * 2);

    // We'll map pipeName -> indices
    std::unordered_map<std::string, int> leftIndexOf;
    std::unordered_map<std::string, int> rightIndexOf;

    {
        int idx = 0;
        for (auto& kv : simulations) {
            const std::string& pipeName = kv.first;
            Simulation* simPtr         = kv.second.get();

            // left side
            leftIndexOf[pipeName] = idx;
            sideMapping.push_back({ simPtr, Side::LEFT, pipeName });
            idx++;

            // right side
            rightIndexOf[pipeName] = idx;
            sideMapping.push_back({ simPtr, Side::RIGHT, pipeName });
            idx++;
        }
    }

    // Adjacency list for all sides
    std::vector<std::vector<int>> adjacency(sideMapping.size());

    // Helper to connect two sides
    auto addConnection = [&](int a, int b) {
        adjacency[a].push_back(b);
        adjacency[b].push_back(a);
    };

    // ------------------------------------------------------------------------
    // 3) Build adjacency by reading each pipe's left/right connections.
    // ------------------------------------------------------------------------
    for (auto& kv : pipeNodes) {
        const std::string& pipeName = kv.first;
        const auto& node            = kv.second;

        int leftNodeIndex  = leftIndexOf.at(pipeName);
        int rightNodeIndex = rightIndexOf.at(pipeName);

        // left_connections
        if (node["left_connections"] && node["left_connections"].IsSequence()) {
            for (auto& c : node["left_connections"]) {
                std::string otherPipe = c.as<std::string>();
                auto sideOpt = findSideReferencing(pipeNodes[otherPipe], pipeName);
                if (!sideOpt.has_value()) {
                    std::cerr << "[Warning] " << pipeName << ":left -> " << otherPipe
                              << " but " << otherPipe << " does not reference " << pipeName << "!\n";
                    continue;
                }
                int otherIndex = (sideOpt.value() == Side::LEFT)
                    ? leftIndexOf.at(otherPipe)
                    : rightIndexOf.at(otherPipe);

                addConnection(leftNodeIndex, otherIndex);
            }
        }

        // right_connections
        if (node["right_connections"] && node["right_connections"].IsSequence()) {
            for (auto& c : node["right_connections"]) {
                std::string otherPipe = c.as<std::string>();
                auto sideOpt = findSideReferencing(pipeNodes[otherPipe], pipeName);
                if (!sideOpt.has_value()) {
                    std::cerr << "[Warning] " << pipeName << ":right -> " << otherPipe
                              << " but " << otherPipe << " does not reference " << pipeName << "!\n";
                    continue;
                }
                int otherIndex = (sideOpt.value() == Side::LEFT)
                    ? leftIndexOf.at(otherPipe)
                    : rightIndexOf.at(otherPipe);

                addConnection(rightNodeIndex, otherIndex);
            }
        }
    }

    // ------------------------------------------------------------------------
    // 4) Find connected components -> create Hubs
    // ------------------------------------------------------------------------
    std::vector<bool> visited(sideMapping.size(), false);
    std::vector<std::unique_ptr<Hub>> allHubs;
    allHubs.reserve(sideMapping.size());

    for (int start = 0; start < (int)sideMapping.size(); ++start) {
        if (visited[start]) continue;

        // BFS or DFS from 'start' to find all connected sides
        std::queue<int> q;
        q.push(start);
        visited[start] = true;

        std::vector<int> component;
        component.reserve(4);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            component.push_back(curr);

            for (int nxt : adjacency[curr]) {
                if (!visited[nxt]) {
                    visited[nxt] = true;
                    q.push(nxt);
                }
            }
        }

        // Build one hub for this connected component
        auto hubPtr = std::make_unique<Hub>();

        // Wire up each side in the component to that hub
        for (int nodeIndex : component) {
            const auto& ps = sideMapping[nodeIndex];
            if (ps.side == Side::LEFT) {
                ps.simulation->setLeftConnection(hubPtr.get());
                hubPtr->addDirectedConnection({ ps.simulation, Direction::LEFT });
            } else {
                ps.simulation->setRightConnection(hubPtr.get());
                hubPtr->addDirectedConnection({ ps.simulation, Direction::RIGHT });
            }
        }

        allHubs.push_back(std::move(hubPtr));
    }

    // ------------------------------------------------------------------------
    // 5) Initialize the hubs
    // ------------------------------------------------------------------------
    for (auto& hub : allHubs) {
        hub->initializeProbabilities();
    }

    // ------------------------------------------------------------------------
    // 6) Parse "receivers" for each pipe, if present
    // ------------------------------------------------------------------------
    for (auto& kv : simulations) {
        const std::string& pipeName = kv.first;
        Simulation* simPtr          = kv.second.get();

        // Look up the original YAML node
        const auto& pipeCfg = pipeNodes[pipeName];
        if (!pipeCfg["receivers"] || !pipeCfg["receivers"].IsSequence()) {
            // no receivers for this pipe
            continue;
        }

        // For each receiver definition
        for (auto& rcv : pipeCfg["receivers"]) {
            if (!rcv["type"]) {
                std::cerr << "[Warning] A receiver is missing its 'type' field, skipping.\n";
                continue;
            }

            std::string receiverType = rcv["type"].as<std::string>();

            if (receiverType == "Sphere type") {
                // We expect radius, z, r, theta
                double radius = 0.0;
                if (rcv["radius"]) {
                    radius = rcv["radius"].as<double>();
                } else {
                    std::cerr << "[Warning] Sphere receiver has no 'radius'. Defaulting to 1e-6\n";
                    radius = 1e-6;
                }

                // Default to 0 if missing
                double z     = rcv["z"]     ? rcv["z"].as<double>()     : 0.0;
                double rCyl  = rcv["r"]     ? rcv["r"].as<double>()     : 0.0;
                double theta = rcv["theta"] ? rcv["theta"].as<double>() : 0.0;

                // Convert from cylindrical -> cartesian
                glm::dvec3 cylPos(rCyl, theta, z);
                glm::dvec3 cartPos = cylindricalToCartesian(cylPos);

                // Create a spherical receiver
                auto sphere = std::make_unique<SphericalReceiver>(cartPos, radius);
                
                // Set the name if it exists in the YAML
                if (rcv["name"]) {
                    sphere->setName(rcv["name"].as<std::string>());
                }

                // Add it to the simulation
                simPtr->addReceiver(std::move(sphere));
            }
            else if (receiverType == "Ring type") {
                // Example usage for a "Ring type" receiver
                // You can define your own constructor / usage
                double z     = rcv["z"] ? rcv["z"].as<double>() : 0.0;
                // Possibly you want r, theta, or other fields as well.
                // For example:
                double rCyl  = rcv["r"]     ? rcv["r"].as<double>()     : 0.0;
                double theta = rcv["theta"] ? rcv["theta"].as<double>() : 0.0;

                glm::dvec3 cylPos(rCyl, theta, z);
                glm::dvec3 cartPos = cylindricalToCartesian(cylPos);

                // Create your "Ring type" receiver – placeholder name:
                auto trap = std::make_unique<RingReceiver>(cartPos, 2);
                
                // Set the name if it exists in the YAML
                if (rcv["name"]) {
                    trap->setName(rcv["name"].as<std::string>());
                }

                simPtr->addReceiver(std::move(trap));
            }
            else {
                std::cerr << "[Warning] Unknown receiver type: " << receiverType
                          << " for pipe: " << pipeName << ", skipping.\n";
            }
        }
    }
    
    // ------------------------------------------------------------------------
    // 7) Parse "emitters" for each pipe, if present
    // ------------------------------------------------------------------------
    for (auto& kv : simulations) {
        const std::string& pipeName = kv.first;
        Simulation* simPtr = kv.second.get();

        // Look up the original YAML node
        const auto& pipeCfg = pipeNodes[pipeName];
        
        // MODIFICATION: Check if emitters are present in the pipe configuration
        if (!pipeCfg["emitters"] || !pipeCfg["emitters"].IsSequence()) {
            // no emitters for this pipe
            continue;
        }

        // For each emitter definition
        for (auto& emitterCfg : pipeCfg["emitters"]) {
            // MODIFICATION: Parse emitter coordinates from cylindrical to cartesian
            double z = emitterCfg["z"] ? emitterCfg["z"].as<double>() : 0.0;
            double rCyl = emitterCfg["r"] ? emitterCfg["r"].as<double>() : 0.0;
            double theta = emitterCfg["theta"] ? emitterCfg["theta"].as<double>() : 0.0;

            // Convert from cylindrical -> cartesian
            glm::dvec3 cylPos(rCyl, theta, z);
            glm::dvec3 cartPos = cylindricalToCartesian(cylPos);

            // MODIFICATION: Parse emitter pattern
            std::vector<int> emissionPattern;
            if (emitterCfg["emitter_pattern"]) {
                std::string patternStr = emitterCfg["emitter_pattern"].as<std::string>();
                std::stringstream ss(patternStr);
                std::string item;
                
                // Parse comma-separated values into integer vector
                while (std::getline(ss, item, ',')) {
                    try {
                        emissionPattern.push_back(std::stoi(item));
                    } catch (const std::exception& e) {
                        std::cerr << "[Warning] Invalid emitter pattern value: " << item
                                  << " in pipe: " << pipeName << ". Skipping.\n";
                    }
                }
            }

            // Get the pattern type (defaults to "repeat" if not specified)
            std::string patternType = "repeat";
            if (emitterCfg["emitter_pattern_type"]) {
                patternType = emitterCfg["emitter_pattern_type"].as<std::string>();
                // Validate pattern type
                if (patternType != "repeat" && patternType != "complete") {
                    std::cerr << "[Warning] Invalid emitter pattern type: " << patternType
                              << " in pipe: " << pipeName << ". Defaulting to 'repeat'.\n";
                    patternType = "repeat";
                }
            }

            // MODIFICATION: Create and add emitter to simulation
            if (!emissionPattern.empty()) {
                auto emitter = std::make_unique<Emitter>(
                    cartPos,           // position
                    emissionPattern,   // emission pattern
                    simPtr,            // pointer to simulation
                    patternType        // pattern type
                );

                simPtr->addEmitter(std::move(emitter));
            } else {
                std::cerr << "[Warning] Emitter in pipe: " << pipeName
                          << " has no valid emission pattern. Skipping.\n";
            }
        }
    }

    // ------------------------------------------------------------------------
    // 8) Move all Simulations into the SimulationNetwork
    // ------------------------------------------------------------------------
    for (auto& simKV : simulations) {
        network->addSimulation(std::move(simKV.second));
    }

    // If your SimulationNetwork also needs to store the hubs, do that here:
    // e.g.
    for (auto& hub : allHubs) {
     network->addHub(std::move(hub));
    }

    return network;
}
