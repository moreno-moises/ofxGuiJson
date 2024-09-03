/**************************************************************************/
/*!
    @file     ofApp.h

    @brief    This file defines the ofApp class, which is responsible for
              managing the basic application setup, updating, and drawing
              logic in an openFrameworks project.

    @section  LICENSE

              EmotiBit invests time and resources providing this open source code,
              please support EmotiBit and open-source hardware by purchasing
              products from EmotiBit!

              Written by Moises Moreno for EmotiBit.
*/
/**************************************************************************/

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxGuiJson.h"

/*!
 *  @class ofApp
 *  @brief The main application class that handles setup, updates, and drawing in the application.
 */
class ofApp : public ofBaseApp {
public:
    /*!
     *  @brief Sets up the application, including any initial configurations.
     */
    void setup();

    /*!
     *  @brief Updates the application state.
     */
    void update();

    /*!
     *  @brief Renders the application visuals.
     */
    void draw();

    ofxGuiJson guiManager; //!< The GUI manager for handling JSON-based GUI elements.
};
