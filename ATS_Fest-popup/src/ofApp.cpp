#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetColor(255);
    ofBackground(0);
    //ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);

    // scan models dir
    models_dir.listDir("models");
    if(models_dir.size()==0) {
        ofLogError() << "Couldn't find models folder." << msa::tf::missing_data_error();
        assert(false);
        ofExit(1);
    }
    models_dir.sort();
    load_model_index(0); // load first model
    
    
    ////////// DOT LERPER STUFF //////////
    
    float dotSize = 10;
    ofBackground(0);
    ofBuffer buffer = ofBufferFromFile("dot_data/amalg_02.txt");
    
    gMin.x = (77.0 + (2*dotSize));//*1.832;
    gMax.x = (648.0 + (2*dotSize));//*1.832;
    gMin.y = (85.0 - (2*dotSize));//*1.832;
    gMax.y = (626.0 - (2*dotSize));//*1.832;
    //ofScale(.5459, .5459);
    //1.8318
    std::cout << buffer.size() << std::endl;
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
                dotFrame.clear();
                //std::cout << "SCREEEEAAM" << std::endl;
            }
            
            else
            {
                // store the x+y values in a glm::vec2
                glm::vec2 tempXY = {ofToFloat(parsedLine[0]), ofToFloat(parsedLine[1])};
                dotParticle dp;
                
                dp.pos = {(tempXY.x)*1.21 + 106, (tempXY.y)*1.21 - (150), 0};
                dp.vel = {0, 0, 0};
                dp.accel = {0, 0, 0};
                dp.size = 10.2;
                dp.color = ofColor::fromHsb(ofMap(j, 0, 14, 10, 245), 255, 255);
                j++;
                
                dotFrame.push_back(dp);
                //std::cout << dotFrame.size() << std::endl;
                
                if (dotFrame.size() == 14)
                {
                    dotParticles.push_back(dotFrame);
                    j = 0;
                }
                
            }
        }
    }
}


//--------------------------------------------------------------
// Load graph (model trained in and exported from python) by folder NAME, and initialise session
void ofApp::load_model(string model_dir) {
    ofLogVerbose() << "loading model " << model_dir;

    // init the model
    // note that it expects arrays for input op names and output op names, so just use {}
    model.setup(ofFilePath::join(model_dir, "graph_frz.pb"), {input_op_name}, {output_op_name});
    if(! model.is_loaded()) {
        ofLogError() << "Model init error." << msa::tf::missing_data_error();
        assert(false);
        ofExit(1);
    }

    img_in.allocate(input_shape[1], input_shape[0], OF_IMAGE_COLOR);
    img_out.allocate(output_shape[1], output_shape[0], OF_IMAGE_COLOR);
    fbo_comp.allocate(input_shape[1], input_shape[0], GL_RGB);
    fbo_comp.begin();
    ofClear(0);
    fbo_comp.end();

    // init tensor for input. shape should be: {batch size, image height, image width, number of channels}
    // (ideally the SimpleModel graph loader would read this info from the graph_def and call this internally)
    model.init_inputs(tensorflow::DT_FLOAT, {1, input_shape[0], input_shape[1], 3});

    // load test image
    ofLogVerbose() << "loading test image";
    img_in.allocate(input_shape[1], input_shape[0], OF_IMAGE_COLOR);
    img_in.load(ofFilePath::join(model_dir, "test_image.png"));
    

    // allocating output image with correct dimensions and no alpha channel
    ofLogVerbose() << "allocating output image " << output_shape;
    img_out.allocate(output_shape[1], output_shape[0], OF_IMAGE_COLOR);
}


//--------------------------------------------------------------
// Load model by folder INDEX
void ofApp::load_model_index(int index) {
    cur_model_index = ofClamp(index, 0, models_dir.size()-1);
    load_model(models_dir.getPath(cur_model_index));
}


