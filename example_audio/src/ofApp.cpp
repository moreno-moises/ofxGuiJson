#include "ofApp.h"

void ofApp::setup() {
    actionMap["toggleMute"] = &ofApp::onToggleChanged;
    actionMap["onFreqToggle1"] = &ofApp::onFreqToggle1;
    actionMap["onFreqToggle2"] = &ofApp::onFreqToggle2;
    actionMap["onFreqToggle3"] = &ofApp::onFreqToggle3;

    ofAddListener(guiManager.onToggleEvent, this, &ofApp::handleGuiJsonToggle);
    guiManager.setup("config.json"); //pass contents as string instead of file and handle multiple json loading

    if (guiManager.getConfig().is_null()) {
        ofLogError() << "Configuration is null after loading.";
        return;
    }

    // Audio settings
    auto audioSettings = guiManager.getConfig()["audio_settings"];
    ofSoundStreamSettings settings;
    settings.numOutputChannels = audioSettings.value("outputChannels", 0);
    settings.numInputChannels = audioSettings.value("inputChannels", 0);
    settings.sampleRate = audioSettings.value("sampleRate", 0);
    settings.bufferSize = audioSettings.value("bufferSize", 0);
    settings.setApi(ofSoundDevice::Api::MS_DS);
    settings.setOutListener(this);
    soundStream.setup(settings);

}


void ofApp::handleGuiJsonToggle(std::pair<std::string, bool>& toggleInfo) {
    ofLogNotice() << "handleGuiJsonToggle called with action: " << toggleInfo.first << " and state: " << toggleInfo.second;

    // Check if the action corresponds to the mute toggle
    if (toggleInfo.first == "toggleMute") {
        ofLogNotice() << "Mute action identified with state: " << toggleInfo.second;
        onToggleChanged(toggleInfo.second);
    }
    else {
        // Handle other actions
        auto it = actionMap.find(toggleInfo.first);
        if (it != actionMap.end()) {
            (this->*it->second)(toggleInfo.second); // Call the function pointer for the action
        }
        else {
            ofLogNotice() << "Unhandled toggle action: " << toggleInfo.first;
        }
    }
}

void ofApp::onToggleChanged(bool& value) {
    isMuted = value;
    if (isMuted) {
        ofLogNotice() << "Mute state changed to: Muted";
    }
    else {
        ofLogNotice() << "Mute state changed to: Unmuted";
    }
    ofLogNotice() << "Internal mute state is now: " << (isMuted ? "Muted" : "Unmuted");
}



void ofApp::onButtonPressed() {
    // Handle button press
}



void ofApp::update() {
    // Check if mouseYFrequency interaction is enabled
    if (guiManager.getConfig()["interaction"].value("enable", false) &&
        guiManager.getConfig()["interaction"]["type"] == "mouseYFrequency") {
        int currentMouseY = ofGetMouseY();
        if (abs(currentMouseY - oldMouseY) > guiManager.getConfig()["audio_settings"]["sensitivity"].get<int>()) {
            frequency = ofMap(currentMouseY, 0, ofGetHeight(),
                guiManager.getConfig()["audio_settings"]["minFrequency"].get<int>(),
                guiManager.getConfig()["audio_settings"]["maxFrequency"].get<int>(), true); //move out of gui manager
            oldMouseY = currentMouseY;
        }
    }

    // Update wave line with the current mouse position if the mouse is pressed
    if (isMousePressed) {
        waveLine.addVertex(ofGetMouseX(), ofGetMouseY());
    }

    if (waveLine.size() > 1000) {
        waveLine.getVertices().erase(waveLine.getVertices().begin());
    }

    std::map<std::string, std::string> labelUpdates;

    // Check for specific conditions or retrieve data, then prepare label text
    if (guiManager.getConfig()["interaction"].value("enable", false)) {
        int mouseX = ofGetMouseX();
        int mouseY = ofGetMouseY();
        labelUpdates["cursorLabel"] = "Cursor: X=" + std::to_string(mouseX) + " Y=" + std::to_string(mouseY);

        double currentFrequency = frequency;
        labelUpdates["frequencyLabel"] = "Frequency: " + std::to_string(currentFrequency);
    }

    if (frequencyMin != frequencyMax) {
        frequency = ofMap(ofGetMouseY(), 0, ofGetHeight(), frequencyMin, frequencyMax, true);
    }
    // Pass updates to ofxGuiJson
    guiManager.updateGuiLabels(labelUpdates);
}

