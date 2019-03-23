#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "dotParticle.h"

class dotParticle {
    
public:
    // functions
    void update();
    void checkWalls();
    void applyForce();
    void applyMelt();
    void applyWind(glm::vec3 currPos);
    
    // variables
    glm::vec3 startPos;
    glm::vec3 currPos;
    glm::vec3 vel;
    glm::vec3 accel;
    glm::vec3 force;
    float drag = 0.99;
    
    // bounding box
    glm::vec2 upperBound;
    glm::vec2 lowerBound;
    
    
    
    ofColor color;
    float size;
    
    
};
