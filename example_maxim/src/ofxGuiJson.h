#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include <json.hpp>
#include <fstream>
#include <memory> // for std::shared_ptr
#include <map>   
#include <string>

using json = nlohmann::json;

class ListenerClass {
public:
    bool* toggleState;

    ListenerClass(bool* state) : toggleState(state) {}

    void toggleStateChange(bool& newState) {
        *toggleState = newState; // Update the internal state to the new state.
        ofLogNotice("ListenerClass") << "Toggle state changed to: " << (*toggleState ? "ON" : "OFF");
    }
};

class ofApp;  // Forward declaration

class ofxGuiJson {
public:
    using ToggleAction = void (ofApp::*)(bool&);// Member function pointer type for toggle actions

    void setup(const std::string& configFile);
    void loadJson(const std::string& path);
    void bindEvents();
    const json& getConfig() const { return config; }
    void setAppInstance(ofApp* app) { appInstance = app; }
    void updateGuiLabels(const std::map<std::string, std::string>& labelUpdates);
    void setupGuiElements();

    void createAndAddToggle(const std::string& label, const std::string& id);
    void ofxGuiJson::removeLastToggle();
    void ofxGuiJson::rebuildGui();
    std::set<std::string> dynamicToggles;
    std::set<std::string> staticToggles;
    std::vector<std::string> orderedElements;

    std::map<std::string, ToggleAction> actionMap;  // Stores member function pointers
    json config;

    ofxPanel& getGui() { return gui; }
    std::shared_ptr<ofxBaseGui> getGuiElement(const std::string& label);

private:
    ofApp* appInstance = nullptr;
    ofxPanel gui;
    void parseGuiElements();
    std::map<std::string, std::shared_ptr<ofxBaseGui>> guiElementsMap;
};
