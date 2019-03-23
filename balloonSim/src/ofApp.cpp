/* This app creates an environment to manipulate x,y coordinates that make
 * up a skeleton body.
 *
 * 1. Take in the x, y data
 */

#include "ofApp.h"


void ofApp::setup()
{
    fbo.allocate(1024, 512, GL_RGB);
    savePixels.allocate(1024, 512, GL_RGB);
    fbo.begin();
    ofClear(255,255,255, 0);
    fbo.end();
    
    ofSetFrameRate(60);
    
    float dotSize = 10;
    ofBackground(0);
    ofBuffer buffer = ofBufferFromFile("14-coordinates.txt");
    
    gMin.x = (77.0 + (2*dotSize));//*1.832;
    gMax.x = (648.0 + (2*dotSize));//*1.832;
    gMin.y = (85.0 - (2*dotSize));//*1.832;
    gMax.y = (626.0 - (2*dotSize));//*1.832;
    //ofScale(.5459, .5459);
    //1.8318
    
    if(buffer.size())
    {
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
        {
            
            // grab the line
            string line = *it;
            
            // split the line by the comma to separate the x+y values
            std::vector<string> parsedLine = ofSplitString(line, ",");
            
            // Clear the dotFrame whenever there's an empty line
            if (line.empty())
            {
                std::cout << "SCREEEEAAM" << std::endl;
            }
            
            else
            {
                // store the x+y values in a glm::vec2
                glm::vec2 tempXY = {ofToFloat(parsedLine[0]), ofToFloat(parsedLine[1])};
                dotParticle dp;
                
                dp.currPos = {(tempXY.x)*1.21 + 106, (tempXY.y)*1.21 - (150), 0};
                dp.startPos = dp.currPos;
                dp.vel = {0, 0, 0};
                dp.accel = {0, 0, 0};
                dp.size = 10.2;
                dp.color = ofColor::fromHsb(ofMap(j, 0, 14, 10, 245), 255, 255);
                j++;
                
                dotParticles.push_back(dp);
                
            }
        }
    }
    
    
}


void ofApp::update()
{
    glm::vec3 mouse(ofGetMouseX(), ofGetMouseY(), 0);
    
    float forceScaler = 1;
    
    if (ofGetMousePressed())
    {
        forceScaler = 20;
    
        for(auto& dp : dotParticles)
        {
            float r = glm::distance(mouse, dp.currPos);
            
            float magnitude = forceScaler / (r + 10);
            
            glm::vec3 forceDirection = glm::normalize(dp.currPos - mouse);
            
            glm::vec3 force = forceDirection * magnitude;
            
            dp.force += force;
        }
    }
    else
    {
        forceScaler = 7;
        
        
        for(auto& dp : dotParticles)
        {
            if(glm::distance(dp.startPos, dp.currPos) != 0)
            {
                float r = glm::distance(dp.startPos, dp.currPos);

                float magnitude = 1;
                
                if(glm::distance(dp.startPos, dp.currPos) < 1)
                {
                    dp.force = {0, 0, 0};
                    dp.vel = {0, 0, 0};
                    dp.accel = {0, 0, 0};
                    
                    
                }
                else
                {
                    magnitude = forceScaler / (r + 10);
                    glm::vec3 forceDirection = glm::normalize(dp.startPos - dp.currPos);
                    
                    glm::vec3 force = forceDirection * magnitude;
                    
                    dp.force = force;
                }

                
            }
        }
    }
    
    for(auto& dp : dotParticles)
    {
        
        if(ofGetKeyPressed(' '))
        {
            dp.applyForce();
        }
        else if(ofGetKeyPressed('m'))
        {
            dp.applyMelt();
        }
        dp.checkWalls();
        dp.update();
        
    }
}


void ofApp::draw()
{
    
    for(auto& dp : dotParticles)
    {
//        ofSetColor(255, 100);
//        ofDrawLine(dp.currPos.x, dp.currPos.y, dp.startPos.x, ofGetHeight());
        ofSetColor(dp.color);
        ofDrawCircle(dp.currPos, dp.size);
       
    }
    
    if(record)
    {
        fbo.begin();
        ofClear(0);
    
    for(auto& dp : dotParticles)
    {
        ofSetColor(dp.color);
        ofDrawCircle(dp.currPos, dp.size);
    }
    
        fbo.end();
        fbo.readToPixels(savePixels);
        
        
        
        if(saveCount < 10)
        {
            saveName = "0000" + ofToString(saveCount) + ".png";
        }
        else if(saveCount < 100 && saveCount > 9)
        {
            saveName = "000" + ofToString(saveCount) + ".png";
        }
        else if(saveCount < 1000 && saveCount > 99)
        {
            saveName = "00" + ofToString(saveCount) + ".png";
        }
        else if(saveCount < 10000 && saveCount > 999)
        {
            saveName = "0" + ofToString(saveCount) + ".png";
        }
        else
        {
            saveName = ofToString(saveCount) + ".png";
        }
        
        ofSaveImage(savePixels, "../exports/01/particles" + saveName);
        saveCount++;
    }
}


void ofApp::keyPressed(int key)
{
    if(key == 'e')
    {
        std::exit(1);
    }
    
    else if (key == 'r')
    {
        record = !record;
    }
}
