
#include "ofxGuiJson.h"

void ofxGuiJson::setup(const std::string& configFile) {
    loadJson(configFile);
    setBackgroundColor();  // Set background color based on JSON config
    parseGuiElements();
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

    json panelsConfig = config["panels"];
    if (panelsConfig.is_null() || !panelsConfig.is_array()) {
        ofLogError() << "Invalid or missing 'panels' configuration.";
        return;
    }

    for (auto& panelConfig : panelsConfig) {
        auto panel = std::make_shared<ofxPanel>();
        setupPanel(*panel, panelConfig);
        panels.push_back(panel);
    }
}

void ofxGuiJson::setupPanel(ofxPanel& panel, const json& panelConfig) {
    if (panelConfig.is_null() || !panelConfig.contains("gui") || !panelConfig["gui"].is_object()) {
        ofLogError() << "Invalid 'gui' configuration for panel.";
        return;
    }
    auto& guiConfig = panelConfig["gui"];
    panel.setup(guiConfig.value("title", "Default Panel"), "", guiConfig.value("width", 400), guiConfig.value("height", 300));
    panel.setPosition(guiConfig["position"].value("x", 0), guiConfig["position"].value("y", 0));

    auto elementsConfig = panelConfig["elements"];
    if (elementsConfig.is_array()) {
        for (auto& elementConfig : elementsConfig) {
            addGuiElement(panel, elementConfig);
        }
    }
}


void ofxGuiJson::addGuiElement(ofxPanel& panel, const json& elementConfig) {
    std::string type = elementConfig.value("type", "");
    std::string label = elementConfig.value("label", "");
    std::string id = elementConfig.value("id", "");
    float posX = elementConfig["position"].value("x", 0);
    float posY = elementConfig["position"].value("y", 0);
    float width = elementConfig["size"].value("width", 300);
    float height = elementConfig["size"].value("height", 30);

    std::shared_ptr<ofxBaseGui> guiElement;

    if (type == "toggle") {
        auto toggle = std::make_shared<ofxToggle>();
        toggle->setup(label, elementConfig.value("default", false), width, height);
        toggle->setPosition(posX, posY);
        panel.add(toggle.get());
        guiElement = toggle;

        // Map the ofParameter to the toggle ID
        toggleIdMap[&toggle->getParameter().cast<bool>()] = id;

        // Bind the listener to track the state changes
        toggle->getParameter().cast<bool>().addListener(this, &ofxGuiJson::onToggleAction);
    }

    else if (type == "label") {
        auto labelControl = std::make_shared<ofxLabel>();
        labelControl->setup(label, width, height);
        labelControl->setPosition(posX, posY);
        guiElement = labelControl;
        panel.add(guiElement.get());
    }


    if (guiElement) {
        guiElementsMap[id] = guiElement;
    }
}



std::vector<std::shared_ptr<ofxPanel>>& ofxGuiJson::getPanels() {
    return panels;
}


void ofxGuiJson::onToggleAction(bool& state) {
    // Find the ID associated with the parameter that triggered this action
    for (const auto& pair : toggleIdMap) {
        if (&pair.first->get() == &state) {
            std::string id = pair.second;
            ofLogNotice() << "onToggleAction called for toggle ID: " << id << " with state: " << state;

            // Process the toggle action using the ID
            auto guiElement = getGuiElement(id);
            if (guiElement) {
                std::string action = "";
                for (const auto& el : config["panels"][0]["elements"]) {
                    if (el.contains("id") && el["id"].get<std::string>() == id) {
                        action = el.value("action", "");
                        break;
                    }
                }
                if (!action.empty()) {
                    std::pair<std::string, bool> eventParam = make_pair(action, state);
                    ofNotifyEvent(onToggleEvent, eventParam, this);
                }
            }
            return; // Exit after processing the correct toggle
        }
    }

    ofLogError() << "No ID found for the given parameter!";
}




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
    for (const auto& id : staticToggles) { 
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

void ofxGuiJson::setBackgroundColor() {
    if (config.contains("background_color") && config["background_color"].is_array()) {
        auto bgColor = config["background_color"];
        if (bgColor.size() == 3) {
            int r = bgColor[0];
            int g = bgColor[1];
            int b = bgColor[2];
            ofSetBackgroundColor(r, g, b);
        }
        else {
            ofLogError() << "Invalid 'background_color' format. Expected an array of 3 integers.";
        }
    }
    else {
        ofLogWarning() << "'background_color' not specified or in incorrect format. Using default background color.";
    }
}

