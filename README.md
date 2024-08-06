# Documentation for ofxGUIJson Project

## Overview
This project aims to create a dynamic and scalable GUI system using OpenFrameworks, which allows the user to select a JSON file containing pre-determined GUI parameters. The system will load the JSON file and create a GUI based on the specifications within the file. The project consists of the following main components:

- `ofxGuiJson.cpp`
- `ofxGuiJson.h`
- JSON configuration file
- `ofApp.cpp`
- `ofApp.h`

## File Descriptions

### 1. `ofxGuiJson.cpp`
This file handles the setup and management of the GUI based on the JSON configuration file. It performs the following tasks:

- **Load JSON**: Reads the JSON configuration file.
- **Parse GUI Elements & Actions**: Interprets the JSON to create GUI elements and map actions.
- **Initialize GUI Components**: Sets up GUI elements such as buttons, toggles, and labels.
- **Setup Action Map**: Maps actions defined in the JSON to corresponding functions.
- **Bind Events**: Connects GUI elements to their respective event handlers.

#### Key Functions:
- `void setup(const std::string& configFile)`: Initializes the GUI setup process.
- `void loadJson(const std::string& relativePath)`: Loads the JSON configuration file.
- `void parseGuiElements()`: Parses the JSON to create GUI elements.
- `void bindEvents()`: Binds GUI elements to their respective event handlers.
- `std::shared_ptr<ofxBaseGui> getGuiElement(const std::string& label)`: Retrieves a GUI element by its label.
- `void updateGuiLabels(const std::map<std::string, std::string>& labelUpdates)`: Updates the labels on the GUI elements.

### 2. `ofxGuiJson.h`
This header file contains the class definitions and function declarations for `ofxGuiJson.cpp`. It defines reusable GUI classes and provides declarations for the functions used in the implementation file.

#### Key Declarations:
- **Class `ofxGuiJson`**: Manages the GUI setup and event binding.
- `void setup(const std::string& configFile)`: Initializes the GUI setup.
- `void loadJson(const std::string& path)`: Loads the JSON configuration.
- `void bindEvents()`: Binds the events to the GUI elements.
- `std::shared_ptr<ofxBaseGui> getGuiElement(const std::string& label)`: Retrieves a GUI element.
- `void updateGuiLabels(const std::map<std::string, std::string>& labelUpdates)`: Updates GUI labels.

### 3. JSON Configuration File
The JSON file defines the structure and properties of the GUI elements. It includes specifications for each GUI component, such as type, label, position, size, default values, and actions.

#### Sample JSON Structure:
```json
{
  "gui_elements": [
    {
      "type": "label",
      "label": "Control Panel",
      "id": "controlPanelLabel",
      "position": {"x": 50, "y": 25},
      "size": {"width": 240, "height": 50}
    }
    ...
  ],
  "audio_settings": {
    "outputChannels": 2,
    "inputChannels": 0,
    "sampleRate": 44100,
    "bufferSize": 512,
    "minFrequency": 220,
    "maxFrequency": 880,
    "sensitivity": 10
  },
  "gui": {
    "title": "Dynamic Audio Control",
    "settings_file": "settings.xml",
    "width": 500,
    "height": 700
  },
  "interaction": {
    "enable": true,
    "type": "mouseYFrequency"
  }
}
