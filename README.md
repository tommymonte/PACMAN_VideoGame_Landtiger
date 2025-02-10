# Pac-Man on LandTiger Board

## Overview

This project implements the classic Pac-Man game on the LandTiger board using firmware written in C. The firmware is designed to manage and integrate various hardware peripherals to deliver a complete gaming experience. The key peripherals include:

- **LCD Display:** Renders game graphics and user interface.
- **CAN Interface:** Handles communication protocols.
- **Speaker:** Outputs audio effects.
- **Joystick and Buttons:** Provides game control inputs.
- **Timers:** Manages game timing and event scheduling.

## Features

- **Classic Gameplay:** Enjoy the retro arcade experience of Pac-Man.
- **Real-Time Performance:** Utilizes hardware timers for smooth, responsive game play.
- **Peripheral Integration:** Seamless coordination of LCD, CAN, speaker, joystick, and button controls.
- **Modular Firmware:** Written in C with clear separation of functionalities for easier maintenance and upgrades.

## Hardware and Software Requirements

### Hardware

- **LandTiger Board:** The primary development board.
- **LCD Display:** For visual output.
- **CAN Transceiver:** For CAN communication.
- **Speaker:** For sound output.
- **Joystick Module & Buttons:** For user input.
- **Additional Components:** Wires, connectors, and power supply as required.

### Software

- **C Compiler:** (e.g., GCC) for building the firmware if compiling via command line.
- **KEIL IDE:** Used for compiling, debugging, and flashing the firmware onto the LandTiger board.
- **Make Utility (Optional):** For projects that support a command line build process.
- **Debugging Tools (Optional):** For troubleshooting firmware issues.
