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

// Headers for your classes
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

// Return which side of `pipeNode` references `otherPipeName`
// If none, we return an empty optional
enum class Side { LEFT, RIGHT };
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

// A small struct to hold a "pipe side" in our internal graph
// We'll map each pipe's left side and right side to an integer node index
struct PipeSide
{
    Simulation* simulation;  // pointer to the simulation
    Side        side;        // which side (LEFT or RIGHT)
    // For convenience, store the pipeName if you want to debug
    std::string pipeName;
};

} // anonymous namespace

std::unique_ptr<SimulationNetwork>
SimulationNetworkLoader::loadFromYAML(const std::string& filename)
{
    YAML::Node config = YAML::LoadFile(filename);
    auto network = std::make_unique<SimulationNetwork>();

    // 1. Read all pipe definitions and build Simulation objects
    std::unordered_map<std::string, std::unique_ptr<Simulation>> simulations;
    simulations.reserve(config["pipes"].size());

    // We'll also store each pipe's YAML node for later lookups:
    std::unordered_map<std::string, YAML::Node> pipeNodes;
    pipeNodes.reserve(config["pipes"].size());

    for (auto it = config["pipes"].begin(); it != config["pipes"].end(); ++it) {
        std::string pipeName = it->first.as<std::string>();
        const auto& pipeCfg = it->second;

        double length = pipeCfg["length"].as<double>();
        double radius = pipeCfg["radius"].as<double>();

        // Build your Simulation
        auto sim = std::make_unique<Simulation>(/*particleCount=*/100, radius, length);
        simulations[pipeName] = std::move(sim);
        pipeNodes[pipeName]   = pipeCfg;
    }

    // 2. We now create a graph where each Simulation has 2 nodes:
    //    "left" side, and "right" side.
    // We'll store them in a vector for adjacency.  The index
    // in "sideMapping" corresponds to that node in the adjacency list.
    // E.g. for pipe "A", two nodes: A_left, A_right.
    // We'll do a simple approach: the first 2*N entries are all we need.

    // We'll map pipeName + ":left" => an index,  pipeName + ":right" => another index
    // Then adjacency[nodeIndex] is a list of node indices that share a hub with that side.
    // After we build adjacency, we do BFS or DFS to find connected components => hubs.

    std::vector<PipeSide> sideMapping;   // sideMapping[i] = {which simulation, which side, pipeName}
    sideMapping.reserve(simulations.size() * 2);

    // Map "pipeName:left" => index
    std::unordered_map<std::string, int> leftIndexOf;
    // Map "pipeName:right" => index
    std::unordered_map<std::string, int> rightIndexOf;

    // We'll just assign indices in a loop
    {
        int idx = 0;
        for (auto& kv : simulations) {
            const std::string& pipeName = kv.first;
            Simulation* simPtr = kv.second.get();

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

    // adjacency list of all sides
    std::vector<std::vector<int>> adjacency(sideMapping.size());

    // A small lambda to add edges
    auto addConnection = [&](int a, int b) {
        adjacency[a].push_back(b);
        adjacency[b].push_back(a);
    };

    // 3. Build adjacency by reading the connections from the YAML.
    //    If pipe X says "left_connections: [Y1, Y2...]", we find which side
    //    in Y1 references X, etc. Then we add an edge between X:left and Y1:thatSide.
    for (auto& kv : pipeNodes) {
        const std::string& pipeName = kv.first;
        const auto& node            = kv.second;

        // We'll get the node indices for the left and right side of this pipe
        int leftNodeIndex  = leftIndexOf.at(pipeName);
        int rightNodeIndex = rightIndexOf.at(pipeName);

        // For left_connections
        if (node["left_connections"] && node["left_connections"].IsSequence()) {
            for (auto& c : node["left_connections"]) {
                std::string otherPipe = c.as<std::string>();

                // find which side of 'otherPipe' references 'pipeName'
                auto sideOpt = findSideReferencing(pipeNodes[otherPipe], pipeName);
                if (!sideOpt.has_value()) {
                    // If you want to allow one-sided references, you can simply connect here
                    // But typically you'd want a symmetrical reference.  You can either skip
                    // or throw an error.
                    // For now, let's throw an error so the user sees the mismatch:
                    std::cerr << "Warning: " << pipeName << ":left references " << otherPipe
                              << " but " << otherPipe << " does not reference " << pipeName << "!\n";
                    continue;
                }
                int otherIndex = (sideOpt.value() == Side::LEFT)
                    ? leftIndexOf.at(otherPipe)
                    : rightIndexOf.at(otherPipe);

                // connect these two sides in the graph
                addConnection(leftNodeIndex, otherIndex);
            }
        }

        // For right_connections
        if (node["right_connections"] && node["right_connections"].IsSequence()) {
            for (auto& c : node["right_connections"]) {
                std::string otherPipe = c.as<std::string>();

                // find which side of 'otherPipe' references 'pipeName'
                auto sideOpt = findSideReferencing(pipeNodes[otherPipe], pipeName);
                if (!sideOpt.has_value()) {
                    std::cerr << "Warning: " << pipeName << ":right references " << otherPipe
                              << " but " << otherPipe << " does not reference " << pipeName << "!\n";
                    continue;
                }
                int otherIndex = (sideOpt.value() == Side::LEFT)
                    ? leftIndexOf.at(otherPipe)
                    : rightIndexOf.at(otherPipe);

                // connect these two sides in the graph
                addConnection(rightNodeIndex, otherIndex);
            }
        }
    }

    // 4. Find connected components in the adjacency graph.
    std::vector<bool> visited(sideMapping.size(), false);

    // We'll store all hubs we create, so we can later finalize them
    std::vector<std::unique_ptr<Hub>> allHubs;

    for (int start = 0; start < (int)sideMapping.size(); ++start) {
        if (visited[start]) continue;

        // BFS (or DFS) to collect all sides in this connected component
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

        // Now "component" holds all sides in one connected hub.
        // Let's create a Hub for them:
        auto hubPtr = std::make_unique<Hub>();

        // Wire up each side in that component to this hub
        for (int nodeIndex : component) {
            const auto& ps = sideMapping[nodeIndex];
            Simulation* sim = ps.simulation;
            if (ps.side == Side::LEFT) {
                sim->setLeftConnection(hubPtr.get());
                hubPtr->addDirectedConnection({ sim, Direction::LEFT });
            } else {
                sim->setRightConnection(hubPtr.get());
                hubPtr->addDirectedConnection({ sim, Direction::RIGHT });
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

    // If you want to store hubs in the SimulationNetwork as well,
    // you can do something like:
    //   network->addHub(std::move(hub));
    // but since your class doesn't have addHub in the snippet,
    // you might do something like:
    //   network->hubs.push_back(std::move(hub));
    // but that requires making `hubs` public or having a suitable method.

    // For illustration, if you want them inside the network:
     for (auto& hub : allHubs) {
         network->addHub(std::move(hub));
     }

    // Return the constructed network
    return network;
}
