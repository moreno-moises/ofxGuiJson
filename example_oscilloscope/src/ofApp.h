#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxGuiJson.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofxGuiJson guiManager;
};
