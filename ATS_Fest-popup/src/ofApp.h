#pragma once

#include "ofMain.h"
#include "ofxMSATensorFlow.h"
#include "dotParticle.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void load_model(string model_dir);
    void load_model_index(int index);
    void keyPressed(int key);
    void dragEvent(ofDragInfo dragInfo);
    void draw();

	msa::tf::SimpleModel model;

    // a bunch of properties of the models
    // ideally should read from disk and vary with the model
    // but trying to keep the code minimal so hardcoding them since they're the same for all models
    const int input_shape[2] = {512, 512}; // dimensions {height, width} for input image
    const int output_shape[2] = {512, 512}; // dimensions {height, width} for output image
    const ofVec2f input_range = {-1, 1}; // range of values {min, max} that model expects for input
    const ofVec2f output_range = {-1, 1}; // range of values {min, max} that model outputs
    const string input_op_name = "generator/generator_inputs"; // name of op to feed input to
    const string output_op_name = "generator/generator_outputs"; // name of op to fetch output from

    ofFbo fbo_comp;

    // images in and out of model
    ofFloatImage img_in; // input to the model
    ofFloatImage img_out; // output from the model

    // model file management
    ofDirectory models_dir;    // data/models folder which contains subfolders for each model
    int cur_model_index = 0; // which model (i.e. folder) we're currently using

    bool newPattern = false;
    
    
    ///// MODE STUFF /////
    
    bool lerpMode = true;
    bool randomMode = false;
    
    bool debug = false;
    
    ////////// DOT LERPER STUFF //////////
    
    std::vector<dotParticle> dotFrame;
    std::vector<std::vector<dotParticle>> dotParticles;
    
    glm::vec2 gMin;
    glm::vec2 gMax;
    int j = 0;
    
    ofFbo fbo;
    ofPixels savePixels;
    
    float scaler = .75;
    float xPos = -148;
    float yPos = 60;
    
    bool record = false;
    
    // LERP STUFF
    int saveCount = 0;
    float lerpRate = .75;
    float currDx = 1;
    float prevDx = 1;
    int currFigure = 2;
    int prevFigure = 0;
    float currInc = 0;
    float prevInc = 0;
    int k = 0;
    int randomIndex = 0;
    
    int startIndex = 10;
    int endIndex = 30000;
    string saveName = "";

    float globalX = 170;
    float globalY = -210;

    float globalScale = 3;

    bool changeSpeed = true;

    float figureScaler = 1;
		
};
