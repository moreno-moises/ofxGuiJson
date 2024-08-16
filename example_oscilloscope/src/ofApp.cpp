#include "ofApp.h"

void ofApp::setup() {
    // Set up the GUI manager with the configuration file
    guiManager.setup("config.json");

}

void ofApp::update() {
}

void ofApp::draw() {
    GLuint defaultTexture;
    glGenTextures(1, &defaultTexture);
    glBindTexture(GL_TEXTURE_2D, defaultTexture);

    for (auto& panel : guiManager.getPanels()) {
        panel->draw();
    }

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the default texture
    glDeleteTextures(1, &defaultTexture); // Clean up the texture
}

