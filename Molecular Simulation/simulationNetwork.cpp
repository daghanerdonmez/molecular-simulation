//
//  simulationNetwork.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 3.02.2025.
//

#include "simulationNetwork.hpp"

SimulationNetwork::SimulationNetwork()
{
    //TODO: buralar çok karıştı bunların ne olduğuna bakmam lazım her şey birbirine girmek üzere
    simulations.emplace_back(std::make_unique<Simulation>());
    simulations.emplace_back(std::make_unique<Simulation>());
//    simulations.emplace_back(std::make_unique<Simulation>());
    hubs.emplace_back();
//    hubs.emplace_back();
    Hub* hub1 = &hubs[0];
//    Hub* hub2 = &hubs[1];
    simulations[0]->setLeftConnection(hub1);
    simulations[1]->setRightConnection(hub1);
    DirectedConnection dc1{simulations[0].get(), Direction::LEFT};
    DirectedConnection dc2{simulations[1].get(), Direction::RIGHT};
    hub1->addDirectedConnection(dc1);
    hub1->addDirectedConnection(dc2);
    hub1->initializeProbabilities();
//    simulations[1]->setLeftConnection(hub2);
//    simulations[2]->setRightConnection(hub2);
//    DirectedConnection dc3{simulations[1].get(), Direction::LEFT};
//    DirectedConnection dc4{simulations[2].get(), Direction::RIGHT};
//    hub2->addDirectedConnection(dc3);
//    hub2->addDirectedConnection(dc4);
//    hub2->initializeProbabilities();
    
}

void SimulationNetwork::iterateNetwork(int iterationCount, int currentFrame)
{
    for (int i = 0; i < iterationCount; ++i) {
        for(int j = 0; j < simulations.size(); ++j) {
            simulations[j]->iterateSimulation(1, currentFrame);
            std::cout << simulations[j]->getAliveParticleCount() << std::endl;
        }
        std::cout << "--" << std::endl;
    }
}

Simulation* SimulationNetwork::getFirstSimulation()
{
    return simulations[0].get();
}



