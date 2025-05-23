//
//  emitter.hpp
//  Molecular Simulation
//
//  Created by Dağhan Erdönmez on 5.03.2025.
//

#ifndef emitter_hpp
#define emitter_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "glm/glm.hpp"

class Simulation;
class Particle;

class Emitter {
private:
    glm::dvec3 position;
    std::vector<int> emissionPattern;
    size_t currentPatternIndex;
    Simulation* simulation;
    std::string patternType; // "repeat" or "complete"
    bool patternCompleted;
    int totalEmitted; // Total number of particles emitted
    
public:
    Emitter(glm::dvec3 position, const std::vector<int>& pattern, Simulation* simulation, const std::string& patternType);
    
    void emit(int currentFrame);
    
    glm::dvec3 getPosition() const;
    void setPosition(const glm::dvec3& pos);
    
    const std::vector<int>& getEmissionPattern() const;
    void setEmissionPattern(const std::vector<int>& pattern);
    
    const std::string& getPatternType() const;
    void setPatternType(const std::string& type);
    
    bool isPatternCompleted() const;
    void resetPattern();
    
    // Getter for total emitted particles
    int getTotalEmitted() const;
};

#endif /* emitter_hpp */
