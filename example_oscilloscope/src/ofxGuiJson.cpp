#include "ofxGuiJson.h"
#include "ofApp.h" 

void ofxGuiJson::setup(const std::string& configFile) {
    loadJson(configFile);
    parseGuiElements();
    //bindEvents();
}

void ofxGuiJson::loadJson(const std::string& relativePath) {
    std::string fullPath = ofToDataPath(relativePath, true);
    std::ifstream i(fullPath);
    if (!i.is_open()) {
        ofLogError() << "Failed to open JSON config file: " << fullPath;
        return;
    }

    try {
        i >> config;
        ofLogNotice() << "JSON loaded successfully from: " << fullPath;
        ofLogVerbose() << "JSON Content: " << config.dump(4); // Dump JSON content with indentation for easier reading
    }
    catch (const nlohmann::json::parse_error& e) {
        ofLogError() << "JSON parse error at byte " << e.byte << ": " << e.what();
    }
    catch (const std::exception& e) {
        ofLogError() << "Exception parsing JSON: " << e.what();
    }
}

void ofxGuiJson::parseGuiElements() {
    if (config.is_null()) {
        ofLogError() << "JSON config is null, cannot parse GUI elements.";
        return;
    }

    if (!config.contains("gui_elements") || !config["gui_elements"].is_array()) {
        ofLogError() << "Missing or invalid 'gui_elements' in JSON.";
        return;
    }
    try {
        if (config.is_null() || !config.contains("gui_elements") || !config["gui_elements"].is_array()) {
            ofLogError() << "JSON config is invalid or missing 'gui_elements'.";
            return;
        }

        if (config.contains("gui")) {
            float guiWidth = config["gui"].value("width", 0); // Default width if not specified
            float guiHeight = config["gui"].value("height", 0); // Default height if not specified
            gui.setup(config["gui"].value("title", "Default Title"), config["gui"].value("settings_file", "settings.xml"));
            gui.setSize(guiWidth, guiHeight);
            std::cout << "GUI size set to: " << guiWidth << "x" << guiHeight << std::endl;
            int posX = config["gui"]["position"].value("x", 0);
            int posY = config["gui"]["position"].value("y", 0);
            gui.setPosition(posX, posY);
        }
        else {
            ofLogError() << "GUI general configuration section is missing.";
            return;
        }

        // Setup individual GUI elements
        if (!config.contains("gui_elements") || !config["gui_elements"].is_array()) {
            ofLogError() << "Missing or invalid 'gui_elements' in JSON.";
            return;
        }

        for (const auto& el : config["gui_elements"]) {
            std::string type = el.value("type", "");
            std::string label = el.value("label", "");
            std::string action = el.value("action", "");
            std::string id = el.value("id", label);  // Ensuring an ID is set, defaulting to label if not provided.
            float width = el.value("size", json::object()).value("width", 0);
            float height = el.value("size", json::object()).value("height", 0);
            float posX = 0, posY = 0;

            std::shared_ptr<ofxBaseGui> guiElement;

            if (type == "toggle") {
                auto toggle = std::make_shared<ofxToggle>();
                if (!toggle) {
                    ofLogError() << "Failed to create toggle for label: " << label;
                    continue;
                }
                toggle->setup(label, el.value("default", false), width, height);
                toggle->setPosition(posX, posY);
                guiElement = toggle;
                ofLogNotice() << "Toggle created and positioned: " << id;
            }
            else if (type == "label") {
                auto labelControl = std::make_shared<ofxLabel>();
                if (!labelControl) {
                    ofLogError() << "Failed to create label for: " << label;
                    continue;
                }
                labelControl->setup(label, width, height);
                labelControl->setPosition(posX, posY);
                guiElement = labelControl;
                ofLogNotice() << "Label created and positioned: " << id;
            }
            else {
                ofLogError() << "Unsupported GUI element type: " << type;
                continue;  // Skip this element if the type is unsupported
            }

            // Set position and add to GUI
            guiElement->setPosition(posX, posY);
            gui.add(guiElement.get());
            guiElementsMap[id] = guiElement;  // Store the element

            if (!action.empty() && type == "toggle") {
                ofLogNotice() << "Attempting to bind action for: " << id;
            }
        }
    }
    catch (const nlohmann::json::exception& e) {
        ofLogError() << "Exception while parsing GUI elements: " << e.what();
    }
}


