![N|Solid](https://arribada.org/wp-content/uploads/2022/01/arribada_web_logo_g.svg)
# Argos SMD AT Kineis Firmware

This repository contains the STM32WL firmware for the Argos SMD module with the Kineis stack. It provides two demo-application examples (standalone or AT commands) on an STM32WL55xx-based hardware reference design (KRD_MP).
This project is based on krd_fw_package_v6.2.1 from CLS adapted for the [argos-smd](https://github.com/arribada/argos-smd-hw) module

In GUI mode, the module can answer to AT command and interact with KimGUI from CLS.

## Features

    - Compiles standalone/GUI application by default
    - Supports periodic SAT message transmission
    - Configurable MAC profiles and radio settings
    - Low Power Mode (LPM) management

## Getting Started
### Configuration
#### Credentials Management

    Contact Kinéis for credentials (ID, address, secret key) necessary for the stack.
    Credentials should be securely stored; the provided package stores them in RAM/FLASH without protection.
    Update device_id and address in mcu_nvm.c file

#### Radio Configuration Management

    Obtain radio configuration binary files from Kinéis.
    Store configuration data securely; in the provided package, it's hardcoded as a byte table in mcu_nvm.c.
    or
    Use AT+RCONF= command 
    or
    Change it diretly by comment/uncomment correspoindg configuration inside mcu_nvm.c file

### MAC Profile Configuration Management

    Choose between BASIC and BLIND MAC profiles by setting compilation flags in kns_app.c.
    Configure specific parameters like repetition period and number of repetitions for the BLIND profile.

## Building the Project

### From STM32CubeIDE

    Install STM32CubeIDE.
    Open the project from the Kinéis package archive.
    Clean the project before rebuilding.
    Build the project to generate the binary file.

###From Terminal

    Ensure the GNU Make and ARM toolchain are available.
    Clean the current build before rebuilding.
    Run make clean and make in the root directory to build the binary.

###Compilation Flags

    Adjust log levels, LPM levels, and board types in the makefile as needed.

## AT Commands

The available AT commands for the GUI application are managed by the AT command manager and include:

    General Commands:
        AT+VERSION: Get AT commands version
        AT+PING: Ping command
        AT+FW: Get firmware version
        AT+ADDR: Get device address
        AT+ID: Get device ID
        AT+SN: Get device serial number
        AT+RCONF: Get/Set radio configuration
        AT+SAVE_RCONF: Save radio configuration into Flash
        AT+LPM: Get/Set low power mode
        AT+UDATE: Update UTC date/time

    Forward message
        AT+TX: Transmit data
        AT+ATXRP: Get/Set TX repetition commands

    Certification Commands:
        AT+CW: Continuous Wave/MW commands

    Satellite Pass Predictions Commands:
        AT+PREPASS_EN: Get/Set PREVIPASS algorithm



Refer to the mgr_at_cmd_list.h file for a complete list of supported AT commands.
##Documentation

    Auto-generated Doxygen documentation is available in the Kineis/Doc folder, open index.html

##License

This project is licensed under the GNU General Public License v3. See the LICENSE file for details.

