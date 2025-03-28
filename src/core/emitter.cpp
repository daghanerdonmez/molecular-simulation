//
//  emitter.cpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 5.03.2025.
//

#include "emitter.hpp"
#include "simulation.hpp"
#include "particle.hpp"

Emitter::Emitter(glm::dvec3 pos, const std::vector<int>& pattern, Simulation* sim, const std::string& patternType)
    : position(pos), emissionPattern(pattern), currentPatternIndex(0), simulation(sim), 
      patternType(patternType), patternCompleted(false), totalEmitted(0) {}

void Emitter::emit(int currentFrame) {
    if (emissionPattern.empty()) return;
    
    // For "complete" pattern type, check if we've already gone through the entire pattern
    if (patternType == "complete" && patternCompleted) {
        return;
    }
    
    // Get the number of particles to emit from the current pattern index
    int particlesToEmit = emissionPattern[currentPatternIndex];
    
    // Move to the next pattern index
    currentPatternIndex++;
    
    // Check if we've completed a full cycle through the pattern
    if (currentPatternIndex >= emissionPattern.size()) {
        currentPatternIndex = 0;  // Reset to beginning of pattern
        
        // If pattern type is "complete", mark as completed after one full cycle
        if (patternType == "complete") {
            patternCompleted = true;
        }
    }
    
    // Emit the particles
    for (int i = 0; i < particlesToEmit; ++i) {
        Particle newParticle(position.x, position.y, position.z);
        newParticle.setBoundary(simulation->getBoundary());
        newParticle.setSimulation(simulation);
        
        // Add particle to simulation
        simulation->addParticle(newParticle);
        
        // Increment total emitted count
        totalEmitted++;
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
    patternCompleted = false;
}

const std::string& Emitter::getPatternType() const {
    return patternType;
}

void Emitter::setPatternType(const std::string& type) {
    patternType = type;
    if (patternType != "complete") {
        patternCompleted = false;
    }
}

bool Emitter::isPatternCompleted() const {
    return patternCompleted;
}

void Emitter::resetPattern() {
    currentPatternIndex = 0;
    patternCompleted = false;
}

int Emitter::getTotalEmitted() const {
    return totalEmitted;
}
