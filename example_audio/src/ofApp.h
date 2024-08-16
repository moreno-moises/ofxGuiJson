#pragma once

#include "ofxGui.h"
#include "ofxMaxim.h"
#include "ofxGuiJson.h"
#include <json.hpp>


using json = nlohmann::json;

class ofApp : public ofBaseApp {
public:
    using ToggleAction = void (ofApp::*)(bool&);

    void setup();
    void update();
    void draw();
    void exit();

    void audioOut(ofSoundBuffer& buffer);

    void mousePressed(int x, int y, int button) override;

    void mouseReleased(int x, int y, int button) override;

    void onButtonPressed();
    void onToggleChanged(bool& value);

    void onFreqToggle1(bool& state);
    void onFreqToggle2(bool& state);
    void onFreqToggle3(bool& state);
    void setFrequencyRange(int minFreq, int maxFreq);
    void ofApp::updateToggleState(const std::string& toggleId, bool newState);

    void keyPressed(int key) override;

    ofxPanel gui;
    ofPolyline waveLine;
    maxiOsc myOsc;
    ofSoundStream soundStream;
    double frequency = 440;
    bool isMuted = false;
    bool isMousePressed = false;
    int oldMouseY = 0;

    std::map<std::string, ToggleAction> actionMap;

    ofxGuiJson guiManager;

    void ofApp::handleGuiJsonToggle(std::pair<std::string, bool>& toggleInfo);

private:
    int frequencyMin = 220;
    int frequencyMax = 440;

};
