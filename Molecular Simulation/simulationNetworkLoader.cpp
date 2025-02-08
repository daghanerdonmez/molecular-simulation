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

// Your classes
#include "simulation.hpp"
#include "hub.hpp"
#include "simulationNetwork.hpp"

namespace {

// A small helper to see if a YAML node has a string list
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

// Return which side of `pipeNode` references `otherPipeName`.
std::optional<Side> findSideReferencing(
    const YAML::Node& pipeNode,
    const std::string& otherPipeName)
{
    // Does left_connections contain otherPipeName?
    if (hasStringInNode(pipeNode["left_connections"], otherPipeName)) {
        return Side::LEFT;
    }
    // Does right_connections contain otherPipeName?
    if (hasStringInNode(pipeNode["right_connections"], otherPipeName)) {
        return Side::RIGHT;
    }
    return std::nullopt;
}

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

    // 1. Read all pipe definitions and build Simulation objects
    std::unordered_map<std::string, std::unique_ptr<Simulation>> simulations;
    simulations.reserve(config["pipes"].size());

    // Also store each pipe's YAML node for later lookups
    std::unordered_map<std::string, YAML::Node> pipeNodes;
    pipeNodes.reserve(config["pipes"].size());

    for (auto it = config["pipes"].begin(); it != config["pipes"].end(); ++it) {
        std::string pipeName = it->first.as<std::string>();
        const auto& pipeCfg  = it->second;

        // Read all required parameters
        double length = pipeCfg["length"].as<double>();
        double radius = pipeCfg["radius"].as<double>();

        // If "particle_count" is missing, default to 100
        int particleCount = pipeCfg["particle_count"] ? pipeCfg["particle_count"].as<int>() : 100;

        auto sim = std::make_unique<Simulation>(particleCount, radius, length);
        simulations[pipeName] = std::move(sim);
        pipeNodes[pipeName]   = pipeCfg;
    }

    // 2. Create graph nodes for each Simulation side
    std::vector<PipeSide> sideMapping;
    sideMapping.reserve(simulations.size() * 2);

    // Maps to store index in sideMapping for quick adjacency building
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

    // adjacency list for all sides
    std::vector<std::vector<int>> adjacency(sideMapping.size());

    // helper to add edges
    auto addConnection = [&](int a, int b) {
        adjacency[a].push_back(b);
        adjacency[b].push_back(a);
    };

    // 3. Build adjacency by reading connections
    for (auto& kv : pipeNodes) {
        const std::string& pipeName = kv.first;
        const auto& node            = kv.second;

        int leftNodeIndex  = leftIndexOf.at(pipeName);
        int rightNodeIndex = rightIndexOf.at(pipeName);

        // Handle left_connections
        if (node["left_connections"] && node["left_connections"].IsSequence()) {
            for (auto& c : node["left_connections"]) {
                std::string otherPipe = c.as<std::string>();
                auto sideOpt = findSideReferencing(pipeNodes[otherPipe], pipeName);
                if (!sideOpt.has_value()) {
                    // If references are expected to be symmetrical, warn or skip
                    std::cerr << "[Warning] " << pipeName << ":left -> " << otherPipe
                              << " but no reference back to " << pipeName << "\n";
                    continue;
                }
                int otherIndex = (sideOpt.value() == Side::LEFT)
                    ? leftIndexOf.at(otherPipe)
                    : rightIndexOf.at(otherPipe);

                addConnection(leftNodeIndex, otherIndex);
            }
        }

        // Handle right_connections
        if (node["right_connections"] && node["right_connections"].IsSequence()) {
            for (auto& c : node["right_connections"]) {
                std::string otherPipe = c.as<std::string>();
                auto sideOpt = findSideReferencing(pipeNodes[otherPipe], pipeName);
                if (!sideOpt.has_value()) {
                    // If references are expected to be symmetrical, warn or skip
                    std::cerr << "[Warning] " << pipeName << ":right -> " << otherPipe
                              << " but no reference back to " << pipeName << "\n";
                    continue;
                }
                int otherIndex = (sideOpt.value() == Side::LEFT)
                    ? leftIndexOf.at(otherPipe)
                    : rightIndexOf.at(otherPipe);

                addConnection(rightNodeIndex, otherIndex);
            }
        }
    }

    // 4. Find connected components => create Hubs
    std::vector<bool> visited(sideMapping.size(), false);
    std::vector<std::unique_ptr<Hub>> allHubs;
    allHubs.reserve(sideMapping.size()); // rough upper bound

    for (int start = 0; start < (int)sideMapping.size(); ++start) {
        if (visited[start]) continue;

        // BFS or DFS
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

        // build one hub for this connected component
        auto hubPtr = std::make_unique<Hub>();

        // wire up each side in the component
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

    // 5. Initialize the hubs
    for (auto& hub : allHubs) {
        hub->initializeProbabilities();
    }

    // 6. Move all simulations into the network
    for (auto& simKV : simulations) {
        network->addSimulation(std::move(simKV.second));
    }

    // (If you want to store the hubs in the network too,
    //  add them likewise. For example:
    //
    for (auto &hub : allHubs) {
       network->addHub(std::move(hub));
    }
    //
    // But that requires a suitable interface in `SimulationNetwork`.)

    return network;
}