//--------------------------------------------------------------
void ofApp::draw() {
    float t = ofGetElapsedTimef();


    // run model on it
    if(model.is_loaded())
        model.run_image_to_image(img_in, img_out, input_range, output_range);

    ////////// ANIMATING MODES //////////
    
    // RANDOM
    if(randomMode)
    {
        newPattern = false;
        fbo_comp.begin();
        ofClear(0);

        for(int i = 0; i < 14; i++)
        {
            ofSetColor(ofColor::fromHsb(ofMap(i, 0, 13, 10, 245), 255, 255));
            ofDrawCircle(ofRandom(512), ofRandom(512), 8);
        }

        fbo_comp.end();


        fbo_comp.readToPixels(img_in.getPixels());
    }
    
    // LERP MODE
    if(lerpMode)
    {
        fbo_comp.begin();
        ofClear(0);


        
        for(int i = 0; i < dotParticles[0].size(); i++)
        {
            
            float initX = dotParticles[prevFigure][i].pos.x;  // 0 1
            float initY = dotParticles[prevFigure][i].pos.y;
            float endX = dotParticles[currFigure][i].pos.x;   // 1 2
            float endY = dotParticles[currFigure][i].pos.y;
            float increment = ofMap(sin(t/lerpRate), -1, 1, 0., 1.);
            ofColor currColor = dotParticles[currFigure][i].color;
            
            float resultX = 0;
            float resultY = 0;
            
            if(currDx > 0)
            {
                resultX = ofLerp(initX, endX, increment);
                resultY = ofLerp(initY, endY, increment);
            }
            
            else
            {
                resultX = ofLerp(endX, initX, increment);
                resultY = ofLerp(endY, initY, increment);
            }
            ofSetColor(currColor);
            
            ofPushMatrix();
            ofTranslate(xPos, yPos);
            ofScale(scaler, scaler);
            ofDrawCircle(resultX, resultY, 10.2);
            ofPopMatrix();
        }
        
        fbo_comp.end();
        
        
        fbo_comp.readToPixels(img_in.getPixels());
    }

    // DISPLAY STUFF
    ofPushMatrix();
    //ofScale(2, 2);
    ofSetColor(255);
    int x = 0;
    if(debug)
    {
        fbo_comp.draw(x, 0);
        x += img_in.getWidth();
        stringstream str;
        str << ofGetFrameRate() << endl;
        str << "scaler: " << scaler << endl;
        str << "xPos: " << xPos << endl;
        str << "yPos: " << yPos << endl;
        
        
        ofDrawBitmapString(str.str(), x+20, img_in.getHeight()+20);

            ofPushStyle();
            ofSetColor(30, 30, 30, 150);
            ofDrawRectangle(0, 0, 512, 512);
            ofPopStyle();
        
    }
    
    img_out.draw(x, 0);
    x += img_out.getWidth();
    ofPopMatrix();

    
    // str << endl;
    // str << "Press number key to load model: " << endl;
    // for(int i=0; i<models_dir.size(); i++) {
    //     auto marker = (i==cur_model_index) ? ">" : " ";
    //     str << " " << (i+1) << " : " << marker << " " << models_dir.getName(i) << endl;
    // }
   
    
    // Deal with incrementing k to the next dot arrangement
    currInc = ofMap(sin(t/lerpRate), -1, 1, .0, 1.);
    
    if(currInc - prevInc > 0)
    {
        currDx = 1;
    }
    else
    {
        currDx = -1;
    }
    prevInc = currInc;
    
    // after a lerp cycle
    if(currDx != prevDx)
    {
        k++;
        prevFigure = currFigure;
        currFigure = ofRandom(startIndex, endIndex);

        //if(decrease)
        //lerpRate+=.1;



    }
    
    prevDx = currDx;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    // switch(key) {
    // case '1':
    // case '2':
    // case '3':
    // case '4':
    // case '5':
    // case '6':
    // case '7':
    // case '8':
    // case '9':
    //     load_model_index(key-'1');
    //     break;
    // }

    if(key == ' ')
    {
        newPattern = !newPattern;
    }
    
    else if(key == 'e')
    {
        std::exit(1);
    }
    
    else if (key == 'r')
    {
        record = !record;
    }
    
    else if (key == '-' && scaler > .125)
    {
        scaler-=.03125;
    }
    else if (key == '=')
    {
        scaler+=.03125;
    }
    
    else if (key == 'w')
    {
        yPos-=10;
    }
    else if (key == 's')
    {
        yPos+=10;
    }
    
    else if (key == 'a')
    {
        xPos-=10;
    }
    else if (key == 'd')
    {
        xPos+=10;
    }
    // MODE SWITCHES
    else if (key == 'r')
    {
        randomMode = true;
        lerpMode = false;
    }
    else if (key == 'l')
    {
        randomMode = false;
        lerpMode = true;
    }
    else if (key == '`')
    {
        debug = !debug;
    }
    
    
    switch(key)
    {
        case OF_KEY_RIGHT:
            if(lerpRate < 64) lerpRate+=.1;
            break;
        case OF_KEY_LEFT:
            if(lerpRate > 0.1) lerpRate-=.1;
            break;
    }
    
}


//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
    // if(dragInfo.files.empty()) return;

    // string file_path = dragInfo.files[0];

    // //// only PNGs work for some reason when Tensorflow is linked in
    // img_in.load(file_path);
}
