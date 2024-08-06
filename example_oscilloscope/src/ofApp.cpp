#include "ofApp.h"

void ofApp::setup() {
    // Set up the GUI manager with the configuration file
    guiManager.setup("config.json");

}

void ofApp::update() {
    // Update logic can be added here if needed
}

void ofApp::draw() {

    // Draw the GUI
    guiManager.getGui().draw();
}