/*
void ofxGuiJson::bindEvents() {
    ofLogNotice() << "Checking Action Map Contents:";
    for (const auto& pair : appInstance->actionMap) {
        ofLogNotice() << "Action Map Entry: " << pair.first;
    }

    static bool eventsBound = false; // Add a static flag to ensure binding happens only once
    if (!eventsBound) {
        for (const auto& el : config["gui_elements"]) {
            std::string id = el.value("id", "");
            std::string action = el.value("action", "");

            auto guiElement = std::dynamic_pointer_cast<ofxToggle>(getGuiElement(id));
            if (guiElement && !action.empty() && appInstance->actionMap.find(action) != appInstance->actionMap.end()) {
                guiElement->addListener(appInstance, appInstance->actionMap[action]);
                ofLogNotice() << "Action bound for: " << id;
            }
            else {
                ofLogError() << "Action not found or undefined for toggle: " << id;
            }
        }
        eventsBound = true;
    }
}
*/

std::shared_ptr<ofxBaseGui> ofxGuiJson::getGuiElement(const std::string& id) {
    auto it = guiElementsMap.find(id);
    if (it != guiElementsMap.end() && it->second) {
        return it->second;
    }
    else {
        ofLogError() << "GUI element with ID '" << id << "' not found or is null.";
        return nullptr; // Return nullptr explicitly if not found or null
    }
}


void ofxGuiJson::updateGuiLabels(const std::map<std::string, std::string>& labelUpdates) { // potentially only have string string instead of map
    for (const auto& update : labelUpdates) {
        auto labelElement = std::dynamic_pointer_cast<ofxLabel>(getGuiElement(update.first));
        if (labelElement) {
            labelElement->setName(update.second);
        }
        else {
            ofLogError() << "Label with ID '" << update.first << "' not found for update.";
        }
    }
}

void ofxGuiJson::createAndAddToggle(const std::string& label, const std::string& baseId) {
    static int toggleCount = 0; // Static counter to ensure unique IDs
    std::string id = baseId + std::to_string(toggleCount++); // Generate unique ID

    auto toggle = std::make_shared<ofxToggle>();
    float width = 100; // Default width
    float height = 20; // Default height
    float posX = 50;   // Default x position
    float posY = 50 + (toggleCount * (height + 10)); // Stack toggles vertically

    toggle->setup(label, false, width, height);
    toggle->setPosition(posX, posY);
    gui.add(toggle.get());
    guiElementsMap[id] = toggle;
    dynamicToggles.insert(id); // Maintain a set of dynamically added toggles

    // Adjust GUI size or enable scrolling as needed
    if (posY + height > gui.getHeight()) {
        gui.setSize(gui.getWidth(), posY + height + 20);
    }

    ofLogNotice() << "New toggle added: " << label << " with ID: " << id;
}


void ofxGuiJson::removeLastToggle() {
    if (!guiElementsMap.empty()) {
        auto it = guiElementsMap.end();
        do {
            if (it == guiElementsMap.begin()) {
                ofLogError() << "No dynamic toggles to remove.";
                return;  // Exit if there are no dynamic toggles or if we've checked all toggles
            }
            --it;  // Move to the previous element
        } while (dynamicToggles.find(it->first) == dynamicToggles.end());  // Continue while the toggle is not dynamic

        // Remove the toggle from the dynamicToggles set and guiElementsMap
        dynamicToggles.erase(it->first);
        guiElementsMap.erase(it);

        // Rebuild the GUI
        rebuildGui();
        ofLogNotice() << "Dynamic toggle removed.";
    }
    else {
        ofLogNotice() << "No toggles to remove.";
    }
}

void ofxGuiJson::rebuildGui() {
    gui.clear();
    for (const auto& id : staticToggles) {  // This now includes all types of elements.
        if (guiElementsMap.find(id) != guiElementsMap.end()) {
            gui.add(guiElementsMap[id].get());
        }
    }
    for (const auto& id : dynamicToggles) {  // Ensure dynamic elements are added after static ones.
        if (guiElementsMap.find(id) != guiElementsMap.end()) {
            gui.add(guiElementsMap[id].get());
        }
    }
}

