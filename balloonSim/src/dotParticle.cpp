/* This app creates an environment to manipulate x,y coordinates that make
 * up a skeleton body.
 *
 * 1. Take in the x, y data
 */

#include "dotParticle.h"

void dotParticle::update()
{
    vel += accel + force;
    vel *= drag;
    currPos += vel;
    force = {0, 0, 0};
        
}

void dotParticle::checkWalls()
{
    if(currPos.x > ofGetWidth()-size*2)
    {
        currPos.x = ofGetWidth()-size*2;
        vel.x*=-.9;
    }
    
    if(currPos.x < size*2)
    {
        currPos.x = size*2;
        vel.x*=-.9;
    }
    
    if(currPos.y < size*2)
    {
        currPos.y = size*2;
        vel.y*=-.9;
    }
    
    if(currPos.y > ofGetHeight()-size*2)
    {
        currPos.y = ofGetHeight()-size*2;
        vel.y*=-.9;
    }
}

void dotParticle::applyForce()
{
    vel = {ofRandom(-10, 10), ofRandom(-10, 10), 0};
    accel = {0, ofRandom(.2, .4), 0};
}

void dotParticle::applyMelt()
{
    //vel = {ofRandom(-10, 10), ofRandom(-10, 10), 0};
    accel = {0.01 * ofRandom(-1, 1), ofRandom(.008, .01), 0};
}

void dotParticle::applyWind(glm::vec3 currPos)
{
    glm::vec3 mousePos(ofGetMouseY(), ofGetMouseY(), 0);
    
    float velX = atan2(currPos.x, currPos.x);
    float velY = atan2(currPos.y, currPos.y);
    
    vel = {velX, velY, 0};
    
}
