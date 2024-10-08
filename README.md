# Security System on FRDM-K66F

This repository contains the code for a security system implemented on the NXP FRDM-K66F development board. The system integrates various hardware components to simulate a basic home security setup, featuring motion detection, keypad input, and LCD display for user interaction.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation and Setup](#installation-and-setup)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## Overview

The security system is designed to demonstrate embedded systems programming and hardware integration using the FRDM-K66F board. It serves as a practical example for students and enthusiasts interested in embedded security applications.

## Features

- **Motion Detection**: Utilizes a PIR motion sensor to detect movement.
- **Keypad Input**: Allows users to input a security code to arm or disarm the system.
- **LCD Display**: Provides real-time feedback and system status updates.
- **Alarm Activation**: Triggers an alarm when unauthorized access is detected.
- **User Authentication**: Implements code verification for enhanced security.

## Hardware Requirements

- **NXP FRDM-K66F Development Board**
- **PIR Motion Sensor**
- **4x4 Matrix Keypad**
- **16x2 Character LCD Display**
- **Buzzer or Alarm Module**
- **Breadboard and Jumper Wires**
- **USB Cable** for programming and power supply

## Software Requirements

- **MCUXpresso IDE** or any compatible IDE for ARM Cortex-M processors
- **Kinetis SDK** for board support packages and drivers
- **USB Drivers** for FRDM-K66F board communication
- **Git** for cloning the repository (optional)

## Installation and Setup

1. **Clone the Repository**

   ```bash
   git clone https://github.com/InderParmar/Security-System-FRDM-K66F.git
   ```

2. **Import the Project into Your IDE**

   - Open your IDE (e.g., MCUXpresso).
   - Select **File > Import**.
   - Choose **Existing Projects into Workspace**.
   - Browse to the cloned repository and import the project.

3. **Hardware Connections**

   - **PIR Sensor**: Connect to the designated GPIO pins for motion detection.
   - **Keypad**: Connect rows and columns to the specified GPIO pins for input handling.
   - **LCD Display**: Connect data and control lines according to the wiring diagram.
   - **Buzzer**: Connect to a GPIO pin configured for output to act as an alarm.

   *Refer to the `docs/wiring_diagram.png` for detailed connections.*

4. **Build and Flash the Firmware**

   - Compile the project within your IDE.
   - Flash the compiled binary to the FRDM-K66F board using a USB connection.

## Usage

- **System Initialization**

  Upon powering up, the system initializes all peripherals and displays a welcome message on the LCD screen.

- **Arming the System**

  Enter the predefined security code using the keypad to arm the system. The LCD will display an "Armed" status.

- **Motion Detection**

  When armed, the PIR sensor monitors for any motion. Detection of movement triggers the alarm.

- **Disarming the System**

  Before entering the secured area, input the security code to disarm the system. The LCD will display a "Disarmed" status.

- **Alarm Reset**

  If the alarm is triggered, enter the security code to reset the system.

## Project Structure

- **`/src`**: Source code files.
- **`/include`**: Header files.
- **`/docs`**: Documentation and wiring diagrams.
- **`/lib`**: External libraries and dependencies.
- **`/README.md`**: Project documentation.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Commit your changes with clear messages.
4. Submit a pull request detailing your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
