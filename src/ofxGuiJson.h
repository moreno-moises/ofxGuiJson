/**************************************************************************/
/*!
    @file     ofxGuiJson.h

    @brief    This file defines the ofxGuiJson class, which handles GUI elements
              using JSON configuration in an openFrameworks project.

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
#include <json.hpp>
#include <fstream>
#include <memory> 
#include <map>   
#include <string>
#include <set>
#include <vector>

using json = nlohmann::json;

/*!
 *  @class ofxGuiJson
 *  @brief A class to manage GUI elements using JSON configuration.
 */
class ofxGuiJson {
public:
    /*!
     *  @brief Sets up the GUI from a configuration file.
     *  @param configFile The path to the JSON configuration file.
     */
    void setup(const std::string& configFile);

    /*!
     *  @brief Loads JSON configuration from a file.
     *  @param path The path to the JSON file.
     */
    void loadJson(const std::string& path);

    /*!
     *  @brief Returns the current JSON configuration.
     *  @return A reference to the JSON configuration.
     */
    const json& getConfig() const { return config; }

    /*!
     *  @brief Updates the labels of the GUI elements.
     *  @param labelUpdates A map of label updates where the key is the old label and the value is the new label.
     */
    void updateGuiLabels(const std::map<std::string, std::string>& labelUpdates);

    /*!
     *  @brief Creates and adds a toggle button to the GUI.
     *  @param label The label for the toggle button.
     *  @param id The identifier for the toggle button.
     */
    void createAndAddToggle(const std::string& label, const std::string& id);

    /*!
     *  @brief Removes the last added toggle from the GUI.
     */
    void removeLastToggle();

    /*!
     *  @brief Rebuilds the entire GUI.
     */
    void rebuildGui();

    /*!
     *  @brief Parses the GUI elements from the JSON configuration.
     */
    void parseGuiElements();

    /*!
     *  @brief Returns a reference to the vector of shared pointers to panels.
     *  @return A reference to the vector of shared pointers to panels.
     */
    std::vector<std::shared_ptr<ofxPanel>>& getPanels();

    /*!
     *  @brief Sets up a GUI panel based on the JSON configuration.
     *  @param panel The panel to be set up.
     *  @param panelConfig The JSON configuration for the panel.
     */
    void setupPanel(ofxPanel& panel, const json& panelConfig);

    /*!
     *  @brief Sets the background color of the GUI.
     */
    void setBackgroundColor();

    /*!
     *  @brief Event triggered when a toggle action occurs.
     */
    ofEvent<std::pair<std::string, bool>> onToggleEvent;

    /*!
     *  @brief Handles the action triggered by a toggle.
     *  @param state The state of the toggle (true or false).
     */
    void onToggleAction(bool& state);

    /*!
     *  @brief Returns a reference to the main GUI panel.
     *  @return A reference to the main GUI panel.
     */
    ofxPanel& getGui() { return gui; }

    /*!
     *  @brief Gets a GUI element by its label.
     *  @param label The label of the GUI element.
     *  @return A shared pointer to the GUI element.
     */
    std::shared_ptr<ofxBaseGui> getGuiElement(const std::string& label);

    /*!
     *  @brief Map linking toggle parameters to their IDs.
     */
    std::map<ofParameter<bool>*, std::string> toggleIdMap;

    /*!
     *  @brief A set of dynamically created toggles.
     */
    std::set<std::string> dynamicToggles;

    /*!
     *  @brief A set of static toggles.
     */
    std::set<std::string> staticToggles;

    /*!
    *  @brief A vector of shared pointers to GUI panels.
    */
    std::vector<std::shared_ptr<ofxPanel>> panels;

    /*!
     *  @brief Current JSON configuration.
     */
    json config;

private:
    /*!
     *  @brief The main GUI panel.
     */
    ofxPanel gui;

    /*!
     *  @brief A map linking element labels to their shared pointers.
     */
    std::map<std::string, std::shared_ptr<ofxBaseGui>> guiElementsMap;

    /*!
     *  @brief Adds a GUI element to a panel based on the JSON configuration.
     *  @param panel The panel to which the element is added.
     *  @param elementConfig The JSON configuration for the GUI element.
     */
    void addGuiElement(ofxPanel& panel, const json& elementConfig);

};
