#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include <json.hpp>
#include <fstream>
#include <memory> // for std::shared_ptr
#include <map>   
#include <string>

using json = nlohmann::json;

class ofxGuiJson {
public:

    void setup(const std::string& configFile);
    void loadJson(const std::string& path);
    const json& getConfig() const { return config; }
    void updateGuiLabels(const std::map<std::string, std::string>& labelUpdates);

    void createAndAddToggle(const std::string& label, const std::string& id);
    void ofxGuiJson::removeLastToggle();
    void ofxGuiJson::rebuildGui();
    std::set<std::string> dynamicToggles;
    std::set<std::string> staticToggles;

    void parseGuiElements();
    std::vector<std::shared_ptr<ofxPanel>>& getPanels();  // Return a reference to the vector of shared_ptr
    void ofxGuiJson::setupPanel(ofxPanel& panel, const json& panelConfig);
    void setBackgroundColor();


    ofEvent<std::pair<std::string, bool>> onToggleEvent;
    void ofxGuiJson::onToggleAction(bool& state);

    json config;

    ofxPanel& getGui() { return gui; }
    std::shared_ptr<ofxBaseGui> getGuiElement(const std::string& label);
    std::map<ofParameter<bool>*, std::string> toggleIdMap;

private:
    ofxPanel gui;
    std::map<std::string, std::shared_ptr<ofxBaseGui>> guiElementsMap;
    void addGuiElement(ofxPanel& panel, const json& elementConfig);
    std::vector<std::shared_ptr<ofxPanel>> panels; // Use shared_ptr for panels
};
