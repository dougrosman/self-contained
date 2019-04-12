#pragma once

#include "ofMain.h"
#include "dotParticle.h"

class dotParticle {
    
public:
    // functions
    void update();
    void checkWalls();
    void applyForce();
    
    // variables
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 accel;
    float drag = 0.99;
    
    // bounding box
    glm::vec2 upperBound;
    glm::vec2 lowerBound;
    
    
    
    ofColor color;
    float size;
    
    
};
