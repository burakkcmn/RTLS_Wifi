# RTLS WiFi System with DWM1001 and ESP8266

This project involves a Real-Time Location System (RTLS) using the DWM1001 module as an anchor. An ESP8266 is used to transmit the distance information calculated by the anchor to a server. The ESP8266 is connected to the DWM1001 via UART and handles various control and communication tasks.

## Features
- **UART Communication:** Establishes communication between the ESP8266 and DWM1001.
- **Device Initialization:** Controls the startup of the DWM1001, retrieves necessary information (chip ID, Bluetooth ID, version info, etc.), and performs version checks and updates.
- **UDP Communication:** Uses custom packet structures for communication between the server and the anchor, including debug, location, registration, etc.
- **Server Registration:** Registers the device with the server and maintains a heartbeat mechanism.
- **Command Execution:** Executes various commands from the server, such as enabling/disabling logs, request retries, blinking, and changing IDs.
- **Software Updates:** Updates both the ESP8266 and DWM1001 firmware.
- **WiFi Configuration:** Starts in AP mode for WiFi setup through a web interface, with credentials stored in EEPROM.

## Hardware Requirements
- DWM1001 module
- ESP8266 module
- Power supply for both modules

## Software Requirements
- Arduino IDE or PlatformIO
- ESP8266 board support package
- Required libraries: `ESP8266WiFi`, `ESP8266WebServer`, `ArduinoOTA`, `EEPROM`, `FastCRC`, `Adafruit DAP library`

## Installation
1. Clone this repository:
    ```sh
    git clone https://github.com/burakkcmn/RTLS_Wifi.git
    ```
2. Open the project in Arduino IDE or PlatformIO.
3. Install the necessary libraries using Library Manager or PlatformIO.
4. Configure your WiFi credentials and other settings in the code:
    ```cpp
    const char* ssid = "your_SSID";
    const char* password = "your_PASSWORD";
    ```
5. Upload the code to your ESP8266 module.

## Usage
1. Power on the DWM1001 and ESP8266 modules.
2. The ESP8266 will start in AP mode for initial WiFi configuration. Connect to the AP and enter WiFi credentials via the web interface.
3. The ESP8266 will connect to the specified WiFi network and retrieve necessary information from the DWM1001.
4. The device will check for any firmware updates and perform updates if necessary.
5. The ESP8266 will register the device with the server and start sending distance information.
6. The server and anchor will communicate using UDP with custom packet structures.

## Code Structure
- `main.h`: Main header file including necessary includes and definitions.
- `setup()`: Initializes UART, WiFi, web server, OTA, and other components.
- `loop()`: Handles serial communication, server commands, web server, and OTA.
- `bbaEspUploadManager`: Manages OTA updates and checks for firmware updates.
- `dwmGpiocontroller`: Controls GPIO operations for DWM1001.

## Example
Here's a snippet of the setup function:
```cpp
void setup() {
    bbaSerial::setup(bbaGlobalSymbols::getSerialBaudrrate());
    bbaDWMMessenger::getLastErrorMessageRequest();
    eepromUtil::begin();
    bbaWifiManager::WifiLoad(true);
    espUploadManager.OtaConfig(bbaEspUploadManager::UpdateMode::HTTP);
    espUploadManager.httpUpdateNow();
    webServer.begin(bbaGlobalSymbols::getWebServicePort());
    heartbeat.begin();
    dwmGpiocontroller.begin();
    dwmGpiocontroller.startDwm();
    bbaDWMMessenger::registrationRequest();
}
```

## Contributing
Contributions are welcome! Please submit a pull request or open an issue to discuss your changes.

## License
This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.

This README provides a comprehensive overview of your project, including setup instructions, usage details, and code structure. Adjust the content as needed to better fit your specific project requirements.

## Author
- **Burak KOCAMAN**
  - GitHub: [burakkcmn](https://github.com/burakkcmn)
  - Email: [kocaman.burak.bk@gmail.com](mailto:kocaman.burak.bk@gmail.com)