void ofApp::draw() {
    ofBackground(0); // Clear the background to black
    waveLine.draw();
    GLuint defaultTexture;
    glGenTextures(1, &defaultTexture);
    glBindTexture(GL_TEXTURE_2D, defaultTexture);

    for (auto& panel : guiManager.getPanels()) {
        panel->draw();
    }

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the default texture
    glDeleteTextures(1, &defaultTexture); // Clean up the texture
}


void ofApp::audioOut(ofSoundBuffer& buffer) {
    for (size_t i = 0; i < buffer.getNumFrames(); ++i) {
        if (!isMuted) {
            double wave = myOsc.sinewave(frequency); // Use the dynamically updated frequency
            buffer[i * buffer.getNumChannels()] = wave;
            if (buffer.getNumChannels() > 1) {
                buffer[i * buffer.getNumChannels() + 1] = wave;
            }
        }
    }
}

void ofApp::exit() {
    ofRemoveListener(guiManager.onToggleEvent, this, &ofApp::handleGuiJsonToggle);

    soundStream.close();  // Close any audio streams or other cleanup needed
}


void ofApp::mousePressed(int x, int y, int button) {
    if (button == 0) { // Check if left mouse button
        isMousePressed = true;
        waveLine.clear(); // Clear the wave line when the mouse is pressed
    }
    if (button == 2) { // Check if the right mouse button is pressed
        guiManager.createAndAddToggle("New Device", "newDeviceToggle");
        ofLogNotice("ofApp::mousePressed") << "Right mouse button pressed, adding toggle.";
    }
}

void ofApp::mouseReleased(int x, int y, int button) {
    if (button == 0) { // Check if left mouse button
        isMousePressed = false;
    }
}

void ofApp::onFreqToggle1(bool& state) {
    if (state) {
        setFrequencyRange(220, 440);
        updateToggleState("freqToggle2", false);
        updateToggleState("freqToggle3", false);
    }
}

void ofApp::onFreqToggle2(bool& state) {
    if (state) {
        setFrequencyRange(440, 660);
        updateToggleState("freqToggle1", false);
        updateToggleState("freqToggle3", false);
    }
}

void ofApp::onFreqToggle3(bool& state) {
    if (state) {
        setFrequencyRange(660, 880);
        updateToggleState("freqToggle1", false);
        updateToggleState("freqToggle2", false);
    }
}

void ofApp::updateToggleState(const std::string& toggleId, bool newState) {
    auto toggle = std::dynamic_pointer_cast<ofxToggle>(guiManager.getGuiElement(toggleId));
    if (toggle) {
        ofParameter<bool>& param = toggle->getParameter().cast<bool>();
        param.set(newState);
    }
}

void ofApp::setFrequencyRange(int minFreq, int maxFreq) {
    ofLogNotice() << "Setting frequency range: " << minFreq << " to " << maxFreq;
    frequencyMin = minFreq;
    frequencyMax = maxFreq;
    frequency = (frequencyMin + frequencyMax) / 2;
    ofLogNotice() << "Current frequency set to: " << frequency;
}

void ofApp::keyPressed(int key) {
    ofLogNotice() << "Key pressed: " << key;  // Log the ASCII value of the key

    if (key == 'n' || key == 'N') {
        ofLogNotice() << "Creating a new toggle for 'New Device'";
        guiManager.createAndAddToggle("New Device", "newDeviceToggle");
    }

    if (key == 'x' || key == 'X') {
        guiManager.removeLastToggle();
        ofLogNotice("ofApp::keyPressed") << "Key 'x' pressed, removing last toggle.";
    }
}

