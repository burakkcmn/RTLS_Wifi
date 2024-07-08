# InterconnectedCOMsDebug Project
## Overview
The InterconnectedCOMsDebug project is designed to facilitate debugging and monitoring of communication between multiple serial ports on a computer. It provides a user interface (UI) for selecting ports, configuring parameters, and displaying log messages.

## Features
- Port Selection: Choose between multiple COM ports for debugging.
- Dynamic Configuration: Configure baud rate, data bits, stop bits, and parity for each port.
- Log Display: View real-time logs of communication activities.
- Customization: Options to auto-scroll logs, show only message content, and format message displays.

## Setup

### Requirements
- Java Development Kit (JDK) 8 or higher
- IDE (e.g., Eclipse, IntelliJ IDEA) for Java development

### Installation

1. Clone the repository:

```bash
git clone https://github.com/burakkcmn/InterconnectedCOMsDebug.git
cd InterconnectedCOMsDebug
```

2. Import the project into your IDE.

3. Compile and run the debugToolUI class.

## Usage

### Command Line Parameters

The program supports command line parameters for configuring the COM ports and their settings. Examples:

- For basic port configuration:


```bash
java  port1=COM4;port2=COM5
```


```bash
java  com.debug.tool.debugTool port1=COM4;baudrate1=115200;databits1=8;stopbits1=1;parity1=NONE;port2=COM5;baudrate2=115200;databits2=8;stopbits2=1;parity2=NONE
```

## User Interface
Upon launching the application, you will see a graphical user interface (GUI) with options to select ports, set parameters, and view communication logs. Key components include:

- Port Selection: Choose between available COM ports.
- Parameter Configuration: Adjust baud rate, data bits, stop bits, and parity settings.
- Log Display: Real-time display of communication logs, categorized by port and message type.

## Customization
- Auto Scroll: Enable or disable automatic scrolling of log messages.
- Show Just Message: Display only the message content in logs.
- Format Message: Format log messages for better readability.

## Contributing
Contributions are welcome! Feel free to submit issues or pull requests for any enhancements or bug fixes.

## License
This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.

This README provides a comprehensive overview of your project, including setup instructions, usage details, and code structure. Adjust the content as needed to better fit your specific project requirements.

## Author
- **Burak KOCAMAN**
  - GitHub: [burakkcmn](https://github.com/burakkcmn)
  - Email: [kocaman.burak.bk@gmail.com](mailto:kocaman.burak.bk@gmail.com)
