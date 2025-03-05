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
#include "glm/glm.hpp"

class Simulation;
class Particle;

class Emitter {
private:
    glm::dvec3 position;
    std::vector<int> emissionPattern;
    size_t currentPatternIndex;
    Simulation* simulation;
    
public:
    Emitter(glm::dvec3 position, const std::vector<int>& pattern, Simulation* simulation);
    
    void emit(int currentFrame);
    
    glm::dvec3 getPosition() const;
    void setPosition(const glm::dvec3& pos);
    
    const std::vector<int>& getEmissionPattern() const;
    void setEmissionPattern(const std::vector<int>& pattern);
};

#endif /* emitter_hpp */
