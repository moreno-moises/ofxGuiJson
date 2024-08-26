/**************************************************************************/
/*!
    @file     ofApp.h

    @brief    This file defines the ofApp class, which handles the main application
              logic for an openFrameworks project. It includes audio processing, GUI
              interactions, and input handling.

    @section  LICENSE

              EmotiBit invests time and resources providing this open source code,
              please support EmotiBit and open-source hardware by purchasing
              products from EmotiBit!

              Written by Moises Moreno for EmotiBit.
*/
/**************************************************************************/

#pragma once

#include "ofxGui.h"
#include "ofxMaxim.h"
#include "ofxGuiJson.h"
#include <json.hpp>

using json = nlohmann::json;

/*!
 *  @class ofApp
 *  @brief The main application class that handles setup, updates, drawing, and input events.
 */
class ofApp : public ofBaseApp {
public:
    using ToggleAction = void (ofApp::*)(bool&);

    /*!
     *  @brief Sets up the application, including GUI and audio settings.
     */
    void setup();

    /*!
     *  @brief Updates the application logic, such as audio and GUI states.
     */
    void update();

    /*!
     *  @brief Renders the application visuals.
     */
    void draw();

    /*!
     *  @brief Cleans up resources when the application exits.
     */
    void exit();

    /*!
     *  @brief Processes audio output.
     *  @param buffer The sound buffer to fill with audio data.
     */
    void audioOut(ofSoundBuffer& buffer);

    /*!
     *  @brief Handles mouse press events.
     *  @param x The x-coordinate of the mouse.
     *  @param y The y-coordinate of the mouse.
     *  @param button The mouse button pressed.
     */
    void mousePressed(int x, int y, int button) override;

    /*!
     *  @brief Handles mouse release events.
     *  @param x The x-coordinate of the mouse.
     *  @param y The y-coordinate of the mouse.
     *  @param button The mouse button released.
     */
    void mouseReleased(int x, int y, int button) override;

    /*!
     *  @brief Callback function for button press events.
     */
    void onButtonPressed();

    /*!
     *  @brief Callback function for toggle change events.
     *  @param value The new state of the toggle.
     */
    void onToggleChanged(bool& value);

    /*!
     *  @brief Handles the action for the first frequency toggle.
     *  @param state The state of the toggle.
     */
    void onFreqToggle1(bool& state);

    /*!
     *  @brief Handles the action for the second frequency toggle.
     *  @param state The state of the toggle.
     */
    void onFreqToggle2(bool& state);

    /*!
     *  @brief Handles the action for the third frequency toggle.
     *  @param state The state of the toggle.
     */
    void onFreqToggle3(bool& state);

    /*!
     *  @brief Sets the frequency range for the oscillator.
     *  @param minFreq The minimum frequency.
     *  @param maxFreq The maximum frequency.
     */
    void setFrequencyRange(int minFreq, int maxFreq);

    /*!
     *  @brief Updates the state of a toggle based on its ID.
     *  @param toggleId The ID of the toggle.
     *  @param newState The new state to set.
     */
    void updateToggleState(const std::string& toggleId, bool newState);

    /*!
     *  @brief Handles key press events.
     *  @param key The key that was pressed.
     */
    void keyPressed(int key) override;

    /*!
     *  @brief Handles GUI JSON toggle events.
     *  @param toggleInfo The toggle information containing the ID and state.
     */
    void handleGuiJsonToggle(std::pair<std::string, bool>& toggleInfo);

    ofxPanel gui;                               //!< The main GUI panel.
    ofPolyline waveLine;                        //!< The polyline for visualizing the waveform.
    maxiOsc myOsc;                              //!< The oscillator for generating sound.
    ofSoundStream soundStream;                  //!< The sound stream for audio output.
    double frequency = 440;                     //!< The current frequency of the oscillator.
    bool isMuted = false;                       //!< Flag indicating if the sound is muted.
    bool isMousePressed = false;                //!< Flag indicating if the mouse is pressed.
    int oldMouseY = 0;                          //!< The previous y-coordinate of the mouse.

    std::map<std::string, ToggleAction> actionMap;  //!< A map linking toggle IDs to their corresponding actions.

    ofxGuiJson guiManager;                      //!< The GUI manager for handling JSON-based GUI elements.

private:
    int frequencyMin = 220;                     //!< The minimum frequency range.
    int frequencyMax = 440;                     //!< The maximum frequency range.
};
