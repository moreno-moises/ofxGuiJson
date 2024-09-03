## README for ofxGUIJson Project

The ofxGUIJson project leverages OpenFrameworks to create a dynamic and scalable GUI system. It uses a JSON configuration file to define GUI elements and map user interactions dynamically to various functions within an application. This README provides essential guidance on setup, configuration, and a detailed overview of the project's structure.

### System Requirements
- **OpenFrameworks**: Install openFrameworks 0.11.2 from the official [openFrameworks GitHub repository](https://github.com/openframeworks/openFrameworks/releases/tag/0.11.2). Choose the appriopriate version for your operating system.
- **Compiler**: Currently, the project is known to compile with Visual Studio 2022. Ensure your development environment is set up with this version.
- **Dependencies**: This addon was built as an extension for the [ofxGui](https://github.com/openframeworks/openFrameworks/tree/master/addons/ofxGui) addon.
  - [ofxMaxim](https://github.com/micknoise/Maximilian) is required for the 'example_audio' project.

### Installation
To set up the **ofxGUIJson** project in your environment, follow these steps:

1. **Clone the Repository:**
    - Clone the repository or download the source code.
   ```bash
   git clone https://github.com/moreno-moises/ofxGuiJson.git
   
2. **Add to OpenFrameworks:**
    - Copy the downloaded `ofxGUIJson` folder into the `addons` directory of your OpenFrameworks installation.

3. **Create a Project:**
    - Use the OpenFrameworks project generator to create a new project.
    - Include `ofxGuiJson` and `ofxGui` as addons in your project setup.

4. **Build and Run:**
    - Open your project in the respective IDE configured with OpenFrameworks.
    - Build and run the application to verify the setup.

### Usage
#### Configuration
The GUI configuration is driven by a JSON file which specifies the structure, properties, and initial settings of the GUI elements:
```json
{
  "panels": [
    {
      "gui": {
        "title": "Dynamic Audio Control",
        "width": 500,
        "height": 700,
        "position": {"x": 0, "y": 0}
      },
      "elements": [
        {
          "type": "toggle",
          "label": "Mute Sound",
          "id": "muteToggle",
          "default": false,
          "action": "toggleMute"
        }
      ]
    }
  ]
}
```
This JSON structure allows you to define multiple panels and elements, including labels, toggles, and other controls, with specific actions tied to user interactions.

### Function Mapping
Functions are mapped to GUI elements through the JSON configuration. This mapping connects user interactions with specific callbacks defined in your OpenFrameworks application:
```cpp
void ofApp::setup() {
    guiManager.setup("config.json"); // Load GUI configuration
    actionMap["toggleMute"] = &ofApp::onToggleChanged; // Map "toggleMute" action to a function
}
```
Each action in the JSON file corresponds to a member function in the application, allowing dynamic interaction management.

### Project Structure

![image](https://github.com/user-attachments/assets/01ce8220-c844-4de6-aef9-08b2b7a3578a)


- **ofxGuiJson.cpp and ofxGuiJson.h**: These files form the backbone of the project, handling the loading of JSON configurations, parsing GUI elements and actions, initializing GUI components, setting up action maps, and binding events. The `.h` file includes general and reusable GUI class definitions and function declarations, making this component versatile for various applications.
- **JSON Configuration File**: Acts as the blueprint for the GUI by specifying elements, properties, and action mappings that define the interface.
- **ofApp.cpp and ofApp.h**: Specific to the application being developed, these files extend the functionality provided by ofxGuiJson. The `.cpp` file defines actions and handles events, particularly focusing on application-specific functionalities. The `.h` file contains necessary class definitions and function declarations related to the specific application needs.

### Lower Level Project Structure Diagram

![image (3)](https://github.com/user-attachments/assets/81d4fe7c-a0dd-470e-8834-1fad57c76138)

This diagram illustrates the modular and event-driven architecture of ofxGuiJson, where GUI actions are tightly integrated with event handling and callback mechanisms, facilitating a responsive and dynamic user interface


### Known Issues
For a list of known issues and feature requests, check the [issues section](https://github.com/moreno-moises/ofxGuiJson/issues) on GitHub. Please report any new issues or bugs you encounter.

### Future Work
As ofxGuiJson continues to evolve, the following are key enhancements planned for future versions, aimed at making the GUI more flexible and user-friendly:
1. **Implement note adding (log note) with ofxTextField** - Allow users to add notes directly within the GUI.
2. **Font Customization Features**:
   - **Color**: Enable changing font colors.
   - **Size**: Adjust font sizes for better readability.
   - **Type**: Incorporate various font types to enhance GUI aesthetics.
3. **GUI Groups**: Organize elements into logical groups for better management.
4. **Multi-Select and Single-Select Lists**: Implement systems for selecting multiple options from lists, enhancing JSON compatibility.
5. **Device Connection Conditionals**: Add GUI elements that react based on device connectivity.
6. **Toggle Enabling/Disabling**: Provide options to disable or enable toggles dynamically.
7. **Responsive Design**: Adapt GUI widths as percentages to maintain layout integrity when resizing windows.
8. **Propagation of Positions and Dimensions in Multiple Panels**: Ensure consistent layout across multiple panels.
9. **Enhanced File Loading**: Allow loading multiple JSON files, facilitating easier customization and scalability.




