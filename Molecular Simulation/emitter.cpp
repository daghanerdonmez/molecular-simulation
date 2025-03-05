//
//  emitter.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 5.03.2025.
//

#include "emitter.hpp"
#include "simulation.hpp"
#include "particle.hpp"

Emitter::Emitter(glm::dvec3 pos, const std::vector<int>& pattern, Simulation* sim)
    : position(pos), emissionPattern(pattern), currentPatternIndex(0), simulation(sim) {}

void Emitter::emit(int currentFrame) {
    if (emissionPattern.empty()) return;
    
    int particlesToEmit = emissionPattern[currentPatternIndex];
    
    currentPatternIndex = (currentPatternIndex + 1) % emissionPattern.size();
    
    for (int i = 0; i < particlesToEmit; ++i) {
        Particle newParticle(position.x, position.y, position.z);
        newParticle.setBoundary(simulation->getBoundary());
        newParticle.setSimulation(simulation);
        
        // Add particle to simulation
        simulation->addParticle(newParticle);
    }
}

glm::dvec3 Emitter::getPosition() const {
    return position;
}

void Emitter::setPosition(const glm::dvec3& pos) {
    position = pos;
}

const std::vector<int>& Emitter::getEmissionPattern() const {
    return emissionPattern;
}

void Emitter::setEmissionPattern(const std::vector<int>& pattern) {
    emissionPattern = pattern;
    currentPatternIndex = 0;
}
