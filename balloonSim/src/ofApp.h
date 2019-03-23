#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "dotParticle.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
        void applyForce();
    
    std::vector<dotParticle> dotParticles;
    
    glm::vec2 gMin;
    glm::vec2 gMax;
    int j = 0;
    
    ofFbo fbo;
    ofPixels savePixels;
    int saveCount = 0;
    string saveName = "";
    
    bool record = false;
		
};
